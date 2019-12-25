/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MSR_H
#define MSR_H

/**
 * @addtogroup vp-base_vmsr
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

/* architectural (common) MSRs */

#define MSR_IA32_TIME_STAMP_COUNTER 0x00000010U
#define MSR_IA32_APIC_BASE          0x0000001BU
#define MSR_IA32_FEATURE_CONTROL    0x0000003AU
#define MSR_IA32_TSC_ADJUST         0x0000003BU
/* Prediction Command */
#define MSR_IA32_PRED_CMD                0x00000049U
#define MSR_IA32_BIOS_UPDT_TRIG          0x00000079U
#define MSR_IA32_BIOS_SIGN_ID            0x0000008BU
#define MSR_IA32_SGXLEPUBKEYHASH0        0x0000008CU
#define MSR_IA32_SGXLEPUBKEYHASH1        0x0000008DU
#define MSR_IA32_SGXLEPUBKEYHASH2        0x0000008EU
#define MSR_IA32_SGXLEPUBKEYHASH3        0x0000008FU
#define MSR_IA32_SMM_MONITOR_CTL         0x0000009BU
#define MSR_IA32_SMBASE                  0x0000009EU
#define MSR_IA32_PMC0                    0x000000C1U
#define MSR_IA32_PMC1                    0x000000C2U
#define MSR_IA32_PMC2                    0x000000C3U
#define MSR_IA32_PMC3                    0x000000C4U
#define MSR_IA32_PMC4                    0x000000C5U
#define MSR_IA32_PMC5                    0x000000C6U
#define MSR_IA32_PMC6                    0x000000C7U
#define MSR_IA32_PMC7                    0x000000C8U
#define MSR_IA32_MTRR_CAP                0x000000FEU
#define MSR_IA32_ARCH_CAPABILITIES       0x0000010AU
#define MSR_IA32_MCG_CAP                 0x00000179U
#define MSR_IA32_MCG_STATUS              0x0000017AU
#define MSR_IA32_MCG_CTL                 0x0000017BU
#define MSR_IA32_PERFEVTSEL0             0x00000186U
#define MSR_IA32_PERFEVTSEL1             0x00000187U
#define MSR_IA32_PERFEVTSEL2             0x00000188U
#define MSR_IA32_PERFEVTSEL3             0x00000189U
#define MSR_IA32_PERF_CTL                0x00000199U
#define MSR_IA32_MISC_ENABLE             0x000001A0U
#define MSR_IA32_SMRR_PHYSBASE           0x000001F2U
#define MSR_IA32_SMRR_PHYSMASK           0x000001F3U
#define MSR_IA32_MTRR_PHYSBASE_0         0x00000200U
#define MSR_IA32_MTRR_PHYSMASK_0         0x00000201U
#define MSR_IA32_MTRR_PHYSBASE_1         0x00000202U
#define MSR_IA32_MTRR_PHYSMASK_1         0x00000203U
#define MSR_IA32_MTRR_PHYSBASE_2         0x00000204U
#define MSR_IA32_MTRR_PHYSMASK_2         0x00000205U
#define MSR_IA32_MTRR_PHYSBASE_3         0x00000206U
#define MSR_IA32_MTRR_PHYSMASK_3         0x00000207U
#define MSR_IA32_MTRR_PHYSBASE_4         0x00000208U
#define MSR_IA32_MTRR_PHYSMASK_4         0x00000209U
#define MSR_IA32_MTRR_PHYSBASE_5         0x0000020AU
#define MSR_IA32_MTRR_PHYSMASK_5         0x0000020BU
#define MSR_IA32_MTRR_PHYSBASE_6         0x0000020CU
#define MSR_IA32_MTRR_PHYSMASK_6         0x0000020DU
#define MSR_IA32_MTRR_PHYSBASE_7         0x0000020EU
#define MSR_IA32_MTRR_PHYSMASK_7         0x0000020FU
#define MSR_IA32_MTRR_PHYSBASE_8         0x00000210U
#define MSR_IA32_MTRR_PHYSMASK_8         0x00000211U
#define MSR_IA32_MTRR_PHYSBASE_9         0x00000212U
#define MSR_IA32_MTRR_PHYSMASK_9         0x00000213U
#define MSR_IA32_MTRR_FIX64K_00000       0x00000250U
#define MSR_IA32_MTRR_FIX16K_80000       0x00000258U
#define MSR_IA32_MTRR_FIX16K_A0000       0x00000259U
#define MSR_IA32_MTRR_FIX4K_C0000        0x00000268U
#define MSR_IA32_MTRR_FIX4K_C8000        0x00000269U
#define MSR_IA32_MTRR_FIX4K_D0000        0x0000026AU
#define MSR_IA32_MTRR_FIX4K_D8000        0x0000026BU
#define MSR_IA32_MTRR_FIX4K_E0000        0x0000026CU
#define MSR_IA32_MTRR_FIX4K_E8000        0x0000026DU
#define MSR_IA32_MTRR_FIX4K_F0000        0x0000026EU
#define MSR_IA32_MTRR_FIX4K_F8000        0x0000026FU
#define MSR_IA32_PAT                     0x00000277U
#define MSR_IA32_MTRR_DEF_TYPE           0x000002FFU
#define MSR_SGXOWNEREPOCH0               0x00000300U
#define MSR_SGXOWNEREPOCH1               0x00000301U
#define MSR_IA32_FIXED_CTR0              0x00000309U
#define MSR_IA32_FIXED_CTR1              0x0000030AU
#define MSR_IA32_FIXED_CTR2              0x0000030BU
#define MSR_IA32_PERF_CAPABILITIES       0x00000345U
#define MSR_IA32_FIXED_CTR_CTL           0x0000038DU
#define MSR_IA32_PERF_GLOBAL_STATUS      0x0000038EU
#define MSR_IA32_PERF_GLOBAL_CTRL        0x0000038FU
#define MSR_IA32_PERF_GLOBAL_OVF_CTRL    0x00000390U
#define MSR_IA32_PERF_GLOBAL_STATUS_SET  0x00000391U
#define MSR_IA32_PERF_GLOBAL_INUSE       0x00000392U
#define MSR_IA32_VMX_BASIC               0x00000480U
#define MSR_IA32_VMX_PINBASED_CTLS       0x00000481U
#define MSR_IA32_VMX_PROCBASED_CTLS      0x00000482U
#define MSR_IA32_VMX_EXIT_CTLS           0x00000483U
#define MSR_IA32_VMX_ENTRY_CTLS          0x00000484U
#define MSR_IA32_VMX_MISC                0x00000485U
#define MSR_IA32_VMX_CR0_FIXED0          0x00000486U
#define MSR_IA32_VMX_CR0_FIXED1          0x00000487U
#define MSR_IA32_VMX_CR4_FIXED0          0x00000488U
#define MSR_IA32_VMX_CR4_FIXED1          0x00000489U
#define MSR_IA32_VMX_VMCS_ENUM           0x0000048AU
#define MSR_IA32_VMX_PROCBASED_CTLS2     0x0000048BU
#define MSR_IA32_VMX_EPT_VPID_CAP        0x0000048CU
#define MSR_IA32_VMX_TRUE_PINBASED_CTLS  0x0000048DU
#define MSR_IA32_VMX_TRUE_PROCBASED_CTLS 0x0000048EU
#define MSR_IA32_VMX_TRUE_EXIT_CTLS      0x0000048FU
#define MSR_IA32_VMX_TRUE_ENTRY_CTLS     0x00000490U
#define MSR_IA32_VMX_VMFUNC              0x00000491U
#define MSR_IA32_A_PMC0                  0x000004C1U
#define MSR_IA32_A_PMC1                  0x000004C2U
#define MSR_IA32_A_PMC2                  0x000004C3U
#define MSR_IA32_A_PMC3                  0x000004C4U
#define MSR_IA32_A_PMC4                  0x000004C5U
#define MSR_IA32_A_PMC5                  0x000004C6U
#define MSR_IA32_A_PMC6                  0x000004C7U
#define MSR_IA32_A_PMC7                  0x000004C8U
#define MSR_IA32_MCG_EXT_CTL             0x000004D0U
#define MSR_IA32_SGX_SVN_STATUS          0x00000500U
#define MSR_IA32_RTIT_OUTPUT_BASE        0x00000560U
#define MSR_IA32_RTIT_OUTPUT_MASK_PTRS   0x00000561U
#define MSR_IA32_RTIT_CTL                0x00000570U
#define MSR_IA32_RTIT_STATUS             0x00000571U
#define MSR_IA32_RTIT_CR3_MATCH          0x00000572U
#define MSR_IA32_RTIT_ADDR0_A            0x00000580U
#define MSR_IA32_RTIT_ADDR0_B            0x00000581U
#define MSR_IA32_RTIT_ADDR1_A            0x00000582U
#define MSR_IA32_RTIT_ADDR1_B            0x00000583U
#define MSR_IA32_RTIT_ADDR2_A            0x00000584U
#define MSR_IA32_RTIT_ADDR2_B            0x00000585U
#define MSR_IA32_RTIT_ADDR3_A            0x00000586U
#define MSR_IA32_RTIT_ADDR3_B            0x00000587U
#define MSR_IA32_DS_AREA                 0x00000600U
#define MSR_IA32_TSC_DEADLINE            0x000006E0U

#define MSR_IA32_EXT_XAPICID          0x00000802U
#define MSR_IA32_EXT_APIC_EOI         0x0000080BU
#define MSR_IA32_EXT_APIC_LDR         0x0000080DU
#define MSR_IA32_EXT_APIC_SIVR        0x0000080FU
#define MSR_IA32_EXT_APIC_ISR0        0x00000810U
#define MSR_IA32_EXT_APIC_ISR7        0x00000817U
#define MSR_IA32_EXT_APIC_LVT_CMCI    0x0000082FU
#define MSR_IA32_EXT_APIC_ICR         0x00000830U
#define MSR_IA32_EXT_APIC_LVT_TIMER   0x00000832U
#define MSR_IA32_EXT_APIC_LVT_THERMAL 0x00000833U
#define MSR_IA32_EXT_APIC_LVT_PMI     0x00000834U
#define MSR_IA32_EXT_APIC_LVT_LINT0   0x00000835U
#define MSR_IA32_EXT_APIC_LVT_LINT1   0x00000836U
#define MSR_IA32_EXT_APIC_LVT_ERROR   0x00000837U
#define MSR_IA32_DEBUG_INTERFACE      0x00000C80U
#define MSR_IA32_L3_QOS_CFG           0x00000C81U
#define MSR_IA32_L2_QOS_CFG           0x00000C82U
#define MSR_IA32_QM_EVTSEL            0x00000C8DU
#define MSR_IA32_QM_CTR               0x00000C8EU
#define MSR_IA32_PQR_ASSOC            0x00000C8FU
#define MSR_IA32_L3_MASK_BASE         0x00000C90U
#define MSR_IA32_XSS                  0x00000DA0U
#define MSR_IA32_BNDCFGS              0x00000D90U
#define MSR_IA32_EFER                 0xC0000080U
#define MSR_IA32_STAR                 0xC0000081U
#define MSR_IA32_LSTAR                0xC0000082U
#define MSR_IA32_FMASK                0xC0000084U
#define MSR_IA32_FS_BASE              0xC0000100U
#define MSR_IA32_GS_BASE              0xC0000101U
#define MSR_IA32_KERNEL_GS_BASE       0xC0000102U
#define MSR_IA32_TSC_AUX              0xC0000103U

#define MSR_PRMRR_PHYS_BASE        0x000001F4U
#define MSR_PRMRR_PHYS_MASK        0x000001F5U
#define MSR_PRMRR_VALID_CONFIG     0x000001FBU
#define MSR_UNCORE_PRMRR_PHYS_BASE 0x000002F4U
#define MSR_UNCORE_PRMRR_PHYS_MASK 0x000002F5U

#define MSR_IA32_EFER_LME_BIT (1UL << 8U) /* IA32e mode enable */
#define MSR_IA32_EFER_LMA_BIT (1UL << 10U) /* IA32e mode active */
#define MSR_IA32_EFER_NXE_BIT (1UL << 11U)

/* FEATURE CONTROL bits */
#define MSR_IA32_FEATURE_CONTROL_LOCK       (1U << 0U)
#define MSR_IA32_FEATURE_CONTROL_VMX_NO_SMX (1U << 2U)

/* PAT memory type definitions */
#define PAT_MEM_TYPE_UC  0x00UL /* uncached */
#define PAT_MEM_TYPE_WT  0x04UL /* write through */
#define PAT_MEM_TYPE_WB  0x06UL /* writeback */
#define PAT_MEM_TYPE_UCM 0x07UL /* uncached minus */

#define MSR_IA32_MISC_ENABLE_MONITOR_ENA (1UL << 18U)
#define MSR_IA32_MISC_ENABLE_LIMIT_CPUID (1UL << 22U)
#define MSR_IA32_MISC_ENABLE_XD_DISABLE  (1UL << 34U)

#ifndef ASSEMBLER
static inline bool pat_mem_type_invalid(uint64_t x)
{
	return ((x & ~0x7UL) != 0UL || (x & 0x6UL) == 0x2UL);
}

static inline bool is_x2apic_msr(uint32_t msr)
{
	/*
	 * if msr is in the range of x2APIC MSRs
	 */
	return ((msr >= 0x800U) && (msr < 0x900U));
}

struct acrn_vcpu;

void init_msr_emulation(struct acrn_vcpu *vcpu);
uint32_t vmsr_get_guest_msr_index(uint32_t msr);
void update_msr_bitmap_x2apic_passthru(struct acrn_vcpu *vcpu);

#endif /* ASSEMBLER */

/* 5 high-order bits in every field are reserved */
#define PAT_FIELD_RSV_BITS (0xF8U)

#define PAT_POWER_ON_VALUE                                                                                  \
	(PAT_MEM_TYPE_WB + (PAT_MEM_TYPE_WT << 8U) + (PAT_MEM_TYPE_UCM << 16U) + (PAT_MEM_TYPE_UC << 24U) + \
		(PAT_MEM_TYPE_WB << 32U) + (PAT_MEM_TYPE_WT << 40U) + (PAT_MEM_TYPE_UCM << 48U) +           \
		(PAT_MEM_TYPE_UC << 56U))

#define PAT_ALL_UC_VALUE                                                                                   \
	(PAT_MEM_TYPE_UC + (PAT_MEM_TYPE_UC << 8U) + (PAT_MEM_TYPE_UC << 16U) + (PAT_MEM_TYPE_UC << 24U) + \
		(PAT_MEM_TYPE_UC << 32U) + (PAT_MEM_TYPE_UC << 40U) + (PAT_MEM_TYPE_UC << 48U) +           \
		(PAT_MEM_TYPE_UC << 56U))

#define IA32_ARCH_CAP_SKIP_L1DFL_VMENTRY (1UL << 3U)
#define IA32_ARCH_CAP_SSB_NO             (1UL << 4U)
#define IA32_ARCH_CAP_MDS_NO             (1UL << 5U)
#define IA32_ARCH_CAP_IF_PSCHANGE_MC_NO  (1UL << 6U)

/**
 * @}
 */

#endif /* MSR_H */
