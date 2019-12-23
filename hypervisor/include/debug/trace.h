/*
 * ACRN TRACE
 *
 * Copyright (C) 2017 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Li Fei <fei1.li@intel.com>
 *
 */

#ifndef TRACE_H
#define TRACE_H

/**
 * @addtogroup debug
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */
#include <per_cpu.h>

#define TRACE_VM_EXIT      0x10U
#define TRACE_VM_ENTER     0X11U
#define TRACE_VMEXIT_ENTRY 0x10000U

#define TRACE_VMEXIT_EXCEPTION_OR_NMI     (TRACE_VMEXIT_ENTRY + 0x00000000U)
#define TRACE_VMEXIT_EXTERNAL_INTERRUPT   (TRACE_VMEXIT_ENTRY + 0x00000001U)
#define TRACE_VMEXIT_INTERRUPT_WINDOW     (TRACE_VMEXIT_ENTRY + 0x00000002U)
#define TRACE_VMEXIT_CPUID                (TRACE_VMEXIT_ENTRY + 0x00000004U)
#define TRACE_VMEXIT_CR_ACCESS            (TRACE_VMEXIT_ENTRY + 0x0000001CU)
#define TRACE_VMEXIT_IO_INSTRUCTION       (TRACE_VMEXIT_ENTRY + 0x0000001EU)
#define TRACE_VMEXIT_RDMSR                (TRACE_VMEXIT_ENTRY + 0x0000001FU)
#define TRACE_VMEXIT_WRMSR                (TRACE_VMEXIT_ENTRY + 0x00000020U)
#define TRACE_VMEXIT_EPT_VIOLATION        (TRACE_VMEXIT_ENTRY + 0x00000030U)
#define TRACE_VMEXIT_EPT_MISCONFIGURATION (TRACE_VMEXIT_ENTRY + 0x00000031U)

#define TRACE_VMEXIT_UNEXPECTED 0x20000U

void TRACE_2L(uint32_t evid, uint64_t e, uint64_t f);
void TRACE_4I(uint32_t evid, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

/**
 * @}
 */

#endif /* TRACE_H */
