/* SPDX-License-Identifier: GPL-2.0-only */

/* TODO: Update for Glinda */

#include <amdblocks/cpu.h>
#include <amdblocks/mca.h>
#include <cpu/amd/microcode.h>
#include <cpu/cpu.h>
#include <device/device.h>
#include <smbios.h>
#include <soc/cpu.h>

_Static_assert(CONFIG_MAX_CPUS == 24, "Do not override MAX_CPUS. To reduce the number of "
	"available cores, use the downcore_mode and disable_smt devicetree settings instead.");

unsigned int smbios_processor_external_clock(void)
{
	return 100; // 100 MHz
}

unsigned int smbios_cache_error_correction_type(u8 level)
{
	return SMBIOS_CACHE_ERROR_CORRECTION_MULTI_BIT;
}

unsigned int smbios_cache_conf_operation_mode(u8 level)
{
	return SMBIOS_CACHE_OP_MODE_WRITE_BACK;
}

unsigned int smbios_cache_sram_type(void)
{
	return SMBIOS_CACHE_SRAM_TYPE_PIPELINE_BURST;
}

static void zen_2_3_init(struct device *dev)
{
	check_mca();
	set_cstate_io_addr();

	amd_apply_microcode_patch();
}

static struct device_operations cpu_dev_ops = {
	.init = zen_2_3_init,
};

static struct cpu_device_id cpu_table[] = {
	{ X86_VENDOR_AMD, GLINDA_A0_CPUID, CPUID_ALL_STEPPINGS_MASK },
	{ X86_VENDOR_AMD, GLINDA_B0_CPUID, CPUID_ALL_STEPPINGS_MASK },
	CPU_TABLE_END
};

static const struct cpu_driver zen_2_3 __cpu_driver = {
	.ops      = &cpu_dev_ops,
	.id_table = cpu_table,
};
