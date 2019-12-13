/*-
 * Copyright (c) 1997, Stefan Esser <se@freebsd.org>
 * Copyright (c) 2011 NetApp, Inc.
 * Copyright (c) 2018 Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NETAPP, INC ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL NETAPP, INC OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef PCI_H_
#define PCI_H_

/**
 * @addtogroup hwmgmt_pci
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

/*
 * PCIM_xxx: mask to locate subfield in register
 * PCIR_xxx: config register offset
 * PCIC_xxx: device class
 * PCIS_xxx: device subclass
 * PCIP_xxx: device programming interface
 * PCIV_xxx: PCI vendor ID (only required to fixup ancient devices)
 * PCID_xxx: device ID
 * PCIY_xxx: capability identification number
 * PCIZ_xxx: extended capability identification number
 */

#define PCI_BAR_COUNT 0x6U
#define PCI_REGMAX    0xFFU

/* I/O ports */
#define PCI_CONFIG_ADDR 0xCF8U
#define PCI_CONFIG_DATA 0xCFCU

#define PCI_CFG_ENABLE 0x80000000U

/* PCI config header registers for all devices */
#define PCIR_VENDOR               0x00U
#define PCIR_DEVICE               0x02U
#define PCIR_COMMAND              0x04U
#define PCIM_CMD_INTxDIS          0x400U
#define PCIR_REVID                0x08U
#define PCIR_SUBCLASS             0x0AU
#define PCIR_CLASS                0x0BU
#define PCIR_HDRTYPE              0x0EU
#define PCIM_HDRTYPE_NORMAL       0x00U
#define PCIM_MFDEV                0x80U
#define PCIR_BARS                 0x10U
#define PCIM_BAR_SPACE            0x01U
#define PCIM_BAR_IO_SPACE         0x01U
#define PCIM_BAR_MEM_TYPE         0x06U
#define PCIM_BAR_MEM_32           0x00U
#define PCIM_BAR_MEM_64           0x04U
#define PCIR_CAP_PTR              0x34U
#define PCI_BASE_ADDRESS_MEM_MASK (~0x0fUL)
#define PCI_BASE_ADDRESS_IO_MASK  (~0x03UL)

/* Capability Register Offsets */
#define PCICAP_ID      0x0U
#define PCICAP_NEXTPTR 0x1U

/* Capability Identification Numbers */
#define PCIY_MSI 0x05U

/* PCI Message Signalled Interrupts (MSI) */
#define PCIR_MSI_CTRL           0x02U
#define PCIM_MSICTRL_64BIT      0x80U
#define PCIM_MSICTRL_MSI_ENABLE 0x01U
#define PCIR_MSI_ADDR           0x4U
#define PCIR_MSI_ADDR_HIGH      0x8U
#define PCIR_MSI_DATA           0x8U
#define PCIR_MSI_DATA_64BIT     0xCU
#define PCIM_MSICTRL_MMC_MASK   0x000EU
#define PCIM_MSICTRL_MME_MASK   0x0070U

/* PCI device class */
#define PCIC_BRIDGE      0x06U
#define PCIS_BRIDGE_HOST 0x00U

union pci_bdf {
	uint16_t value;
	struct {
		uint8_t f : 3; /* BITs 0-2 */
		uint8_t d : 5; /* BITs 3-7 */
		uint8_t b; /* BITs 8-15 */
	} bits;
	struct {
		uint8_t devfun; /* BITs 0-7 */
		uint8_t bus; /* BITs 8-15 */
	} fields;
};

enum pci_bar_type {
	PCIBAR_NONE = 0,
	PCIBAR_IO_SPACE,
	PCIBAR_MEM32,
	PCIBAR_MEM64,
	PCIBAR_MEM64HI,
};

struct pci_pdev {
};

static inline uint32_t pci_bar_offset(uint32_t idx)
{
	return PCIR_BARS + (idx << 2U);
}

static inline uint32_t pci_bar_index(uint32_t offset)
{
	return (offset - PCIR_BARS) >> 2U;
}

static inline bool is_bar_offset(uint32_t nr_bars, uint32_t offset)
{
	bool ret;

	if ((offset >= pci_bar_offset(0U)) && (offset < pci_bar_offset(nr_bars))) {
		ret = true;
	} else {
		ret = false;
	}

	return ret;
}

static inline enum pci_bar_type pci_get_bar_type(uint32_t val)
{
	enum pci_bar_type type = PCIBAR_NONE;

	if ((val & PCIM_BAR_SPACE) != PCIM_BAR_IO_SPACE) {
		switch (val & PCIM_BAR_MEM_TYPE) {
		case PCIM_BAR_MEM_32:
			type = PCIBAR_MEM32;
			break;

		case PCIM_BAR_MEM_64:
			type = PCIBAR_MEM64;
			break;

		default:
			/*no actions are required for other cases.*/
			break;
		}
	}

	return type;
}

static inline bool bdf_is_equal(union pci_bdf a, union pci_bdf b)
{
	return (a.value == b.value);
}

uint32_t pci_pdev_read_cfg(union pci_bdf bdf, uint32_t offset, uint32_t bytes);
void pci_pdev_write_cfg(union pci_bdf bdf, uint32_t offset, uint32_t bytes, uint32_t val);
void enable_disable_pci_intx(union pci_bdf bdf, bool enable);

/**
 * @}
 */

#endif /* PCI_H_ */
