/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <soc/gpio.h>
#include <types.h>
#include <vendorcode/google/chromeos/chromeos.h>

/* Pad configuration in ramstage */
static const struct pad_config gpio_table[] = {
	/* A14 : USB_OC1# ==> USB_A0_FAULT_ODL */
	PAD_CFG_NF_LOCK(GPP_A14, NONE, NF1, LOCK_CONFIG),
	/* A15 : USB_OC2# ==> USB_A1_FAULT_ODL */
	PAD_CFG_NF_LOCK(GPP_A15, NONE, NF1, LOCK_CONFIG),
};

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
	/* TODO */
};

/* Fill romstage gpio configuration */
static const struct pad_config romstage_gpio_table[] = {
	/* TODO */
};

const struct pad_config *variant_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(gpio_table);
	return gpio_table;
}

const struct pad_config *variant_gpio_override_table(size_t *num)
{
	*num = 0;
	return nullptr;
}

const struct pad_config *variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

static const struct cros_gpio cros_gpios[] = {
	/* TODO */
};
DECLARE_CROS_GPIOS(cros_gpios);

const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}
