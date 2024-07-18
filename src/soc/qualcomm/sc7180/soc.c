/* SPDX-License-Identifier: GPL-2.0-only */

#include <symbols.h>
#include <device/device.h>
#include <soc/mmu.h>
#include <soc/mmu_common.h>
#include <soc/symbols_common.h>
#include <soc/aop_common.h>

static void soc_read_resources(struct device *dev)
{
	void *start = nullptr;
	void *end = nullptr;

	ram_range(dev, 0, (uintptr_t)region_offset(ddr_region), region_sz(ddr_region));
	reserved_ram_range(dev, 1, (uintptr_t)_dram_aop, REGION_SIZE(dram_aop));
	reserved_ram_range(dev, 2, (uintptr_t)_dram_soc, REGION_SIZE(dram_soc));
	if (soc_modem_carve_out(&start, &end))
		reserved_ram_range(dev, 3, (uintptr_t)start, end - start);
}

static void soc_init(struct device *dev)
{
	aop_fw_load_reset();
}

static struct device_operations soc_ops = {
	.read_resources = soc_read_resources,
	.set_resources = noop_set_resources,
	.init = soc_init,
};

static void enable_soc_dev(struct device *dev)
{
	dev->ops = &soc_ops;
}

struct chip_operations soc_qualcomm_sc7180_ops = {
	.name = "SOC Qualcomm SC7180",
	.enable_dev = enable_soc_dev,
};
