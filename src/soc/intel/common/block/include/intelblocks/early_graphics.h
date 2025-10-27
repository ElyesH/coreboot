/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef SOC_INTEL_COMMON_BLOCK_GRAPHICS_EARLY_H
#define SOC_INTEL_COMMON_BLOCK_GRAPHICS_EARLY_H


/*
 * SoC override
 *
 * All new SoC must implement below functionality.
 */

/* Configure display panel */
void early_graphics_soc_panel_init(void);

/*
 * Early graphics module API
 * Graphics at this stage is limited to VGA text mode.
 */

/* Initialize graphics.  Return true if VGA text mode is ready to use. */
bool early_graphics_init(void);

/* Clear graphics configuration, turn off the displays. */
void early_graphics_stop(void);

/* Allow early configuration of any display related GPIOs as needed */
const struct pad_config *variant_early_graphics_gpio_table(size_t *num);

#endif	/* SOC_INTEL_COMMON_BLOCK_GRAPHICS_EARLY_H */
