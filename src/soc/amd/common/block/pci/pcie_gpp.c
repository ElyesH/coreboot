/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <acpi/acpi_device.h>
#include <acpi/acpigen.h>
#include <acpi/acpigen_pci.h>
#include <amdblocks/amd_pci_util.h>
#include <assert.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pciexp.h>
#include <soc/pci_devs.h>
#include <stdio.h>
#include <stdlib.h>

static const char *pcie_gpp_acpi_name(const struct device *dev)
{
	char *name;

	if (dev->path.type != DEVICE_PATH_PCI)
		return nullptr;

	name = malloc(ACPI_NAME_BUFFER_SIZE);
	snprintf(name, ACPI_NAME_BUFFER_SIZE, "GP%02X", dev->path.pci.devfn);
	name[4] = '\0';

	return name;
}

static void acpi_device_write_gpp_pci_dev(const struct device *dev)
{
	const char *scope = acpi_device_scope(dev);
	const char *name = acpi_device_name(dev);

	assert(dev->path.type == DEVICE_PATH_PCI);
	assert(name);
	assert(scope);

	acpigen_write_scope(scope);
	acpigen_write_device(name);

	acpigen_write_ADR_pci_device(dev);
	acpigen_write_STA(acpi_device_status(dev));

	acpigen_write_pci_GNB_PRT(dev);

	acpigen_pop_len(); /* Device */
	acpigen_pop_len(); /* Scope */
}

/* Latency tolerance reporting, max snoop/non-snoop latency value 1.049ms */
#define PCIE_LTR_MAX_LATENCY_1049US 0x1001

static void pcie_get_ltr_max_latencies(u16 *max_snoop, u16 *max_nosnoop)
{
	*max_snoop = PCIE_LTR_MAX_LATENCY_1049US;
	*max_nosnoop = PCIE_LTR_MAX_LATENCY_1049US;
}

static struct pci_operations pcie_ops = {
	.get_ltr_max_latencies	= pcie_get_ltr_max_latencies,
	.set_subsystem		= pci_dev_set_subsystem,
};

struct device_operations amd_internal_pcie_gpp_ops = {
	.read_resources		= pci_bus_read_resources,
	.set_resources		= pci_dev_set_resources,
	.enable_resources	= pci_bus_enable_resources,
	.scan_bus		= pci_scan_bridge,
	.reset_bus		= pci_bus_reset,
	.acpi_name		= pcie_gpp_acpi_name,
	.acpi_fill_ssdt		= acpi_device_write_gpp_pci_dev,
};

struct device_operations amd_external_pcie_gpp_ops = {
	.read_resources		= pci_bus_read_resources,
	.set_resources		= pci_dev_set_resources,
	.enable_resources	= pci_bus_enable_resources,
	.scan_bus		= pciexp_scan_bridge,
	.reset_bus		= pci_bus_reset,
	.acpi_name		= pcie_gpp_acpi_name,
	.acpi_fill_ssdt		= acpi_device_write_gpp_pci_dev,
	.ops_pci		= &pcie_ops,
};
