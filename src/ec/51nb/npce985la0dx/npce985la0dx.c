/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <device/pnp.h>

/*
 * This embedded controller looks awfully like a Super I/O chip. LDNs 5 and 6
 * need to be enabled to turn on the keyboard and mouse controller, and LDN
 * 0x11 needs to be enabled to turn on ACPI embedded controller functionality.
 */
static struct pnp_info dev_infos[] = {
	{ nullptr, 0x05 }, { nullptr, 0x06 }, { nullptr, 0x11 }
};

static void ec_51nb_npce985la0dx_ops_enable(struct device *dev)
{
	pnp_enable_devices(dev, &pnp_ops, ARRAY_SIZE(dev_infos), dev_infos);
}

struct chip_operations ec_51nb_npce985la0dx_ops = {
	.name = "51NB EC",
	.enable_dev = ec_51nb_npce985la0dx_ops_enable,
};
