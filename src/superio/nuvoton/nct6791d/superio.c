/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <device/device.h>
#include <device/pnp.h>
#include <pc80/keyboard.h>
#include <superio/conf_mode.h>
#include <superio/common/ssdt.h>
#include <acpi/acpi.h>
#include "nct6791d.h"

static void nct6791d_init(struct device *dev)
{
	if (!dev->enabled)
		return;

	switch (dev->path.pnp.device) {
	case NCT6791D_KBC:
		pc_keyboard_init(NO_AUX_DEVICE);
		break;
	}
}

#if CONFIG(HAVE_ACPI_TABLES)
/* Provide ACPI HIDs for generic Super I/O SSDT */
static const char *nct6791d_acpi_hid(const struct device *dev)
{
	if ((dev->path.type != DEVICE_PATH_PNP) ||
		(dev->path.pnp.port == 0) ||
		((dev->path.pnp.device & 0xff) > NCT6791D_DS))
		return nullptr;

	switch (dev->path.pnp.device & 0xff) {
	case NCT6791D_SP1: /* fallthrough */
	case NCT6791D_SP2:
		return ACPI_HID_COM;
	case NCT6791D_KBC:
		return ACPI_HID_KEYBOARD;
	default:
		return ACPI_HID_PNP;
	}
}
#endif

static struct device_operations ops = {
	.read_resources   = pnp_read_resources,
	.set_resources    = pnp_set_resources,
	.enable_resources = pnp_enable_resources,
	.enable           = pnp_alt_enable,
	.init             = nct6791d_init,
	.ops_pnp_mode     = &pnp_conf_mode_8787_aa,
#if CONFIG(HAVE_ACPI_TABLES)
	.acpi_fill_ssdt   = superio_common_fill_ssdt_generator,
	.acpi_name        = superio_common_ldn_acpi_name,
	.acpi_hid         = nct6791d_acpi_hid,
#endif
};

static struct pnp_info pnp_dev_info[] = {
	{ nullptr, NCT6791D_PP, PNP_IO0 | PNP_IRQ0 | PNP_DRQ0,
		0x0ff8, },
	{ nullptr, NCT6791D_SP1, PNP_IO0 | PNP_IRQ0,
		0x0ff8, },
	{ nullptr, NCT6791D_SP2, PNP_IO0 | PNP_IRQ0,
		0x0ff8, },
	{ nullptr, NCT6791D_KBC, PNP_IO0 | PNP_IO1 | PNP_IRQ0 | PNP_IRQ1,
		0x0fff, 0x0fff, },
	{ nullptr, NCT6791D_CIR, PNP_IO0 | PNP_IRQ0,
		0x0ff8, },
	{ nullptr, NCT6791D_ACPI},
	{ nullptr, NCT6791D_HWM_FPLED, PNP_IO0 | PNP_IO1 | PNP_IRQ0,
		0x0ffe, 0x0ffe, },
	{ nullptr, NCT6791D_BCLK_WDT2_WDTMEM},
	{ nullptr, NCT6791D_CIRWUP, PNP_IO0 | PNP_IRQ0,
		0x0ff8, },
	{ nullptr, NCT6791D_GPIO_PP_OD},
	{ nullptr, NCT6791D_PORT80},
	{ nullptr, NCT6791D_WDT1},
	{ nullptr, NCT6791D_WDTMEM},
	{ nullptr, NCT6791D_GPIOBASE, PNP_IO0,
		0x0ff8, },
	{ nullptr, NCT6791D_GPIO0},
	{ nullptr, NCT6791D_GPIO1},
	{ nullptr, NCT6791D_GPIO2},
	{ nullptr, NCT6791D_GPIO3},
	{ nullptr, NCT6791D_GPIO4},
	{ nullptr, NCT6791D_GPIO5},
	{ nullptr, NCT6791D_GPIO6},
	{ nullptr, NCT6791D_GPIO7},
	{ nullptr, NCT6791D_GPIO8},
	{ nullptr, NCT6791D_DS5},
	{ nullptr, NCT6791D_DS3},
	{ nullptr, NCT6791D_PCHDSW},
	{ nullptr, NCT6791D_DSWWOPT},
	{ nullptr, NCT6791D_DS3OPT},
	{ nullptr, NCT6791D_DSDSS},
	{ nullptr, NCT6791D_DSPU},
};

static void enable_dev(struct device *dev)
{
	pnp_enable_devices(dev, &ops, ARRAY_SIZE(pnp_dev_info), pnp_dev_info);
}

struct chip_operations superio_nuvoton_nct6791d_ops = {
	.name = "NUVOTON NCT6791D Super I/O",
	.enable_dev = enable_dev,
};
