/* SPDX-License-Identifier: GPL-2.0-only */

#include <cbfs.h>
#include <console/console.h>
#include <device/dram/ddr3.h>
#include <soc/pei_data.h>
#include <soc/pei_wrapper.h>
#include <spd.h>
#include <string.h>
#include <types.h>

#define SPD_DENSITY_BANKS	4
#define SPD_ADDRESSING		5
#define SPD_ORGANIZATION	7
#define SPD_BUS_DEV_WIDTH	8

static void print_spd_info(uint8_t spd[])
{
	const int spd_banks[8] = {  8, 16, 32, 64, -1, -1, -1, -1 };
	const int spd_capmb[8] = {  1,  2,  4,  8, 16, 32, 64,  0 };
	const int spd_rows[8]  = { 12, 13, 14, 15, 16, -1, -1, -1 };
	const int spd_cols[8]  = {  9, 10, 11, 12, -1, -1, -1, -1 };
	const int spd_ranks[8] = {  1,  2,  3,  4, -1, -1, -1, -1 };
	const int spd_devw[8]  = {  4,  8, 16, 32, -1, -1, -1, -1 };
	const int spd_busw[8]  = {  8, 16, 32, 64, -1, -1, -1, -1 };
	char spd_name[SPD_DDR3_PART_LEN + 1] = { 0 };

	int banks = spd_banks[(spd[SPD_DENSITY_BANKS] >> 4) & 7];
	int capmb = spd_capmb[spd[SPD_DENSITY_BANKS] & 7] * 256;
	int rows  = spd_rows[(spd[SPD_ADDRESSING] >> 3) & 7];
	int cols  = spd_cols[spd[SPD_ADDRESSING] & 7];
	int ranks = spd_ranks[(spd[SPD_ORGANIZATION] >> 3) & 7];
	int devw  = spd_devw[spd[SPD_ORGANIZATION] & 7];
	int busw  = spd_busw[spd[SPD_BUS_DEV_WIDTH] & 7];

	/* Module type */
	printk(BIOS_INFO, "SPD: module type is ");
	switch (spd[SPD_MEMORY_TYPE]) {
	case SPD_MEMORY_TYPE_SDRAM_DDR3:
		printk(BIOS_INFO, "DDR3\n");
		break;
	case SPD_MEMORY_TYPE_LPDDR3_INTEL:
		printk(BIOS_INFO, "LPDDR3\n");
		break;
	default:
		printk(BIOS_INFO, "Unknown (%02x)\n", spd[SPD_MEMORY_TYPE]);
		break;
	}

	/* Module Part Number */
	memcpy(spd_name, &spd[SPD_DDR3_PART_NUM], SPD_DDR3_PART_LEN);
	spd_name[SPD_DDR3_PART_LEN] = 0;
	printk(BIOS_INFO, "SPD: module part is %s\n", spd_name);

	printk(BIOS_INFO, "SPD: banks %d, ranks %d, rows %d, columns %d, "
	       "density %d Mb\n", banks, ranks, rows, cols, capmb);
	printk(BIOS_INFO, "SPD: device width %d bits, bus width %d bits\n",
	       devw, busw);

	if (capmb > 0 && busw > 0 && devw > 0 && ranks > 0) {
		/* SIZE = DENSITY / 8 * BUS_WIDTH / SDRAM_WIDTH * RANKS */
		printk(BIOS_INFO, "SPD: module size is %u MB (per channel)\n",
		       capmb / 8 * busw / devw * ranks);
	}
}

void copy_spd(struct pei_data *pei_data, struct spd_info *spdi)
{
	size_t spd_file_len;
	uint8_t *spd_file = cbfs_map("spd.bin", &spd_file_len);

	printk(BIOS_DEBUG, "SPD index %d\n", spdi->spd_index);

	if (!spd_file)
		die("SPD data not found.");

	if (spd_file_len < SPD_SIZE_MAX_DDR3)
		die("Missing SPD data.");

	if (spd_file_len < ((spdi->spd_index + 1) * SPD_SIZE_MAX_DDR3)) {
		printk(BIOS_ERR, "SPD index override to 0 - old hardware?\n");
		spdi->spd_index = 0;
	}

	uint8_t *const spd = spd_file + (spdi->spd_index * SPD_SIZE_MAX_DDR3);

	/* Make sure a valid SPD was found */
	if (spd[0] == 0)
		die("Invalid SPD data.");

	print_spd_info(spd);

	for (size_t i = 0; i < ARRAY_SIZE(spdi->addresses); i++) {
		if (spdi->addresses[i] == SPD_MEMORY_DOWN)
			memcpy(pei_data->spd_data[i / 2][i % 2], spd, SPD_SIZE_MAX_DDR3);
	}
}
