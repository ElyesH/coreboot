/* SPDX-License-Identifier: GPL-2.0-only */

#include <amdblocks/acpimmio.h>
#include <amdblocks/espi.h>
#include <bootblock_common.h>
#include <baseboard/variants.h>
#include <console/console.h>
#include <delay.h>
#include <device/pci_ops.h>
#include <soc/pci_devs.h>
#include <timer.h>

#define FC350_PCIE_INIT_DELAY_US (20 * USECS_PER_MSEC)
struct stopwatch pcie_init_timeout_sw;

void mb_set_up_early_espi(void)
{
	/*
	 * We don't need to initialize all of the GPIOs that are done
	 * in bootblock_mainboard_early_init(), but we need to release
	 * the EC eSPI reset and do the rest of the configuration.
	 *
	 * This will not be present in the normal boot flow.
	 */
	bootblock_mainboard_early_init();
}

void bootblock_mainboard_early_init(void)
{
	uint32_t dword;
	size_t base_num_gpios, override_num_gpios;
	const struct soc_amd_gpio *base_gpios, *override_gpios;

	dword = pci_read_config32(SOC_LPC_DEV, 0x78);
	dword &= 0xFFFFF9F3;
	dword |= 0x200;
	pci_write_config32(SOC_LPC_DEV, 0x78, dword);
	pci_write_config32(SOC_LPC_DEV, 0x44, 0);
	pci_write_config32(SOC_LPC_DEV, 0x48, 0);

	if (CONFIG(VBOOT_STARTS_BEFORE_BOOTBLOCK))
		return;

	base_gpios = variant_early_gpio_table(&base_num_gpios);
	override_gpios = variant_early_override_gpio_table(&override_num_gpios);

	gpio_configure_pads_with_override(base_gpios, base_num_gpios,
			override_gpios, override_num_gpios);

	/* Set a timer to make sure there's enough delay for
	 * the Fibocom 350 PCIe init
	 */
	stopwatch_init_usecs_expire(&pcie_init_timeout_sw, FC350_PCIE_INIT_DELAY_US);
	printk(BIOS_DEBUG, "Bootblock configure eSPI\n");

	dword = pm_read32(0x90);
	dword |= 1 << 16;
	pm_write32(0x90, dword);

	dword = pm_read32(0x74);
	dword |= 3 << 10;
	pm_write32(0x74, dword);
}

void bootblock_mainboard_init(void)
{
	size_t base_num_gpios, override_num_gpios;
	const struct soc_amd_gpio *base_gpios, *override_gpios;
	int i = 0;

	/* Make sure that at least 20ms has elapsed since enabling WWAN power
	 * in bootblock_mainboard_early_init.
	 * This is only applicable if verstage is not in the PSP and the board
	 * is using the fibocom 350 WLAN card, so this typically will not be hit.
	 */
	if (!CONFIG(VBOOT_STARTS_BEFORE_BOOTBLOCK) && variant_has_pcie_wwan()) {
		while (!stopwatch_expired(&pcie_init_timeout_sw)) {
			mdelay(1);
			i++;
		};
		if (i)
			printk(BIOS_DEBUG, "Delayed %d ms for PCIe\n", i);
	}

	base_gpios = variant_bootblock_gpio_table(&base_num_gpios);
	override_gpios = variant_bootblock_override_gpio_table(&override_num_gpios);

	gpio_configure_pads_with_override(base_gpios, base_num_gpios, override_gpios,
					  override_num_gpios);

	/* FPMCU check needs to happen after EC initialization for FW_CONFIG bits */
	if (variant_has_fpmcu())
		variant_fpmcu_reset();
}
