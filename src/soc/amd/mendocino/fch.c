/* SPDX-License-Identifier: GPL-2.0-only */

#include <amdblocks/acpi.h>
#include <amdblocks/acpimmio.h>
#include <amdblocks/amd_pci_util.h>
#include <amdblocks/gpio.h>
#include <amdblocks/pci_clk_req.h>
#include <amdblocks/reset.h>
#include <amdblocks/smi.h>
#include <assert.h>
#include <bootstate.h>
#include <cpu/x86/smm.h>
#include <amdblocks/i2c.h>
#include <soc/amd_pci_int_defs.h>
#include <soc/iomap.h>
#include <soc/i2c.h>
#include <soc/smi.h>
#include <soc/southbridge.h>
#include "chip.h"

/*
 * Table of APIC register index and associated IRQ name. Using IDX_XXX_NAME
 * provides a visible association with the index, therefore helping
 * maintainability of table. If a new index/name is defined in
 * amd_pci_int_defs.h, just add the pair at the end of this table.
 * Order is not important.
 */
static const struct irq_idx_name irq_association[] = {
	{ PIRQ_A,	"INTA#" },
	{ PIRQ_B,	"INTB#" },
	{ PIRQ_C,	"INTC#" },
	{ PIRQ_D,	"INTD#" },
	{ PIRQ_E,	"INTE#" },
	{ PIRQ_F,	"INTF#/GENINT2" },
	{ PIRQ_G,	"INTG#" },
	{ PIRQ_H,	"INTH#" },
	{ PIRQ_MISC,	"Misc" },
	{ PIRQ_MISC0,	"Misc0" },
	{ PIRQ_HPET_L,	"HPET_L" },
	{ PIRQ_HPET_H,	"HPET_H" },
	{ PIRQ_SIRQA,	"Ser IRQ INTA" },
	{ PIRQ_SIRQB,	"Ser IRQ INTB" },
	{ PIRQ_SIRQC,	"Ser IRQ INTC" },
	{ PIRQ_SIRQD,	"Ser IRQ INTD" },
	{ PIRQ_SCI,	"SCI" },
	{ PIRQ_SMBUS,	"SMBUS" },
	{ PIRQ_ASF,	"ASF" },
	{ PIRQ_PMON,	"PerMon" },
	{ PIRQ_SD,	"SD" },
	{ PIRQ_SDIO,	"SDIO" },
	{ PIRQ_CIR,	"CIR" },
	{ PIRQ_GPIOA,	"GPIOa" },
	{ PIRQ_GPIOB,	"GPIOb" },
	{ PIRQ_GPIOC,	"GPIOc" },
	{ PIRQ_GPP0,	"GPP0" },
	{ PIRQ_GPP1,	"GPP1" },
	{ PIRQ_GPP2,	"GPP2" },
	{ PIRQ_GPP3,	"GPP3" },
	{ PIRQ_GPIO,	"GPIO" },
	{ PIRQ_I2C0,	"I2C0" },
	{ PIRQ_I2C1,	"I2C1" },
	{ PIRQ_I2C2,	"I2C2" },
	{ PIRQ_I2C3,	"I2C3" },
	{ PIRQ_UART0,	"UART0" },
	{ PIRQ_UART1,	"UART1" },
	{ PIRQ_I2C4,	"I2C4" },
	{ PIRQ_UART4,	"UART4" },
	{ PIRQ_UART2,	"UART2" },
	{ PIRQ_UART3,	"UART3" },
};

const struct irq_idx_name *sb_get_apic_reg_association(size_t *size)
{
	*size = ARRAY_SIZE(irq_association);
	return irq_association;
}

static void fch_clk_output_48Mhz(void)
{
	uint32_t ctrl = misc_read32(MISC_CLK_CNTL0);
	/* Enable BP_X48M0 Clock Output */
	ctrl |= BP_X48M0_OUTPUT_EN;
	/* Disable clock output in S0i3 */
	ctrl |= BP_X48M0_S0I3_DIS;
	misc_write32(MISC_CLK_CNTL0, ctrl);
}

static void fch_init_acpi_ports(void)
{
	u32 reg;

	/* We use some of these ports in SMM regardless of whether or not
	 * ACPI tables are generated. Enable these ports indiscriminately.
	 */

	pm_write16(PM_EVT_BLK, ACPI_PM_EVT_BLK);
	pm_write16(PM1_CNT_BLK, ACPI_PM1_CNT_BLK);
	pm_write16(PM_TMR_BLK, ACPI_PM_TMR_BLK);
	pm_write16(PM_GPE0_BLK, ACPI_GPE0_BLK);

	if (CONFIG(HAVE_SMI_HANDLER)) {
		/* APMC - SMI Command Port */
		pm_write16(PM_ACPI_SMI_CMD, APM_CNT);
		configure_smi(SMITYPE_SMI_CMD_PORT, SMI_MODE_SMI);

		/* SMI on SlpTyp requires sending SMI before completion
		   response of the I/O write. */
		reg = pm_read32(PM_PCI_CTRL);
		reg |= FORCE_SLPSTATE_RETRY;
		pm_write32(PM_PCI_CTRL, reg);

		/* Disable SlpTyp feature */
		reg = pm_read8(PM_RST_CTRL1);
		reg &= ~SLPTYPE_CONTROL_EN;
		pm_write8(PM_RST_CTRL1, reg);

		configure_smi(SMITYPE_SLP_TYP, SMI_MODE_SMI);
	} else {
		pm_write16(PM_ACPI_SMI_CMD, 0);
	}

	/* Decode ACPI registers and enable standard features */
	pm_write8(PM_ACPI_CONF, PM_ACPI_DECODE_STD |
				PM_ACPI_GLOBAL_EN |
				PM_ACPI_RTC_EN_EN |
				PM_ACPI_TIMER_EN_EN);
}

/* configure the general purpose PCIe clock outputs according to the devicetree settings */
static void gpp_clk_setup(void)
{
	struct soc_amd_mendocino_config *cfg = config_of_soc();
	gpp_clk_setup_common(&cfg->gpp_clk_config[0], ARRAY_SIZE(cfg->gpp_clk_config));
}

static void cgpll_clock_gate_init(void)
{
	uint32_t t;

	t = misc_read32(MISC_CLKGATEDCNTL);
	t |= ALINKCLK_GATEOFFEN;
	t |= BLINKCLK_GATEOFFEN;
	t |= XTAL_PAD_S0I3_TURNOFF_EN;
	t |= XTAL_PAD_S3_TURNOFF_EN;
	t |= XTAL_PAD_S5_TURNOFF_EN;
	misc_write32(MISC_CLKGATEDCNTL, t);

	t = misc_read32(MISC_CGPLL_CONFIGURATION0);
	t |= USB_PHY_CMCLK_S3_DIS;
	t |= USB_PHY_CMCLK_S0I3_DIS;
	t |= USB_PHY_CMCLK_S5_DIS;
	misc_write32(MISC_CGPLL_CONFIGURATION0, t);

	t = pm_read32(PM_ISACONTROL);
	t |= ABCLKGATEEN;
	pm_write32(PM_ISACONTROL, t);
}

void fch_init(void *chip_info)
{
	set_resets_to_cold();
	i2c_soc_init();
	fch_init_acpi_ports();

	acpi_pm_gpe_add_events_print_events();
	gpio_add_events();

	gpp_clk_setup();
	fch_clk_output_48Mhz();
	cgpll_clock_gate_init();
}

void fch_final(void *chip_info)
{
}

static void set_pci_irqs(void *unused)
{
	/* Write PCI_INTR regs 0xC00/0xC01 */
	write_pci_int_table();

	/* pirq_data is consumed by `write_pci_cfg_irqs` */
	populate_pirq_data();

	/* Write IRQs for all devicetree enabled devices */
	write_pci_cfg_irqs();
}

/*
 * Hook this function into the PCI state machine
 * on entry into BS_DEV_ENABLE.
 */
BOOT_STATE_INIT_ENTRY(BS_DEV_ENABLE, BS_ON_ENTRY, set_pci_irqs, nullptr);
