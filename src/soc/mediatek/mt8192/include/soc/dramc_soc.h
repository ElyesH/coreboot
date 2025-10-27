/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __SOC_MEDIATEK_DRAMC_SOC_H__
#define __SOC_MEDIATEK_DRAMC_SOC_H__

#include <soc/dramc_soc_common.h>

typedef enum : uint8_t {
	CHANNEL_A = 0,
	CHANNEL_B,
	CHANNEL_MAX
} DRAM_CHANNEL_T;

typedef enum : uint8_t {
	RANK_0 = 0,
	RANK_1,
	RANK_MAX
} DRAM_RANK_T;

typedef enum : uint8_t {
	DRAM_DFS_SHUFFLE_1 = 0,
	DRAM_DFS_SHUFFLE_2,
	DRAM_DFS_SHUFFLE_3,
	DRAM_DFS_SHUFFLE_4,
	DRAM_DFS_SHUFFLE_5,
	DRAM_DFS_SHUFFLE_6,
	DRAM_DFS_SHUFFLE_7,
	DRAM_DFS_SHUFFLE_MAX
} DRAM_DFS_SHUFFLE_TYPE_T; // DRAM SHUFFLE RG type

#define DRAM_DFS_SHU_MAX	DRAM_DFS_SHUFFLE_MAX

#endif	/* __SOC_MEDIATEK_DRAMC_SOC_H__ */
