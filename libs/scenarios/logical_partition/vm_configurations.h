/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef VM_CONFIGURATIONS_H
#define VM_CONFIGURATIONS_H

/**
 * @addtogroup hwmgmt_configs
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

#include <pci_devices.h>
#include <misc_cfg.h>

/* Bits mask of guest flags that can be programmed by device model. Other bits are set by hypervisor only */
#define DM_OWNED_GUEST_FLAG_MASK 0UL

#define CONFIG_MAX_VM_NUM 2U

/* The VM CONFIGs like:
 *	VMX_CONFIG_VCPU_AFFINITY
 *	VMX_CONFIG_MEM_START_HPA
 *	VMX_CONFIG_MEM_SIZE
 *	VMX_CONFIG_OS_BOOTARG_ROOT
 *	VMX_CONFIG_OS_BOOTARG_MAX_CPUS
 *	VMX_CONFIG_OS_BOOTARG_CONSOLE
 * might be different on your board, please modify them per your needs.
 */

#define VM0_CONFIG_VCPU_AFFINITY \
	{                        \
		AFFINITY_CPU(0U) \
	}
#define VM0_CONFIG_MEM_START_HPA      0x100000000UL
#define VM0_CONFIG_MEM_SIZE           0x20000000UL
#define VM0_CONFIG_OS_BOOTARG_ROOT    ROOTFS_0
#define VM0_CONFIG_OS_BOOTARG_MAXCPUS "maxcpus=2 "
#define VM0_CONFIG_OS_BOOTARG_CONSOLE "console=ttyS0 "

#define VM1_CONFIG_VCPU_AFFINITY                                     \
	{                                                            \
		AFFINITY_CPU(1U), AFFINITY_CPU(2U), AFFINITY_CPU(3U) \
	}
#define VM1_CONFIG_MEM_START_HPA      0x120000000UL
#define VM1_CONFIG_MEM_SIZE           0x20000000UL
#define VM1_CONFIG_OS_BOOTARG_ROOT    ROOTFS_0
#define VM1_CONFIG_OS_BOOTARG_MAXCPUS "maxcpus=3 "
#define VM1_CONFIG_OS_BOOTARG_CONSOLE "console=ttyS0 "

/* VM pass-through devices assign policy:
 * VM0: one Mass Storage controller, one Network controller;
 * VM1: one Mass Storage controller, one Network controller(if a secondary Network controller class device exist);
 */
#define VM0_NETWORK_CONTROLLER ETHERNET_CONTROLLER
#define VM0_CONFIG_PCI_DEV_NUM 2U

#define VM1_STORAGE_CONTROLLER USB_CONTROLLER
#define VM1_CONFIG_PCI_DEV_NUM 2U

/**
 * @}
 */

#endif /* VM_CONFIGURATIONS_H */
