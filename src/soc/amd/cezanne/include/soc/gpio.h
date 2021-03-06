/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef AMD_CEZANNE_GPIO_H
#define AMD_CEZANNE_GPIO_H

#define GPIO_DEVICE_NAME	"AMD0030"
#define GPIO_DEVICE_DESC	"GPIO Controller"

#ifndef __ACPI__
#include <soc/iomap.h>
#include <amdblocks/gpio_banks.h>
#endif /* !__ACPI__ */

#include <amdblocks/gpio_defs.h>

/* The following sections describe only the GPIOs defined for this SOC */

#define SOC_GPIO_TOTAL_PINS		272

/* Bank 0: GPIO_0 - GPIO_63 */
#define GPIO_0				0
#define GPIO_1				1
#define GPIO_2				2
#define GPIO_3				3
#define GPIO_4				4
#define GPIO_5				5
#define GPIO_6				6
#define GPIO_7				7
#define GPIO_8				8
#define GPIO_9				9
#define GPIO_10				10
#define GPIO_11				11
#define GPIO_12				12
#define GPIO_16				16
#define GPIO_17				17
#define GPIO_18				18
#define GPIO_19				19
#define GPIO_20				20
#define GPIO_21				21
#define GPIO_22				22
#define GPIO_23				23
#define GPIO_24				24
#define GPIO_26				26
#define GPIO_27				27
#define GPIO_29				29
#define GPIO_30				30
#define GPIO_31				31
#define GPIO_32				32
#define GPIO_40				40
#define GPIO_42				42

/* Bank 1: GPIO_64 - GPIO_127 */
#define GPIO_67				67
#define GPIO_68				68
#define GPIO_69				69
#define GPIO_70				70
#define GPIO_74				74
#define GPIO_75				75
#define GPIO_76				76
#define GPIO_84				84
#define GPIO_85				85
#define GPIO_86				86
#define GPIO_87				87
#define GPIO_88				88
#define GPIO_89				89
#define GPIO_90				90
#define GPIO_91				91
#define GPIO_92				92
#define GPIO_104			104
#define GPIO_105			105
#define GPIO_106			106
#define GPIO_107			107
#define GPIO_108			108
#define GPIO_109			109
#define GPIO_113			113
#define GPIO_114			114
#define GPIO_115			115
#define GPIO_116			116
#define GPIO_120			120
#define GPIO_121			121

/* Bank 2: GPIO_128 - GPIO_191 */
#define GPIO_129			129
#define GPIO_130			130
#define GPIO_131			131
#define GPIO_132			132
#define GPIO_140			140
#define GPIO_141			141
#define GPIO_142			142
#define GPIO_143			143
#define GPIO_144			144
#define GPIO_145			145
#define GPIO_146			146
#define GPIO_147			147
#define GPIO_148			148

/* remote GPIO bank: GPIO_256 - GPIO_271 */
#define GPIO_256			256
#define GPIO_257			257
#define GPIO_258			258
#define GPIO_259			259
#define GPIO_260			260
#define GPIO_261			261
#define GPIO_262			262
#define GPIO_263			263
#define GPIO_264			264
#define GPIO_265			265
#define GPIO_266			266
#define GPIO_267			267
#define GPIO_268			268
#define GPIO_269			269
#define GPIO_270			270
#define GPIO_271			271

/* IOMUX function names and values */
#define GPIO_0_IOMUX_PWR_BTN_L 0
#define GPIO_0_IOMUX_GPIOxx 1
#define GPIO_1_IOMUX_SYS_RESET_L 0
#define GPIO_1_IOMUX_GPIOxx 1
#define GPIO_2_IOMUX_WAKE_L 0
#define GPIO_2_IOMUX_GPIOxx 1
#define GPIO_3_IOMUX_GPIOxx 0
#define GPIO_4_IOMUX_GPIOxx 0
#define GPIO_5_IOMUX_GPIOxx 0
#define GPIO_5_IOMUX_DEVSLP0 1
#define GPIO_5_IOMUX_DEVSLP2 2
#define GPIO_6_IOMUX_GPIOxx 0
#define GPIO_6_IOMUX_DEVSLP1 1
#define GPIO_6_IOMUX_DEVSLP3 2
#define GPIO_7_IOMUX_GPIOxx 0
#define GPIO_7_IOMUX_ACP_I2S_SDIN 1
#define GPIO_8_IOMUX_GPIOxx 0
#define GPIO_8_IOMUX_ACP_I2S_LRCLK 1
#define GPIO_9_IOMUX_GPIOxx 0
/* GPIO 9 IOMUX == 1 is reserved */
#define GPIO_9_IOMUX_MDIO1_SCL 2
#define GPIO_10_IOMUX_GPIOxx 0
#define GPIO_10_IOMUX_S0A3 1
/* GPIO 10 IOMUX == 2 is reserved */
#define GPIO_10_IOMUX_MDIO0_SCL 3
#define GPIO_11_IOMUX_GPIOxx 0
#define GPIO_11_IOMUX_BLINK 1
#define GPIO_12_IOMUX_LLB_L 0
#define GPIO_12_IOMUX_GPIOxx 1
#define GPIO_16_IOMUX_USB_OC0_L 0
#define GPIO_16_IOMUX_GPIOxx 1
#define GPIO_17_IOMUX_USB_OC1_L 0
#define GPIO_17_IOMUX_GPIOxx 1
#define GPIO_18_IOMUX_USB_OC2_L 0
#define GPIO_18_IOMUX_GPIOxx 1
#define GPIO_19_IOMUX_SCL1 0
#define GPIO_19_IOMUX_I2C3_SCL 1
#define GPIO_19_IOMUX_GPIOxx 2
#define GPIO_20_IOMUX_SDA1 0
#define GPIO_20_IOMUX_I2C3_SDA 1
#define GPIO_20_IOMUX_GPIOxx 2
#define GPIO_21_IOMUX_LPC_PD_L 0
#define GPIO_21_IOMUX_EMMC_CMD 1
#define GPIO_21_IOMUX_GPIOxx 2
#define GPIO_22_IOMUX_LPC_PME_L 0
#define GPIO_22_IOMUX_EMMC_PWR_CTRL 1
#define GPIO_22_IOMUX_GPIOxx 2
#define GPIO_23_IOMUX_AC_PRES 0
/* GPIO 23 IOMUX == 1 is reserved */
#define GPIO_23_IOMUX_MDIO1_SDA 2
#define GPIO_23_IOMUX_GPIOxx 3
#define GPIO_24_IOMUX_USB_OC3_L 0
#define GPIO_24_IOMUX_GPIOxx 1
#define GPIO_26_IOMUX_PCIE_RST_L 0
#define GPIO_26_IOMUX_GPIOxx 1
#define GPIO_27_IOMUX_GPIOxx 0
#define GPIO_27_IOMUX_PCIE_RST1_L 1
#define GPIO_29_IOMUX_SPI_TPM_CS_L 0
#define GPIO_29_IOMUX_GPIOxx 1
#define GPIO_30_IOMUX_SPI_CS2_L 0
#define GPIO_30_IOMUX_ESPI_CS_L 1
#define GPIO_30_IOMUX_GPIOxx 2
#define GPIO_31_IOMUX_SPI_CS3_L 0
#define GPIO_31_IOMUX_ESPI_CS_L 1
#define GPIO_31_IOMUX_GPIOxx 2
#define GPIO_32_IOMUX_LPC_RST_L 0
#define GPIO_32_IOMUX_SD_WP_L 1
#define GPIO_32_IOMUX_GPIOxx 2
#define GPIO_40_IOMUX_GPIOxx 0
/* GPIO 40 IOMUX == 1 is reserved */
#define GPIO_40_IOMUX_MDIO0_SDA 2
#define GPIO_42_IOMUX_GPIOxx 0
#define GPIO_67_IOMUX_SPI_ROM_REQ 0
#define GPIO_67_IOMUX_GPIOxx 1
#define GPIO_68_IOMUX_GPIOxx 0
#define GPIO_68_IOMUX_EMMC_CD 1
#define GPIO_69_IOMUX_GPIOxx 0
#define GPIO_70_IOMUX_GPIOxx 0
#define GPIO_70_IOMUX_EMMC_CLK 1
#define GPIO_70_IOMUX_SD_CLK 2
#define GPIO_74_IOMUX_LPCCLK0 0
#define GPIO_74_IOMUX_EMMC_DATA4 1
#define GPIO_74_IOMUX_GPIOxx 2
#define GPIO_75_IOMUX_LPCCLK1 0
#define GPIO_75_IOMUX_EMMC_DATA6 1
#define GPIO_75_IOMUX_GPIOxx 2
#define GPIO_76_IOMUX_SPI_ROM_GNT 0
#define GPIO_76_IOMUX_GPIOxx 1
#define GPIO_84_IOMUX_FANIN0 0
#define GPIO_84_IOMUX_GPIOxx 1
#define GPIO_85_IOMUX_FANOUT0 0
#define GPIO_85_IOMUX_GPIOxx 1
#define GPIO_86_IOMUX_LPC_SMI_L 0
#define GPIO_86_IOMUX_GPIOxx 1
#define GPIO_86_IOMUX_SPI_CLK 2
#define GPIO_87_IOMUX_SERIRQ 0
#define GPIO_87_IOMUX_EMMC_DATA7 1
#define GPIO_87_IOMUX_GPIOxx 2
#define GPIO_88_IOMUX_LPC_CLKRUN_L 0
#define GPIO_88_IOMUX_EMMC_DATA5 1
#define GPIO_88_IOMUX_GPIOxx 2
#define GPIO_89_IOMUX_GENINT1_L 0
#define GPIO_89_IOMUX_PSP_INTR0 1
#define GPIO_89_IOMUX_GPIOxx 2
#define GPIO_90_IOMUX_GENINT2_L 0
#define GPIO_90_IOMUX_PSP_INTR1 1
#define GPIO_90_IOMUX_GPIOxx 2
#define GPIO_91_IOMUX_SPKR 0
#define GPIO_91_IOMUX_GPIOxx 1
#define GPIO_92_IOMUX_CLK_REQ0_L 0
#define GPIO_92_IOMUX_SATA_IS0_L 1
#define GPIO_92_IOMUX_SATA_ZP0_L 2
#define GPIO_92_IOMUX_GPIOxx 3
#define GPIO_104_IOMUX_LAD0 0
#define GPIO_104_IOMUX_SPI2_DO_ESPI2_D0 1
#define GPIO_104_IOMUX_SD0_DATA0 2
#define GPIO_104_IOMUX_GPIOxx 3
#define GPIO_105_IOMUX_LAD1 0
#define GPIO_105_IOMUX_SPI2_DI_ESPI2_D1 1
#define GPIO_105_IOMUX_SD0_DATA1 2
#define GPIO_105_IOMUX_GPIOxx 3
#define GPIO_106_IOMUX_LAD2 0
#define GPIO_106_IOMUX_EMMC_SPI2_WP_L_ESPI2_D2 1
#define GPIO_106_IOMUX_EMMC_SD0_DATA2 2
#define GPIO_106_IOMUX_GPIOxx 3
#define GPIO_107_IOMUX_LAD3 0
#define GPIO_107_IOMUX_SPI2_HOLD_L_ESPI2_D3 1
#define GPIO_107_IOMUX_SD0_DATA3 2
#define GPIO_107_IOMUX_GPIOxx 3
#define GPIO_108_IOMUX_LDRQ0_L 0
#define GPIO_108_IOMUX_ESPI_ALERT_D1 1
#define GPIO_108_IOMUX_GPIOxx 2
#define GPIO_109_IOMUX_LFRAME_L 0
#define GPIO_109_IOMUX_EMMC_DS 1
#define GPIO_109_IOMUX_GPIOxx 2
#define GPIO_113_IOMUX_SCL0 0
#define GPIO_113_IOMUX_I2C2_SCL 1
#define GPIO_113_IOMUX_GPIOxx 2
#define GPIO_114_IOMUX_SDA0 0
#define GPIO_114_IOMUX_I2C2_SDA 1
#define GPIO_114_IOMUX_GPIOxx 2
#define GPIO_115_IOMUX_CLK_REQ1_L 0
#define GPIO_115_IOMUX_GPIOxx 1
#define GPIO_116_IOMUX_CLK_REQ2_L 0
#define GPIO_116_IOMUX_GPIOxx 1
#define GPIO_120_IOMUX_CLK_REQ5_L 0
#define GPIO_120_IOMUX_GPIOxx 1
#define GPIO_121_IOMUX_CLK_REQ6_L 0
#define GPIO_121_IOMUX_GPIOxx 1
#define GPIO_129_IOMUX_KBRST_L 0
/* GPIO 129 IOMUX == 1 is reserved */
#define GPIO_129_IOMUX_GPIOxx 2
#define GPIO_130_IOMUX_SATA_ACT_L 0
#define GPIO_130_IOMUX_GPIOxx 1
#define GPIO_131_IOMUX_CLK_REQ3_L 0
#define GPIO_131_IOMUX_SATA_IS1_L 1
#define GPIO_131_IOMUX_SATA_ZP1_L 2
#define GPIO_131_IOMUX_GPIOxx 3
#define GPIO_132_IOMUX_CLK_REQ4_L 0
#define GPIO_132_IOMUX_OSCIN 1
#define GPIO_132_IOMUX_GPIOxx 2
#define GPIO_140_IOMUX_GPIOxx 0
#define GPIO_140_IOMUX_UART0_CTS_L 1
#define GPIO_140_IOMUX_UART1_TXD 2
#define GPIO_140_IOMUX_SD0_DATA1 3
#define GPIO_141_IOMUX_GPIOxx 0
#define GPIO_141_IOMUX_UART0_RXD 1
#define GPIO_141_IOMUX_SD0_DATA3 2
#define GPIO_142_IOMUX_GPIOxx 0
#define GPIO_142_IOMUX_UART0_RTS_L 1
#define GPIO_142_IOMUX_UART1_RXD 2
#define GPIO_142_IOMUX_SD0_DATA0 3
#define GPIO_143_IOMUX_GPIOxx 0
#define GPIO_143_IOMUX_UART0_TXD 1
#define GPIO_143_IOMUX_SD0_DATA2 2
#define GPIO_144_IOMUX_GPIOxx 0
/* GPIO 144 IOMUX == 1 is reserved */
#define GPIO_144_IOMUX_UART0_INTR 2
#define GPIO_145_IOMUX_I2C0_SCL 0
#define GPIO_145_IOMUX_GPIOxx 1
#define GPIO_146_IOMUX_I2C0_SDA 0
#define GPIO_146_IOMUX_GPIOxx 1
#define GPIO_147_IOMUX_I2C1_SCL 0
#define GPIO_147_IOMUX_GPIOxx 1
#define GPIO_148_IOMUX_I2C1_SDA 0
#define GPIO_148_IOMUX_GPIOxx 1

/* Remote GPIOs */
#define GPIO_256_IOMUX_GPIOxx 1
#define GPIO_257_IOMUX_GPIOxx 1
#define GPIO_258_IOMUX_GPIOxx 1
#define GPIO_259_IOMUX_GPIOxx 1
#define GPIO_260_IOMUX_GPIOxx 1
#define GPIO_261_IOMUX_GPIOxx 1
#define GPIO_262_IOMUX_GPIOxx 1
#define GPIO_263_IOMUX_GPIOxx 1
#define GPIO_264_IOMUX_GPIOxx 1
#define GPIO_265_IOMUX_GPIOxx 1
#define GPIO_266_IOMUX_GPIOxx 1
#define GPIO_267_IOMUX_GPIOxx 1
#define GPIO_268_IOMUX_GPIOxx 1
#define GPIO_269_IOMUX_GPIOxx 1
#define GPIO_270_IOMUX_GPIOxx 1
#define GPIO_271_IOMUX_GPIOxx 1

#endif /* AMD_CEZANNE_GPIO_H */
