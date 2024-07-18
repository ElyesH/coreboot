/* SPDX-License-Identifier: GPL-2.0-only OR MIT */

/*
 * This file is created based on MT8188 Functional Specification
 * Chapter number: 5.2
 */

#include <device/mmio.h>
#include <gpio.h>

#define SPI_AP_GPIO_BASE	125

enum {
	SPI_AP_CLK_ROM = 0,
	SPI_AP_CS_ROM_L = 1,
	SPI_AP_DO_ROM_DI = 2,
	SPI_AP_DI_ROM_DO = 3,
};

static const struct gpio_drv_info bootblock_gpio_driving_info[] = {
	[SPI_AP_CLK_ROM] = { 0x20, 24, 3, },
	[SPI_AP_CS_ROM_L] = { 0x20, 24, 3, },
	[SPI_AP_DO_ROM_DI] = { 0x20, 24, 3, },
	[SPI_AP_DI_ROM_DO] = { 0x20, 27, 3, },
};

static const struct gpio_drv_info gpio_driving_info[] = {
	[0] = { 0x00, 24, 3, },
	[1] = { 0x00, 27, 3, },
	[2] = { 0x10, 0, 3, },
	[3] = { 0x10, 3, 3, },
	[4] = { 0x20, 9, 3, },
	[5] = { 0x20, 9, 3, },
	[6] = { 0x20, 9, 3, },
	[7] = { 0x10, 6, 3, },
	[8] = { 0x10, 9, 3, },
	[9] = { 0x10, 12, 3, },
	[10] = { 0x10, 15, 3, },
	[11] = { 0x20, 12, 3, },
	[12] = { 0x10, 24, 3, },
	[13] = { 0x10, 27, 3, },
	[14] = { 0x20, 0, 3, },
	[15] = { 0x20, 3, 3, },
	[16] = { 0x10, 15, 3, },
	[17] = { 0x10, 15, 3, },
	[18] = { 0x00, 27, 3, },
	[19] = { 0x00, 27, 3, },
	[20] = { 0x00, 27, 3, },
	[21] = { 0x00, 27, 3, },
	[22] = { 0x00, 0, 3, },
	[23] = { 0x00, 3, 3, },
	[24] = { 0x00, 6, 3, },
	[25] = { 0x20, 6, 3, },
	[26] = { 0x20, 6, 3, },
	[27] = { 0x20, 6, 3, },
	[28] = { 0x20, 9, 3, },
	[29] = { 0x20, 3, 3, },
	[30] = { 0x20, 6, 3, },
	[31] = { 0x20, 12, 3, },
	[32] = { 0x20, 12, 3, },
	[33] = { 0x20, 15, 3, },
	[34] = { 0x20, 15, 3, },
	[35] = { 0x20, 12, 3, },
	[36] = { 0x20, 15, 3, },
	[37] = { 0x10, 27, 3, },
	[38] = { 0x10, 18, 3, },
	[39] = { 0x10, 21, 3, },
	[40] = { 0x10, 24, 3, },
	[41] = { 0x20, 0, 3, },
	[42] = { 0x20, 18, 3, },
	[43] = { 0x20, 18, 3, },
	[44] = { 0x20, 18, 3, },
	[45] = { 0x20, 21, 3, },
	[46] = { 0x10, 15, 3, },
	[47] = { 0x20, 3, 3, },
	[48] = { 0x20, 3, 3, },
	[49] = { 0x20, 3, 3, },
	[50] = { 0x00, 6, 3, },
	[51] = { 0x00, 3, 3, },
	[52] = { 0x00, 0, 3, },
	[53] = { 0x00, 9, 3, },
	[54] = { 0x00, 12, 3, },
	[55] = { 0x20, 27, 3, },
	[56] = { 0x30, 6, 3, },
	[57] = { 0x30, 9, 3, },
	[58] = { 0x30, 15, 3, },
	[59] = { 0x30, 0, 3, },
	[60] = { 0x30, 9, 3, },
	[61] = { 0x30, 3, 3, },
	[62] = { 0x30, 12, 3, },
	[63] = { 0x30, 12, 3, },
	[64] = { 0x30, 18, 3, },
	[65] = { 0x10, 0, 3, },
	[66] = { 0x10, 6, 3, },
	[67] = { 0x10, 3, 3, },
	[68] = { 0x10, 9, 3, },
	[69] = { 0x30, 18, 3, },
	[70] = { 0x30, 15, 3, },
	[71] = { 0x40, 0, 3, },
	[72] = { 0x30, 27, 3, },
	[73] = { 0x30, 21, 3, },
	[74] = { 0x30, 24, 3, },
	[75] = { 0x40, 6, 3, },
	[76] = { 0x40, 3, 3, },
	[77] = { 0x40, 12, 3, },
	[78] = { 0x40, 9, 3, },
	[79] = { 0x10, 15, 3, },
	[80] = { 0x10, 12, 3, },
	[81] = { 0x10, 21, 3, },
	[82] = { 0x10, 18, 3, },
	[83] = { 0x30, 0, 3, },
	[84] = { 0x20, 27, 3, },
	[85] = { 0x30, 0, 3, },
	[86] = { 0x20, 6, 3, },
	[87] = { 0x20, 6, 3, },
	[88] = { 0x20, 6, 3, },
	[89] = { 0x20, 6, 3, },
	[90] = { 0x30, 0, 3, },
	[91] = { 0x30, 0, 3, },
	[92] = { 0x20, 9, 3, },
	[93] = { 0x20, 9, 3, },
	[94] = { 0x20, 9, 3, },
	[95] = { 0x20, 9, 3, },
	[96] = { 0x20, 21, 3, },
	[97] = { 0x20, 21, 3, },
	[98] = { 0x20, 24, 3, },
	[99] = { 0x20, 21, 3, },
	[100] = { 0x30, 6, 3, },
	[101] = { 0x00, 0, 3, },
	[102] = { 0x00, 15, 3, },
	[103] = { 0x00, 9, 3, },
	[104] = { 0x00, 12, 3, },
	[105] = { 0x00, 3, 3, },
	[106] = { 0x00, 6, 3, },
	[107] = { 0x20, 6, 3, },
	[108] = { 0x20, 6, 3, },
	[109] = { 0x20, 6, 3, },
	[110] = { 0x20, 6, 3, },
	[111] = { 0x20, 15, 3, },
	[112] = { 0x20, 15, 3, },
	[113] = { 0x20, 15, 3, },
	[114] = { 0x20, 12, 3, },
	[115] = { 0x20, 12, 3, },
	[116] = { 0x20, 12, 3, },
	[117] = { 0x20, 12, 3, },
	[118] = { 0x20, 12, 3, },
	[119] = { 0x20, 15, 3, },
	[120] = { 0x20, 18, 3, },
	[121] = { 0x10, 3, 3, },
	[122] = { 0x10, 12, 3, },
	[123] = { 0x10, 9, 3, },
	[124] = { 0x10, 6, 3, },
	[125] = { 0x20, 24, 3, },
	[126] = { 0x20, 24, 3, },
	[127] = { 0x20, 24, 3, },
	[128] = { 0x20, 27, 3, },
	[129] = { 0x20, 27, 3, },
	[130] = { 0x20, 27, 3, },
	[131] = { 0x00, 0, 3, },
	[132] = { 0x00, 15, 3, },
	[133] = { 0x00, 18, 3, },
	[134] = { 0x00, 21, 3, },
	[135] = { 0x20, 15, 3, },
	[136] = { 0x20, 18, 3, },
	[137] = { 0x20, 18, 3, },
	[138] = { 0x20, 18, 3, },
	[139] = { 0x20, 18, 3, },
	[140] = { 0x20, 21, 3, },
	[141] = { 0x20, 21, 3, },
	[142] = { 0x20, 21, 3, },
	[143] = { 0x00, 3, 3, },
	[144] = { 0x00, 6, 3, },
	[145] = { 0x00, 9, 3, },
	[146] = { 0x00, 12, 3, },
	[147] = { 0x20, 21, 3, },
	[148] = { 0x20, 24, 3, },
	[149] = { 0x20, 24, 3, },
	[150] = { 0x20, 24, 3, },
	[151] = { 0x10, 15, 3, },
	[152] = { 0x10, 12, 3, },
	[153] = { 0x10, 9, 3, },
	[154] = { 0x10, 6, 3, },
	[155] = { 0x10, 21, 3, },
	[156] = { 0x00, 21, 3, },
	[157] = { 0x00, 18, 3, },
	[158] = { 0x10, 3, 3, },
	[159] = { 0x10, 0, 3, },
	[160] = { 0x00, 27, 3, },
	[161] = { 0x00, 24, 3, },
	[162] = { 0x10, 18, 3, },
	[163] = { 0x00, 12, 3, },
	[164] = { 0x00, 9, 3, },
	[165] = { 0x00, 15, 3, },
	[166] = { 0x00, 18, 3, },
	[167] = { 0x00, 21, 3, },
	[168] = { 0x00, 24, 3, },
	[169] = { 0x00, 18, 3, },
	[170] = { 0x00, 15, 3, },
	[171] = { 0x00, 21, 3, },
	[172] = { 0x00, 24, 3, },
	[173] = { 0x00, 27, 3, },
	[174] = { 0x10, 0, 3, },
	[175] = { 0x30, 3, 3, },
	[176] = { 0x30, 3, 3, },
};

_Static_assert(ARRAY_SIZE(gpio_driving_info) == GPIO_NUM,
	       "gpio_driving_info array size not match");

/* Unimplemented GPIOs are intentionally omitted here with width=0 */
static const struct gpio_drv_info gpio_driving_adv_info[] = {
	[53] = { 0x20, 0, 3, },
	[54] = { 0x20, 3, 3, },
	[55] = { 0x60, 0, 3, },
	[56] = { 0x60, 9, 3, },
	[57] = { 0x50, 0, 3, },
	[58] = { 0x50, 6, 3, },
	[59] = { 0x60, 3, 3, },
	[60] = { 0x60, 12, 3, },
	[61] = { 0x60, 6, 3, },
	[62] = { 0x60, 15, 3, },
	[63] = { 0x50, 3, 3, },
	[64] = { 0x50, 9, 3, },
	[65] = { 0x30, 0, 3, },
	[66] = { 0x30, 6, 3, },
	[67] = { 0x30, 3, 3, },
	[68] = { 0x30, 9, 3, },
	[175] = { 0x50, 12, 3, },
	[176] = { 0x50, 15, 3, },
};

void *gpio_find_reg_addr(gpio_t gpio)
{
	void *reg_addr;
	switch (gpio.base & 0x0f) {
	case 1:
		reg_addr = (void *)IOCFG_RM_BASE;
		break;
	case 2:
		reg_addr = (void *)IOCFG_LT_BASE;
		break;
	case 3:
		reg_addr = (void *)IOCFG_LM_BASE;
		break;
	case 4:
		reg_addr = (void *)IOCFG_RT_BASE;
		break;
	default:
		reg_addr = nullptr;
		break;
	}

	return reg_addr;
}

const struct gpio_drv_info *get_gpio_driving_info(uint32_t raw_id)
{
	if (ENV_BOOTBLOCK) {
		uint32_t id = raw_id - SPI_AP_GPIO_BASE;

		if (id >= ARRAY_SIZE(bootblock_gpio_driving_info))
			return nullptr;
		return &bootblock_gpio_driving_info[id];
	} else {
		if (raw_id >= ARRAY_SIZE(gpio_driving_info))
			return nullptr;
		return &gpio_driving_info[raw_id];
	}
}

const struct gpio_drv_info *get_gpio_driving_adv_info(uint32_t raw_id)
{
	if (ENV_BOOTBLOCK) {
		return nullptr;
	} else {
		if (raw_id >= ARRAY_SIZE(gpio_driving_adv_info))
			return nullptr;
		return &gpio_driving_adv_info[raw_id];
	}
}
