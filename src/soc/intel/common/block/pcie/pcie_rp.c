/* SPDX-License-Identifier: GPL-2.0-only */

#include <stdint.h>

#include <commonlib/helpers.h>
#include <console/console.h>
#include <device/device.h>
#include <device/pci_def.h>
#include <device/pci_ops.h>
#include <device/pci_type.h>
#include <intelblocks/pcie_rp.h>

static int pcie_rp_original_idx(
		const struct pcie_rp_group *const group,
		const unsigned int offset,
		const pci_devfn_t dev)
{
	const uint16_t clist = pci_s_find_capability(dev, PCI_CAP_ID_PCIE);
	if (clist == 0) {
		printk(BIOS_WARNING,
		       "%s: Can't find PCIe capapilities for PCI: 00:%02x.%x, ignoring.\n",
		       __func__, group->slot, PCI_FUNC(PCI_DEV2DEVFN(dev)));
		return -1;
	}

	const uint16_t xcap = pci_s_read_config16(dev, clist + PCI_EXP_FLAGS);
	if ((xcap & PCI_EXP_FLAGS_TYPE) >> 4 != PCI_EXP_TYPE_ROOT_PORT) {
		printk(BIOS_WARNING, "%s: Non root-port found at PCI: 00:%02x.%x, ignoring.\n",
		       __func__, group->slot, PCI_FUNC(PCI_DEV2DEVFN(dev)));
		return -1;
	}

	const uint32_t lcap = pci_s_read_config32(dev, clist + PCI_EXP_LNKCAP);

	/* Read n-based absolute port number from LCAP register.
	   This reflects the numbering scheme that Intel uses in their
	   documentation and what we use as index (0-based, though) in
	   our mapping. */
	const unsigned int port_num = (lcap & PCI_EXP_LNKCAP_PORT) >> 24;

	/* Subtract lcap_port_base from port_num to get 0-based index */
	const unsigned int port_idx = port_num - group->lcap_port_base;

	/* Check if port_idx (0-based) is out of bounds */
	if (port_idx < offset || port_idx >= offset + group->count) {
		printk(BIOS_WARNING, "%s: Unexpected root-port number '%u'"
				     " at PCI: 00:%02x.%x, ignoring.\n",
		       __func__, port_num, group->slot, PCI_FUNC(PCI_DEV2DEVFN(dev)));
		return -1;
	}

	return port_idx;
}

/* Scan actual PCI config space to reconstruct current mapping */
static void pcie_rp_scan_groups(int mapping[], const struct pcie_rp_group *const groups)
{
	unsigned int offset = 0;
	const struct pcie_rp_group *group;
	for (group = groups; group->count; ++group) {
		unsigned int fn;
		for (fn = rp_start_fn(group); fn <= rp_end_fn(group); ++fn) {
			const pci_devfn_t dev = PCI_DEV(0, group->slot, fn);
			const uint16_t did = pci_s_read_config16(dev, PCI_DEVICE_ID);
			if (did == 0xffff) {
				if (fn == 0)
					break;
				continue;
			}

			const int rp_idx = pcie_rp_original_idx(group, offset, dev);
			if (rp_idx < 0)
				continue;
			if (mapping[rp_idx] != -1) {
				printk(BIOS_WARNING, "%s: Root Port #%u reported by PCI: "
				       "00:%02x.%x already reported by PCI: 00:%02x.%x!\n",
				       __func__, rp_idx + 1, group->slot, fn,
				       group->slot, mapping[rp_idx]);
				continue;
			}

			printk(BIOS_INFO, "Found PCIe Root Port #%u at PCI: 00:%02x.%x.\n",
			       rp_idx + 1, group->slot, fn);
			mapping[rp_idx] = fn;
		}
		offset += group->count;
	}
}

/* Returns `true` if the device should be unlinked. */
static bool pcie_rp_update_dev(
		struct device *const dev,
		const struct pcie_rp_group *const groups,
		const int mapping[])
{
	if (dev->path.type != DEVICE_PATH_PCI)
		return false;

	/* Find matching group and offset. */
	unsigned int offset = 0;
	const struct pcie_rp_group *group;
	for (group = groups; group->count; ++group) {
		if (PCI_SLOT(dev->path.pci.devfn) == group->slot &&
		    PCI_FUNC(dev->path.pci.devfn) >= rp_start_fn(group) &&
		    PCI_FUNC(dev->path.pci.devfn) <= rp_end_fn(group))
			break;
		offset += group->count;
	}
	if (!group->count)
		return false;

	/* Now update based on what we know. */
	const int rp_idx = offset + PCI_FUNC(dev->path.pci.devfn);
	const int new_fn = mapping[rp_idx];
	if (new_fn < 0) {
		if (dev->enabled) {
			printk(BIOS_NOTICE, "%s: Couldn't find PCIe Root Port #%u "
			       "(originally %s) which was enabled in devicetree, removing and disabling.\n",
			       __func__, rp_idx + 1, dev_path(dev));
			dev->enabled = 0;
		}
		return true;
	} else if (PCI_FUNC(dev->path.pci.devfn) != new_fn) {
		printk(BIOS_INFO,
		       "Remapping PCIe Root Port #%u from %s to new function number %u.\n",
		       rp_idx + 1, dev_path(dev), new_fn);
		dev->path.pci.devfn = PCI_DEVFN(PCI_SLOT(dev->path.pci.devfn), new_fn);
	}
	return false;
}

void pcie_rp_update_devicetree(const struct pcie_rp_group *const groups)
{
	/* Maps absolute root-port numbers to function numbers.
	   Negative if disabled, new function number otherwise. */
	int mapping[CONFIG_MAX_ROOT_PORTS];
	unsigned int offset, i;

	if (!groups || !groups->count)
		return;

	struct bus *const root = pci_root_bus();
	if (!root)
		return;

	offset = 0;
	const struct pcie_rp_group *group;
	for (group = groups; group->count; ++group)
		offset += group->count;

	if (offset > ARRAY_SIZE(mapping)) {
		printk(BIOS_ERR, "%s: Error: Group exceeds CONFIG_MAX_ROOT_PORTS.\n", __func__);
		return;
	}

	/* Assume everything we don't encounter later is disabled */
	for (i = 0; i < ARRAY_SIZE(mapping); ++i)
		mapping[i] = -1;

	pcie_rp_scan_groups(mapping, groups);

	struct device *dev;
	struct device **link = &root->children;
	for (dev = *link; dev; dev = *link) {
		if (pcie_rp_update_dev(dev, groups, mapping)) {
			/* Unlink vanished device. */
			*link = dev->sibling;
			dev->sibling = nullptr;
			continue;
		}

		link = &dev->sibling;
	}
}
