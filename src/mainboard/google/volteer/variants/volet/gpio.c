/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <commonlib/helpers.h>

/* Pad configuration in ramstage */
static const struct pad_config override_gpio_table[] = {
	/* A7  : I2S2_SCLK ==> EN_PP3300_TRACKPAD */
	PAD_CFG_GPO(GPP_A7, 1, DEEP),
	/* A8  : I2S2_SFRM ==> EN_PP3300_TOUCHSCREEN */
	PAD_CFG_GPO(GPP_A8, 1, DEEP),
	/* A10 : I2S2_RXD ==> EN_SPKR_PA */
	PAD_CFG_GPO(GPP_A10, 1, DEEP),
	/* A13 : PMC_I2C_SCL ==> BT_DISABLE_L */
	PAD_CFG_GPO(GPP_A13, 1, DEEP),
	/* A16 : USB_OC3# ==> USB_C0_OC_ODL */
	PAD_CFG_NF(GPP_A16, NONE, DEEP, NF1),
	/* A18 : DDSP_HPDB ==> HDMI_HPD */
	PAD_CFG_NF(GPP_A18, NONE, DEEP, NF1),
	/* A22 : DDPC_CTRLDATA ==> EN_HDMI_PWR */
	PAD_CFG_GPO(GPP_A22, 1, DEEP),
	/* A23 : I2S1_SCLK ==> I2S1_SPKR_SCLK */
	PAD_CFG_NF(GPP_A23, NONE, DEEP, NF1),

	/* B2  : VRALERT# ==> EN_PP3300_SSD */
	PAD_CFG_GPO(GPP_B2, 1, PLTRST),
	/* B3  : CPU_GP2 ==> PEN_DET_ODL */
	PAD_CFG_GPI_GPIO_DRIVER(GPP_B3, NONE, PLTRST),
	/* B5  : ISH_I2C0_CVF_SDA */
	PAD_CFG_NF(GPP_B5, NONE, DEEP, NF1),
	/* B6  : ISH_I2C0_CVF_SCL */
	PAD_CFG_NF(GPP_B6, NONE, DEEP, NF1),
	/* B7  : ISH_12C1_SDA ==> ISH_I2C0_SENSOR_SDA */
	PAD_CFG_NF(GPP_B7, NONE, DEEP, NF1),
	/* B8  : ISH_I2C1_SCL ==> ISH_I2C0_SENSOR_SCL */
	PAD_CFG_NF(GPP_B8, NONE, DEEP, NF1),
	/* B9  : I2C5_SDA ==> PCH_I2C5_TRACKPAD_SDA */
	PAD_CFG_NF(GPP_B9, NONE, DEEP, NF1),
	/* B10 : I2C5_SCL ==> PCH_I2C5_TRACKPAD_SCL */
	PAD_CFG_NF(GPP_B10, NONE, DEEP, NF1),
	/* B18 : GSPI0_MOSI ==> PCH_GSPI0_H1_TPM_MOSI_STRAP */
	PAD_CFG_NF(GPP_B18, DN_20K, DEEP, NF1),
	/* B19 : GSPI1_CS0# ==> PCH_GSPI1_FPMCU_CS_L */
	PAD_CFG_NF(GPP_B19, NONE, DEEP, NF1),
	/* B20 : GSPI1_CLK ==> PCH_GSPI1_FPMCU_CLK */
	PAD_CFG_NF(GPP_B20, NONE, DEEP, NF1),
	/* B21 : GSPI1_MISO ==> PCH_GSPI1_FPMCU_MISO */
	PAD_CFG_NF(GPP_B21, NONE, DEEP, NF1),
	/* B23 : SML1ALERT# ==> GPP_B23_STRAP # */
	PAD_NC(GPP_B23, DN_20K),

	/* C0  : SMBCLK ==> EN_PP3300_WLAN */
	PAD_CFG_GPO(GPP_C0, 1, DEEP),
	/* C2  : SMBALERT# ==> GPP_C2_STRAP */
	PAD_NC(GPP_C2, DN_20K),
	/* C3  : SML0CLK ==> USB4_SMB_SCL */
	PAD_CFG_NF(GPP_C3, NONE, DEEP, NF1),
	/* C4  : SML0DATA ==> USB4_SMB_SDA */
	PAD_CFG_NF(GPP_C4, NONE, DEEP, NF1),
	/* C5  : SML0ALERT# ==> GPP_C5_BOOT_STRAP0 */
	PAD_NC(GPP_C5, DN_20K),
	/* C7  : SML1DATA ==> EN_PP5000_PEN */
	PAD_CFG_GPO(GPP_C7, 1, DEEP),
	/* C10 : UART0_RTS# ==> USI_RST_L */
	PAD_CFG_GPO(GPP_C10, 1, DEEP),
	/* C13 : UART1_TXD ==> EN_PP5000_TRACKPAD */
	PAD_CFG_GPO(GPP_C13, 1, DEEP),
	/* C16 : I2C0_SDA ==> PCH_I2C0_1V8_AUDIO_SDA */
	PAD_CFG_NF(GPP_C16, NONE, DEEP, NF1),
	/* C17 : I2C0_SCL ==> PCH_I2C0_1V8_AUDIO_SCL */
	PAD_CFG_NF(GPP_C17, NONE, DEEP, NF1),
	/* C18 : I2C1_SDA ==> PCH_I2C1_TOUCH_USI_SDA */
	PAD_CFG_NF(GPP_C18, NONE, DEEP, NF1),
	/* C19 : I2C1_SCL ==> PCH_I2C1_TOUCH_USI_SCL */
	PAD_CFG_NF(GPP_C19, NONE, DEEP, NF1),
	/* C20 : UART2_RXD ==> FPMCU_INT_L */
	PAD_CFG_GPI_INT(GPP_C20, NONE, PLTRST, LEVEL),
	/* C22 : UART2_RTS# ==> PCH_FPMCU_BOOT0 */
	PAD_CFG_GPO(GPP_C22, 0, DEEP),

	/* D0  : ISH_GP0 ==> ISH_IMU_INT_L */
	PAD_CFG_GPI(GPP_D0, NONE, DEEP),
	/* D1  : ISH_GP1 ==> ISH_ACCEL_INT_L */
	PAD_CFG_GPI(GPP_D1, NONE, DEEP),
	/* D2  : ISH_GP2 ==> ISH_LID_OPEN */
	PAD_CFG_GPI(GPP_D2, NONE, DEEP),
	/* D3  : ISH_GP3 ==> ISH_ALS_RGB_INT_L */
	PAD_CFG_GPI(GPP_D3, NONE, DEEP),
	/* D4  : IMGCLKOUT0 ==> FCAM_RST_L */
	PAD_CFG_GPO(GPP_D4, 0, PLTRST),
	/* D6  : SRCCLKREQ1# ==> WLAN_CLKREQ_ODL */
	PAD_CFG_NF(GPP_D6, NONE, DEEP, NF1),
	/* D7  : SRCCLKREQ2# ==> WWAN_CLKREQ_ODL */
	PAD_CFG_NF(GPP_D7, NONE, DEEP, NF1),
	/* D8  : SRCCLKREQ3# ==> SD_CLKREQ_ODL */
	PAD_CFG_NF(GPP_D8, NONE, DEEP, NF1),
	/* D9  : ISH_SPI_CS# ==> PCH_GSPI2_CVF_CS_L */
	PAD_CFG_NF(GPP_D9, NONE, DEEP, NF7),
	/* D10 : ISH_SPI_CLK ==> PCH_GSPI2_CVF_CLK_STRAP */
	PAD_CFG_NF(GPP_D10, DN_20K, DEEP, NF7),
	/* D11 : ISH_SPI_MISO ==> PCH_GSPI2_CVF_MISO */
	PAD_CFG_NF(GPP_D11, NONE, DEEP, NF7),
	/* D12 : ISH_SPI_MOSI ==> PCH_GSPI2_CVF_MISO_STRAP */
	PAD_CFG_NF(GPP_D12, DN_20K, DEEP, NF7),
	/* D13 : ISH_UART0_RXD ==> UART_ISH_RX_DEBUG_TX */
	PAD_CFG_NF(GPP_D13, NONE, DEEP, NF1),
	/* D14 : ISH_UART0_TXD ==> UART_ISH_TX_DEBUG_RX */
	PAD_CFG_NF(GPP_D14, NONE, DEEP, NF1),
	/* D16 : ISH_UART0_CTS# ==> EN_PP3300_SD */
	PAD_CFG_GPO(GPP_D16, 1, DEEP),
	/* D17 : ISH_GP4 ==> EN_FCAM_PWR */
	PAD_CFG_GPO(GPP_D17, 1, DEEP),

	/* E1  : SPI1_IO2 ==> PEN_DET_ODL */
	PAD_CFG_GPI_SCI(GPP_E1, NONE, DEEP, EDGE_SINGLE, NONE),
	/* E2  : SPI1_IO3 ==> WLAN_PCIE_WAKE_ODL */
	PAD_CFG_GPI(GPP_E2, NONE, DEEP),
	/* E3  : CPU_GP0 ==> USI_REPORT_EN */
	PAD_CFG_GPO(GPP_E3, 1, DEEP),
	/* E7  : CPU_GP1 ==> USI_INT */
	PAD_CFG_GPI_APIC(GPP_E7, NONE, PLTRST, LEVEL, NONE),
	/* E8  : SPI1_CS1# ==> SLP_S0IX */
	PAD_CFG_GPO(GPP_E8, 0, DEEP),
	/* E11 : SPI1_CLK ==> SD_PE_WAKE_ODL */
	PAD_CFG_GPI(GPP_E11, NONE, DEEP),
	/* E12 : SPI1_MISO_IO1 ==> PEN_OC_ODL */
	PAD_CFG_GPI(GPP_E12, NONE, DEEP),
	/* E15 : ISH_GP6 ==> TRACKPAD_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_E15, NONE, PLTRST, LEVEL, INVERT),
	/* E16 : ISH_GP7 ==> WWAN_SIM1_DET_OD */
	PAD_CFG_GPI(GPP_E16, NONE, DEEP),
	/* E17 : THC0_SPI1_INT# ==> WWAN_PERST_L */
	PAD_CFG_GPO(GPP_E17, 1, DEEP),
	/* E19 : DDP1_CTRLDATA ==> USB0_C0_LSX_SOC_RX_STRAP */
	PAD_CFG_NF(GPP_E19, DN_20K, DEEP, NF4),

	/* F6  : CNV_PA_BLANKING ==> WWAN_WLAN_COEX3 */
	PAD_CFG_NF(GPP_F6, NONE, DEEP, NF1),
	/* F7  : GPPF7_STRAP */
	PAD_NC(GPP_F7, DN_20K),
	/* F8  : I2S_MCLK2_INOUT ==> HP_INT_L */
	PAD_CFG_GPI_INT(GPP_F8, NONE, PLTRST, EDGE_BOTH),
	/* F11 : THC1_SPI2_CLK ==> EN_PP3300_WWAN */
	PAD_CFG_GPO(GPP_F11, 1, DEEP),
	/* F12 : GSXDOUT ==> WWAN_RST_ODL */
	PAD_CFG_GPI(GPP_F12, NONE, DEEP),
	/* F13 : GSXDOUT ==> WiFi_DISABLE_L */
	PAD_CFG_GPO(GPP_F13, 1, DEEP),
	/* F14 : GSXDIN ==> SAR0_INT_L */
	PAD_CFG_GPI_SCI_LOW(GPP_F14, NONE, PLTRST, EDGE_SINGLE),
	/* F16 : GSXCLK ==> WWAN_DPR_SAR_ODL */
	PAD_CFG_GPO(GPP_F16, 1, DEEP),
	/* F17 : WWAN_RF_DISABLE_ODL */
	PAD_CFG_GPO(GPP_F17, 1, DEEP),
	/* F18 : THC1_SPI2_INT# ==> WWAN_PCIE_WAKE_ODL */
	PAD_CFG_GPI_SCI_LOW(GPP_F18, NONE, DEEP, EDGE_SINGLE),
	/* F19 : SRCCLKREQ6# ==> WLAN_INT_L */
	PAD_CFG_GPI_SCI_LOW(GPP_F19, NONE, DEEP, EDGE_SINGLE),

	/* H0  : GPPH0_BOOT_STRAP1 */
	PAD_NC(GPP_H0, DN_20K),
	/* H1  : GPPH1_BOOT_STRAP2 */
	PAD_NC(GPP_H1, DN_20K),
	/* H2  : GPPH2_BOOT_STRAP3 */
	PAD_NC(GPP_H2, DN_20K),
	/* H3  : SX_EXIT_HOLDOFF# ==> SD_PERST_L */
	PAD_CFG_GPO(GPP_H3, 1, DEEP),
	/* H6  : I2C3_SDA ==> PCH_I2C3_CAM_SAR1_SDA */
	PAD_CFG_NF(GPP_H6, NONE, DEEP, NF1),
	/* H7  : I2C3_SCL ==> PCH_I2C3_CAM_SAR1_SCL */
	PAD_CFG_NF(GPP_H7, NONE, DEEP, NF1),
	/* H8  : I2C4_SDA ==> WWAN_WLAN_COEX1 */
	PAD_CFG_NF(GPP_H8, NONE, DEEP, NF2),
	/* H9  : I2C4_SCL ==> WWAN_WLAN_COEX2 */
	PAD_CFG_NF(GPP_H9, NONE, DEEP, NF2),
	/* H12 : M2_SKT2_CFG0 ==> WWAN_CONFIG0 */
	PAD_CFG_GPI(GPP_H12, NONE, DEEP),
	/* H13 : M2_SKT2_CFG1 # ==> SPKR_INT_L */
	PAD_CFG_GPI(GPP_H13, NONE, DEEP),
	/* H15 : M2_SKT2_CFG3 # ==> WWAN_CONFIG3 */
	PAD_CFG_GPI(GPP_H15, NONE, DEEP),
	/* H16 : DDPB_CTRLCLK ==> DDPB_HDMI_CTRLCLK */
	PAD_CFG_NF(GPP_H16, NONE, DEEP, NF1),
	/* H17 : DDPB_CTRLDATA ==> DDPB_HDMI_CTRLDATA */
	PAD_CFG_NF(GPP_H17, NONE, DEEP, NF1),
	/* H19 : TIME_SYNC0 ==> USER_PRES_FP_ODL_R */
	PAD_CFG_GPI(GPP_H19, NONE, DEEP),
	/* H20 : IMGCLKOUT1 ==> EN_MIPI_RCAM_PWR */
	PAD_CFG_GPO(GPP_H20, 0, DEEP),
	/* H21 : IMGCLKOUT2 ==> CAM_MCLK1 */
	PAD_CFG_NF(GPP_H21, NONE, DEEP, NF1),
	/* H22 : IMGCLKOUT3 ==> CAM_MCLK0 */
	PAD_CFG_NF(GPP_H22, NONE, DEEP, NF1),

	/* R0 : HDA_BCLK ==> I2S0_HP_SCLK */
	PAD_CFG_NF(GPP_R0, NONE, DEEP, NF2),
	/* R1 : HDA_SYNC ==> I2S0_HP_SFRM */
	PAD_CFG_NF(GPP_R1, NONE, DEEP, NF2),
	/* R2 : HDA_SDO ==> I2S0_PCH_TX_HP_RX_STRAP */
	PAD_CFG_NF(GPP_R2, DN_20K, DEEP, NF2),
	/* R3 : HDA_SDIO ==> I2S0_PCH_RX_HP_TX */
	PAD_CFG_NF(GPP_R3, NONE, DEEP, NF2),
	/* R5 : HDA_SDI1 ==> I2S1_PCH_RX_SPKR_TX */
	PAD_CFG_NF(GPP_R5, NONE, DEEP, NF2),
	/* R6 : I2S1_TXD ==> I2S1_PCH_RX_SPKR_RX */
	PAD_CFG_NF(GPP_R6, NONE, DEEP, NF2),
	/* R7 : I2S1_SFRM ==> I2S1_SPKR_SFRM */
	PAD_CFG_NF(GPP_R7, NONE, DEEP, NF2),

	/* S0 : SNDW0_CLK ==> SNDW0_HP_CLK */
	PAD_CFG_NF(GPP_S0, NONE, DEEP, NF1),
	/* S1 : SNDW0_DATA ==> SNDW0_HP_DATA */
	PAD_CFG_NF(GPP_S1, NONE, DEEP, NF1),
	/* S2 : SNDW1_CLK ==> SNDW1_SPKR_CLK */
	PAD_CFG_NF(GPP_S2, NONE, DEEP, NF1),
	/* S3 : SNDW1_DATA ==> SNDW1_SPKR_DATA */
	PAD_CFG_NF(GPP_S3, NONE, DEEP, NF1),
	/* S6 : SNDW3_CLK ==> DMIC_CLK0 */
	PAD_CFG_NF(GPP_S6, NONE, DEEP, NF2),
	/* S7 : SNDW3_DATA ==> DMIC_DATA0 */
	PAD_CFG_NF(GPP_S7, NONE, DEEP, NF2),

	/* GPD9: SLP_WLAN# ==> SLP_WLAN_L */
	PAD_CFG_NF(GPD9, NONE, DEEP, NF1),
};

const struct pad_config *variant_override_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(override_gpio_table);
	return override_gpio_table;
}

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
	/* A12 : SATAXPCIE1 ==> M2_SSD_PEDET */
	PAD_CFG_NF(GPP_A12, NONE, DEEP, NF1),
	/* A13 : PMC_I2C_SCL ==> BT_DISABLE_L */
	/* assert reset on reboot */
	PAD_CFG_GPO(GPP_A13, 0, DEEP),
	/* A17 : DDSP_HPDC ==> MEM_CH_SEL */
	PAD_CFG_GPI(GPP_A17, NONE, DEEP),

	/* B2  : VRALERT# ==> EN_PP3300_SSD */
	PAD_CFG_GPO(GPP_B2, 1, PLTRST),
	/* B11 : PMCALERT# ==> PCH_WP_OD */
	PAD_CFG_GPI_GPIO_DRIVER(GPP_B11, NONE, DEEP),
	/* B15 : GSPI0_CS0# ==> PCH_GSPI0_H1_TPM_CS_L */
	PAD_CFG_NF(GPP_B15, NONE, DEEP, NF1),
	/* B16 : GSPI0_CLK ==> PCH_GSPI0_H1_TPM_CLK */
	PAD_CFG_NF(GPP_B16, NONE, DEEP, NF1),
	/* B17 : GSPI0_MISO ==> PCH_GSPIO_H1_TPM_MISO */
	PAD_CFG_NF(GPP_B17, NONE, DEEP, NF1),
	/* B18 : GSPI0_MOSI ==> PCH_GSPI0_H1_TPM_MOSI_STRAP */
	PAD_CFG_NF(GPP_B18, DN_20K, DEEP, NF1),

	/* C0  : SMBCLK ==> EN_PP3300_WLAN */
	PAD_CFG_GPO(GPP_C0, 1, DEEP),
	/* C8 : UART0 RX */
	PAD_CFG_NF(GPP_C8, NONE, DEEP, NF1),
	/* C9 : UART0 TX */
	PAD_CFG_NF(GPP_C9, NONE, DEEP, NF1),
	/* C21 : UART2_TXD ==> H1_PCH_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_C21, NONE, PLTRST, LEVEL, INVERT),
	/* C22 : UART2_RTS# ==> PCH_FPMCU_BOOT0 */
	PAD_CFG_GPO(GPP_C22, 0, DEEP),

	/* D16 : ISH_UART0_CTS# ==> EN_PP3300_SD */
	PAD_CFG_GPO(GPP_D16, 1, DEEP),
};

const struct pad_config *variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

/* GPIO settings before entering S5 */
static const struct pad_config s5_sleep_gpio_table[] = {
	PAD_CFG_GPO(GPP_C23, 0, DEEP), /* FPMCU_RST_ODL */
	PAD_CFG_GPO(GPP_A21, 0, DEEP), /* EN_FP_PWR */
};

const struct pad_config *variant_sleep_gpio_table(u8 slp_typ, size_t *num)
{
	if (slp_typ == ACPI_S5) {
		*num = ARRAY_SIZE(s5_sleep_gpio_table);
		return s5_sleep_gpio_table;
	}
	*num = 0;
	return nullptr;
}

/* GPIOs needed to be set in romstage. */
static const struct pad_config romstage_gpio_table[] = {
	/* Enable touchscreen, hold in reset */
	/* A8  : I2S2_SFRM ==> EN_PP3300_TOUCHSCREEN */
	PAD_CFG_GPO(GPP_A8, 1, DEEP),
	/* C10 : UART0_RTS# ==> USI_RST_L */
	PAD_CFG_GPO(GPP_C10, 0, DEEP),
};

const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}
