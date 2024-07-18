/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <soc/gpio.h>
#include <types.h>
#include <vendorcode/google/chromeos/chromeos.h>

/* Pad configuration in ramstage */
static const struct pad_config gpio_table[] = {
	/* A0 thru A4, A9 and A10 come configured out of reset, do not touch */
	/* A0  : ESPI_IO0 ==> ESPI_SOC_D0_EC */
	/* A1  : ESPI_IO1 ==> ESPI_SOC_D1_EC */
	/* A2  : ESPI_IO2 ==> ESPI_SOC_D2_EC */
	/* A3  : ESPI_IO3 ==> ESPI_SOC_D3_EC */
	/* A4  : ESPI_CS0# ==> ESPI_SOC_CS_EC_L */
	/* A5  : ESPI_ALERT0# ==> NC */
	PAD_NC(GPP_A5, NONE),
	/* A6  : ESPI_ALERT1# ==> NC */
	PAD_NC(GPP_A6, NONE),
	/* A7  : NC */
	PAD_NC(GPP_A7, NONE),
	/* A8  : NC */
	PAD_NC(GPP_A8, NONE),
	/* A9  : ESPI_CLK ==> ESPI_SOC_CLK */
	/* A10 : ESPI_RESET# ==> ESPI_SOC_RST_EC_L */
	/* A11 : EN_SPK_PA ==> NC */
	PAD_NC(GPP_A11, NONE),
	/* A12 : NC */
	PAD_NC(GPP_A12, NONE),
	/* A13 : GPP_A13 ==> SOC_BT_ON */
	PAD_CFG_GPO_LOCK(GPP_A13, 1, LOCK_CONFIG),
	/* A14 : USB_OC1# ==> NC */
	PAD_NC(GPP_A14, NONE),
	/* A15 : USB_OC2# ==> NC */
	PAD_NC(GPP_A15, NONE),
	/* A16 : USB_OC3# ==> NC */
	PAD_NC_LOCK(GPP_A16, NONE, LOCK_CONFIG),
	/* A17 : GPP_A17 ==> GSC_SOC_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_A17, NONE, PLTRST, LEVEL, INVERT),
	/* A18 : DDSP_HPDB ==> DDI2_HPD */
	PAD_CFG_NF(GPP_A18, NONE, DEEP, NF1),
	/* A19 : DDSP_HPD1 ==> USBC_DP_HPD */
	PAD_CFG_NF(GPP_A19, NONE, DEEP, NF1),
	/* A20 : NC */
	PAD_NC(GPP_A20, NONE),
	/* A21 : DDPC_CTRLCLK ==> TCHSCR_REPORT_EN */
	PAD_CFG_GPO(GPP_A21, 0, DEEP),
	/* A22 : GPP_A22 ==> TPM_PIRQ# */
	PAD_CFG_GPI(GPP_A22, NONE, DEEP),
	/* A23 : GPP_A23 ==> NC */
	PAD_NC(GPP_A23, NONE),

	/* B0  : CORE_VID0 ==> VCCIN_AUX_VID0 */
	PAD_CFG_NF(GPP_B0, NONE, DEEP, NF1),
	/* B1  : CORE_VID1 ==> VCCIN_AUX_VID1 */
	PAD_CFG_NF(GPP_B1, NONE, DEEP, NF1),
	/* B2  : NC */
	PAD_NC(GPP_B2, NONE),
	/* B3  : CPU_GP2 ==> EC_TP_INT */
	PAD_CFG_GPI_IRQ_WAKE_LOCK(GPP_B3, NONE, LEVEL, INVERT, LOCK_CONFIG),
	/* B4  : PROC_GP3 ==> EN_PP3300_UCAM_X */
	PAD_CFG_GPO_LOCK(GPP_B4, 1, LOCK_CONFIG),
	/* B5  : GPP_B5 ==> ISH_I2C0_SCL */
	PAD_CFG_NF_LOCK(GPP_B5, NONE, NF1, LOCK_CONFIG),
	/* B6  : GPP_B6 ==> ISH_I2C0_SDA */
	PAD_CFG_NF_LOCK(GPP_B6, NONE, NF1, LOCK_CONFIG),
	/* B7  : GPP_B7 ==> NC */
	PAD_NC_LOCK(GPP_B7, NONE, LOCK_CONFIG),
	/* B8  : GPP_B8 ==> NC */
	PAD_NC_LOCK(GPP_B8, NONE, LOCK_CONFIG),
	/* B9  : Not available */
	PAD_NC(GPP_B9, NONE),
	/* B10 : Not available */
	PAD_NC(GPP_B10, NONE),
	/* B11 : SOC_PD_INT# */
	PAD_CFG_NF(GPP_B11, NONE, DEEP, NF1),
	/* B12 : SLP_S0# ==> PM_SLP_S0# */
	PAD_CFG_NF(GPP_B12, NONE, DEEP, NF1),
	/* B13 : PLTRST# ==> PLT_RST_L */
	PAD_CFG_NF_LOCK(GPP_B13, NONE, NF1, LOCK_CONFIG),
	/* B14 : SPKR ==> GPP_B14_STRAP */
	PAD_NC_LOCK(GPP_B14, NONE, LOCK_CONFIG),
	/* B15 : NC */
	PAD_NC_LOCK(GPP_B15, NONE, LOCK_CONFIG),
	/* B16  : GPP_B16 ==> I2C_5_SDA */
	PAD_CFG_NF_LOCK(GPP_B16, NONE, NF2, LOCK_CONFIG),
	/* B17  : GPP_B17 ==> I2C_5_SCL */
	PAD_CFG_NF_LOCK(GPP_B17, NONE, NF2, LOCK_CONFIG),
	/* B18 : GPP_B18 ==> GPP_B18_STRAP */
	PAD_NC(GPP_B18, NONE),
	/* B19 : Not available */
	PAD_NC(GPP_B19, NONE),
	/* B20 : Not available */
	PAD_NC(GPP_B20, NONE),
	/* B21 : Not available */
	PAD_NC(GPP_B21, NONE),
	/* B22 : Not available */
	PAD_NC(GPP_B22, NONE),
	/* B23 : SML1ALERT# ==> PCHHOT_ODL_STRAP */
	PAD_NC(GPP_B23, NONE),

	/* C0  : NC */
	PAD_NC(GPP_C0, NONE),
	/* C1  : NC */
	PAD_NC(GPP_C1, NONE),
	/* C2  : SMBALERT# ==> GPP_C2_STRAP */
	PAD_NC(GPP_C2, NONE),
	/* C3  : NC */
	PAD_NC(GPP_C3, NONE),
	/* C4  : NC */
	PAD_NC(GPP_C4, NONE),
	/* C5  : SML0ALERT# ==> GPP_C5_STRAP */
	PAD_NC(GPP_C5, NONE),
	/* C6  : SML1_SMBCLK */
	PAD_CFG_NF(GPP_C6, NONE, DEEP, NF1),
	/* C7  : SML1_SMBDATA */
	PAD_CFG_NF(GPP_C7, NONE, DEEP, NF1),

	/* D0  : ISH_GP0 ==> SOC_GSEN1_INT# */
	PAD_CFG_NF_LOCK(GPP_D0, NONE, NF1, LOCK_CONFIG),
	/* D1  : ISH_GP1 ==> SOC_GSEN2_INT# */
	PAD_CFG_NF_LOCK(GPP_D1, NONE, NF1, LOCK_CONFIG),
	/* D2  : ISH_GP2 ==> SEN_MODE_EC_PCH_INT_ODL */
	PAD_CFG_GPI_LOCK(GPP_D2, NONE, LOCK_CONFIG),
	/* D3  : NC */
	PAD_NC_LOCK(GPP_D3, NONE, LOCK_CONFIG),
	/* D4  : NC */
	PAD_NC(GPP_D4, NONE),
	/* D5  : NC */
	PAD_NC(GPP_D5, NONE),
	/* D6  : NC */
	PAD_NC(GPP_D6, NONE),
	/* D7  : GPP_D7 ==> CLKREQ_PCIE#2 */
	PAD_CFG_NF(GPP_D7, NONE, DEEP, NF1),
	/* D8  : SRCCLKREQ3# ==> NC */
	PAD_NC(GPP_D8, NONE),
	/* D9  : NC */
	PAD_NC_LOCK(GPP_D9, NONE, LOCK_CONFIG),
	/* D10 : ISH_SPI_CLK ==> GPP_D10_STRAP */
	PAD_NC_LOCK(GPP_D10, NONE, LOCK_CONFIG),
	/* D11 : NC */
	PAD_NC_LOCK(GPP_D11, NONE, LOCK_CONFIG),
	/* D12 : ISH_SPI_MOSI ==> GPP_D12_STRAP */
	PAD_NC_LOCK(GPP_D12, NONE, LOCK_CONFIG),
	/* D13 : UART0_ISH_RX_DBG_TX */
	PAD_CFG_NF_LOCK(GPP_D13, NONE, NF1, LOCK_CONFIG),
	/* D14 : UART0_ISH_TX_DBG_RX */
	PAD_CFG_NF_LOCK(GPP_D14, NONE, NF1, LOCK_CONFIG),
	/* D15  : GPP_D15 ==> SOC_TS_I2C_RST# */
	PAD_CFG_GPO_LOCK(GPP_D15, 1, LOCK_CONFIG),
	/* D16 : ISH_UART0_CTS# ==> SOC_TS_I2C_INT# */
	PAD_CFG_GPI_LOCK(GPP_D16, NONE, LOCK_CONFIG),
	/* D17 : NC */
	PAD_NC_LOCK(GPP_D17, NONE, LOCK_CONFIG),
	/* D18 : NC */
	PAD_NC_LOCK(GPP_D18, NONE, LOCK_CONFIG),
	/* D19 : I2S_MCLK1_OUT ==> NC */
	PAD_NC(GPP_D19, NONE),

	/* E0  : NC */
	PAD_NC(GPP_E0, NONE),
	/* E1  : THC0_SPI1_IO2 ==> RAM_ID1 */
	PAD_CFG_GPI_LOCK(GPP_E1, NONE, LOCK_CONFIG),
	/* E2  : THC0_SPI1_IO3 ==> RAM_ID0 */
	PAD_CFG_GPI_LOCK(GPP_E2, NONE, LOCK_CONFIG),
	/* E3  : PROC_GP0 ==> SOC_WP_OD */
	PAD_CFG_GPI_GPIO_DRIVER_LOCK(GPP_E3, NONE, LOCK_CONFIG),
	/* E4  : NC */
	PAD_NC(GPP_E4, NONE),
	/* E5  : NC */
	PAD_NC(GPP_E5, NONE),
	/* E6  : THC0_SPI1_RST# ==> GPP_E6_STRAP */
	PAD_NC_LOCK(GPP_E6, NONE, LOCK_CONFIG),
	/* E7  : NC */
	PAD_NC(GPP_E7, NONE),
	/* E8  : GPP_E8 ==> WLAN_DISABLE_L */
	PAD_CFG_GPO(GPP_E8, 1, DEEP),
	/* E9  : NC */
	PAD_NC_LOCK(GPP_E9, NONE, LOCK_CONFIG),
	/* E10 : NC */
	PAD_NC_LOCK(GPP_E10, NONE, LOCK_CONFIG),
	/* E11 : NC */
	PAD_NC_LOCK(GPP_E11, NONE, LOCK_CONFIG),
	/* E12 : THC0_SPI1_IO1 ==> RAM_ID2 */
	PAD_CFG_GPI_LOCK(GPP_E12, NONE, LOCK_CONFIG),
	/* E13 : NC */
	PAD_NC_LOCK(GPP_E13, NONE, LOCK_CONFIG),
	/* E14 : DDSP_HPDA ==> EDP_HPD */
	PAD_CFG_NF(GPP_E14, NONE, DEEP, NF1),
	/* E15 : NC */
	PAD_NC(GPP_E15, NONE),
	/* E16 : NC */
	PAD_NC(GPP_E16, NONE),
	/* E17  : GPP_E17 ==> SOC_TS_PWR_EN */
	PAD_CFG_GPO_LOCK(GPP_E17, 1, LOCK_CONFIG),
	/* E18 : NC */
	PAD_NC(GPP_E18, NONE),
	/* E19 : DDP1_CTRLDATA ==> GPP_E19_STRAP */
	PAD_NC(GPP_E19, NONE),
	/* E20 : NC */
	PAD_NC(GPP_E20, NONE),
	/* E21 : NC */
	PAD_NC(GPP_E21, NONE),
	/* E22 : DDPA_CTRLCLK ==> TCP0_DISP_AUX_P_BIAS */
	PAD_CFG_NF(GPP_E22, NONE, DEEP, NF6),
	/* E23 : DDPA_CTRLDATA ==> TCP0_DISP_AUX_N_BIAS */
	PAD_CFG_NF(GPP_E23, NONE, DEEP, NF6),

	/* F0  : CNV_BRI_DT ==> CNV_BRI_CTX_DRX */
	PAD_CFG_NF(GPP_F0, NONE, DEEP, NF1),
	/* F1  : CNV_BRI_RSP ==> CNV_BRI_CRX_DTX */
	PAD_CFG_NF(GPP_F1, UP_20K, DEEP, NF1),
	/* F2  : CNV_RGI_DT ==> CNV_RGI_CTX_DRX */
	PAD_CFG_NF(GPP_F2, NONE, DEEP, NF1),
	/* F3  : CNV_RGI_RSP ==> CNV_RGI_CRX_DTX */
	PAD_CFG_NF(GPP_F3, UP_20K, DEEP, NF1),
	/* F4  : CNV_RF_RESET# ==> CNV_RF_RESET# */
	PAD_CFG_NF(GPP_F4, NONE, DEEP, NF1),
	/* F5  : MODEM_CLKREQ ==> CLKREQ_CNV# */
	PAD_CFG_NF(GPP_F5, NONE, DEEP, NF2),
	/* F6  : CNV_PA_BLANKING ==> NC */
	PAD_NC(GPP_F6, NONE),
	/* F7  : GPP_F7 ==> GPP_F7_STRAP */
	PAD_NC(GPP_F7, NONE),
	/* F8  : Not available */
	PAD_NC(GPP_F8, NONE),
	/* F9  : Not available */
	PAD_NC(GPP_F9, NONE),
	/* F10 : GPP_F10 ==> GPP_F10_STRAP */
	PAD_NC(GPP_F10, NONE),
	/* F11 : NC */
	PAD_NC_LOCK(GPP_F11, NONE, LOCK_CONFIG),
	/* F12 : NC */
	PAD_NC_LOCK(GPP_F12, NONE, LOCK_CONFIG),
	/* F13 : NC */
	PAD_NC(GPP_F13, NONE),
	/* F14 : NC */
	PAD_NC(GPP_F14, NONE),
	/* F15 : NC */
	PAD_NC_LOCK(GPP_F15, NONE, LOCK_CONFIG),
	/* F16 : NC */
	PAD_NC_LOCK(GPP_F16, NONE, LOCK_CONFIG),
	/* F17 : THC1_SPI2_RST# ==> EC_SOC_WAKE_ODL */
	PAD_CFG_GPI_IRQ_WAKE_LOCK(GPP_F17, NONE, EDGE_SINGLE, INVERT, LOCK_CONFIG),
	/* F18 : THC1_SPI2_INT# ==> EC_IN_RW_OD */
	PAD_CFG_GPI_LOCK(GPP_F18, NONE, LOCK_CONFIG),
	/* F19 : Not available */
	PAD_NC(GPP_F19, NONE),
	/* F20 : Not available */
	PAD_NC(GPP_F20, NONE),
	/* F21 : Not available */
	PAD_NC(GPP_F21, NONE),
	/* F22 : NC */
	PAD_NC(GPP_F22, NONE),
	/* F23 : V1P05_CTRL ==> V1P05EXT_CTRL */
	PAD_CFG_NF(GPP_F23, NONE, DEEP, NF1),

	/* H0  : GPP_H0_STRAP */
	PAD_NC(GPP_H0, NONE),
	/* H1  : GPP_H1_STRAP */
	PAD_NC(GPP_H1, NONE),
	/* H2  : GPP_H2_STRAP */
	PAD_NC(GPP_H2, NONE),
	/* H3  : SX_EXIT_HOLDOFF# ==> WLAN_PCIE_WAKE_ODL */
	PAD_CFG_GPI_SCI_LOW_LOCK(GPP_H3, NONE, EDGE_SINGLE, LOCK_CONFIG),
	/* H4  : I2C0_SDA ==> SOC_I2C_GSC_SDA */
	PAD_CFG_NF_LOCK(GPP_H4, NONE, NF1, LOCK_CONFIG),
	/* H5  : I2C0_SCL ==> SOC_I2C_GSC_SCL */
	PAD_CFG_NF_LOCK(GPP_H5, NONE, NF1, LOCK_CONFIG),
	/* H6 : I2C1_SDA ==> I2C_1_SDA */
	PAD_CFG_NF(GPP_H6, NONE, DEEP, NF1),
	/* H7 : I2C1_SCL ==> I2C_1_SCL */
	PAD_CFG_NF(GPP_H7, NONE, DEEP, NF1),
	/* H8  : NC */
	PAD_NC(GPP_H8, NONE),
	/* H9  : NC */
	PAD_NC(GPP_H9, NONE),
	/* H10 : UART0_RXD ==> UART_2_CRXD_DTXD */
	PAD_CFG_NF(GPP_H10, NONE, DEEP, NF2),
	/* H11 : UART0_TXD ==> UART_2_CTXD_DRXD */
	PAD_CFG_NF(GPP_H11, NONE, DEEP, NF2),
	/* H12 : UART0_RTS# ==> NC */
	PAD_NC_LOCK(GPP_H12, NONE, LOCK_CONFIG),
	/* H13 : UART0_CTS# ==> NC */
	PAD_NC_LOCK(GPP_H13, NONE, LOCK_CONFIG),
	/* H14 : Not available */
	PAD_NC(GPP_H14, NONE),
	/* H15  : DDPB_CTRLCLK ==> SOC_DP2_CTRL_CLK */
	PAD_CFG_NF(GPP_H15, NONE, DEEP, NF1),
	/* H16 : Not available */
	PAD_NC(GPP_H16, NONE),
	/* H17  : DDPB_CTRLDATA ==> SOC_DP2_CTRL_DATA */
	PAD_CFG_NF(GPP_H17, NONE, DEEP, NF1),
	/* H18 : PROC_C10_GATE# ==> CPU_C10_GATE# */
	PAD_CFG_NF(GPP_H18, NONE, DEEP, NF1),
	/* H19 : NC */
	PAD_NC(GPP_H19, NONE),
	/* H20 : IMGCLKOUT1 ==> WLAN_PERST_L */
	PAD_CFG_GPO(GPP_H20, 1, DEEP),
	/* H21 : NC */
	PAD_NC(GPP_H21, NONE),
	/* H22 : NC */
	PAD_NC(GPP_H22, NONE),
	/* H23 : NC */
	PAD_NC(GPP_H23, NONE),

	/* R0  : HDA_BCLK ==> HDA_BIT_CLK */
	PAD_CFG_NF(GPP_R0, NONE, DEEP, NF1),
	/* R1  : HDA_SYNC ==> HDA_SYNC */
	PAD_CFG_NF(GPP_R1, NONE, DEEP, NF1),
	/* R2 : HDA_SDO ==> HDA_SDOUT */
	PAD_CFG_NF(GPP_R2, NONE, DEEP, NF1),
	/* R3 : HDA_SDI0 ==> HDA_SDIN0 */
	PAD_CFG_NF(GPP_R3, NONE, DEEP, NF1),
	/* R4 : HDA_RST# ==> HDA_RST# */
	PAD_CFG_NF(GPP_R4, NONE, DEEP, NF1),
	/* R5 : HDA_SDI1 ==> HDA_SDIN1 */
	PAD_CFG_NF(GPP_R5, NONE, DEEP, NF1),
	/* R6  : DMIC_CLK_A_1A ==> NC */
	PAD_NC(GPP_R6, NONE),
	/* R7  : DMIC_DATA_1A ==> NC */
	PAD_NC(GPP_R7, NONE),

	/* S0  : I2S_SPK_BCLK_R ==> NC */
	PAD_NC(GPP_S0, NONE),
	/* S1  : I2S_SPK_LRCK_R ==> NC */
	PAD_NC(GPP_S1, NONE),
	/* S2 : DMIC_CKL_A0 ==> PCH_DMIC_CLK */
	PAD_CFG_NF(GPP_S2, NONE, DEEP, NF2),
	/* S3 : DMIC_DATA0 ==> PCH_DMIC_DATA */
	PAD_CFG_NF(GPP_S3, NONE, DEEP, NF2),
	/* S4  : NC */
	PAD_NC(GPP_S4, NONE),
	/* S5  : NC */
	PAD_NC(GPP_S5, NONE),
	/* S6  : NC */
	PAD_NC(GPP_S6, NONE),
	/* S7  : NC */
	PAD_NC(GPP_S7, NONE),

	/* I5  : NC */
	PAD_NC(GPP_I5, NONE),
	/* I7  : EMMC_CMD ==> EMMC_CMD */
	PAD_CFG_NF(GPP_I7, NONE, DEEP, NF1),
	/* I8  : EMMC_DATA0 ==> EMMC_D0 */
	PAD_CFG_NF(GPP_I8, NONE, DEEP, NF1),
	/* I9  : EMMC_DATA1 ==> EMMC_D1 */
	PAD_CFG_NF(GPP_I9, NONE, DEEP, NF1),
	/* I10 : EMMC_DATA2 ==> EMMC_D2 */
	PAD_CFG_NF(GPP_I10, NONE, DEEP, NF1),
	/* I11 : EMMC_DATA3 ==> EMMC_D3 */
	PAD_CFG_NF(GPP_I11, NONE, DEEP, NF1),
	/* I12 : EMMC_DATA4 ==> EMMC_D4 */
	PAD_CFG_NF(GPP_I12, NONE, DEEP, NF1),
	/* I13 : EMMC_DATA5 ==> EMMC_D5 */
	PAD_CFG_NF(GPP_I13, NONE, DEEP, NF1),
	/* I14 : EMMC_DATA6 ==> EMMC_D6 */
	PAD_CFG_NF(GPP_I14, NONE, DEEP, NF1),
	/* I15 : EMMC_DATA7 ==> EMMC_D7 */
	PAD_CFG_NF(GPP_I15, NONE, DEEP, NF1),
	/* I16 : EMMC_RCLK ==> EMMC_RCLK */
	PAD_CFG_NF(GPP_I16, NONE, DEEP, NF1),
	/* I17 : EMMC_CLK ==> EMMC_CLK */
	PAD_CFG_NF(GPP_I17, NONE, DEEP, NF1),
	/* I18 : EMMC_RESET# ==> EMMC_RST_L */
	PAD_CFG_NF(GPP_I18, NONE, DEEP, NF1),

	/* GPD0  : BATLOW# ==> SOC_BATLOW_L */
	PAD_CFG_NF(GPD0, NONE, DEEP, NF1),
	/* GPD1  : ACPRESENT ==> SOC_ACPRESENT */
	PAD_CFG_NF(GPD1, NONE, DEEP, NF1),
	/* GPD2  : EC_SOC_INT_ODL */
	PAD_CFG_GPI_APIC(GPD2, NONE, PLTRST, LEVEL, INVERT),
	/* GPD3  : PWRBTN# ==> EC_SOC_PWR_BTN_ODL */
	PAD_CFG_NF(GPD3, NONE, DEEP, NF1),
	/* GPD4  : SLP_S3# ==> SLP_S3_L */
	PAD_CFG_NF(GPD4, NONE, DEEP, NF1),
	/* GPD5  : SLP_S4# ==> SLP_S4_L */
	PAD_CFG_NF(GPD5, NONE, DEEP, NF1),
	/* GPD6  : SLP_A# ==> NC */
	PAD_NC(GPD6, NONE),
	/* GPD7  : GPD7_STRAP */
	PAD_NC(GPD7, NONE),
	/* GPD8  : SUSCLK ==> PCH_SUSCLK */
	PAD_CFG_NF(GPD8, NONE, DEEP, NF1),
	/* GPD9  : NC */
	PAD_NC(GPD9, NONE),
	/* GPD10 : SLP_S5# ==> NC */
	PAD_NC(GPD10, NONE),
	/* GPD11 : NC */
	PAD_NC(GPD11, NONE),

	/* Configure the unused virtual CNVi Bluetooth UART pads to NC mode. */
	/* vCNV_BT_UART_TXD */
	PAD_NC(GPP_VGPIO_6, NONE),
	/* vCNV_BT_UART_RXD */
	PAD_NC(GPP_VGPIO_7, NONE),
	/* vCNV_BT_UART_CTS_B */
	PAD_NC(GPP_VGPIO_8, NONE),
	/* vCNV_BT_UART_RTS_B */
	PAD_NC(GPP_VGPIO_9, NONE),

	/* Configure the unused vUART for Bluetooth pads to NC mode. */
	/* vUART0_TXD */
	PAD_NC(GPP_VGPIO_18, NONE),
	/* vUART0_RXD */
	PAD_NC(GPP_VGPIO_19, NONE),
	/* vUART0_CTS_B */
	PAD_NC(GPP_VGPIO_20, NONE),
	/* vUART0_RTS_B */
	PAD_NC(GPP_VGPIO_21, NONE),
};

/* Early pad configuration in bootblock */
static const struct pad_config early_gpio_table[] = {
	/* A17 : GPP_A17 ==> GSC_SOC_INT_ODL */
	PAD_CFG_GPI_APIC(GPP_A17, NONE, PLTRST, LEVEL, INVERT),
	/* E3 : PROC_GP0 ==> SOC_WP_OD */
	PAD_CFG_GPI_GPIO_DRIVER(GPP_E3, NONE, DEEP),
	/* F18 : THC1_SPI2_INT# ==> EC_IN_RW_OD */
	PAD_CFG_GPI(GPP_F18, NONE, DEEP),
	/* H4  : I2C0_SDA ==> SOC_I2C_GSC_SDA */
	PAD_CFG_NF(GPP_H4, NONE, DEEP, NF1),
	/* H5  : I2C0_SCL ==> SOC_I2C_GSC_SCL */
	PAD_CFG_NF(GPP_H5, NONE, DEEP, NF1),
	/* H10 : UART0_RXD ==> UART_SOC_RX_DBG_TX */
	PAD_CFG_NF(GPP_H10, NONE, DEEP, NF2),
	/* H11 : UART0_TXD ==> UART_SOC_TX_DBG_RX */
	PAD_CFG_NF(GPP_H11, NONE, DEEP, NF2),
};

/* Fill romstage gpio configuration */
static const struct pad_config romstage_gpio_table[] = {
	/* Enable touchscreen, hold in reset */
	/* E17  : GPP_E17 ==> SOC_TS_PWR_EN */
	PAD_CFG_GPO(GPP_E17, 1, DEEP),
	/* D15  : GPP_D15 ==> SOC_TS_I2C_RST# */
	PAD_CFG_GPO(GPP_D15, 0, DEEP),
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
	CROS_GPIO_REC_AL(CROS_GPIO_VIRTUAL, CROS_GPIO_DEVICE_NAME),
	CROS_GPIO_WP_AH(GPIO_PCH_WP, CROS_GPIO_DEVICE_NAME),
};
DECLARE_CROS_GPIOS(cros_gpios);

const struct pad_config *variant_romstage_gpio_table(size_t *num)
{
	*num = ARRAY_SIZE(romstage_gpio_table);
	return romstage_gpio_table;
}
