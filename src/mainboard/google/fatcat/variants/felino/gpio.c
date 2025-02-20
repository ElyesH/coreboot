/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <boardid.h>
#include <soc/gpio.h>

/* Pad configuration in ramstage*/
static const struct pad_config gpio_table[] = {
	/* GPP_A00:     ESPI_IO0_EC_R */
	/*  GPP_A00 : GPP_A00 ==> ESPI_IO0_EC_R configured on reset, do not touch */

	/* GPP_A01:     ESPI_IO1_EC_R */
	/*  GPP_A01 : GPP_A01 ==> ESPI_IO1_EC_R configured on reset, do not touch */

	/* GPP_A02:     ESPI_IO2_EC_R */
	/*  GPP_A02 : GPP_A02 ==> ESPI_IO2_EC_R configured on reset, do not touch */

	/* GPP_A03:     ESPI_IO3_EC_R */
	/*  GPP_A03 : GPP_A03 ==> ESPI_IO3_EC_R configured on reset, do not touch */

	/* GPP_A04:     ESPI_CS0_EC_R_N */
	/*  GPP_A04 : GPP_A04 ==> ESPI_CS0_HDR_L configured on reset, do not touch */

	/* GPP_A05:     ESPI_CLK_EC_R */
	/*  GPP_A05 : GPP_A05 ==> ESPI_CLK_HDR configured on reset, do not touch */

	/* GPP_A06:     ESPI_RST_EC_R_N */
	/*  GPP_A06 : GPP_A06 ==> ESPI_RST_HDR configured on reset, do not touch */

	/* GPP_A08:     MIPI_RGB_XSHUTDN */
	PAD_CFG_GPO(GPP_A08, 1, PLTRST),
	/* GPP_A09:     MIPI_RGB_OSC_EN */
	PAD_CFG_GPO(GPP_A09, 1, PLTRST),
	/* GPP_A10:     WWAN_RF_DISABLE_ODL */
	PAD_CFG_GPO(GPP_A10, 1, PLTRST),
	/* GPP_A11:     WLAN_SOC_RST_N */
	PAD_CFG_GPO(GPP_A11, 1, PLTRST),
	/* GPP_A12:     WAKE_PCIE_N_SOC */
	PAD_CFG_GPI_SCI_LOW(GPP_A12, NONE, DEEP, LEVEL),
	/* GPP_A13:     Not used */
	PAD_NC(GPP_A13, NONE),
	/* GPP_A15:     HW_ID1 */
	PAD_CFG_NF(GPP_A15, NONE, DEEP, NF1),
	/* GPP_A16:     BT_RF_KILL_N */
	PAD_CFG_GPO(GPP_A16, 1, DEEP),
	/* GPP_A17:     PCH_WLAN_OFF_N */
	PAD_CFG_GPO(GPP_A17, 1, DEEP),

	/* GPP_B00:     TYPEC_PD_SOC_CLK */
	PAD_CFG_NF(GPP_B00, NONE, DEEP, NF1),
	/* GPP_B01:     TYPEC_PD_SOC_DAT */
	PAD_CFG_NF(GPP_B01, NONE, DEEP, NF1),
	/* GPP_B02:     I2C_SDA_E3_SOC */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B02, NONE, DEEP, NF3),
	/* GPP_B03:     I2C_SCL_E3_SOC */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B03, NONE, DEEP, NF3),
	/* GPP_B04:     NC */
	PAD_NC(GPP_B04, NONE),
	/* GPP_B05:     MIPI_IR_LED_PWM */
	PAD_CFG_NF(GPP_B05, NONE, DEEP, NF4),
	/* GPP_B06:     NC */
	PAD_NC(GPP_B06, NONE),
	/* GPP_B07:     NC */
	PAD_NC(GPP_B07, NONE),
	/* GPP_B08:     NC */
	PAD_NC(GPP_B08, NONE),
	/* GPP_B09:     NC */
	PAD_NC(GPP_B09, NONE),
	/* GPP_B10:     SOC_DP1_HDMI_HPD */
	PAD_CFG_NF(GPP_B10, NONE, DEEP, NF1),
	/* GPP_B11:     PD1_OC_P0_P1_N */
	PAD_NC(GPP_B11, NONE),
	/* GPP_B12:     SLP_S0_SOC_N */
	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
	/* GPP_B13:     PLT_RST_N */
	PAD_CFG_NF(GPP_B13, NONE, DEEP, NF1),
	/* GPP_B14:     NC */
	PAD_NC(GPP_B14, NONE),
	/* GPP_B15:     USB_A_OC3_L */
	PAD_CFG_NF(GPP_B15, NONE, DEEP, NF1),
	/* GPP_B16:     NC */
	PAD_NC(GPP_B16, NONE),
	/* GPP_B17:     NC */
	PAD_NC(GPP_B17, NONE),
	/* GPP_B18:     ISH_I2C_EC_SDA */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B18, NONE, DEEP, NF1),
	/* GPP_B19:     ISH_I2C_EC_SCL */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_B19, NONE, DEEP, NF1),
	/* GPP_B20:     I2C2_SOC_SDA */
	PAD_CFG_GPO(GPP_B20, 1, PLTRST),
	/* GPP_B21:     I2C2_SOC_SCL */
	PAD_CFG_GPO(GPP_B21, 0, DEEP),
	/* GPP_B22:     Cable_INT_N */
	PAD_CFG_NF(GPP_B22, NONE, DEEP, NF4),
	/* GPP_B23:     HW_ID0 */
	PAD_CFG_NF(GPP_B23, NONE, DEEP, NF4),
	/* GPP_B24:     ESPI_ALERT0_N */
	PAD_NC(GPP_B24, NONE),
	/* GPP_B25:     MIPI_RGB_LDO_EN */
	PAD_CFG_GPI_SCI_LOW(GPP_B25, NONE, DEEP, LEVEL),

	/* GPP_C00:     NC */
	PAD_NC(GPP_C00, NONE),
	/* GPP_C01:     NC */
	PAD_NC(GPP_C01, NONE),
	/* GPP_C02:     GPPC_C2_SMB_ALERT_N */
	PAD_CFG_NF(GPP_C02, NONE, DEEP, NF1),
	/* GPP_C03:     SMBUS_CLK0 */
	PAD_CFG_NF(GPP_C03, NONE, DEEP, NF1),
	/* GPP_C04:     SMBUS_DAT0 */
	PAD_CFG_NF(GPP_C04, NONE, DEEP, NF1),
	/* GPP_C05:     GPPC_C5 */
	PAD_NC(GPP_C05, NONE),
	/* GPP_C06:     NC */
	PAD_NC(GPP_C06, NONE),
	/* GPP_C07:     NC */
	PAD_NC(GPP_C07, NONE),
	/* GPP_C08:     PCH_WP_OD */
	PAD_CFG_GPO(GPP_C08, 1, PLTRST),
	/* GPP_C09:     PCIE_CLKREQ_SSD1_N */
	PAD_CFG_NF(GPP_C09, NONE, DEEP, NF1),
	/* GPP_C10:     NC */
	PAD_NC(GPP_C10, NONE),
	/* GPP_C11:     NC */
	PAD_NC(GPP_C11, NONE),
	/* GPP_C12:     NC */
	PAD_NC(GPP_C12, NONE),
	/* GPP_C13:     PCIE_CLKREQ_WLAN_N */
	PAD_CFG_NF(GPP_C13, NONE, DEEP, NF1),
	/* GPP_C14:     NC */
	PAD_NC(GPP_C14, NONE),
	/* GPP_C15:     GPP_C15 */
	PAD_CFG_GPO(GPP_C15, 1, PLTRST),
	/* GPP_C16:     NC  */
	PAD_NC(GPP_C16, NONE),
	/* GPP_C17:     NC  */
	PAD_NC(GPP_C17, NONE),
	/* GPP_C18:     TCP3_DDC_SCL  */
	PAD_CFG_NF(GPP_C18, NONE, DEEP, NF1),
	/* GPP_C19:     TCP3_DDC_SDA */
	PAD_CFG_NF(GPP_C19, NONE, DEEP, NF1),
	/* GPP_C20:     TBT_LSX1_TXD */
	PAD_CFG_NF(GPP_C20, NONE, DEEP, NF1),
	/* GPP_C21:     TBT_LSX1_RXD */
	PAD_CFG_NF(GPP_C21, NONE, DEEP, NF1),
	/* GPP_C22:     TBT_LSX0_TXD */
	PAD_CFG_NF(GPP_C22, NONE, DEEP, NF1),
	/* GPP_C23:     TBT_LSX0_RXD */
	PAD_CFG_NF(GPP_C23, NONE, DEEP, NF1),

	/* GPP_D00:     MIPI_RGB_IR_MCLK */
	PAD_CFG_NF(GPP_D00, NONE, DEEP, NF1),
	/* GPP_D01:     ALS_I2C_SDA */
	PAD_CFG_NF(GPP_D01, NONE, DEEP, NF3),
	/* GPP_D02:     ALS_I2C_SCL */
	PAD_CFG_NF(GPP_D02, NONE, DEEP, NF3),
	/* GPP_D03:     SLP_S0_GATE_R */
	PAD_CFG_GPO(GPP_D03, 1, PLTRST),
	/* GPP_D04:     MIPI_RGB_MCLK */
	PAD_CFG_NF(GPP_D04, NONE, DEEP, NF1),
	/* GPP_D05:     UART0_ISH_RX_DBG_TX_U */
	PAD_CFG_NF(GPP_D05, NONE, DEEP, NF1),
	/* GPP_D06:     UART0_ISH_TX_DBG_RX_U */
	PAD_CFG_NF(GPP_D06, NONE, DEEP, NF1),
	/* GPP_D07:     CAM_VDD_EN_SOC */
	PAD_CFG_GPO(GPP_D07, 1, PLTRST),
	/* GPP_D08:     NC */
	PAD_NC(GPP_D08, NONE),
	/* GPP_D09:     USB_MUX_SEL */
	PAD_CFG_GPO(GPP_D09, 1, PLTRST),
	/* GPP_D10:     PMC_WLAN_CLK */
	PAD_CFG_NF(GPP_D10, NONE, DEEP, NF1),
	/* GPP_D11:     CNV_PCM_SYNC */
	PAD_CFG_NF(GPP_D11, NONE, DEEP, NF1),
	/* GPP_D12:     PMC_SOC_OUT */
	PAD_CFG_NF(GPP_D12, NONE, DEEP, NF1),
	/* GPP_D13:     PMC_SOC_IN */
	PAD_CFG_NF(GPP_D13, NONE, DEEP, NF1),
	/* GPP_D14:     NC */
	PAD_NC(GPP_D14, NONE),
	/* GPP_D15:     NC */
	PAD_NC(GPP_D15, NONE),
	/* GPP_D16:     PCH_DMIC_CLK1 */
	PAD_CFG_NF(GPP_D16, NONE, DEEP, NF2),
	/* GPP_D17:     PCH_DMIC_DATA1 */
	PAD_CFG_NF(GPP_D17, NONE, DEEP, NF2),
	/* GPP_D18:     PCIE_CLKREQ_SD_N */
	PAD_CFG_NF(GPP_D18, NONE, DEEP, NF1),
	/* GPP_D19:     NC */
	PAD_NC(GPP_D19, NONE),
	/* GPP_D20:     NC */
	PAD_NC(GPP_D20, NONE),
	/* GPP_D21:     NC */
	PAD_NC(GPP_D21, NONE),
	/* GPP_D22:     DG_I3C_SDA */
	PAD_CFG_NF(GPP_D22, NONE, DEEP, NF1),
	/* GPP_D23:     DG_I3C_SCL */
	PAD_CFG_NF(GPP_D23, NONE, DEEP, NF1),
	/* GPP_D24:     MIPI_IR_LDO_EN */
	PAD_CFG_GPI_SCI_LOW(GPP_D24, NONE, DEEP, LEVEL),
	/* GPP_D25:     MIPI_IR_OSC_EN */
	PAD_CFG_GPI_SCI_LOW(GPP_D25, NONE, DEEP, LEVEL),

	/* GPP_E01:     SLOW_R */
	PAD_CFG_GPI(GPP_E01, NONE, PLTRST),
	/* GPP_E02:     NC  */
	PAD_NC(GPP_E02, NONE),
	/* GPP_E03:     NC */
	PAD_NC(GPP_E03, NONE),
	/* GPP_E05:     NC */
	PAD_NC(GPP_E05, NONE),
	/* GPP_E06:     GPP_E06 */
	PAD_CFG_GPI_TRIG_OWN(GPP_E06, NONE, PLTRST, LEVEL, ACPI),
	/* GPP_E07:     NC */
	PAD_NC(GPP_E07, NONE),
	/* GPP_E08:     NC */
	PAD_NC(GPP_E08, NONE),
	/* GPP_E09:     NC */
	PAD_NC(GPP_E09, NONE),
	/* GPP_E10:     NC */
	PAD_NC(GPP_E10, NONE),
	/* GPP_E11:     NC */
	PAD_NC(GPP_E11, NONE),
	/* GPP_E12:     I2C4_SOC_SCL */
	PAD_CFG_NF(GPP_E12, NONE, DEEP, NF8),
	/* GPP_E13:     I2C4_SOC_SDA */
	PAD_CFG_NF(GPP_E13, NONE, DEEP, NF8),
	/* GPP_E14:     NC */
	PAD_NC(GPP_E14, NONE),
	/* GPP_E15:     NC */
	PAD_NC(GPP_E15, NONE),
	/* GPP_E16:     NC */
	PAD_NC(GPP_E16, NONE),
	/* GPP_E17:     NC */
	PAD_NC(GPP_E17, NONE),
	/* GPP_E18:     TOUCHPAD_INT_N */
	PAD_CFG_GPI_APIC(GPP_E18, NONE, PLTRST, LEVEL, NONE),
	/* GPP_E19:     HW_ID5 */
	PAD_CFG_GPO(GPP_E19, 1, PLTRST),
	/* GPP_E20:     HW_ID4 */
	PAD_CFG_GPO(GPP_E20, 1, PLTRST),
	/* GPP_E21:     TYPEC_PD_SOC_INT */
	PAD_CFG_NF(GPP_E21, NONE, DEEP, NF1),
	/* GPP_E22:     MIPI_IR_XSHUTDN */
	PAD_CFG_NF(GPP_E22, NONE, DEEP, NF3),

	/* GPP_F00:     CNV_BRI_DT */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F00, NONE, DEEP, NF1),
	/* GPP_F01:     CNV_BRI_RSP_SOC */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F01, NONE, DEEP, NF1),
	/* GPP_F02:     CNV_RGI_DT */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F02, NONE, DEEP, NF1),
	/* GPP_F03:     CNV_RGI_RSP_SOC */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F03, NONE, DEEP, NF1),
	/* GPP_F04:     CNV_RF_RESET_R_N */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F04, NONE, DEEP, NF1),
	/* GPP_F05:     CRF_CLKREQ_R */
	/* NOTE: IOSSTAGE: 'Ignore' for S0ix */
	PAD_CFG_NF_IOSTANDBY_IGNORE(GPP_F05, NONE, DEEP, NF3),
	/* GPP_F06:     NC */
	PAD_NC(GPP_F06, NONE),
	/* GPP_F07:     NC */
	PAD_NC(GPP_F07, NONE),
	/* GPP_F08:     NC */
	PAD_NC(GPP_F08, NONE),
	/* GPP_F09:     NC */
	PAD_NC(GPP_F09, NONE),
	/* GPP_F10:     HW_ID7 */
	PAD_CFG_GPO(GPP_F10, 0, PLTRST),
	/* GPP_F11:     TC_RETIMER_FORCE_PWR */
	PAD_CFG_GPO(GPP_F11, 1, DEEP),
	/* GPP_F12:     NC */
	PAD_NC(GPP_F12, NONE),
	/* GPP_F13:     NC */
	PAD_NC(GPP_F13, NONE),
	/* GPP_F14:     NC */
	PAD_NC(GPP_F14, NONE),
	/* GPP_F15:     GSC_PCH_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_F15, NONE, PLTRST, LEVEL, INVERT),
	/* GPP_F16:     NC */
	PAD_NC(GPP_F16, NONE),
	/* GPP_F17:     NC */
	PAD_NC(GPP_F17, NONE),
	/* GPP_F18:     NC */
	PAD_NC(GPP_F18, NONE),
	/* GPP_F19:     GPP_F19 */
	PAD_CFG_GPO(GPP_F19, 0, PLTRST),
	/* GPP_F20:     NC */
	PAD_NC(GPP_F20, NONE),
	/* GPP_F22:     NC */
	PAD_NC(GPP_F22, NONE),
	/* GPP_F23:     NC */
	PAD_NC(GPP_F23, NONE),

	/* GPP_H00:     GPP_H00 */
	PAD_NC(GPP_H00, NONE),
	/* GPP_H01:     GPP_H01 */
	PAD_NC(GPP_H01, NONE),
	/* GPP_H02:     GPP_H02 */
	PAD_NC(GPP_H02, NONE),
	/* GPP_H03:     NC */
	PAD_NC(GPP_H03, NONE),
	/* GPP_H04:     MIPI_IR_SDA */
	PAD_CFG_NF(GPP_H04, NONE, DEEP, NF1),
	/* GPP_H05:     MIPI_IR_SCL */
	PAD_CFG_NF(GPP_H05, NONE, DEEP, NF1),
	/* GPP_H06:     CAM_I2C_DAT_CONN */
	PAD_CFG_NF(GPP_H06, NONE, DEEP, NF1),
	/* GPP_H07:     CAM_I2C_CLK_CONN */
	PAD_CFG_NF(GPP_H07, NONE, DEEP, NF1),
	/* GPP_H08:     UART0_BUF_RXD */
	PAD_CFG_NF(GPP_H08, NONE, DEEP, NF1),
	/* GPP_H09:     UART0_BUF_TXD */
	PAD_CFG_NF(GPP_H09, NONE, DEEP, NF1),
	/* GPP_H10:     HW_ID6 */
	PAD_CFG_NF(GPP_H10, NONE, DEEP, NF1),
	/* GPP_H11:     NC */
	PAD_NC(GPP_H11, NONE),
	/* GPP_H13:     CPU_C10_GATE_N */
	PAD_CFG_NF(GPP_H13, NONE, DEEP, NF1),
	/* GPP_H14:     AUTOOPEN_ALS_I2C_SDA */
	PAD_CFG_NF(GPP_H14, NONE, DEEP, NF3),
	/* GPP_H15:     AUTOOPEN_ALS_I2C_SCL */
	PAD_CFG_NF(GPP_H15, NONE, DEEP, NF3),
	/* GPP_H16:     NC */
	PAD_NC(GPP_H16, NONE),
	/* GPP_H17:     SOC_SCI_N */
	PAD_CFG_NF(GPP_H17, NONE, DEEP, NF1),
	/* GPP_H19:     HW_ID3 */
	PAD_CFG_NF(GPP_H19, NONE, DEEP, NF2),
	/* GPP_H20:     HW_ID2 */
	PAD_CFG_NF(GPP_H20, NONE, DEEP, NF2),
	/* GPP_H21:     PCH_I2C_GSC_SDA */
	PAD_CFG_NF(GPP_H21, NONE, DEEP, NF1),
	/* GPP_H22:     PCH_I2C_GSC_SCL */
	PAD_CFG_NF(GPP_H22, NONE, DEEP, NF1),

	/* GPP_S00:     SNDW3_CLK_CODEC */
	PAD_CFG_NF(GPP_S00, NONE, DEEP, NF1),
	/* GPP_S01:     SNDW3_DATA0_CODEC */
	PAD_CFG_NF(GPP_S01, NONE, DEEP, NF1),
	/* GPP_S02:     SNDW3_DATA1_CODEC */
	PAD_CFG_NF(GPP_S02, NONE, DEEP, NF1),
	/* GPP_S03:     SNDW3_DATA2_CODEC */
	PAD_CFG_NF(GPP_S03, NONE, DEEP, NF1),
	/* GPP_S04:     SNDW2_CLK */
	PAD_CFG_NF(GPP_S04, NONE, DEEP, NF2),
	/* GPP_S05:     SNDW2_DATA0 */
	PAD_CFG_NF(GPP_S05, NONE, DEEP, NF2),
	/* GPP_S06:     SWIRE_AMP_CLK */
	PAD_NC(GPP_S06, NONE),
	/* GPP_S07:     SWIRE_AMP_DATA0 */
	PAD_NC(GPP_S07, NONE),

	/* GPP_V00:     PM_BATLOW_N */
	PAD_CFG_NF(GPP_V00, NONE, DEEP, NF1),
	/* GPP_V01:     BC_ACOK_MCP */
	PAD_CFG_NF(GPP_V01, NONE, DEEP, NF1),
	/* GPP_V02:     NC */
	PAD_NC(GPP_V02, NONE),
	/* GPP_V03:     PWRBTN_MCP_N */
	PAD_CFG_NF(GPP_V03, NONE, DEEP, NF1),
	/* GPP_V04:     PM_SLP_S3_N */
	PAD_CFG_NF(GPP_V04, NONE, DEEP, NF1),
	/* GPP_V05:     PM_SLP_S4_N */
	PAD_CFG_NF(GPP_V05, NONE, DEEP, NF1),
	/* GPP_V06:     NC */
	PAD_NC(GPP_V06, NONE),
	/* GPP_V07:     Not used */
	PAD_CFG_NF(GPP_V07, NONE, DEEP, NF1),
	/* GPP_V08:     SLP_WLAN_N */
	PAD_CFG_NF(GPP_V08, NONE, DEEP, NF1),
	/* GPP_V09:     NC */
	PAD_NC(GPP_V09, NONE),
	/* GPP_V10:     NC */
	PAD_NC(GPP_V10, NONE),
	/* GPP_V11:     NC */
	PAD_NC(GPP_V11, NONE),
	/* GPP_V12:     NC */
	PAD_NC(GPP_V12, NONE),
	/* GPP_V13:     NC */
	PAD_NC(GPP_V13, NONE),
	/* GPP_V14:     NC */
	PAD_NC(GPP_V14, NONE),
	/* GPP_V15:     NC */
	PAD_NC(GPP_V15, NONE),
	/* GPP_V16:     VCCST_EN */
	PAD_CFG_NF(GPP_V16, NONE, DEEP, NF1),
	/* GPP_V17:     GPP_V17 */
	PAD_NC(GPP_V17, NONE),
};

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
	/* GPP_H08:     UART0_BUF_RXD */
	PAD_CFG_NF(GPP_H08, NONE, DEEP, NF1),
	/* GPP_H09:     UART0_BUF_TXD */
	PAD_CFG_NF(GPP_H09, NONE, DEEP, NF1),

	/* GPP_H21:     PCH_I2C_GSC_SDA */
	PAD_CFG_NF(GPP_H21, NONE, DEEP, NF1),
	/* GPP_H22:     PCH_I2C_GSC_SCL */
	PAD_CFG_NF(GPP_H22, NONE, DEEP, NF1),
	/* GPP_F15: SPI_TPM_INT_N */
	PAD_CFG_GPI_APIC(GPP_F15, NONE, PLTRST, LEVEL, INVERT),
};

/* Pad configuration in romstage */
static const struct pad_config romstage_gpio_table[] = {
	/* GPP_C03:     GPP_C0_SMBCLK */
	PAD_CFG_NF(GPP_C03, NONE, DEEP, NF1),
	/* GPP_C04:     GPP_C0_SMBDATA */
	PAD_CFG_NF(GPP_C04, NONE, DEEP, NF1),
};

const struct pad_config *variant_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(gpio_table);
	return gpio_table;
}

const struct pad_config *variant_early_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(early_gpio_table);
	return early_gpio_table;
}

/* Create the stub for romstage gpio, typically use for power sequence */
const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}

static const struct cros_gpio cros_gpios[] = {
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE0_NAME),
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE1_NAME),
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE2_NAME),
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE3_NAME),
};

DECLARE_CROS_GPIOS(cros_gpios);
