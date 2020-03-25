/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <types.h>
#include <bits.h>
#include <msr.h>
#include <cpu.h>
#include <per_cpu.h>
#include <cpu_caps.h>
#include <lapic.h>

/**
 * @defgroup hwmgmt_apic hwmgmt.apic
 * @ingroup hwmgmt
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

union lapic_base_msr {
	uint64_t value;
	struct {
		uint32_t rsvd_1 : 8;
		uint32_t bsp : 1;
		uint32_t rsvd_2 : 1;
		uint32_t x2APIC_enable : 1;
		uint32_t xAPIC_enable : 1;
		uint32_t lapic_paddr : 24;
		uint32_t rsvd_3 : 28;
	} fields;
};

static void clear_lapic_isr(void)
{
	uint32_t i;
	uint32_t isr_reg;

	/* This is a Intel recommended procedure and assures that the processor
	 * does not get hung up due to already set "in-service" interrupts left
	 * over from the boot loader environment. This actually occurs in real
	 * life, therefore we will ensure all the in-service bits are clear.
	 */
	for (isr_reg = MSR_IA32_EXT_APIC_ISR7; isr_reg >= MSR_IA32_EXT_APIC_ISR0; isr_reg--) {
		if (msr_read(isr_reg) != 0U) {
			for (i = 0U; i < 32U; i++) {
				msr_write(MSR_IA32_EXT_APIC_EOI, 0U);
			}
		}
	}
}

void early_init_lapic(void)
{
	union lapic_base_msr base;

	/* Get local APIC base address */
	base.value = msr_read(MSR_IA32_APIC_BASE);

	/* Enable LAPIC in x2APIC mode*/
	/* The following sequence of msr writes to enable x2APIC
	 * will work irrespective of the state of LAPIC
	 * left by BIOS
	 */
	/* Step1: Enable LAPIC in xAPIC mode */
	base.fields.xAPIC_enable = 1U;
	msr_write(MSR_IA32_APIC_BASE, base.value);
	/* Step2: Enable LAPIC in x2APIC mode */
	base.fields.x2APIC_enable = 1U;
	msr_write(MSR_IA32_APIC_BASE, base.value);
}

/**
 * @pre pcpu_id < 8U
 */
void init_lapic(uint16_t pcpu_id)
{
	per_cpu(lapic_ldr, pcpu_id) = (uint32_t)msr_read(MSR_IA32_EXT_APIC_LDR);
	/* Mask all LAPIC LVT entries before enabling the local APIC */
	msr_write(MSR_IA32_EXT_APIC_LVT_CMCI, LAPIC_LVT_MASK);
	msr_write(MSR_IA32_EXT_APIC_LVT_TIMER, LAPIC_LVT_MASK);
	msr_write(MSR_IA32_EXT_APIC_LVT_THERMAL, LAPIC_LVT_MASK);
	msr_write(MSR_IA32_EXT_APIC_LVT_PMI, LAPIC_LVT_MASK);
	msr_write(MSR_IA32_EXT_APIC_LVT_LINT0, LAPIC_LVT_MASK);
	msr_write(MSR_IA32_EXT_APIC_LVT_LINT1, LAPIC_LVT_MASK);
	msr_write(MSR_IA32_EXT_APIC_LVT_ERROR, LAPIC_LVT_MASK);

	/* Enable Local APIC */
	/* TODO: add spurious-interrupt handler */
	msr_write(MSR_IA32_EXT_APIC_SIVR, LAPIC_SVR_VECTOR);

	/* Reset timer */
	msr_write(MSR_IA32_EXT_APIC_DIV_CONF, 0x00000000U);
	msr_write(MSR_IA32_TSC_DEADLINE, 0x00000000U);

	/* Reset interrupt control registers */
	msr_write(MSR_IA32_EXT_APIC_ICR, 0x0000000000000000U);

	/* Ensure there are no ISR bits set. */
	clear_lapic_isr();
}

uint32_t get_cur_lapic_id(void)
{
	uint32_t lapic_id;

	lapic_id = (uint32_t)msr_read(MSR_IA32_EXT_XAPICID);

	return lapic_id;
}

/**
 * @pre cpu_startup_shorthand < INTR_CPU_STARTUP_UNKNOWN
 */
void send_startup_ipi(__unused enum intr_cpu_startup_shorthand cpu_startup_shorthand, uint16_t dest_pcpu_id,
	uint64_t cpu_startup_start_address)
{
	union apic_icr icr;
	uint8_t shorthand;

	icr.value = 0U;
	icr.bits.destination_mode = INTR_LAPIC_ICR_PHYSICAL;
	shorthand = INTR_LAPIC_ICR_USE_DEST_ARRAY;
	icr.value_32.hi_32 = per_cpu(lapic_id, dest_pcpu_id);

	/* Assert INIT IPI */
	icr.bits.shorthand = shorthand;
	icr.bits.delivery_mode = INTR_LAPIC_ICR_INIT;
	msr_write(MSR_IA32_EXT_APIC_ICR, icr.value);

	/* Send Start IPI with page number of secondary reset code */
	icr.value_32.lo_32 = 0U;
	icr.bits.shorthand = shorthand;
	icr.bits.delivery_mode = INTR_LAPIC_ICR_STARTUP;
	icr.bits.vector = (uint8_t)(cpu_startup_start_address >> 12U);
	msr_write(MSR_IA32_EXT_APIC_ICR, icr.value);
}

/**
 * @pre pcpu_id < MAX_PCPU_NUM
 *
 * @return None
 */
void send_single_init(uint16_t pcpu_id)
{
	union apic_icr icr;

	/*
	 * Intel SDM Vol3 23.8:
	 *   The INIT signal is blocked whenever a logical processor is in VMX root operation.
	 *   It is not blocked in VMX nonroot operation. Instead, INITs cause VM exits
	 */
	icr.value_32.hi_32 = per_cpu(lapic_id, pcpu_id);
	icr.value_32.lo_32 = (INTR_LAPIC_ICR_PHYSICAL << 11U) | (INTR_LAPIC_ICR_INIT << 8U);

	msr_write(MSR_IA32_EXT_APIC_ICR, icr.value);
}

/**
 * @}
 */
