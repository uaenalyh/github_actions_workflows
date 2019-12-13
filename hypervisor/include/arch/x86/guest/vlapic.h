/*-
 * Copyright (c) 2011 NetApp, Inc.
 * Copyright (c) 2017 Intel Corporation
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

#ifndef VLAPIC_H
#define VLAPIC_H

#include <page.h>
#include <timer.h>
#include <apicreg.h>

/**
 * @file vlapic.h
 *
 * @brief public APIs for virtual LAPIC
 */

struct acrn_vlapic {
	/*
	 * Please keep 'apic_page' and 'pir_desc' be the first two fields in
	 * current structure, as below alignment restrictions are mandatory
	 * to support APICv features:
	 * - 'apic_page' MUST be 4KB aligned.
	 * - 'pir_desc' MUST be 64 bytes aligned.
	 * IRR, TMR and PIR could be accessed by other vCPUs when deliver
	 * an interrupt to vLAPIC.
	 */
	struct lapic_regs apic_page;

	struct acrn_vm *vm;
	struct acrn_vcpu *vcpu;

	uint64_t msr_apicbase;

	const struct acrn_apicv_ops *ops;

} __aligned(PAGE_SIZE);

struct acrn_apicv_ops {
	void (*accept_intr)(struct acrn_vlapic *vlapic, uint32_t vector, bool level);
	bool (*inject_intr)(struct acrn_vlapic *vlapic, bool guest_irq_enabled, bool injected);
	bool (*has_pending_delivery_intr)(struct acrn_vcpu *vcpu);
	bool (*apic_read_access_may_valid)(uint32_t offset);
	bool (*apic_write_access_may_valid)(uint32_t offset);
	bool (*x2apic_read_msr_may_valid)(uint32_t offset);
	bool (*x2apic_write_msr_may_valid)(uint32_t offset);
};

extern const struct acrn_apicv_ops ptapic_ops;

/**
 * @brief virtual LAPIC
 *
 * @addtogroup acrn_vlapic ACRN vLAPIC
 * @{
 */

uint64_t vlapic_get_tsc_deadline_msr(const struct acrn_vlapic *vlapic);
void vlapic_set_tsc_deadline_msr(struct acrn_vlapic *vlapic, uint64_t val_arg);
uint64_t vlapic_get_apicbase(const struct acrn_vlapic *vlapic);
int32_t vlapic_x2apic_read(struct acrn_vcpu *vcpu, uint32_t msr, uint64_t *val);
int32_t vlapic_x2apic_write(struct acrn_vcpu *vcpu, uint32_t msr, uint64_t val);

uint32_t vlapic_get_apicid(const struct acrn_vlapic *vlapic);
void vlapic_create(struct acrn_vcpu *vcpu);
/**
 * @pre vlapic->vm != NULL
 * @pre vlapic->vcpu->vcpu_id < MAX_VCPUS_PER_VM
 */
void vlapic_init(struct acrn_vlapic *vlapic);
void vlapic_reset(struct acrn_vlapic *vlapic, const struct acrn_apicv_ops *ops);
void vlapic_calc_dest(struct acrn_vm *vm, uint64_t *dmask, bool is_broadcast, uint32_t dest, bool phys, bool lowprio);
/**
 * @}
 */
/* End of acrn_vlapic */
#endif /* VLAPIC_H */
