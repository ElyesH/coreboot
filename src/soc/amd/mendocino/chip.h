/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef MENDOCINO_CHIP_H
#define MENDOCINO_CHIP_H

#include <amdblocks/chip.h>
#include <amdblocks/i2c.h>
#include <amdblocks/pci_clk_req.h>
#include <gpio.h>
#include <soc/i2c.h>
#include <soc/southbridge.h>
#include <drivers/i2c/designware/dw_i2c.h>
#include <types.h>
#include <vendorcode/amd/fsp/mendocino/FspUsb.h>

/* force USB3 port to gen1, bit0 - controller0 Port0, bit1 - Port1, etc */
union __packed usb3_force_gen1 {
		struct {
			uint8_t xhci0_port0:1;
			uint8_t xhci1_port0:1;
			uint8_t xhci1_port1:1;
		} ports;
		uint8_t usb3_port_force_gen1_en;
};

struct soc_amd_mendocino_config {
	struct soc_amd_common_config common_config;
	u8 i2c_scl_reset;
	struct dw_i2c_bus_config i2c[I2C_CTRLR_COUNT];
	struct i2c_pad_control i2c_pad[I2C_CTRLR_COUNT];

	/* Enable S0iX support */
	bool s0ix_enable;

	enum : uint8_t {
		DOWNCORE_AUTO = 0,
		DOWNCORE_1 = 1, /* Run with 1 physical core */
		DOWNCORE_2 = 3, /* Run with 2 physical cores */
		DOWNCORE_3 = 4, /* Run with 3 physical cores */
		DOWNCORE_4 = 6, /* Run with 4 physical cores */
		DOWNCORE_5 = 8, /* Run with 5 physical cores */
		DOWNCORE_6 = 9, /* Run with 6 physical cores */
		DOWNCORE_7 = 10, /* Run with 7 physical cores */
	} downcore_mode;
	bool disable_smt; /* disable second thread on all physical cores */

	uint8_t stt_control;
	uint8_t stt_pcb_sensor_count;
	uint16_t stt_min_limit;
	uint16_t stt_m1;
	uint16_t stt_m2;
	uint16_t stt_m3;
	uint16_t stt_m4;
	uint16_t stt_m5;
	uint16_t stt_m6;
	uint16_t stt_c_apu;
	uint16_t stt_c_gpu;
	uint16_t stt_c_hs2;
	uint16_t stt_alpha_apu;
	uint16_t stt_alpha_gpu;
	uint16_t stt_alpha_hs2;
	uint16_t stt_skin_temp_apu;
	uint16_t stt_skin_temp_gpu;
	uint16_t stt_skin_temp_hs2;
	uint16_t stt_error_coeff;
	uint16_t stt_error_rate_coefficient;

	/* Default */
	uint8_t stapm_boost;
	uint32_t stapm_time_constant_s;
	uint32_t apu_only_sppt_limit;
	uint32_t sustained_power_limit_mW;
	uint32_t fast_ppt_limit_mW;
	uint32_t slow_ppt_limit_mW;
	uint32_t slow_ppt_time_constant_s;
	uint32_t thermctl_limit_degreeC;
	uint32_t vrm_current_limit_mA;
	uint32_t vrm_maximum_current_limit_mA;
	uint32_t vrm_soc_current_limit_mA;
	/* Throttle (e.g., Low/No Battery) */
	uint32_t vrm_current_limit_throttle_mA;
	uint32_t vrm_maximum_current_limit_throttle_mA;
	uint32_t vrm_soc_current_limit_throttle_mA;

	/* tablet mode.*/
	uint16_t stt_m1_tablet;
	uint16_t stt_m2_tablet;
	uint16_t stt_c_apu_tablet;
	uint16_t stt_alpha_apu_tablet;

	/* Thermal profile B*/
	uint32_t fast_ppt_limit_mW_B;
	uint32_t slow_ppt_limit_mW_B;
	uint32_t slow_ppt_time_constant_s_B;
	uint16_t stt_min_limit_B;
	uint16_t stt_m1_B;
	uint16_t stt_m2_B;
	uint16_t stt_c_apu_B;
	uint16_t stt_skin_temp_apu_B;

	/* Thermal profile C*/
	uint32_t fast_ppt_limit_mW_C;
	uint32_t slow_ppt_limit_mW_C;
	uint32_t slow_ppt_time_constant_s_C;
	uint16_t stt_min_limit_C;
	uint16_t stt_m1_C;
	uint16_t stt_m2_C;
	uint16_t stt_c_apu_C;
	uint16_t stt_skin_temp_apu_C;

	/* Thermal profile D*/
	uint32_t fast_ppt_limit_mW_D;
	uint32_t slow_ppt_limit_mW_D;
	uint32_t slow_ppt_time_constant_s_D;
	uint16_t stt_min_limit_D;
	uint16_t stt_m1_D;
	uint16_t stt_m2_D;
	uint16_t stt_c_apu_D;
	uint16_t stt_skin_temp_apu_D;

	/* Thermal profile E*/
	uint32_t fast_ppt_limit_mW_E;
	uint32_t slow_ppt_limit_mW_E;
	uint32_t slow_ppt_time_constant_s_E;
	uint16_t stt_min_limit_E;
	uint16_t stt_m1_E;
	uint16_t stt_m2_E;
	uint16_t stt_c_apu_E;
	uint16_t stt_skin_temp_apu_E;


	/* Thermal profile F*/
	uint32_t fast_ppt_limit_mW_F;
	uint32_t slow_ppt_limit_mW_F;
	uint32_t slow_ppt_time_constant_s_F;
	uint16_t stt_min_limit_F;
	uint16_t stt_m1_F;
	uint16_t stt_m2_F;
	uint16_t stt_c_apu_F;
	uint16_t stt_skin_temp_apu_F;

	uint8_t smartshift_enable;

	uint8_t system_configuration;

	uint8_t cppc_ctrl;
	uint8_t cppc_perf_limit_max_range;
	uint8_t cppc_perf_limit_min_range;
	uint8_t cppc_epp_max_range;
	uint8_t cppc_epp_min_range;
	uint8_t cppc_preferred_cores;

	/* telemetry settings */
	uint32_t telemetry_vddcrvddfull_scale_current_mA;
	uint32_t telemetry_vddcrvddoffset;
	uint32_t telemetry_vddcrsocfull_scale_current_mA;
	uint32_t telemetry_vddcrsocoffset;

	/* The array index is the general purpose PCIe clock output number. Values in here
	   aren't the values written to the register to have the default to be always on. */
	enum gpp_clk_req gpp_clk_config[GPP_CLK_OUTPUT_AVAILABLE];

	/* performance policy for the PCIe links: power consumption vs. link speed */
	enum : uint8_t {
		DXIO_PSPP_DISABLED = 0,
		DXIO_PSPP_PERFORMANCE,
		DXIO_PSPP_BALANCED,
		DXIO_PSPP_POWERSAVE,
	} pspp_policy;

	bool usb_phy_custom;
	struct usb_phy_config usb_phy;
	/* Set for PCIe optimization w/a and a double confirming on the result of PCIe Signal
	   Integrity is highly recommended. */
	uint8_t dxio_tx_vboost_enable;

	/* Force USB3 port to gen1, bit0 - controller0 Port0, bit1 - Port1 */
	union usb3_force_gen1 usb3_port_force_gen1;

	/* Set for eDP power sequence adjustment timing T8 (from varybl to blon). */
	uint8_t edp_panel_t8_ms;
	/* Set for eDP power sequence adjustment timing T9 (from bloff to varybloff). */
	uint8_t edp_panel_t9_ms;

};

#endif /* MENDOCINO_CHIP_H */
