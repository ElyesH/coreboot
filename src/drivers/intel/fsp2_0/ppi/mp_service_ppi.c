/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <cpu/cpu.h>
#include <cpu/x86/mp.h>
#include <cpu/intel/microcode.h>
#include <fsp/api.h>
#include <fsp/ppi/mp_service_ppi.h>
#include <intelblocks/cpulib.h>
#include <intelblocks/mp_init.h>
#include <types.h>

#define BSP_CPU_SLOT 0

struct efi_ap_procedure_caller_params {
	efi_ap_procedure procedure;
	void *argument;
};

static void efi_ap_procedure_caller(void *arg)
{
	struct efi_ap_procedure_caller_params *params =
		(struct efi_ap_procedure_caller_params *)arg;
	params->procedure(params->argument);
}

efi_return_status_t mp_get_number_of_processors(efi_uintn_t *number_of_processors,
	efi_uintn_t *number_of_enabled_processors)
{
	if (number_of_processors == nullptr || number_of_enabled_processors ==
			nullptr)
		return FSP_INVALID_PARAMETER;

	*number_of_processors = get_cpu_count();
	*number_of_enabled_processors = get_cpu_count();

	return FSP_SUCCESS;
}

efi_return_status_t mp_get_processor_info(efi_uintn_t processor_number,
	efi_processor_information *processor_info_buffer)
{
	if (processor_number >= MIN(get_cpu_count(), CONFIG_MAX_CPUS))
		return FSP_NOT_FOUND;

	extern struct cpu_info cpu_infos[];
	struct cpu_info *info = &cpu_infos[processor_number];
	if (!info)
		return FSP_DEVICE_ERROR;

	struct device *dev = info->cpu;

	if (processor_info_buffer == nullptr)
		return FSP_INVALID_PARAMETER;

	processor_info_buffer->StatusFlag = PROCESSOR_HEALTH_STATUS_BIT
			| PROCESSOR_ENABLED_BIT;

	if (processor_number == BSP_CPU_SLOT)
		processor_info_buffer->StatusFlag |= PROCESSOR_AS_BSP_BIT;

	processor_info_buffer->ProcessorId = dev->path.apic.apic_id;
	processor_info_buffer->Location.Package = dev->path.apic.package_id;
	processor_info_buffer->Location.Core = dev->path.apic.core_id;
	processor_info_buffer->Location.Thread = dev->path.apic.thread_id;

	return FSP_SUCCESS;
}

efi_return_status_t mp_startup_all_aps(efi_ap_procedure procedure,
	bool run_serial, efi_uintn_t timeout_usec, void *argument)
{
	struct efi_ap_procedure_caller_params params = {
		.procedure = procedure,
		.argument = argument
	};

	if (!cpu_info())
		return FSP_DEVICE_ERROR;

	if (procedure == nullptr)
		return FSP_INVALID_PARAMETER;

	if (mp_run_on_all_aps((void *)efi_ap_procedure_caller, &params,
			      timeout_usec, !run_serial) != CB_SUCCESS) {
		printk(BIOS_DEBUG, "%s: Exit with Failure\n", __func__);
		return FSP_NOT_STARTED;
	}

	return FSP_SUCCESS;
}

efi_return_status_t mp_startup_all_cpus(efi_ap_procedure procedure,
	efi_uintn_t timeout_usec, void *argument)
{
	struct efi_ap_procedure_caller_params params = {
		.procedure = procedure,
		.argument = argument
	};

	if (!cpu_info())
		return FSP_DEVICE_ERROR;

	if (procedure == nullptr)
		return FSP_INVALID_PARAMETER;

	/* Run on BSP */
	procedure(argument);

	/*
	 * Run on APs Serially
	 *
	 * FIXME: As per MP service specification, edk2 is allowed to specify the mode
	 * in which a 'func' routine should be executed on APs (i.e. execute serially
	 * or concurrently).
	 *
	 * MP service API `StartupAllCPUs` doesn't specify such requirement.
	 * Hence, running the `CpuCacheInfoCollectCoreAndCacheData`
	 * (UefiCpuPkg/Library/CpuCacheInfoLib/CpuCacheInfoLib.c#194)
	 * simultaneously on APs results in a coherency issue (hang while executing `func`)
	 * due to lack of acquiring a spin lock while accessing common data structure in
	 * multiprocessor environment.
	 */
	if (mp_run_on_all_aps((void *)efi_ap_procedure_caller,
			      &params, timeout_usec, false) != CB_SUCCESS) {
		printk(BIOS_DEBUG, "%s: Exit with Failure\n", __func__);
		return FSP_NOT_STARTED;
	}

	return FSP_SUCCESS;
}

efi_return_status_t mp_startup_this_ap(efi_ap_procedure procedure,
	efi_uintn_t processor_number, efi_uintn_t timeout_usec, void *argument)
{
	struct efi_ap_procedure_caller_params params = {
		.procedure = procedure,
		.argument = argument
	};

	if (!cpu_info())
		return FSP_DEVICE_ERROR;

	if (processor_number > get_cpu_count())
		return FSP_NOT_FOUND;

	if (processor_number == BSP_CPU_SLOT)
		return FSP_INVALID_PARAMETER;

	if (procedure == nullptr)
		return FSP_INVALID_PARAMETER;

	if (mp_run_on_aps((void *)efi_ap_procedure_caller, &params,
			processor_number, timeout_usec) != CB_SUCCESS) {
		printk(BIOS_DEBUG, "%s: Exit with Failure\n", __func__);
		return FSP_NOT_STARTED;
	}

	return FSP_SUCCESS;
}

efi_return_status_t mp_identify_processor(efi_uintn_t *processor_number)
{
	int index;

	if (processor_number == nullptr)
		return FSP_INVALID_PARAMETER;

	index = cpu_index();

	if (index < 0)
		return FSP_DEVICE_ERROR;

	*processor_number = index;

	return FSP_SUCCESS;
}
