/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef IOAPIC_H
#define IOAPIC_H

/**
 * @addtogroup hwmgmt_apic
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

#include <apicreg.h>

#define NR_LEGACY_IRQ 16U

struct ioapic_info {
	uint8_t id; /* IOAPIC ID as indicated in ACPI MADT */
	uint32_t addr; /* IOAPIC Register address */
	uint32_t gsi_base; /* Global System Interrupt where this IO-APIC's interrupt input start */
	uint32_t nr_pins; /* Number of Interrupt inputs as determined by Max. Redir Entry Register */
};

void ioapic_setup_irqs(void);

bool ioapic_irq_is_gsi(uint32_t irq);
int32_t init_ioapic_id_info(void);

/**
 * @defgroup ioapic_ext_apis IOAPIC External Interfaces
 *
 * This is a group that includes IOAPIC External Interfaces.
 *
 * @{
 */

/**
 * @}
 */
/* End of ioapic_ext_apis */

struct gsi_table {
	uint8_t ioapic_id;
	uint32_t pin;
	void *addr;
};

/**
 * @}
 */

#endif /* IOAPIC_H */
