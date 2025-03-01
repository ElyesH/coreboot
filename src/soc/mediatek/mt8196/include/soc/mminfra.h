/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __SOC_MEDIATEK_MMINFRA_H__
#define __SOC_MEDIATEK_MMINFRA_H__

/* mminfra_ao */
#define MMINFRA_GCE_DDREN_SEL		(MMINFRA_AO_CONFIG + 0x418)
#define MMINFRA_GCE_PROT_EN		(MMINFRA_AO_CONFIG + 0x428)

/* mminfra0 */
#define MMINFRA_MM0_GALS_PROT_RX_EN	(MMINFRA_CONFIG + 0x9C0)
#define MMINFRA_MM0_GALS_PROT_RX_RDY	(MMINFRA_CONFIG + 0x9C4)
#define MMINFRA_MM0_GALS_PROT_TX_EN	(MMINFRA_CONFIG + 0x9C8)
#define MMINFRA_MM0_GALS_PROT_TX_RDY	(MMINFRA_CONFIG + 0x9CC)
#define MMINFRA_MM1_GALS_PROT_RX_EN	(MMINFRA_CONFIG + 0x9D0)
#define MMINFRA_MM1_GALS_PROT_RX_RDY	(MMINFRA_CONFIG + 0x9D4)
#define MMINFRA_MM1_GALS_PROT_TX_EN	(MMINFRA_CONFIG + 0x9D8)
#define MMINFRA_MM1_GALS_PROT_TX_RDY	(MMINFRA_CONFIG + 0x9DC)

#define GCE_D_SLEEPPORT_RX_EN		BIT(0)
#define GCE_D_HAND_SLEEPPORT_RX_EN	BIT(1)
#define GCE_D_HAND_SLEEPPORT_TX_EN	BIT(2)
#define GCE_M_SLEEPPORT_RX_EN		BIT(8)
#define GCE_M_HAND_SLEEPPORT_RX_EN	BIT(9)
#define GCE_M_HAND_SLEEPPORT_TX_EN	BIT(10)

#define VLP_AO_RSVD6			(VLP_AO_BASE + 0x918)

/* mmpc */
#define MMPC_PM_BOOT_UP_PWR_CON		(MMPC_BASE + 0x1B8)
#define MM_INFRA0_PM_BOOT_UP		BIT(16)
#define MM_INFRA1_PM_BOOT_UP		BIT(17)

void mminfra_post_init(void);

#endif
