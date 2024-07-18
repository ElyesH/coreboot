/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <amdblocks/acpi.h>
#include <amdblocks/smm.h>
#include <bootstate.h>
#include <console/console.h>
#include <cpu/x86/mp.h>
#include <cpu/x86/msr.h>
#include <types.h>

static void late_smm_lock(void *unused)
{
	/* Finalize SMM settings */
	if (is_smm_locked()) /* Skip if already locked, avoid GPF */
		return;

	if (CONFIG(HAVE_SMI_HANDLER))
		tseg_valid();

	lock_smm();
}

static void late_smm_finalize(void)
{
	printk(BIOS_SPEW, "Lock SMM configuration\n");
	if (mp_run_on_all_cpus(late_smm_lock, nullptr) != CB_SUCCESS)
		printk(BIOS_WARNING, "Failed to finalize all cores\n");
}

static void soc_finalize(void *unused)
{
	if (CONFIG(SOC_AMD_COMMON_LATE_SMM_LOCKING))
		late_smm_finalize();

	if (!acpi_is_wakeup_s3()) {
		acpi_clear_pm_gpe_status();

		if (CONFIG(HAVE_SMI_HANDLER))
			acpi_disable_sci();
		else
			acpi_enable_sci();
	}

	post_code(POSTCODE_OS_BOOT);
}

BOOT_STATE_INIT_ENTRY(BS_OS_RESUME, BS_ON_ENTRY, soc_finalize, nullptr);
BOOT_STATE_INIT_ENTRY(BS_PAYLOAD_LOAD, BS_ON_EXIT, soc_finalize, nullptr);
