/* SPDX-License-Identifier: GPL-2.0-only */

#include <baseboard/variants.h>
#include <boardid.h>
#include <gpio.h>
#include <soc/meminit.h>
#include <variant/gpio.h>

const struct lpddr4_swizzle_cfg baseboard_lpddr4_swizzle = {
	/* CH0_DQA[0:31] SoC pins -> U22 LPDDR4 module pins */
	.phys[LP4_PHYS_CH0A] = {
		/* DQA[0:7] pins of LPDDR4 module. */
		.dqs[LP4_DQS0] = { 4, 6, 7, 5, 3, 2, 1, 0 },
		/* DQA[8:15] pins of LPDDR4 module. */
		.dqs[LP4_DQS1] = { 12, 15, 13, 8, 9, 10, 11, 14 },
		/* DQB[0:7] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS2] = { 17, 18, 19, 16, 23, 20, 21, 22 },
		/* DQB[7:15] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS3] = { 30, 31, 25, 27, 26, 29, 28, 24 },
	},
	.phys[LP4_PHYS_CH0B] = {
		/* DQA[0:7] pins of LPDDR4 module. */
		.dqs[LP4_DQS0] = { 1, 3, 2, 0, 5, 4, 6, 7 },
		/* DQA[8:15] pins of LPDDR4 module. */
		.dqs[LP4_DQS1] = { 15, 14, 13, 12, 8, 9, 11, 10 },
		/* DQB[0:7] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS2] = { 20, 21, 22, 16, 23, 17, 18, 19 },
		/* DQB[7:15] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS3] = { 30, 26, 24, 25, 28, 29, 31, 27 },
	},
	.phys[LP4_PHYS_CH1A] = {
		/* DQA[0:7] pins of LPDDR4 module. */
		.dqs[LP4_DQS0] = { 15, 14, 13, 12, 8, 9, 10, 11 },
		/* DQA[8:15] pins of LPDDR4 module. */
		.dqs[LP4_DQS1] = { 7, 6, 5, 0, 4, 2, 1, 3 },
		/* DQB[0:7] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS2] = { 20, 21, 23, 22, 19, 17, 18, 16 },
		/* DQB[7:15] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS3] = { 24, 27, 26, 30, 25, 31, 28, 29 },
	},
	.phys[LP4_PHYS_CH1B] = {
		/* DQA[0:7] pins of LPDDR4 module. */
		.dqs[LP4_DQS0] = { 0, 4, 7, 1, 6, 5, 3, 2 },
		/* DQA[8:15] pins of LPDDR4 module. */
		.dqs[LP4_DQS1] = { 11, 12, 13, 15, 10, 9, 8, 14 },
		/* DQB[0:7] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS2] = { 19, 21, 17, 16, 22, 23, 18, 20 },
		/* DQB[7:15] pins of LPDDR4 module with offset of 16. */
		.dqs[LP4_DQS3] = { 30, 26, 25, 24, 31, 29, 28, 27 },
	},
};

static const struct lpddr4_sku non_cbi_skus[] = {
	/*
	 * K4F6E304HB-MGCJ - both logical channels While the parts
	 * are listed at 16Gb there are 2 ranks per channel so indicate
	 * the density as 8Gb per rank.
	 */
	[0] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.ch0_dual_rank = 1,
		.ch1_dual_rank = 1,
		.part_num = "K4F6E304HB-MGCJ",
	},
	/* K4F8E304HB-MGCJ - both logical channels  */
	[1] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.part_num = "K4F8E304HB-MGCJ",
	},
	/*
	 * MT53B512M32D2NP-062WT:C - both logical channels. While the parts
	 * are listed at 16Gb there are 2 ranks per channel so indicate
	 * the density as 8Gb per rank.
	 */
	[2] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.ch0_dual_rank = 1,
		.ch1_dual_rank = 1,
		.part_num = "MT53B512M32D2NP",
	},
	/* MT53B256M32D1NP-062 WT:C - both logical channels */
	[3] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.part_num = "MT53B256M32D1NP",
	},
	/*
	 * H9HCNNNBPUMLHR-NLE - both logical channels. While the parts
	 * are listed at 16Gb there are 2 ranks per channel so indicate the
	 * density as 8Gb per rank.
	 */
	[4] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.ch0_dual_rank = 1,
		.ch1_dual_rank = 1,
		.part_num = "H9HCNNNBPUMLHR",
	},
	/* H9HCNNN8KUMLHR-NLE - both logical channels */
	[5] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.part_num = "H9HCNNN8KUMLHR",
	},
	/* K4F6E3S4HM-MGCJ - both logical channels */
	[6] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_16Gb_DENSITY,
		.ch1_rank_density = LP4_16Gb_DENSITY,
		.part_num = "K4F6E3S4HM-MGCJ",
	},
	/* MT53E512M32D2NP-046 - both logical channels */
	[7] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_16Gb_DENSITY,
		.ch1_rank_density = LP4_16Gb_DENSITY,
		.part_num = "MT53E512M32D2NP",
	},
};

static const struct lpddr4_cfg non_cbi_lp4cfg = {
	.skus = non_cbi_skus,
	.num_skus = ARRAY_SIZE(non_cbi_skus),
	.swizzle_config = &baseboard_lpddr4_swizzle,
};

static const struct lpddr4_sku cbi_skus[] = {
	/* Dual Channel Config 4GiB System Capacity */
	[0] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
	},
	/* Dual Channel Config 8GiB System Capacity */
	[1] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_16Gb_DENSITY,
		.ch1_rank_density = LP4_16Gb_DENSITY,
	},
	/* Dual Channel Config 8GiB System Capacity */
	[2] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch1_rank_density = LP4_8Gb_DENSITY,
		.ch0_dual_rank = 1,
		.ch1_dual_rank = 1,
	},
	/* Single Channel Configs 4GiB System Capacity Ch0 populated. */
	[3] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_16Gb_DENSITY,
	},
	/* Single Channel Configs 4GiB System Capacity Ch0 populated. */
	[4] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_8Gb_DENSITY,
		.ch0_dual_rank = 1,
	},
	/* Dual Channel / Dual Rank Config 4GiB System Capacity */
	[5] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_4Gb_DENSITY,
		.ch1_rank_density = LP4_4Gb_DENSITY,
		.ch0_dual_rank = 1,
		.ch1_dual_rank = 1,
	},
	/* Dual Channel Config 6GiB System Capacity */
	[7] = {
		.speed = LP4_SPEED_2400,
		.ch0_rank_density = LP4_12Gb_DENSITY,
		.ch1_rank_density = LP4_12Gb_DENSITY,
	},
};

static const struct lpddr4_cfg cbi_lp4cfg = {
	.skus = cbi_skus,
	.num_skus = ARRAY_SIZE(cbi_skus),
	.swizzle_config = &baseboard_lpddr4_swizzle,
};

const struct lpddr4_cfg *__weak variant_lpddr4_config(void)
{
	if (CONFIG(DRAM_PART_NUM_NOT_ALWAYS_IN_CBI)) {
		/* Fall back non cbi memory config. */
		if ((int)board_id() < CONFIG_DRAM_PART_IN_CBI_BOARD_ID_MIN)
			return &non_cbi_lp4cfg;
	}

	return &cbi_lp4cfg;
}

size_t __weak variant_memory_sku(void)
{
	gpio_t pads[] = {
		[3] = MEM_CONFIG3, [2] = MEM_CONFIG2,
		[1] = MEM_CONFIG1, [0] = MEM_CONFIG0,
	};

	return gpio_base2_value(pads, ARRAY_SIZE(pads));
}
