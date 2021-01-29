/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <cpu/cpu.h>
#include <cpu/x86/cache.h>
#include <cpu/x86/lapic.h>
#include <cpu/x86/msr.h>
#include <device/device.h>

static void configure_mca(void)
{
	msr_t msr;
	int i;
	int num_banks;

	msr = rdmsr(IA32_MCG_CAP);
	num_banks = msr.lo & 0xff;
	msr.lo = msr.hi = 0;
	/* TODO(adurbin): This should only be done on a cold boot. Also, some
	 * of these banks are core vs package scope. For now every CPU clears
	 * every bank. */
	for (i = 0; i < num_banks; i++)
		wrmsr(IA32_MC0_STATUS + (i * 4), msr);
}


static void configure_misc(void)
{
	msr_t msr;

	msr = rdmsr(IA32_MISC_ENABLE);
	msr.lo |= (1 << 3);	/* TM1 enable */
	msr.lo |= (1 << 10);	/* FERR# multiplexing */
	/* FIXME:SpeedStep Enable - here it hang, but it shouldn't */
	//msr.lo |= (1 << 16);
	wrmsr(IA32_MISC_ENABLE, msr);
}

static void model_f6x_init(struct device *cpu)
{

	/* Clear out pending MCEs */
	configure_mca();

	/* Turn on caching if we haven't already */
	x86_enable_cache();

	/* Enable the local CPU APICs */
	setup_lapic();

	/* Configure Enhanced SpeedStep and Thermal Sensors */
	configure_misc();
};

static struct device_operations cpu_dev_ops = {
	.init = model_f6x_init,
};

static const struct cpu_device_id cpu_table[] = {
	{ X86_VENDOR_INTEL, 0x0f62 }, /* Not tested */
	{ X86_VENDOR_INTEL, 0x0f64 }, /* Not tested */
	{ X86_VENDOR_INTEL, 0x0f65 },
	{ X86_VENDOR_INTEL, 0x0f68 }, /* Not tested */
	{ 0, 0 },
};

static const struct cpu_driver model_f6x __cpu_driver = {
	.ops      = &cpu_dev_ops,
	.id_table = cpu_table,
};
