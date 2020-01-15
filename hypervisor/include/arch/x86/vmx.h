/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VMX_H_
#define VMX_H_

/**
 * @addtogroup hwmgmt_vmx
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

/* 16-bit control fields */
#define VMX_VPID 0x00000000U
/* 16-bit guest-state fields */
#define VMX_GUEST_ES_SEL   0x00000800U
#define VMX_GUEST_CS_SEL   0x00000802U
#define VMX_GUEST_SS_SEL   0x00000804U
#define VMX_GUEST_DS_SEL   0x00000806U
#define VMX_GUEST_FS_SEL   0x00000808U
#define VMX_GUEST_GS_SEL   0x0000080aU
#define VMX_GUEST_LDTR_SEL 0x0000080cU
#define VMX_GUEST_TR_SEL   0x0000080eU
/* 16-bit host-state fields */
#define VMX_HOST_ES_SEL 0x00000c00U
#define VMX_HOST_CS_SEL 0x00000c02U
#define VMX_HOST_SS_SEL 0x00000c04U
#define VMX_HOST_DS_SEL 0x00000c06U
#define VMX_HOST_FS_SEL 0x00000c08U
#define VMX_HOST_GS_SEL 0x00000c0aU
#define VMX_HOST_TR_SEL 0x00000c0cU
/* 64-bit control fields */
#define VMX_IO_BITMAP_A_FULL         0x00002000U
#define VMX_IO_BITMAP_B_FULL         0x00002002U
#define VMX_MSR_BITMAP_FULL          0x00002004U
#define VMX_EXIT_MSR_STORE_ADDR_FULL 0x00002006U
#define VMX_EXIT_MSR_LOAD_ADDR_FULL  0x00002008U
#define VMX_ENTRY_MSR_LOAD_ADDR_FULL 0x0000200aU
#define VMX_EXECUTIVE_VMCS_PTR_FULL  0x0000200cU
#define VMX_TSC_OFFSET_FULL          0x00002010U
#define VMX_EPT_POINTER_FULL         0x0000201AU

#define VMX_XSS_EXITING_BITMAP_FULL 0x0000202CU
/* 64-bit read-only data fields */
#define VMX_GUEST_PHYSICAL_ADDR_FULL 0x00002400U
/* 64-bit guest-state fields */
#define VMX_VMS_LINK_PTR_FULL        0x00002800U
#define VMX_GUEST_IA32_DEBUGCTL_FULL 0x00002802U
#define VMX_GUEST_IA32_PAT_FULL      0x00002804U
#define VMX_GUEST_IA32_EFER_FULL     0x00002806U
#define VMX_GUEST_PDPTE0_FULL        0x0000280AU
#define VMX_GUEST_PDPTE1_FULL        0x0000280CU
#define VMX_GUEST_PDPTE2_FULL        0x0000280EU
#define VMX_GUEST_PDPTE3_FULL        0x00002810U
/* 64-bit host-state fields */
#define VMX_HOST_IA32_PAT_FULL  0x00002C00U
#define VMX_HOST_IA32_EFER_FULL 0x00002C02U
/* 32-bit control fields */
#define VMX_PIN_VM_EXEC_CONTROLS       0x00004000U
#define VMX_PROC_VM_EXEC_CONTROLS      0x00004002U
#define VMX_EXCEPTION_BITMAP           0x00004004U
#define VMX_PF_ERROR_CODE_MASK         0x00004006U
#define VMX_PF_ERROR_CODE_MATCH        0x00004008U
#define VMX_CR3_TARGET_COUNT           0x0000400aU
#define VMX_EXIT_CONTROLS              0x0000400cU
#define VMX_EXIT_MSR_STORE_COUNT       0x0000400eU
#define VMX_EXIT_MSR_LOAD_COUNT        0x00004010U
#define VMX_ENTRY_CONTROLS             0x00004012U
#define VMX_ENTRY_MSR_LOAD_COUNT       0x00004014U
#define VMX_ENTRY_INT_INFO_FIELD       0x00004016U
#define VMX_ENTRY_EXCEPTION_ERROR_CODE 0x00004018U
#define VMX_ENTRY_INSTR_LENGTH         0x0000401aU
#define VMX_TPR_THRESHOLD              0x0000401cU
#define VMX_PROC_VM_EXEC_CONTROLS2     0x0000401EU
/* 32-bit read-only data fields */
#define VMX_INSTR_ERROR         0x00004400U
#define VMX_EXIT_REASON         0x00004402U
#define VMX_EXIT_INT_INFO       0x00004404U
#define VMX_EXIT_INT_ERROR_CODE 0x00004406U
#define VMX_IDT_VEC_INFO_FIELD  0x00004408U
#define VMX_IDT_VEC_ERROR_CODE  0x0000440aU
#define VMX_EXIT_INSTR_LEN      0x0000440cU
/* 32-bit guest-state fields */
#define VMX_GUEST_ES_LIMIT              0x00004800U
#define VMX_GUEST_CS_LIMIT              0x00004802U
#define VMX_GUEST_SS_LIMIT              0x00004804U
#define VMX_GUEST_DS_LIMIT              0x00004806U
#define VMX_GUEST_FS_LIMIT              0x00004808U
#define VMX_GUEST_GS_LIMIT              0x0000480aU
#define VMX_GUEST_LDTR_LIMIT            0x0000480cU
#define VMX_GUEST_TR_LIMIT              0x0000480eU
#define VMX_GUEST_GDTR_LIMIT            0x00004810U
#define VMX_GUEST_IDTR_LIMIT            0x00004812U
#define VMX_GUEST_ES_ATTR               0x00004814U
#define VMX_GUEST_CS_ATTR               0x00004816U
#define VMX_GUEST_SS_ATTR               0x00004818U
#define VMX_GUEST_DS_ATTR               0x0000481aU
#define VMX_GUEST_FS_ATTR               0x0000481cU
#define VMX_GUEST_GS_ATTR               0x0000481eU
#define VMX_GUEST_LDTR_ATTR             0x00004820U
#define VMX_GUEST_TR_ATTR               0x00004822U
#define VMX_GUEST_INTERRUPTIBILITY_INFO 0x00004824U
#define VMX_GUEST_ACTIVITY_STATE        0x00004826U
#define VMX_GUEST_SMBASE                0x00004828U
#define VMX_GUEST_IA32_SYSENTER_CS      0x0000482aU
/* 32-bit host-state fields */
#define VMX_HOST_IA32_SYSENTER_CS 0x00004c00U
/* natural-width control fields */
#define VMX_CR0_GUEST_HOST_MASK 0x00006000U
#define VMX_CR4_GUEST_HOST_MASK 0x00006002U
#define VMX_CR0_READ_SHADOW     0x00006004U
#define VMX_CR4_READ_SHADOW     0x00006006U
#define VMX_CR3_TARGET_0        0x00006008U
#define VMX_CR3_TARGET_1        0x0000600aU
#define VMX_CR3_TARGET_2        0x0000600cU
#define VMX_CR3_TARGET_3        0x0000600eU
/* natural-width read-only data fields */
#define VMX_EXIT_QUALIFICATION 0x00006400U
#define VMX_GUEST_LINEAR_ADDR  0x0000640aU
/* natural-width guest-state fields */
#define VMX_GUEST_CR0                  0x00006800U
#define VMX_GUEST_CR3                  0x00006802U
#define VMX_GUEST_CR4                  0x00006804U
#define VMX_GUEST_ES_BASE              0x00006806U
#define VMX_GUEST_CS_BASE              0x00006808U
#define VMX_GUEST_SS_BASE              0x0000680aU
#define VMX_GUEST_DS_BASE              0x0000680cU
#define VMX_GUEST_FS_BASE              0x0000680eU
#define VMX_GUEST_GS_BASE              0x00006810U
#define VMX_GUEST_LDTR_BASE            0x00006812U
#define VMX_GUEST_TR_BASE              0x00006814U
#define VMX_GUEST_GDTR_BASE            0x00006816U
#define VMX_GUEST_IDTR_BASE            0x00006818U
#define VMX_GUEST_DR7                  0x0000681aU
#define VMX_GUEST_RSP                  0x0000681cU
#define VMX_GUEST_RIP                  0x0000681eU
#define VMX_GUEST_RFLAGS               0x00006820U
#define VMX_GUEST_PENDING_DEBUG_EXCEPT 0x00006822U
#define VMX_GUEST_IA32_SYSENTER_ESP    0x00006824U
#define VMX_GUEST_IA32_SYSENTER_EIP    0x00006826U
/* natural-width host-state fields */
#define VMX_HOST_CR0               0x00006c00U
#define VMX_HOST_CR3               0x00006c02U
#define VMX_HOST_CR4               0x00006c04U
#define VMX_HOST_FS_BASE           0x00006c06U
#define VMX_HOST_GS_BASE           0x00006c08U
#define VMX_HOST_TR_BASE           0x00006c0aU
#define VMX_HOST_GDTR_BASE         0x00006c0cU
#define VMX_HOST_IDTR_BASE         0x00006c0eU
#define VMX_HOST_IA32_SYSENTER_ESP 0x00006c10U
#define VMX_HOST_IA32_SYSENTER_EIP 0x00006c12U
#define VMX_HOST_RIP               0x00006c16U
/*
 * Basic VM exit reasons
 */
#define VMX_EXIT_REASON_EXCEPTION_OR_NMI                  0x00000000U
#define VMX_EXIT_REASON_EXTERNAL_INTERRUPT                0x00000001U
#define VMX_EXIT_REASON_TRIPLE_FAULT                      0x00000002U
#define VMX_EXIT_REASON_INIT_SIGNAL                       0x00000003U
#define VMX_EXIT_REASON_STARTUP_IPI                       0x00000004U
#define VMX_EXIT_REASON_IO_SMI                            0x00000005U
#define VMX_EXIT_REASON_OTHER_SMI                         0x00000006U
#define VMX_EXIT_REASON_INTERRUPT_WINDOW                  0x00000007U
#define VMX_EXIT_REASON_NMI_WINDOW                        0x00000008U
#define VMX_EXIT_REASON_TASK_SWITCH                       0x00000009U
#define VMX_EXIT_REASON_CPUID                             0x0000000AU
#define VMX_EXIT_REASON_GETSEC                            0x0000000BU
#define VMX_EXIT_REASON_HLT                               0x0000000CU
#define VMX_EXIT_REASON_INVD                              0x0000000DU
#define VMX_EXIT_REASON_INVLPG                            0x0000000EU
#define VMX_EXIT_REASON_RDPMC                             0x0000000FU
#define VMX_EXIT_REASON_RDTSC                             0x00000010U
#define VMX_EXIT_REASON_RSM                               0x00000011U
#define VMX_EXIT_REASON_VMCALL                            0x00000012U
#define VMX_EXIT_REASON_VMCLEAR                           0x00000013U
#define VMX_EXIT_REASON_VMLAUNCH                          0x00000014U
#define VMX_EXIT_REASON_VMPTRLD                           0x00000015U
#define VMX_EXIT_REASON_VMPTRST                           0x00000016U
#define VMX_EXIT_REASON_VMREAD                            0x00000017U
#define VMX_EXIT_REASON_VMRESUME                          0x00000018U
#define VMX_EXIT_REASON_VMWRITE                           0x00000019U
#define VMX_EXIT_REASON_VMXOFF                            0x0000001AU
#define VMX_EXIT_REASON_VMXON                             0x0000001BU
#define VMX_EXIT_REASON_CR_ACCESS                         0x0000001CU
#define VMX_EXIT_REASON_DR_ACCESS                         0x0000001DU
#define VMX_EXIT_REASON_IO_INSTRUCTION                    0x0000001EU
#define VMX_EXIT_REASON_RDMSR                             0x0000001FU
#define VMX_EXIT_REASON_WRMSR                             0x00000020U
#define VMX_EXIT_REASON_ENTRY_FAILURE_INVALID_GUEST_STATE 0x00000021U
#define VMX_EXIT_REASON_ENTRY_FAILURE_MSR_LOADING         0x00000022U
/* entry 0x23 (35) is missing */
#define VMX_EXIT_REASON_MWAIT        0x00000024U
#define VMX_EXIT_REASON_MONITOR_TRAP 0x00000025U
/* entry 0x26 (38) is missing */
#define VMX_EXIT_REASON_MONITOR                     0x00000027U
#define VMX_EXIT_REASON_PAUSE                       0x00000028U
#define VMX_EXIT_REASON_ENTRY_FAILURE_MACHINE_CHECK 0x00000029U
/* entry 0x2A (42) is missing */
#define VMX_EXIT_REASON_TPR_BELOW_THRESHOLD          0x0000002BU
#define VMX_EXIT_REASON_APIC_ACCESS                  0x0000002CU
#define VMX_EXIT_REASON_VIRTUALIZED_EOI              0x0000002DU
#define VMX_EXIT_REASON_GDTR_IDTR_ACCESS             0x0000002EU
#define VMX_EXIT_REASON_LDTR_TR_ACCESS               0x0000002FU
#define VMX_EXIT_REASON_EPT_VIOLATION                0x00000030U
#define VMX_EXIT_REASON_EPT_MISCONFIGURATION         0x00000031U
#define VMX_EXIT_REASON_INVEPT                       0x00000032U
#define VMX_EXIT_REASON_RDTSCP                       0x00000033U
#define VMX_EXIT_REASON_VMX_PREEMPTION_TIMER_EXPIRED 0x00000034U
#define VMX_EXIT_REASON_INVVPID                      0x00000035U
#define VMX_EXIT_REASON_WBINVD                       0x00000036U
#define VMX_EXIT_REASON_XSETBV                       0x00000037U
#define VMX_EXIT_REASON_APIC_WRITE                   0x00000038U
#define VMX_EXIT_REASON_RDRAND                       0x00000039U
#define VMX_EXIT_REASON_INVPCID                      0x0000003AU
#define VMX_EXIT_REASON_VMFUNC                       0x0000003BU
#define VMX_EXIT_REASON_ENCLS                        0x0000003CU
#define VMX_EXIT_REASON_RDSEED                       0x0000003DU
#define VMX_EXIT_REASON_PAGE_MODIFICATION_LOG_FULL   0x0000003EU
#define VMX_EXIT_REASON_XSAVES                       0x0000003FU
#define VMX_EXIT_REASON_XRSTORS                      0x00000040U

/* VMX execution control bits (pin based) */
#define VMX_PINBASED_CTLS_IRQ_EXIT (1U << 0U)

/* VMX execution control bits (processor based) */
#define VMX_PROCBASED_CTLS_IRQ_WIN     (1U << 2U)
#define VMX_PROCBASED_CTLS_TSC_OFF     (1U << 3U)
#define VMX_PROCBASED_CTLS_INVLPG      (1U << 9U)
#define VMX_PROCBASED_CTLS_MWAIT       (1U << 10U)
#define VMX_PROCBASED_CTLS_RDPMC       (1U << 11U)
#define VMX_PROCBASED_CTLS_CR3_LOAD    (1U << 15U)
#define VMX_PROCBASED_CTLS_CR3_STORE   (1U << 16U)
#define VMX_PROCBASED_CTLS_CR8_LOAD    (1U << 19U)
#define VMX_PROCBASED_CTLS_CR8_STORE   (1U << 20U)
#define VMX_PROCBASED_CTLS_TPR_SHADOW  (1U << 21U)
#define VMX_PROCBASED_CTLS_MOV_DR      (1U << 23U)
#define VMX_PROCBASED_CTLS_IO_BITMAP   (1U << 25U)
#define VMX_PROCBASED_CTLS_MSR_BITMAP  (1U << 28U)
#define VMX_PROCBASED_CTLS_MONITOR     (1U << 29U)
#define VMX_PROCBASED_CTLS_SECONDARY   (1U << 31U)
#define VMX_PROCBASED_CTLS2_VAPIC      (1U << 0U)
#define VMX_PROCBASED_CTLS2_EPT        (1U << 1U)
#define VMX_PROCBASED_CTLS2_RDTSCP     (1U << 3U)
#define VMX_PROCBASED_CTLS2_VPID       (1U << 5U)
#define VMX_PROCBASED_CTLS2_WBINVD     (1U << 6U)
#define VMX_PROCBASED_CTLS2_UNRESTRICT (1U << 7U)
#define VMX_PROCBASED_CTLS2_XSVE_XRSTR (1U << 20U)

#define VMX_EPT_INVEPT_SINGLE_CONTEXT (1U << 25U)
#define VMX_EPT_INVEPT_GLOBAL_CONTEXT (1U << 26U)

#define VMX_VPID_TYPE_SINGLE_CONTEXT 1UL
#define VMX_VPID_TYPE_ALL_CONTEXT    2UL

#define VMX_EXIT_CTLS_HOST_ADDR64 (1U << 9U)
#define VMX_EXIT_CTLS_ACK_IRQ     (1U << 15U)
#define VMX_EXIT_CTLS_SAVE_PAT    (1U << 18U)
#define VMX_EXIT_CTLS_LOAD_PAT    (1U << 19U)
#define VMX_EXIT_CTLS_SAVE_EFER   (1U << 20U)
#define VMX_EXIT_CTLS_LOAD_EFER   (1U << 21U)

#define VMX_ENTRY_CTLS_IA32E_MODE (1U << 9U)
#define VMX_ENTRY_CTLS_LOAD_PAT   (1U << 14U)
#define VMX_ENTRY_CTLS_LOAD_EFER  (1U << 15U)

/* VMX entry/exit Interrupt info */
#define VMX_INT_INFO_ERR_CODE_VALID (1U << 11U)
#define VMX_INT_INFO_VALID          (1U << 31U)
#define VMX_INT_TYPE_MASK           (0x700U)
#define VMX_INT_TYPE_EXT_INT        0U
#define VMX_INT_TYPE_NMI            2U
#define VMX_INT_TYPE_HW_EXP         3U

/* External Interfaces */
void vmx_on(void);
void vmx_off(void);

uint32_t exec_vmread32(uint32_t field);
uint64_t exec_vmread64(uint32_t field_full);
#define exec_vmread exec_vmread64

void exec_vmwrite16(uint32_t field, uint16_t value);
void exec_vmwrite32(uint32_t field, uint32_t value);
void exec_vmwrite64(uint32_t field_full, uint64_t value);
#define exec_vmwrite exec_vmwrite64

void exec_vmclear(void *addr);
void exec_vmptrld(void *addr);

/**
 * @}
 */

#endif /* VMX_H_ */
