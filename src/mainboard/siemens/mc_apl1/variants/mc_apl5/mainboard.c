/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootstate.h>
#include <cf9_reset.h>
#include <device/pci_def.h>
#include <device/pci_ids.h>
#include <device/pci_ops.h>
#include <gpio.h>
#include <hwilib.h>
#include <intelblocks/lpc_lib.h>
#include <intelblocks/pcr.h>
#include <soc/pcr_ids.h>
#include <baseboard/variants.h>
#include <types.h>

#define TX_DWORD3_P0	0xc8c
#define TX_SWING_MASK	0x00ff0000

void variant_mainboard_final(void)
{
	struct device *dev = nullptr;

	/* PIR6 register mapping for PCIe root ports
	   INTA#->PIRQB#, INTB#->PIRQC#, INTC#->PIRQD#, INTD#-> PIRQA# */
	pcr_write16(PID_ITSS, 0x314c, 0x0321);

	/* Enable CLKRUN_EN for power gating LPC */
	lpc_enable_pci_clk_cntl();

	/*
	 * Enable LPC PCE (Power Control Enable) by setting IOSF-SB port 0xD2 offset 0x341D
	 * bit3 and bit0.
	 * Enable LPC CCE (Clock Control Enable) by setting IOSF-SB port 0xD2 offset 0x341C bit
	 * [3:0].
	 */
	pcr_or32(PID_LPC, PCR_LPC_PRC, (PCR_LPC_CCE_EN | PCR_LPC_PCE_EN));

	/*
	 * Correct the SATA transmit signal via the High Speed I/O Transmit
	 * Control Register 3 on SATA port 0.
	 * Bit [23:16] sets the output voltage swing for TX line.
	 * The value 0x5a sets the swing level to 0.7 V.
	 */
	pcr_rmw32(PID_MODPHY, TX_DWORD3_P0, ~TX_SWING_MASK, 0x5a << 16);

	/* Set Master Enable for on-board PCI device if allowed. */
	dev = dev_find_device(PCI_VID_SIEMENS, 0x403e, 0);
	if (dev) {
		if (CONFIG(PCI_ALLOW_BUS_MASTER_ANY_DEVICE))
			pci_or_config16(dev, PCI_COMMAND, PCI_COMMAND_MASTER);

		/* Disable clock outputs 0-3 (CLKOUT) for upstream XIO2001 PCIe to PCI
		   Bridge. */
		struct device *parent = dev->upstream->dev;
		if (parent && parent->device == PCI_DID_TI_XIO2001)
			pci_write_config8(parent, 0xd8, 0x0f);
	}

	/* Disable clock outputs 1-5 (CLKOUT) for another XIO2001 PCIe to PCI Bridge on this
	   mainboard. */
	dev = dev_find_device(PCI_VID_SIEMENS, 0x403f, 0);
	if (dev) {
		struct device *parent = dev->upstream->dev;
		if (parent && parent->device == PCI_DID_TI_XIO2001)
			pci_write_config8(parent, 0xd8, 0x3e);
	}

	/* Set Full Reset Bit in Reset Control Register (I/O port CF9h). When Bit 3 is set to 1
	   and then a warm reset is triggered the PCH will drive SLP_S3 active (low). SLP_S3 is
	   then used on the mainboard to generate the right reset timing. */
	outb(FULL_RST, RST_CNT);
}

static void finalize_boot(void *unused)
{
	/* Set coreboot ready LED. */
	gpio_output(CNV_RGI_DT, 1);
}

BOOT_STATE_INIT_ENTRY(BS_PAYLOAD_BOOT, BS_ON_ENTRY, finalize_boot, nullptr);
