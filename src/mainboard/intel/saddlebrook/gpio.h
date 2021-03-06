/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef MAINBOARD_GPIO_H
#define MAINBOARD_GPIO_H

#include <soc/gpe.h>
#include <soc/gpio.h>

/* EC in RW */
#define GPIO_EC_IN_RW		GPP_C6

/* BIOS Flash Write Protect */
#define GPIO_PCH_WP		GPP_C23

/* EC wake is LAN_WAKE# which is a special DeepSX wake pin */
#define GPE_EC_WAKE		GPE0_LAN_WAK

/* GPP_B16 is WLAN_WAKE. GPP_B group is routed to DW0 in the GPE0 block */
#define GPE_WLAN_WAKE		GPE0_DW0_16

/* Input device interrupt configuration */
#define TOUCHPAD_INT_L		GPP_B3_IRQ
#define TOUCHSCREEN_INT_L	GPP_E7_IRQ
#define MIC_INT_L		GPP_F10_IRQ

/* GPP_E16 is EC_SCI_L. GPP_E group is routed to DW2 in the GPE0 block */
#define EC_SCI_GPI		GPE0_DW2_16
#define EC_SMI_GPI		GPP_E15

#ifndef __ACPI__
/* Pad configuration in ramstage. */
static const struct pad_config gpio_table[] = {
/* EC_PCH_RCIN */	PAD_CFG_NF(GPP_A0, NONE, DEEP, NF1),
/* LPC_LAD_0 */		PAD_CFG_NF(GPP_A1, NONE, DEEP, NF1),
/* LPC_LAD_1 */		PAD_CFG_NF(GPP_A2, NONE, DEEP, NF1),
/* LPC_LAD_2 */		PAD_CFG_NF(GPP_A3, NONE, DEEP, NF1),
/* LPC_LAD_3 */		PAD_CFG_NF(GPP_A4, NONE, DEEP, NF1),
/* LPC_FRAME */		PAD_CFG_NF(GPP_A5, NONE, DEEP, NF1),
/* LPC_SERIRQ */	PAD_CFG_NF(GPP_A6, NONE, DEEP, NF1),
/* PIRQA# */		PAD_CFG_NF(GPP_A7, NONE, DEEP, NF1),
/* LPC_CLKRUN */	PAD_CFG_NF(GPP_A8, NONE, DEEP, NF1),
/* EC_LPC_CLK */	PAD_CFG_NF(GPP_A9, NONE, DEEP, NF1),
/* PCH_LPC_CLK */	PAD_CFG_NF(GPP_A10, NONE, DEEP, NF1),
/* EC_HID_INT */	PAD_CFG_NF(GPP_A11, NONE, DEEP, NF1),
/* ISH_KB_PROX_INT */	PAD_CFG_GPI(GPP_A12, NONE, DEEP),
/* PCH_SUSPWRACB */	PAD_CFG_NF(GPP_A13, NONE, DEEP, NF1),
/* PM_SUS_STAT */	PAD_CFG_NF(GPP_A14, NONE, DEEP, NF1),
/* PCH_SUSACK */	PAD_CFG_NF(GPP_A15, NONE, DEEP, NF1),
/* SD_1P8_SEL */
/* SD_PWR_EN */
/* ACCEL INTERRUPT */	PAD_CFG_NF(GPP_A18, NONE, DEEP, NF1),
/* ISH_GP1 */		/* GPP_A19 */
/* GYRO_DRDY */
/* FLIP_ACCEL_INT */
/* GYRO_INT */
/* ISH_GP5 */		/* GPP_A23 */

/* CORE_VID0 */		/* GPP_B0 */
/* CORE_VID1 */		/* GPP_B1 */
/* HSJ_MIC_DET */
/* TRACKPAD_INT */			PAD_CFG_GPO(GPP_B3, 1, DEEP),
/* BT_RF_KILL */	PAD_CFG_NF(GPP_B4, NONE, DEEP, NF1),
/* SRCCLKREQ0# */	/* GPP_B5 */
/* WIFI_CLK_REQ */
/* KEPLR_CLK_REQ */	PAD_CFG_NF(GPP_B7, NONE, DEEP, NF1),
/* SRCCLKREQ3# */	/* GPP_B8 */
/* SSD_CLK_REQ */
/* SRCCLKREQ5# */	/* GPP_B10 */
/* MPHY_EXT_PWR_GATE */	PAD_CFG_NF(GPP_B11, NONE, DEEP, NF1),
/* PM_SLP_S0 */		PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
/* PCH_PLT_RST */	PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
/* GPP_B_14_SPKR */	PAD_CFG_NF(GPP_B14, DN_20K, DEEP, NF1),
/* GSPI0_CS# */		/* GPP_B15 */
/* WLAN_PCIE_WAKE */
/* SSD_PCIE_WAKE */	PAD_CFG_GPO(GPP_B17, 1, DEEP),
/* GSPI0_MOSI */	/* GPP_B18 */
/* CCODEC_SPI_CS */
/* CODEC_SPI_CLK */
/* CODEC_SPI_MISO */
/* CODEC_SPI_MOSI */
/* SM1ALERT# */		PAD_CFG_NF(GPP_B23, DN_20K, DEEP, NF1),

/* SMB_CLK */		PAD_CFG_NF(GPP_C0, NONE, DEEP, NF1),
/* SMB_DATA */		PAD_CFG_NF(GPP_C1, NONE, DEEP, NF1),
/* SMBALERT# */		PAD_CFG_NF(GPP_C2, DN_20K, DEEP, NF1),
/* M2_WWAN_PWREN */	PAD_CFG_NF(GPP_C3, NONE, DEEP, NF1),
/* SML0DATA */		PAD_CFG_NF(GPP_C4, NONE, DEEP, NF1),
/* SML0ALERT# */	/* GPP_C5 */
/* EC_IN_RW */		PAD_CFG_NF(GPP_C6, NONE, DEEP, NF1),
/* USB_CTL */		PAD_CFG_NF(GPP_C7, NONE, DEEP, NF1),
/* UART0_RXD */		/* GPP_C8 */
/* UART0_TXD */		/* GPP_C9 */
/* NFC_RST* */
/* EN_PP3300_KEPLER */
/* PCH_MEM_CFG0 */
/* PCH_MEM_CFG1 */
/* PCH_MEM_CFG2 */
/* PCH_MEM_CFG3 */
/* I2C0_SDA */
/* I2C0_SCL */
/* I2C1_SDA */
/* I2C1_SCL */
/* GD_UART2_RXD */
/* GD_UART2_TXD */
/* TCH_PNL_PWREN */
/* SPI_WP_STATUS */

/* ITCH_SPI_CS */	/* GPP_D0 */
/* ITCH_SPI_CLK */	/* GPP_D1 */
/* ITCH_SPI_MISO_1 */	/* GPP_D2 */
/* ITCH_SPI_MISO_0 */	/* GPP_D3 */
/* CAM_FLASH_STROBE */
/* EN_PP3300_DX_EMMC */
/* EN_PP1800_DX_EMMC */
/* SH_I2C1_SDA */
/* SH_I2C1_SCL */
/* TBD */
/* USB_A0_ILIM_SEL */
/* USB_A1_ILIM_SEL */
/* EN_PP3300_DX_CAM */
/* EN_PP1800_DX_AUDIO */
/* ISH_UART0_TXD */	/* GPP_D14 */
/* ISH_UART0_RTS */	/* GPP_D15 */
/* ISH_UART0_CTS */	/* GPP_D16 */
/* DMIC_CLK_1 */
/* DMIC_DATA_1 */
/* DMIC_CLK_0 */
/* DMIC_DATA_0 */
/* ITCH_SPI_D2 */	/* GPP_D21 */
/* ITCH_SPI_D3 */	/* GPP_D22 */
/* I2S_MCLK */

/* SPI_TPM_IRQ */	PAD_CFG_NF(GPP_E0, NONE, DEEP, NF1),
/* SATAXPCIE1 */	/* GPP_E1 */
/* SSD_PEDET */
/* CPU_GP0 */
/* SSD_SATA_DEVSLP */	PAD_CFG_NF(GPP_E4, NONE, DEEP, NF1),
/* SATA_DEVSLP1 */	/* GPP_E5 */
/* SATA_DEVSLP2 */	/* GPP_E6 */
/* TCH_PNL_INTR* */
/* SATALED# */		PAD_CFG_NF(GPP_E8, NONE, DEEP, NF1),
/* USB2_OC_0 */		PAD_CFG_NF(GPP_E9, NONE, DEEP, NF1),
/* USB2_OC_1 */		PAD_CFG_NF(GPP_E10, NONE, DEEP, NF1),
/* USB2_OC_2 */		PAD_CFG_NF(GPP_E11, NONE, DEEP, NF1),
/* USB2_OC_3 */		PAD_CFG_NF(GPP_E12, NONE, DEEP, NF1),

/* I2S2_SCLK */
/* I2S2_SFRM */
/* I2S2_TXD */		PAD_CFG_GPO(GPP_F2, 0, DEEP),
/* I2S2_RXD */
/* I2C2_SDA */		/* GPP_F4 */
/* I2C2_SCL */		/* GPP_F5 */
/* I2C3_SDA */		/* GPP_F6 */
/* I2C3_SCL */		/* GPP_F7 */
/* I2C4_SDA */
/* I2C4_SDA */
/* AUDIO_IRQ */
/* I2C5_SCL */		/* GPP_F11 */
/* EMMC_CMD */		PAD_CFG_GPI(GPP_F12, NONE, DEEP),
/* EMMC_DATA0 */	PAD_CFG_NF(GPP_F13, NONE, DEEP, NF1),
/* EMMC_DATA1 */	PAD_CFG_NF(GPP_F14, NONE, DEEP, NF1),
/* EMMC_DATA2 */	PAD_CFG_NF(GPP_F15, NONE, DEEP, NF1),
/* EMMC_DATA3 */	PAD_CFG_NF(GPP_F16, NONE, DEEP, NF1),
/* EMMC_DATA4 */	PAD_CFG_GPO(GPP_F17, 1, DEEP),
/* EMMC_DATA5 */	PAD_CFG_GPO(GPP_F18, 1, DEEP),
/* EMMC_DATA6 */	PAD_CFG_NF(GPP_F19, NONE, DEEP, NF1),
/* EMMC_DATA7 */	PAD_CFG_NF(GPP_F20, NONE, DEEP, NF1),
/* EMMC_RCLK */		PAD_CFG_NF(GPP_F21, NONE, DEEP, NF1),
/* EMMC_CLK */		PAD_CFG_GPO(GPP_F22, 1, DEEP),
			/* GPP_F23 */

/* SD_CMD */		/* GPP_G0 */
/* SD_DATA0 */
/* SD_DATA1 */
/* SD_DATA2 */
/* SD_DATA3 */
/* SD_CD# */
/* SD_CLK */
/* SD_WP */
/* TBD */	PAD_CFG_GPO(GPP_G8, 1, DEEP),
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */	PAD_CFG_GPI(GPP_G23, NONE, DEEP),

/* SD_CMD */		/* GPP_H0 */
/* SD_DATA0 */
/* SD_DATA1 */
/* SD_DATA2 */
/* SD_DATA3 */
/* SD_CD# */
/* SD_CLK */
/* SD_WP */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */
/* TBD */	PAD_CFG_NF(GPP_H19, NONE, DEEP, NF1),
/* TBD */	PAD_CFG_NF(GPP_H20, NONE, DEEP, NF1),
/* TBD */	PAD_CFG_NF(GPP_H21, NONE, DEEP, NF1),
/* TBD */	PAD_CFG_NF(GPP_H22, NONE, DEEP, NF1),
/* TBD */

/* SD_CMD */		PAD_CFG_NF(GPP_I0, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I1, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I2, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I3, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I4, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I5, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I6, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I7, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I8, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I9, NONE, DEEP, NF1),
/* SD_CMD */		PAD_CFG_NF(GPP_I10, NONE, DEEP, NF1),

/* PCH_BATLOW */	PAD_CFG_NF(GPD0, NONE, DEEP, NF1),
/* EC_PCH_ACPRESENT */	PAD_CFG_NF(GPD1, NONE, DEEP, NF1),
/* EC_PCH_WAKE */	PAD_CFG_NF(GPD2, NONE, DEEP, NF1),
/* EC_PCH_PWRBTN */	PAD_CFG_NF(GPD3, NONE, DEEP, NF1),
/* PM_SLP_S3# */	PAD_CFG_NF(GPD4, NONE, DEEP, NF1),
/* PM_SLP_S4# */	PAD_CFG_NF(GPD5, NONE, DEEP, NF1),
/* PM_SLP_SA# */	PAD_CFG_NF(GPD6, NONE, DEEP, NF1),
			/* GPD7 */
/* PM_SUSCLK */		PAD_CFG_NF(GPD8, NONE, DEEP, NF1),
/* PCH_SLP_WLAN# */	PAD_CFG_NF(GPD9, NONE, DEEP, NF1),
/* PM_SLP_S5# */	PAD_CFG_NF(GPD10, NONE, DEEP, NF1),
/* LANPHYC */		PAD_CFG_NF(GPD11, NONE, DEEP, NF1),
};

/* Early pad configuration in romstage. */
static const struct pad_config early_gpio_table[] = {
/* UART0_CTS# */	PAD_CFG_GPO(GPP_C11, 1, DEEP), /* EN_PP3300_KEPLER */
};

#endif

#endif
