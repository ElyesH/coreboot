/* SPDX-License-Identifier: GPL-2.0-only */

/* It is derived from the x86info project, which is GPLv2-licensed. */

#include "coreinfo.h"

#if CONFIG(MODULE_CPUINFO)
#include <arch/rdtsc.h>

#define VENDOR_INTEL 0x756e6547
#define VENDOR_AMD   0x68747541
#define VENDOR_CYRIX 0x69727943
#define VENDOR_IDT   0x746e6543
#define VENDOR_GEODE 0x646f6547
#define VENDOR_RISE  0x52697365
#define VENDOR_RISE2 0x65736952
#define VENDOR_SIS   0x20536953

/* CPUID 0x00000001 EDX flags */
static const char *generic_cap_flags[] = {
	"fpu", "vme", "de", "pse", "tsc", "msr", "pae", "mce",
	"cx8", "apic", nullptr, "sep", "mtrr", "pge", "mca", "cmov",
	"pat", "pse36", "psn", "clflsh", nullptr, "ds", "acpi", "mmx",
	"fxsr", "sse", "sse2", "ss", "ht", "tm", nullptr, "pbe"
};

/* CPUID 0x00000001 ECX flags */
static const char *intel_cap_generic_ecx_flags[] = {
	"sse3", nullptr, nullptr, "monitor", "ds-cpl", "vmx", nullptr, "est",
	"tm2", "ssse3", "cntx-id", nullptr, nullptr, "cx16", "xTPR", nullptr,
	nullptr, nullptr, "dca", nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

/* CPUID 0x80000001 EDX flags */
static const char *intel_cap_extended_edx_flags[] = {
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, "SYSCALL", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, "xd", nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, "em64t", nullptr, nullptr,
};

/* CPUID 0x80000001 ECX flags */
static const char *intel_cap_extended_ecx_flags[] = {
	"lahf_lm", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
};

static const char *amd_cap_generic_ecx_flags[] = {
	"sse3", nullptr, nullptr, "mwait", nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, "cmpxchg16b", nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "popcnt",
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

static const char *amd_cap_extended_edx_flags[] = {
	"fpu", "vme", "de", "pse", "tsc", "msr", "pae", "mce",
	"cx8", "apic", nullptr, "sep", "mtrr", "pge", "mca", "cmov",
	"pat", "pse36", nullptr, "mp", "nx", nullptr, "mmxext", "mmx",
	"fxsr", "ffxsr", "page1gb", "rdtscp",
	nullptr, "lm", "3dnowext", "3dnow"
}; /* "mp" defined for CPUs prior to AMD family 0xf */

static const char *amd_cap_extended_ecx_flags[] = {
	"lahf/sahf", "CmpLegacy", "svm", "ExtApicSpace",
	"LockMovCr0", "abm", "sse4a", "misalignsse",
	"3dnowPref", "osvw", "ibs", nullptr, "skinit", "wdt", nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

static uint32_t vendor;
static unsigned int cpu_khz;

static void decode_flags(WINDOW *win, unsigned long reg, const char **flags,
			 int *row)
{
	int i;
	int lrow = *row;

	wmove(win, lrow, 2);

	for (i = 0; i < 32; i++) {
		if (flags[i] == nullptr)
			continue;

		if (reg & (1 << i))
			wprintw(win, "%s ", flags[i]);

		if (i && (i % 16) == 0) {
			lrow++;
			wmove(win, lrow, 2);
		}
	}

	*row = lrow;
}

static void get_features(WINDOW *win, int *row)
{
	uint32_t eax, ebx, ecx, edx;
	int lrow = *row;

	wmove(win, lrow++, 1);
	wprintw(win, "Features: ");

	docpuid(0x00000001, &eax, &ebx, &ecx, &edx);
	decode_flags(win, edx, generic_cap_flags, &lrow);

	lrow++;

	switch (vendor) {
	case VENDOR_AMD:
		wmove(win, lrow++, 1);
		wprintw(win, "AMD Extended Flags: ");
		decode_flags(win, ecx, amd_cap_generic_ecx_flags, &lrow);
		docpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		decode_flags(win, edx, amd_cap_extended_edx_flags, &lrow);
		decode_flags(win, ecx, amd_cap_extended_ecx_flags, &lrow);
		break;
	case VENDOR_INTEL:
		wmove(win, lrow++, 1);
		wprintw(win, "Intel Extended Flags: ");
		decode_flags(win, ecx, intel_cap_generic_ecx_flags, &lrow);
		docpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		decode_flags(win, edx, intel_cap_extended_edx_flags, &lrow);
		decode_flags(win, ecx, intel_cap_extended_ecx_flags, &lrow);
		break;
	}

	*row = lrow;
}

static void do_name(WINDOW *win, int row)
{
	char name[49], *p;
	uint32_t eax, ebx, ecx, edx;
	int t;

	p = name;

	for (uint32_t i = 0x80000002; i <= 0x80000004; i++) {
		docpuid(i, &eax, &ebx, &ecx, &edx);

		if (eax == 0)
			break;

		for (t = 0; t < 4; t++)
			*p++ = eax >> (8 * t);
		for (t = 0; t < 4; t++)
			*p++ = ebx >> (8 * t);
		for (t = 0; t < 4; t++)
			*p++ = ecx >> (8 * t);
		for (t = 0; t < 4; t++)
			*p++ = edx >> (8 * t);
	}

	mvwprintw(win, row, 1, "Processor: %s", name);
}

static int cpuinfo_module_redraw(WINDOW *win)
{
	uint32_t eax, ebx, ecx, edx;
	unsigned int brand;
	char *vstr;
	int row = 2;

	print_module_title(win, "CPU Information");

	docpuid(0, nullptr, &vendor, nullptr, nullptr);

	switch (vendor) {
	case VENDOR_INTEL:
		vstr = "Intel";
		break;
	case VENDOR_AMD:
		vstr = "AMD";
		break;
	case VENDOR_CYRIX:
		vstr = "Cyrix";
		break;
	case VENDOR_IDT:
		vstr = "IDT";
		break;
	case VENDOR_GEODE:
		vstr = "NatSemi Geode";
		break;
	case VENDOR_RISE:
	case VENDOR_RISE2:
		vstr = "RISE";
		break;
	case VENDOR_SIS:
		vstr = "SiS";
		break;
	default:
		vstr = "Unknown";
		break;
	}

	mvwprintw(win, row++, 1, "Vendor: %s", vstr);

	do_name(win, row++);

	docpuid(0x00000001, &eax, &ebx, &ecx, &edx);

	mvwprintw(win, row++, 1, "Family: %X", (eax >> 8) & 0x0f);
	mvwprintw(win, row++, 1, "Model: %X",
		  ((eax >> 4) & 0xf) | ((eax >> 16) & 0xf) << 4);

	mvwprintw(win, row++, 1, "Stepping: %X", eax & 0xf);

	if (vendor == VENDOR_AMD) {
		docpuid(0x80000001, &eax, &ebx, &ecx, &edx);
		brand = ((ebx >> 9) & 0x1f);

		mvwprintw(win, row++, 1, "Brand: %X", brand);
	}

	if (cpu_khz != 0)
		mvwprintw(win, row++, 1, "CPU Speed: %d MHz", cpu_khz / 1000);
	else
		mvwprintw(win, row++, 1, "CPU Speed: Error");

	row++;
	get_features(win, &row);

	return 0;
}

static unsigned int getticks(void)
{
	unsigned long long start, end;

	/* Read the number of ticks during the period. */
	start = rdtsc();
	mdelay(100);
	end = rdtsc();

	return (unsigned int)((end - start) / 100);
}

static int cpuinfo_module_init(void)
{
	cpu_khz = getticks();
	return 0;
}

struct coreinfo_module cpuinfo_module = {
	.name = "CPU Info",
	.init = cpuinfo_module_init,
	.redraw = cpuinfo_module_redraw,
};

#else

struct coreinfo_module cpuinfo_module = {
};

#endif
