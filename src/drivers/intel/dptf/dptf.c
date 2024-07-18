/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpigen.h>
#include <acpi/acpigen_pci.h>
#include <console/console.h>
#include <device/device.h>
#include <intelblocks/pmc_ipc.h>
#include <stdio.h>
#include <soc/dptf.h>
#include <soc/pci_devs.h>
#include "chip.h"
#include "dptf.h"

/* Generic DPTF participants have a PTYP field to distinguish them */
enum dptf_generic_participant_type {
	DPTF_GENERIC_PARTICIPANT_TYPE_TSR	= 0x3,
	DPTF_GENERIC_PARTICIPANT_TYPE_FAN	= 0x4,
	DPTF_GENERIC_PARTICIPANT_TYPE_TPCH	= 0x5,
	DPTF_GENERIC_PARTICIPANT_TYPE_CHARGER	= 0xB,
	DPTF_GENERIC_PARTICIPANT_TYPE_BATTERY	= 0xC,
	DPTF_GENERIC_PARTICIPANT_TYPE_POWER	= 0x11,
};

#define DEFAULT_CHARGER_STR		"Battery Charger"
#define DEFAULT_TPCH_STR		"Intel PCH FIVR Participant"
#define DEFAULT_POWER_STR		"Power Participant"
#define DEFAULT_BATTERY_STR		"Battery Participant"
#define DEFAULT_FAN_STR			"Fan Participant"

#define PMC_IPC_COMMAND_FIVR_SIZE	0x8

/*
 * Helper method to determine if a device is "used" (called out anywhere as a source or a target
 * of any policies, and therefore should be included in the ACPI tables.
 */
static bool is_participant_used(const struct drivers_intel_dptf_config *config,
				enum dptf_participant participant)
{
	int i;

	/* Active? */
	for (i = 0; i < DPTF_MAX_ACTIVE_POLICIES; ++i)
		if (config->policies.active[i].target == participant)
			return true;

	/* Passive? */
	for (i = 0; i < DPTF_MAX_PASSIVE_POLICIES; ++i)
		if (config->policies.passive[i].source == participant ||
		    config->policies.passive[i].target == participant)
			return true;

	/* Critical? */
	for (i = 0; i < DPTF_MAX_CRITICAL_POLICIES; ++i)
		if (config->policies.critical[i].source == participant)
			return true;

	/* Check fan as well (its use is implicit in the Active policy) */
	if ((participant == DPTF_FAN || participant == DPTF_FAN_2) &&
		config->policies.active[0].target != DPTF_NONE)
		return true;

	return false;
}

/* Return the assigned namestring of the FAN participant */
static const char *fan_namestring_of(enum dptf_participant participant)
{
	switch (participant) {
	case DPTF_FAN:
		return "TFN1";
	case DPTF_FAN_2:
		return "TFN2";
	default:
		return "";
	}
}

static const char *dptf_acpi_name(const struct device *dev)
{
	return "DPTF";
}

static int get_STA_value(const struct drivers_intel_dptf_config *config,
			 enum dptf_participant participant)
{
	return is_participant_used(config, participant) ?
		ACPI_STATUS_DEVICE_ALL_ON :
		ACPI_STATUS_DEVICE_ALL_OFF;
}

static void dptf_write_hid(bool is_eisa, const char *hid)
{
	if (is_eisa)
		acpigen_emit_eisaid(hid);
	else
		acpigen_write_string(hid);
}

/* Devices with GENERIC _HID (distinguished by PTYP) */
static void dptf_write_generic_participant(const char *name,
					   enum dptf_generic_participant_type ptype,
					   const char *str, int sta_val,
					   const struct dptf_platform_info *platform_info)
{
	/* Auto-incrementing UID for generic participants */
	static int generic_uid = 0;

	acpigen_write_device(name);
	acpigen_write_name("_HID");
	dptf_write_hid(platform_info->use_eisa_hids, platform_info->generic_hid);

	acpigen_write_name_integer("_UID", generic_uid++);
	acpigen_write_STA(sta_val);

	if (str)
		acpigen_write_name_unicode("_STR", str);

	acpigen_write_name_integer("PTYP", ptype);

	acpigen_pop_len(); /* Device */
}

/* \_SB.PCI0.TCPU */
static void write_tcpu(const struct device *pci_dev,
		       const struct drivers_intel_dptf_config *config)
{
	/* DPTF CPU device - \_SB.PCI0.TCPU */
	acpigen_write_scope(TCPU_SCOPE);
	acpigen_write_device("TCPU");
	acpigen_write_ADR_pci_device(pci_dev);
	acpigen_write_STA(get_STA_value(config, DPTF_CPU));
	acpigen_pop_len(); /* Device */
	acpigen_pop_len(); /* TCPU Scope */
}

/* \_SB.DPTF.TFNx */
static void write_fan(const struct drivers_intel_dptf_config *config,
			const struct dptf_platform_info *platform_info,
			enum dptf_participant participant)
{
	static int fan_uid = 0;

	acpigen_write_device(fan_namestring_of(participant));
	acpigen_write_name("_HID");
	dptf_write_hid(platform_info->use_eisa_hids, platform_info->fan_hid);
	acpigen_write_name_integer("_UID", fan_uid++);
	acpigen_write_name_unicode("_STR", DEFAULT_FAN_STR);
	acpigen_write_name_integer("PTYP", DPTF_GENERIC_PARTICIPANT_TYPE_FAN);
	acpigen_write_STA(get_STA_value(config, participant));
	acpigen_pop_len(); /* Device */
}

/* \_SB.DPTF */
static void write_imok(void)
{
	acpigen_write_method("IMOK", 1);
	/* Return (Arg0) */
	acpigen_emit_byte(RETURN_OP);
	acpigen_emit_byte(ARG0_OP);
	acpigen_write_method_end();
}

static void write_dcfg_variable(const struct drivers_intel_dptf_config *config)
{
	acpigen_write_name_integer("DCFG", config->dcfg);
}

/* \_SB.DPTF */
static void write_oem_variables(const struct drivers_intel_dptf_config *config)
{
	int i;

	acpigen_write_name("ODVX");
	acpigen_write_package(DPTF_OEM_VARIABLE_COUNT);
	for (i = 0; i < DPTF_OEM_VARIABLE_COUNT; i++)
		acpigen_write_dword(config->oem_data.oem_variables[i]);
	acpigen_write_package_end();

	/*
	 * Method (ODUP, 2)
	 * Arg0 = Index of ODVX to update
	 * Arg1 = Value to place in ODVX[Arg0]
	 */
	acpigen_write_method_serialized("ODUP", 2);
	/* ODVX[Arg0] = Arg1 */
	acpigen_write_store();
	acpigen_emit_byte(ARG1_OP);
	acpigen_emit_byte(INDEX_OP);
	acpigen_emit_namestring("ODVX");
	acpigen_emit_byte(ARG0_OP);
	acpigen_emit_byte(ZERO_OP); /* Ignore Index() Destination */
	acpigen_write_method_end();

	/*
	 * Method (ODGT, 1)
	 * Arg0 = Index of ODVX to get
	 */
	acpigen_write_method_serialized("ODGT", 1);
	 /* Return (ODVX[Arg0]) */
	acpigen_emit_byte(RETURN_OP);
	acpigen_emit_byte(DEREF_OP);
	acpigen_emit_byte(INDEX_OP);
	acpigen_emit_namestring("ODVX");
	acpigen_emit_byte(ARG0_OP);
	acpigen_emit_byte(ZERO_OP); /* Ignore Index() Destination */
	acpigen_write_method_end();

	/* Method (ODVP) { Return (ODVX) } */
	acpigen_write_method_serialized("ODVP", 0);
	acpigen_emit_byte(RETURN_OP);
	acpigen_emit_namestring("ODVX");
	acpigen_write_method_end();
}

/* \_SB.DPTF.xxxx */
static void write_generic_devices(const struct drivers_intel_dptf_config *config,
				  const struct dptf_platform_info *platform_info)
{
	enum dptf_participant participant;
	char name[ACPI_NAME_BUFFER_SIZE];
	int i;

	dptf_write_generic_participant("TCHG", DPTF_GENERIC_PARTICIPANT_TYPE_CHARGER,
				       DEFAULT_CHARGER_STR,
				       get_STA_value(config, DPTF_CHARGER),
				       platform_info);

	for (i = 0, participant = DPTF_TEMP_SENSOR_0; i < DPTF_MAX_TSR; ++i, ++participant) {
		snprintf(name, sizeof(name), "TSR%1d", i);
		dptf_write_generic_participant(name, DPTF_GENERIC_PARTICIPANT_TYPE_TSR,
					       nullptr, get_STA_value(config, participant),
					       platform_info);
	}
}

static const char *get_pmc_ipcs_method(void)
{
	const char *method = acpi_device_path_join(
				pcidev_path_on_root(PCH_DEVFN_PMC), "IPCS");
	if (!method) {
		printk(BIOS_ERR, "%s: Unable to find PMC device IPCS method\n", __func__);
		return nullptr;
	}
	return method;
}

static void write_tpch_write_method(const char *tpch_write_method_name,
				unsigned int ipc_subcmd_ctrl_value)
{
	/* Get IPCS method from the PMC device */
	const char *ipcs = get_pmc_ipcs_method();
	acpigen_write_method_serialized(tpch_write_method_name, 1);
	acpigen_emit_namestring(ipcs);
	acpigen_write_integer(PMC_IPC_CMD_COMMAND_FIVR);
	acpigen_write_integer(PMC_IPC_CMD_CMD_ID_FIVR_WRITE);
	acpigen_write_integer(PMC_IPC_COMMAND_FIVR_SIZE);
	acpigen_write_integer(ipc_subcmd_ctrl_value);
	acpigen_emit_byte(ARG0_OP);
	acpigen_write_zero();
	acpigen_write_zero();
	/* The reason for returning a value here is a W/A for the ESIF shell */
	acpigen_emit_byte(RETURN_OP);
	acpigen_write_package(1);
	acpigen_write_zero();
	acpigen_write_package_end();
	acpigen_write_method_end();
}

static void write_ppkg_package(const uint8_t i)
{
	acpigen_write_store();
	acpigen_emit_byte(DEREF_OP);
	acpigen_emit_byte(INDEX_OP);
	acpigen_emit_byte(ARG0_OP);
	acpigen_write_integer(i);
	acpigen_emit_byte(ZERO_OP);
	acpigen_emit_byte(INDEX_OP);
	acpigen_emit_namestring("PPKG");
	acpigen_write_integer(i);
	acpigen_emit_byte(ZERO_OP);
}

/*
 * Truncate Package received from IPC
 * Arguments:
 *  Arg0: Package returned from the IPCS read call from the Pmc
 * Return Value:
 *  Return Package with just the Status and ReadBuf0
 *  Status returns 0 for success and 2 for device error
 */
static void write_pkgc_method(void)
{
	acpigen_write_method_serialized("PKGC", 1);
	acpigen_write_name("PPKG");
	acpigen_write_package(2);
	acpigen_write_zero();
	acpigen_write_zero();
	acpigen_write_package_end();

	write_ppkg_package(0);
	write_ppkg_package(1);

	acpigen_write_return_namestr("PPKG");
	acpigen_write_method_end();
}

static void write_tpch_read_method(const char *tpch_read_method_name,
				unsigned int ipc_subcmd_ctrl_value)
{
	/* Get IPCS method from the PMC device */
	const char *ipcs = get_pmc_ipcs_method();
	acpigen_write_method_serialized(tpch_read_method_name, 0);
	acpigen_write_store();
	acpigen_emit_namestring(ipcs);
	acpigen_write_integer(PMC_IPC_CMD_COMMAND_FIVR);
	acpigen_write_integer(PMC_IPC_CMD_CMD_ID_FIVR_READ);
	acpigen_write_integer(PMC_IPC_COMMAND_FIVR_SIZE);
	acpigen_write_integer(ipc_subcmd_ctrl_value);
	acpigen_write_zero();
	acpigen_write_zero();
	acpigen_write_zero();
	acpigen_emit_byte(LOCAL0_OP);

	acpigen_write_store();
	acpigen_emit_namestring("PKGC");
	acpigen_emit_byte(LOCAL0_OP);
	acpigen_emit_byte(LOCAL1_OP);

	acpigen_emit_byte(RETURN_OP);
	acpigen_emit_byte(LOCAL1_OP);
	acpigen_write_method_end();
}

static void write_create_tpch(const struct dptf_platform_info *platform_info)
{
	acpigen_write_device("TPCH");
	acpigen_write_name("_HID");
	dptf_write_hid(platform_info->use_eisa_hids, platform_info->tpch_device_hid);
	acpigen_write_name_unicode("_STR", DEFAULT_TPCH_STR);
	acpigen_write_name_integer("PTYP", DPTF_GENERIC_PARTICIPANT_TYPE_TPCH);
	acpigen_write_STA(ACPI_STATUS_DEVICE_ALL_ON);
}

static void write_tpch_methods(const struct dptf_platform_info *platform_info)
{
	write_create_tpch(platform_info);

	const struct {
		enum { READ, WRITE } type;
		const char *method_name;
		unsigned int subcommand;
	} tpch_methods[] = {
		{	.type = WRITE,
			.method_name =
				platform_info->tpch_method_names.set_fivr_low_clock_method,
			.subcommand = PMC_IPC_SUBCMD_RFI_CTRL0_LOGIC
		},
		{	.type = WRITE,
			.method_name =
				platform_info->tpch_method_names.set_fivr_high_clock_method,
			.subcommand = PMC_IPC_SUBCMD_RFI_CTRL4_LOGIC
		},
		{	.type = READ,
			.method_name =
				platform_info->tpch_method_names.get_fivr_low_clock_method,
			.subcommand = PMC_IPC_SUBCMD_RFI_CTRL0_LOGIC
		},
		{	.type = READ,
			.method_name =
				platform_info->tpch_method_names.get_fivr_high_clock_method,
			.subcommand = PMC_IPC_SUBCMD_RFI_CTRL4_LOGIC
		},
		{	.type = READ,
			.method_name =
				platform_info->tpch_method_names.get_fivr_ssc_method,
			.subcommand = PMC_IPC_SUBCMD_EMI_CTRL0_LOGIC
		},
		{	.type = READ,
			.method_name =
			platform_info->tpch_method_names.get_fivr_switching_fault_status,
			.subcommand = PMC_IPC_SUBCMD_FFFC_FAULT_STATUS
		},
		{	.type = READ,
			.method_name =
				platform_info->tpch_method_names.get_fivr_switching_freq_mhz,
			.subcommand = PMC_IPC_SUBCMD_FFFC_RFI_STATUS
		},
	};

	write_pkgc_method();
	for (size_t i = 0; i < ARRAY_SIZE(tpch_methods); i++) {
		if (tpch_methods[i].type == READ) {
			write_tpch_read_method(tpch_methods[i].method_name,
						tpch_methods[i].subcommand);
		} else if (tpch_methods[i].type == WRITE) {
			write_tpch_write_method(tpch_methods[i].method_name,
						tpch_methods[i].subcommand);
		}
	}

	acpigen_write_device_end(); /* TPCH Device */
}

static void write_create_tpwr(const struct drivers_intel_dptf_config *config,
			      const struct dptf_platform_info *platform_info)
{
	acpigen_write_device("TPWR");
	acpigen_write_name("_HID");
	if (platform_info->tpwr_device_hid != nullptr)
		dptf_write_hid(platform_info->use_eisa_hids, platform_info->tpwr_device_hid);
	acpigen_write_name_string("_UID", "TPWR");
	acpigen_write_name_unicode("_STR", DEFAULT_POWER_STR);
	acpigen_write_name_integer("PTYP", DPTF_GENERIC_PARTICIPANT_TYPE_POWER);
	acpigen_write_STA(ACPI_STATUS_DEVICE_ALL_ON);

	/* PROP method */
	if(config->prop != 0) {
		acpigen_write_method_serialized("PROP", 0);
		acpigen_emit_byte(RETURN_OP);
		acpigen_write_integer(config->prop);
		acpigen_pop_len(); /* Method PROP */
	}
	acpigen_write_device_end(); /* TPWR Power Participant Device */
}

static void write_tpwr_methods(const struct drivers_intel_dptf_config *config,
			       const struct dptf_platform_info *platform_info)
{
	write_create_tpwr(config, platform_info);
}

static void write_create_tbat(const struct dptf_platform_info *platform_info)
{
	acpigen_write_device("TBAT");
	acpigen_write_name("_HID");
	if (platform_info->tbat_device_hid != nullptr)
		dptf_write_hid(platform_info->use_eisa_hids, platform_info->tbat_device_hid);
	acpigen_write_name_string("_UID", "1");
	acpigen_write_name_unicode("_STR", DEFAULT_BATTERY_STR);
	acpigen_write_name_integer("PTYP", DPTF_GENERIC_PARTICIPANT_TYPE_BATTERY);
	acpigen_write_STA(ACPI_STATUS_DEVICE_ALL_ON);
	acpigen_write_device_end(); /* TBAT Battery Participant Device */
}


static void write_tbat_methods(const struct dptf_platform_info *platform_info)
{
	write_create_tbat(platform_info);
}


/* \_SB.DPTF - note: leaves the Scope open for child devices */
static void write_open_dptf_device(const struct device *dev,
				   const struct dptf_platform_info *platform_info)
{
	acpigen_write_scope("\\_SB");
	acpigen_write_device(acpi_device_name(dev));
	acpigen_write_name("_HID");
	dptf_write_hid(platform_info->use_eisa_hids, platform_info->dptf_device_hid);
	acpigen_write_name_integer("_UID", 0);
	acpigen_write_STA(ACPI_STATUS_DEVICE_ALL_ON);
}

static const struct dptf_platform_info generic_dptf_platform_info = {
	.use_eisa_hids = CONFIG(DPTF_USE_EISA_HID),
	/* _HID for the toplevel DPTF device, typically \_SB.DPTF */
	.dptf_device_hid = DPTF_DPTF_DEVICE,
	/* _HID for Intel DPTF Generic Device (these require PTYP as well) */
	.generic_hid = DPTF_GEN_DEVICE,
	/* _HID for Intel DPTF Fan Device */
	.fan_hid = DPTF_FAN_DEVICE,

#if CONFIG(DRIVERS_INTEL_DPTF_SUPPORTS_TPWR)
	/* _HID for the toplevel TPWR device, typically \_SB.DPTF.TPWR */
	.tpwr_device_hid = DPTF_TPWR_DEVICE,
#endif

#if CONFIG(DRIVERS_INTEL_DPTF_SUPPORTS_TBAT)
	/* _HID for the toplevel BAT1 device, typically \_SB.DPTF.BAT1 */
	.tbat_device_hid = DPTF_BAT1_DEVICE,
#endif

#if CONFIG(DRIVERS_INTEL_DPTF_SUPPORTS_TPCH)
	/* _HID for the toplevel TPCH device, typically \_SB.TPCH */
	.tpch_device_hid = DPTF_TPCH_DEVICE,

	.tpch_method_names = {
		.set_fivr_low_clock_method = "RFC0",
		.set_fivr_high_clock_method = "RFC1",
		.get_fivr_low_clock_method = "GFC0",
		.get_fivr_high_clock_method = "GFC1",
		.get_fivr_ssc_method = "GEMI",
		.get_fivr_switching_fault_status = "GFFS",
		.get_fivr_switching_freq_mhz = "GFCS",
	},
#endif
};

static const struct dptf_platform_info *get_dptf_platform_info(void)
{
	return &generic_dptf_platform_info;
}

/* Add minimal definitions of DPTF devices into the SSDT */
static void write_device_definitions(const struct device *dev)
{
	const struct dptf_platform_info *platform_info = get_dptf_platform_info();
	const struct drivers_intel_dptf_config *config;
	struct device *parent;
	enum dptf_participant p;

	/* The CPU device gets an _ADR that matches the ACPI PCI address for 00:04.00 */
	parent = dev && dev->upstream ? dev->upstream->dev : nullptr;
	if (!parent || parent->path.type != DEVICE_PATH_PCI) {
		printk(BIOS_ERR, "%s: DPTF objects must live under 00:04.0 PCI device\n",
		       __func__);
		return;
	}

	config = config_of(dev);
	write_tcpu(parent, config);
	write_open_dptf_device(dev, platform_info);

	if (config->dptf_multifan_support) {
		for (p = DPTF_FAN; p <= DPTF_FAN_2; ++p)
			write_fan(config, platform_info, p);
	} else
		write_fan(config, platform_info, DPTF_FAN);

	write_dcfg_variable(config);
	write_oem_variables(config);
	write_imok();
	write_generic_devices(config, platform_info);

	if (CONFIG(DRIVERS_INTEL_DPTF_SUPPORTS_TPCH))
		write_tpch_methods(platform_info);

	if (CONFIG(DRIVERS_INTEL_DPTF_SUPPORTS_TPWR))
		write_tpwr_methods(config, platform_info);

	if (CONFIG(DRIVERS_INTEL_DPTF_SUPPORTS_TBAT))
		write_tbat_methods(platform_info);

	acpigen_pop_len(); /* DPTF Device (write_open_dptf_device) */
	acpigen_pop_len(); /* Scope */
}

/* Emits policy definitions for each policy type */
static void write_policies(const struct drivers_intel_dptf_config *config)
{
	dptf_write_enabled_policies(config->policies.active, DPTF_MAX_ACTIVE_POLICIES,
				    config->policies.passive, DPTF_MAX_PASSIVE_POLICIES,
				    config->policies.critical, DPTF_MAX_CRITICAL_POLICIES);

	dptf_write_active_policies(config->policies.active,
				   DPTF_MAX_ACTIVE_POLICIES, config->dptf_multifan_support);

	dptf_write_passive_policies(config->policies.passive,
				    DPTF_MAX_PASSIVE_POLICIES);

	dptf_write_critical_policies(config->policies.critical,
				     DPTF_MAX_CRITICAL_POLICIES);
}

/* Writes other static tables that are used by DPTF */
static void write_controls(const struct drivers_intel_dptf_config *config)
{
	enum dptf_participant p;
	int fan_num;

	dptf_write_charger_perf(config->controls.charger_perf, DPTF_MAX_CHARGER_PERF_STATES);

	/* Write TFN perf states based on the number of fans on the platform */
	if (config->dptf_multifan_support) {
		for (p = DPTF_FAN, fan_num = 0; p <= DPTF_FAN_2; ++p, ++fan_num)
			dptf_write_multifan_perf(config->controls.multifan_perf,
					DPTF_MAX_FAN_PERF_STATES, p, fan_num);
	} else
		dptf_write_fan_perf(config->controls.fan_perf, DPTF_MAX_FAN_PERF_STATES,
			DPTF_FAN);

	dptf_write_power_limits(&config->controls.power_limits);
}

/* Options to control the behavior of devices */
static void write_options(const struct drivers_intel_dptf_config *config)
{
	enum dptf_participant p;
	int i, fan_num;

	/* Configure Fan options based on the number of fans on the platform */
	if (config->dptf_multifan_support) {
		for (p = DPTF_FAN, fan_num = 0; p <= DPTF_FAN_2; ++p, ++fan_num) {
			dptf_write_scope(p);
			dptf_write_fan_options(
				config->options.multifan_options[fan_num].fine_grained_control,
				config->options.multifan_options[fan_num].step_size,
				config->options.multifan_options[fan_num].low_speed_notify);
			acpigen_pop_len(); /* Scope */
		}
	} else {
		dptf_write_scope(DPTF_FAN);
		dptf_write_fan_options(config->options.fan.fine_grained_control,
			config->options.fan.step_size,
			config->options.fan.low_speed_notify);
		acpigen_pop_len(); /* Scope */
	}

	/* TSR options */
	for (p = DPTF_TEMP_SENSOR_0, i = 0; p <= DPTF_TEMP_SENSOR_4; ++p, ++i) {
		if (is_participant_used(config, p) && (config->options.tsr[i].hysteresis ||
						       config->options.tsr[i].desc)) {
			dptf_write_scope(p);
			dptf_write_tsr_hysteresis(config->options.tsr[i].hysteresis);
			dptf_write_STR(config->options.tsr[i].desc);
			acpigen_pop_len(); /* Scope */
		}
	}
}

/* Add custom tables and methods to SSDT */
static void dptf_fill_ssdt(const struct device *dev)
{
	struct drivers_intel_dptf_config *config = config_of(dev);

	write_device_definitions(dev);
	write_policies(config);
	write_controls(config);
	write_options(config);

	printk(BIOS_INFO, DPTF_DEVICE_PATH ": %s at %s\n", dev->chip_ops->name, dev_path(dev));
}

static struct device_operations dptf_ops = {
	.read_resources		= noop_read_resources,
	.set_resources		= noop_set_resources,
	.acpi_name		= dptf_acpi_name,
	.acpi_fill_ssdt		= dptf_fill_ssdt,
};

static void dptf_enable_dev(struct device *dev)
{
	dev->ops = &dptf_ops;
}

struct chip_operations drivers_intel_dptf_ops = {
	.name = "Intel DPTF",
	.enable_dev = dptf_enable_dev,
};
