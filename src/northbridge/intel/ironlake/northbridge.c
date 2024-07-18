/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <acpi/acpi.h>
#include <arch/hpet.h>
#include <device/pci_ops.h>
#include <stdint.h>
#include <cpu/intel/model_2065x/model_2065x.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include "chip.h"
#include <commonlib/bsd/helpers.h>
#include "ironlake.h"
#include <cpu/intel/smm_reloc.h>

static int bridge_revision_id = -1;

int bridge_silicon_revision(void)
{
	if (bridge_revision_id < 0) {
		uint8_t stepping = cpuid_eax(1) & 0x0f;
		uint8_t bridge_id = pci_read_config16(pcidev_on_root(0, 0), PCI_DEVICE_ID);
		bridge_revision_id = (bridge_id & 0xf0) | stepping;
	}
	return bridge_revision_id;
}

/*
 * Reserve everything between A segment and 1MB:
 *
 * 0xa0000 - 0xbffff: legacy VGA
 * 0xc0000 - 0xcffff: VGA OPROM (needed by kernel)
 * 0xe0000 - 0xfffff: SeaBIOS, if used, otherwise DMI
 */

static void add_fixed_resources(struct device *dev, int index)
{
	/* 0xe0000000-0xf0000000 PCIe config.
	   0xfed10000-0xfed14000 MCH
	   0xfed17000-0xfed18000 HECI
	   0xfed18000-0xfed19000 DMI
	   0xfed19000-0xfed1a000 EPBAR
	   0xfed1c000-0xfed20000 RCBA
	   0xfed90000-0xfed94000 IOMMU
	   0xff800000-0xffffffff ROM. */

	mmio_range(dev, index++, HPET_BASE_ADDRESS, 0x00100000);
	mmio_from_to(dev, index++, 0xa0000, 0xc0000);
	reserved_ram_from_to(dev, index++, 0xc0000, 1 * MiB);
}

#if CONFIG(HAVE_ACPI_TABLES)
static const char *northbridge_acpi_name(const struct device *dev)
{
	if (dev->path.type == DEVICE_PATH_DOMAIN)
		return "PCI0";

	if (!is_pci_dev_on_bus(dev, 0))
		return nullptr;

	switch (dev->path.pci.devfn) {
	case PCI_DEVFN(0, 0):
		return "MCHC";
	}

	return nullptr;
}
#endif

struct device_operations ironlake_pci_domain_ops = {
	.read_resources	= pci_domain_read_resources,
	.set_resources	= pci_domain_set_resources,
	.scan_bus	= pci_host_bridge_scan_bus,
#if CONFIG(HAVE_ACPI_TABLES)
	.acpi_name	= northbridge_acpi_name,
#endif
};

static void mc_read_resources(struct device *dev)
{
	uint32_t tseg_base, tseg_end;
	uint64_t touud;
	uint16_t reg16;
	int index = 3;

	pci_dev_read_resources(dev);

	mmconf_resource(dev, 0x50);

	tseg_base = pci_read_config32(pcidev_on_root(0, 0), TSEG);
	tseg_end = tseg_base + CONFIG_SMM_TSEG_SIZE;
	touud = pci_read_config16(pcidev_on_root(0, 0),
				  TOUUD);

	printk(BIOS_DEBUG, "ram_before_4g_top: 0x%x\n", tseg_base);
	printk(BIOS_DEBUG, "TOUUD: 0x%x\n", (unsigned int)touud);

	/* Report the memory regions */
	ram_range(dev, index++, 0, 0xa0000);
	ram_from_to(dev, index++, 1 * MiB, tseg_base);

	mmio_range(dev, index++, tseg_base, CONFIG_SMM_TSEG_SIZE);

	reg16 = pci_read_config16(pcidev_on_root(0, 0), GGC);
	const int uma_sizes_gtt[16] =
	    { 0, 1, 0, 2, 0, 0, 0, 0, 0, 2, 3, 4, 42, 42, 42, 42 };
	/* Igd memory */
	const int uma_sizes_igd[16] = {
		0, 0, 0, 0, 0, 32, 48, 64, 128, 256, 96, 160, 224, 352, 256, 512
	};
	u32 igd_base, gtt_base;
	int uma_size_igd, uma_size_gtt;

	uma_size_igd = uma_sizes_igd[(reg16 >> 4) & 0xF];
	uma_size_gtt = uma_sizes_gtt[(reg16 >> 8) & 0xF];

	igd_base =
	    pci_read_config32(pcidev_on_root(0, 0), IGD_BASE);
	gtt_base =
	    pci_read_config32(pcidev_on_root(0, 0), GTT_BASE);
	if (gtt_base > tseg_end) {
		/* Reserve the gap. MMIO doesn't work in this range. Keep
		   it uncacheable, though, for easier MTRR allocation. */
		mmio_from_to(dev, index++, tseg_end, gtt_base);
	}
	mmio_range(dev, index++, gtt_base, uma_size_gtt * MiB);
	mmio_range(dev, index++, igd_base, uma_size_igd * MiB);

	upper_ram_end(dev, index++, touud * MiB);

	/* This memory is not DMA-capable. */
	if (touud >= 8192 - 64)
		bad_ram_range(dev, index++, 0x1fc000000ULL, 0x004000000);

	add_fixed_resources(dev, index);
}

static void northbridge_init(struct device *dev)
{
	/* Clear error status bits */
	dmibar_write32(DMIUESTS, 0xffffffff);
	dmibar_write32(DMICESTS, 0xffffffff);

	dmibar_setbits32(DMILLTC, 1 << 29);

	dmibar_setbits32(0x1f8, 1 << 16);

	dmibar_setbits32(DMILCTL, 1 << 1 | 1 << 0);
}

/* Disable unused PEG devices based on devicetree before PCI enumeration */
static void ironlake_init(void *const chip_info)
{
	u32 deven_mask = UINT32_MAX;
	const struct device *dev;

	dev = pcidev_on_root(1, 0);
	if (!dev || !dev->enabled) {
		printk(BIOS_DEBUG, "Disabling PEG10.\n");
		deven_mask &= ~DEVEN_PEG10;
	}
	dev = pcidev_on_root(2, 0);
	if (!dev || !dev->enabled) {
		printk(BIOS_DEBUG, "Disabling IGD.\n");
		deven_mask &= ~DEVEN_IGD;
	}
	const struct device *const d0f0 = pcidev_on_root(0, 0);
	if (d0f0)
		pci_update_config32(d0f0, DEVEN, deven_mask, 0);
}

static struct device_operations mc_ops = {
	.read_resources		= mc_read_resources,
	.set_resources		= pci_dev_set_resources,
	.enable_resources	= pci_dev_enable_resources,
	.init			= northbridge_init,
	.acpi_fill_ssdt		= generate_cpu_entries,
	.ops_pci		= &pci_dev_ops_pci,
};

/*
 * The host bridge PCI device ID can be changed by the firmware. There
 * is no documentation about it, though. There's 'official' IDs, which
 * appear in spec updates and Windows drivers, and 'mysterious' IDs,
 * which Intel doesn't want OSes to know about and thus are not listed.
 *
 * The current coreboot code seems to be able to change the device ID
 * of the host bridge, but it seems to be missing a warm reset so that
 * the device ID changes. Account for the 'mysterious' device IDs in
 * the northbridge driver, so that booting an OS has a chance to work.
 */
static const unsigned short pci_device_ids[] = {
	/* 'Official' DIDs */
	0x0040, /* Clarkdale */
	0x0044, /* Arrandale */
	0x0048, /* Unknown, but it appears in OS drivers and raminit */

	/* Mysterious DIDs, taken from Linux' intel-agp driver */
	0x0062, /* Arrandale A-? */
	0x0069, /* Clarkdale K-0 */
	0x006a, /* Arrandale K-0 */
	0
};

static const struct pci_driver mc_driver_ilk __pci_driver = {
	.ops     = &mc_ops,
	.vendor  = PCI_VID_INTEL,
	.devices = pci_device_ids,
};

struct device_operations ironlake_cpu_bus_ops = {
	.read_resources = noop_read_resources,
	.set_resources = noop_set_resources,
	.init = mp_cpu_bus_init,
};

struct chip_operations northbridge_intel_ironlake_ops = {
	.name = "Intel Ironlake integrated Northbridge",
	.init = ironlake_init,
};
