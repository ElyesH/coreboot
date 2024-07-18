/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <device/device.h>
#include <device/pnp.h>
#include <superio/conf_mode.h>
#include <pc80/keyboard.h>
#include <superio/ite/common/env_ctrl.h>

#include "chip.h"
#include "it8629e.h"

static void it8629e_init(struct device *dev)
{
	const struct superio_ite_it8629e_config *conf = dev->chip_info;
	const struct resource *res;

	if (!dev->enabled)
		return;

	switch (dev->path.pnp.device) {
	/* TODO: Might potentially need code for FDC etc. */
	case IT8629E_EC:
		res = probe_resource(dev, PNP_IDX_IO0);
		if (!conf || !res)
			break;
		ite_ec_init(res->base, &conf->ec);
		break;
	case IT8629E_KBCK:
		set_kbc_ps2_mode();
		pc_keyboard_init(NO_AUX_DEVICE);
		break;
	}
}

static struct device_operations ops = {
	.read_resources   = pnp_read_resources,
	.set_resources    = pnp_set_resources,
	.enable_resources = pnp_enable_resources,
	.enable           = pnp_alt_enable,
	.init             = it8629e_init,
	.ops_pnp_mode     = &pnp_conf_mode_870155_aa,
};

static struct pnp_info pnp_dev_info[] = {
	{ nullptr, IT8629E_FDC, PNP_IO0 | PNP_IRQ0 | PNP_DRQ0, 0x0ff8, },
	{ nullptr, IT8629E_SP1, PNP_IO0 | PNP_IRQ0, 0x0ff8, },
	{ nullptr, IT8629E_SP2, PNP_IO0 | PNP_IRQ0, 0x0ff8, },
	{ nullptr, IT8629E_PP, PNP_IO0 | PNP_IRQ0 | PNP_DRQ0, 0x0ffc, },
	{ nullptr, IT8629E_EC, PNP_IO0 | PNP_IO1 | PNP_IRQ0, 0x0ff8, 0x0ff8, },
	{ nullptr, IT8629E_KBCK, PNP_IO0 | PNP_IO1 | PNP_IRQ0, 0x0fff, 0x0fff, },
	{ nullptr, IT8629E_KBCM, PNP_IRQ0, },
	{ nullptr, IT8629E_GPIO, PNP_IO0 | PNP_IO1 | PNP_IO2 | PNP_IRQ0,
		0x0fff, 0x0ff8, 0x0ff8, },
	{ nullptr, IT8629E_UNKNOWN, PNP_IO0 | PNP_IRQ0, 0xff8, },
	{ nullptr, IT8629E_IR, PNP_IO0 | PNP_IRQ0, 0x0ff8, },
};

static void enable_dev(struct device *dev)
{
	pnp_enable_devices(dev, &ops, ARRAY_SIZE(pnp_dev_info), pnp_dev_info);
}

struct chip_operations superio_ite_it8629e_ops = {
	.name       = "ITE IT8629E Super I/O",
	.enable_dev = enable_dev
};
