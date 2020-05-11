/* SPDX-License-Identifier: GPL-2.0-only */
/* This file is part of the coreboot project. */

#ifndef __ESPI_H__
#define __ESPI_H__

/* ESPI Slave Registers (Document # 327432-004 Revision 1.0 Chapter 7) */

#define ESPI_SLAVE_DEVICE_ID				0x04
#define  ESPI_SLAVE_VERSION_ID_SHIFT			0
#define  ESPI_SLAVE_VERSION_ID_MASK			0xf

#define ESPI_SLAVE_GENERAL_CFG				0x08
#define  ESPI_SLAVE_CRC_ENABLE				(1 << 31)
#define  ESPI_SLAVE_CRC_DISABLE				(0 << 31)
#define  ESPI_SLAVE_RESP_MOD_ENABLE			(1 << 30)
#define  ESPI_SLAVE_RESP_MOD_DISABLE			(0 << 30)
#define  ESPI_SLAVE_ALERT_MODE_PIN			(1 << 28)
#define  ESPI_SLAVE_ALERT_MODE_IO1			(0 << 28)
#define  ESPI_SLAVE_IO_MODE_SEL_SHIFT			26
#define  ESPI_SLAVE_IO_MODE_SEL_MASK			(0x3 << ESPI_SLAVE_IO_MODE_SEL_SHIFT)
#define  ESPI_SLAVE_IO_MODE_SEL_VAL(x)			((x) << ESPI_SLAVE_IO_MODE_SEL_SHIFT)
#define  ESPI_SLAVE_IO_MODE_SEL_SINGLE			ESPI_SLAVE_IO_MODE_SEL_VAL(0)
#define  ESPI_SLAVE_IO_MODE_SEL_DUAL			ESPI_SLAVE_IO_MODE_SEL_VAL(1)
#define  ESPI_SLAVE_IO_MODE_SEL_QUAD			ESPI_SLAVE_IO_MODE_SEL_VAL(2)
#define  ESPI_SLAVE_IO_MODE_SUPP_SHIFT			24
#define  ESPI_SLAVE_IO_MODE_SUPP_MASK			(0x3 << ESPI_SLAVE_IO_MODE_SUPP_SHIFT)
#define  ESPI_SLAVE_IO_MODE_SUPP_VAL(x)			((x) << ESPI_SLAVE_IO_MODE_SUPP_SHIFT)
#define  ESPI_SLAVE_IO_MODE_SUPP_SINGLE_ONLY		ESPI_SLAVE_IO_MODE_SUPP_VAL(0)
#define  ESPI_SLAVE_IO_MODE_SUPP_SINGLE_DUAL		ESPI_SLAVE_IO_MODE_SUPP_VAL(1)
#define  ESPI_SLAVE_IO_MODE_SUPP_SINGLE_QUAD		ESPI_SLAVE_IO_MODE_SUPP_VAL(2)
#define  ESPI_SLAVE_IO_MODE_SUPP_SINGLE_DUAL_QUAD	ESPI_SLAVE_IO_MODE_SUPP_VAL(3)
#define  ESPI_SLAVE_OPEN_DRAIN_ALERT_SEL		(1 << 23)
#define  ESPI_SLAVE_PUSH_PULL_ALERT_SEL			(0 << 23)
#define  ESPI_SLAVE_OP_FREQ_SEL_SHIFT			20
#define  ESPI_SLAVE_OP_FREQ_SEL_MASK			(0x7 << ESPI_SLAVE_OP_FREQ_SEL_SHIFT)
#define  ESPI_SLAVE_OP_FREQ_SEL_VAL(x)			((x) << ESPI_SLAVE_OP_FREQ_SEL_SHIFT)
#define  ESPI_SLAVE_OP_FREQ_SEL_20_MHZ			ESPI_SLAVE_OP_FREQ_SEL_VAL(0)
#define  ESPI_SLAVE_OP_FREQ_SEL_25_MHZ			ESPI_SLAVE_OP_FREQ_SEL_VAL(1)
#define  ESPI_SLAVE_OP_FREQ_SEL_33_MHZ			ESPI_SLAVE_OP_FREQ_SEL_VAL(2)
#define  ESPI_SLAVE_OP_FREQ_SEL_50_MHZ			ESPI_SLAVE_OP_FREQ_SEL_VAL(3)
#define  ESPI_SLAVE_OP_FREQ_SEL_66_MHZ			ESPI_SLAVE_OP_FREQ_SEL_VAL(4)
#define  ESPI_SLAVE_OPEN_DRAIN_ALERT_SUPP		(1 << 19)
#define  ESPI_SLAVE_OP_FREQ_SUPP_SHIFT			16
#define  ESPI_SLAVE_OP_FREQ_SUPP_MASK			(0x7 << ESPI_SLAVE_OP_FREQ_SUPP_SHIFT)
#define  ESPI_SLAVE_OP_FREQ_SUPP_VAL(x)			((x) << ESPI_SLAVE_OP_FREQ_SUPP_SHIFT)
#define  ESPI_SLAVE_OP_FREQ_SUPP_20_MHZ			ESPI_SLAVE_OP_FREQ_SUPP_VAL(0)
#define  ESPI_SLAVE_OP_FREQ_SUPP_25_MHZ			ESPI_SLAVE_OP_FREQ_SUPP_VAL(1)
#define  ESPI_SLAVE_OP_FREQ_SUPP_33_MHZ			ESPI_SLAVE_OP_FREQ_SUPP_VAL(2)
#define  ESPI_SLAVE_OP_FREQ_SUPP_50_MHZ			ESPI_SLAVE_OP_FREQ_SUPP_VAL(3)
#define  ESPI_SLAVE_OP_FREQ_SUPP_66_MHZ			ESPI_SLAVE_OP_FREQ_SUPP_VAL(4)
#define  ESPI_SLAVE_MAX_WAIT_SHIFT			12
#define  ESPI_SLAVE_MAX_WAIT_MASK			(0xf << ESPI_SLAVE_MAX_WAIT_SHIFT)
#define  ESPI_SLAVE_MAX_WAIT_STATE(x)			\
				(((x) << ESPI_SLAVE_MAX_WAIT_SHIFT) & ESPI_MAX_WAIT_MASK)
#define  ESPI_SLAVE_FLASH_CH_SUPP			(1 << 3)
#define  ESPI_SLAVE_OOB_CH_SUPP				(1 << 2)
#define  ESPI_SLAVE_VW_CH_SUPP				(1 << 1)
#define  ESPI_SLAVE_PERIPH_CH_SUPP			(1 << 0)

#define ESPI_SLAVE_PERIPH_CFG				0x10
#define  ESPI_SLAVE_PERIPH_MAX_READ_SIZE_SHIFT		12
#define  ESPI_SLAVE_PERIPH_MAX_READ_SIZE_MASK		\
						(0x7 << ESPI_SLAVE_PERIPH_MAX_READ_SIZE_SHIFT)
#define  ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(x)		\
						((x) << ESPI_SLAVE_PERIPH_MAX_READ_SIZE_SHIFT)
#define  ESPI_SLAVE_PERIPH_MAX_READ_64B			ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(1)
#define  ESPI_SLAVE_PERIPH_MAX_READ_128B		ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(2)
#define  ESPI_SLAVE_PERIPH_MAX_READ_256B		ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(3)
#define  ESPI_SLAVE_PERIPH_MAX_READ_512B		ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(4)
#define  ESPI_SLAVE_PERIPH_MAX_READ_1024B		ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(5)
#define  ESPI_SLAVE_PERIPH_MAX_READ_2048B		ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(6)
#define  ESPI_SLAVE_PERIPH_MAX_READ_4096B		ESPI_SLAVE_PERIPH_MAX_READ_SIZE_VAL(7)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_SHIFT	8
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_MASK	\
					(0x7 << ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_SHIFT)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_VAL(x)	\
					((x) << ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_SHIFT)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_64B	\
						ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_VAL(1)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_128B	\
						ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_VAL(2)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_256B	\
						ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SEL_VAL(3)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_SHIFT	4
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_MASK	\
					(0x7 << ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_SHIFT)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_VAL(x)	\
					((x) << ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_SHIFT)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_64B	\
						ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_VAL(1)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_128B	\
						ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_VAL(2)
#define  ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_256B	\
						ESPI_SLAVE_PERIPH_MAX_PAYLOAD_SIZE_SUPP_VAL(3)
#define  ESPI_SLAVE_PERIPH_BUS_MASTER_ENABLE		(1 << 2)

#define ESPI_SLAVE_VW_CFG				0x20
#define  ESPI_SLAVE_VW_COUNT_SEL_SHIFT			16
#define  ESPI_SLAVE_VW_COUNT_SEL_MASK			(0x3f << ESPI_SLAVE_VW_COUNT_SEL_SHIFT)
/* 0-based field. Value of 0 indicates 1 virtual wire selected. */
#define  ESPI_SLAVE_VW_COUNT_SEL_VAL(x)			\
						((x) << ESPI_SLAVE_VW_COUNT_SEL_SHIFT)
#define  ESPI_SLAVE_VW_COUNT_SUPP_SHIFT			8
#define  ESPI_SLAVE_VW_COUNT_SUPP_MASK			\
						(0x3f << ESPI_SLAVE_VW_COUNT_SUPP_SHIFT)

#define ESPI_SLAVE_OOB_CFG				0x30
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_SHIFT	8
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_MASK	\
					(0x7 << ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_SHIFT)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_VAL(x)	\
					((x) << ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_SHIFT)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_64B	\
						ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_VAL(1)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_128B	\
						ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_VAL(2)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_256B	\
						ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SEL_VAL(3)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_SHIFT	4
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_MASK	\
					(0x7 << ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_SHIFT)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_VAL(x)	\
					((x) << ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_SHIFT)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_64B	\
						ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_VAL(1)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_128B	\
						ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_VAL(2)
#define  ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_256B	\
						ESPI_SLAVE_OOB_MAX_PAYLOAD_SIZE_SUPP_VAL(3)

#define ESPI_SLAVE_FLASH_CFG				0x40
#define  ESPI_SLAVE_FLASH_MAX_READ_SIZE_SHIFT		12
#define  ESPI_SLAVE_FLASH_MAX_READ_SIZE_MASK		\
						(0x7 << ESPI_SLAVE_FLASH_MAX_READ_SIZE_SHIFT)
#define  ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(x)		\
						((x) << ESPI_SLAVE_FLASH_MAX_READ_SIZE_SHIFT)
#define  ESPI_SLAVE_FLASH_MAX_READ_64B			ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(1)
#define  ESPI_SLAVE_FLASH_MAX_READ_128B			ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(2)
#define  ESPI_SLAVE_FLASH_MAX_READ_256B			ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(3)
#define  ESPI_SLAVE_FLASH_MAX_READ_512B			ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(4)
#define  ESPI_SLAVE_FLASH_MAX_READ_1024B		ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(5)
#define  ESPI_SLAVE_FLASH_MAX_READ_2048B		ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(6)
#define  ESPI_SLAVE_FLASH_MAX_READ_4096B		ESPI_SLAVE_FLASH_MAX_READ_SIZE_VAL(7)
#define  ESPI_SLAVE_FLASH_SHARING_MODE_MAF		(1 << 11)
#define  ESPI_SLAVE_FLASH_SHARING_MODE_SAF		(0 << 11)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_SHIFT	8
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_MASK	\
					(0x7 << ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_SHIFT)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_VAL(x)	\
					((x) << ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_SHIFT)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_64B	\
						ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_VAL(1)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_128B	\
						ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_VAL(2)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_256B	\
						ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SEL_VAL(3)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_SHIFT	5
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_MASK	\
					(0x7 << ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_SHIFT)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_VAL(x)	\
					((x) << ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_SHIFT)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_64B	\
						ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_VAL(1)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_128B	\
						ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_VAL(2)
#define  ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_256B	\
						ESPI_SLAVE_FLASH_MAX_PAYLOAD_SIZE_SUPP_VAL(3)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_SHIFT	2
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_MASK		\
					(0x7 << ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_SHIFT)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_VAL(x)	\
					((x) << ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_SHIFT)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_4K		\
						ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_VAL(1)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_64K		\
						ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_VAL(2)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_4K_64K	\
						ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_VAL(3)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_128K		\
						ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_VAL(4)
#define  ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_256K		\
						ESPI_SLAVE_FLASH_BLOCK_ERASE_SIZE_VAL(5)

/*
 * All channels -- peripheral, OOB, VW and flash use the same bits for channel ready and channel
 * enable.
 */
#define  ESPI_SLAVE_CHANNEL_READY		(1 << 1)
#define  ESPI_SLAVE_CHANNEL_ENABLE		(1 << 0)

/* ESPI Slave Registers (Document # 327432-004 Revision 1.0 Chapter 5) */
#define ESPI_VW_INDEX_INTERRUPT_EVENT_0		0	/* Interrupt lines 0 - 127 */
#define ESPI_VW_INDEX_INTERRUPT_EVENT_1		1	/* Interrupt lines 128-255 */
#define  ESPI_VW_INTERRUPT_LEVEL_HIGH		(1 << 7)
#define  ESPI_VW_INTERRUPT_LEVEL_LOW		(0 << 7)

#define ESPI_VW_INDEX_SYSTEM_EVENT_2		2
#define  ESPI_VW_SLP_S5				2
#define  ESPI_VW_SLP_S4				1
#define  ESPI_VW_SLP_S3				0
#define ESPI_VW_INDEX_SYSTEM_EVENT_3		3
#define  ESPI_VW_OOB_RST_WARN			2
#define  ESPI_VW_PLTRST				1
#define  ESPI_VW_SUS_STAT			0
#define ESPI_VW_INDEX_SYSTEM_EVENT_4		4
#define  ESPI_VW_PME				3
#define  ESPI_VW_WAKE				2
#define  ESPI_VW_OOB_RST_ACK			0
#define ESPI_VW_INDEX_SYSTEM_EVENT_5		5
#define  ESPI_VW_SLAVE_BOOT_LOAD_STATUS		3
#define  ESPI_VW_ERROR_NON_FATAL		2
#define  ESPI_VW_ERROR_FATAL			1
#define  ESPI_VW_SLV_BOOT_LOAD_DONE		0
#define ESPI_VW_INDEX_SYSTEM_EVENT_6		6
#define  ESPI_VW_HOST_RST_ACK			3
#define  ESPI_VW_RCIN				2
#define  ESPI_VW_SMI				1
#define  ESPI_VW_SCI				0
#define ESPI_VW_INDEX_SYSTEM_EVENT_7		7
#define  ESPI_VW_NMIOUT				2
#define  ESPI_VW_SMIOUT				1
#define  ESPI_VW_HOST_RST_WARN			0

#define  ESPI_VW_VALID(x)			(1 << ((x) + 4))
#define  ESPI_VW_VALUE(x, v)			((v) << (x))
#define  ESPI_VW_SIGNAL_HIGH(x)			(ESPI_VW_VALID(x) | ESPI_VW_VALUE(1, x))
#define  ESPI_VW_SIGNAL_LOW(x)			(ESPI_VW_VALID(x) | ESPI_VW_VALUE(0, x))
#endif /* __ESPI_H__ */
