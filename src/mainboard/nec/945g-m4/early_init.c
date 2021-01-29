/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <bootblock_common.h>
#include <device/pnp_def.h>
#include <device/pnp_ops.h>
#include <northbridge/intel/i945/i945.h>
#include <southbridge/intel/i82801gx/i82801gx.h>
#include <superio/winbond/common/winbond.h>
#include <superio/winbond/w83627ehg/w83627ehg.h>

#define DUMMY_DEV PNP_DEV(0x2e, 0)
#define GPIO_DEV PNP_DEV(0x2e, W83627EHG_GPIO_SUSLED_V)
#define GPIO_MIDI_DEV PNP_DEV(0x2e, W83627EHG_GPIO_GAME_MIDI_V)
#define ACPI_DEV PNP_DEV(0x2e, W83627EHG_ACPI)
#define HWM_DEV PNP_DEV(0x2e, W83627EHG_HWM)

void bootblock_mainboard_early_init(void)
{
	pnp_enter_conf_state(DUMMY_DEV);
	pnp_write_config(DUMMY_DEV, 0x24, 0x44);
	pnp_write_config(DUMMY_DEV, 0x29, 0x05);
	pnp_write_config(DUMMY_DEV, 0x2c, 0x82);
	pnp_write_config(DUMMY_DEV, 0x2d, 0x20);
	pnp_exit_conf_state(DUMMY_DEV);

	pnp_enter_conf_state(ACPI_DEV);
	pnp_set_logical_device(ACPI_DEV);
	pnp_write_config(ACPI_DEV, 0x30, 0x01);
	pnp_exit_conf_state(ACPI_DEV);

	// GPIO1, GPIO6, game port, MIDI
	pnp_enter_conf_state(GPIO_MIDI_DEV);
	pnp_set_logical_device(GPIO_MIDI_DEV);
	pnp_write_config(GPIO_MIDI_DEV, 0x30, 0x01);
	pnp_write_config(GPIO_MIDI_DEV, 0x60, 0x00);
	pnp_write_config(GPIO_MIDI_DEV, 0x61, 0x00);
	pnp_write_config(GPIO_MIDI_DEV, 0x62, 0x00);
	pnp_write_config(GPIO_MIDI_DEV, 0x63, 0x00);
	pnp_write_config(GPIO_MIDI_DEV, 0x70, 0x00);
	pnp_write_config(GPIO_MIDI_DEV, 0xf0, 0x5f);
	pnp_write_config(GPIO_MIDI_DEV, 0xf1, 0x5f);
	pnp_write_config(GPIO_MIDI_DEV, 0xf5, 0xff);
	pnp_write_config(GPIO_MIDI_DEV, 0xf6, 0xff);
	pnp_exit_conf_state(GPIO_MIDI_DEV);

	// GPIO2, GPIO3, GPIO4, GPIO5, SUSLED
	pnp_enter_conf_state(GPIO_DEV);
	pnp_set_logical_device(GPIO_DEV);
	pnp_write_config(GPIO_DEV, 0x30, 0x0e);
	pnp_write_config(GPIO_DEV, 0xe0, 0xdf);
	pnp_write_config(GPIO_DEV, 0xe2, 0x20);
	pnp_write_config(GPIO_DEV, 0xe4, 0xff);
	pnp_write_config(GPIO_DEV, 0xe5, 0xff);
	pnp_write_config(GPIO_DEV, 0xf0, 0x8c);
	pnp_write_config(GPIO_DEV, 0xf2, 0x00);
	pnp_write_config(GPIO_DEV, 0xf1, 0x8c);
	pnp_write_config(GPIO_DEV, 0xe1, 0);
	pnp_exit_conf_state(GPIO_DEV);

	// Enable HWM
	pnp_enter_conf_state(HWM_DEV);
	pnp_set_logical_device(HWM_DEV);
	pnp_set_iobase(HWM_DEV, PNP_IDX_IO0, 0x290);
	pnp_write_config(HWM_DEV, 0x30, 0x01);
	pnp_exit_conf_state(HWM_DEV);
}

void mainboard_late_rcba_config(void)
{
	/* Enable PCIe Root Port Clock Gate */
	RCBA32(CG) = 0x00000001;
}
