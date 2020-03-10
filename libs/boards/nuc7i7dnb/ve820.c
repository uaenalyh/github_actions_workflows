/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <e820.h>
#include <vm.h>

/**
 * @addtogroup vp-base_vm
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

#define VE820_ENTRIES_KBL_NUC_i7 5U
static const struct e820_entry ve820_entry[VE820_ENTRIES_KBL_NUC_i7] = {
	{ /* usable RAM under 1MB */
		.baseaddr = 0x0UL,
		.length = 0xF0000UL, /* 960KB */
		.type = E820_TYPE_RAM },

	{ /* mptable */
		.baseaddr = 0xF0000UL, /* 960KB */
		.length = 0x10000UL, /* 16KB */
		.type = E820_TYPE_RESERVED },

	{ /* lowmem */
		.baseaddr = 0x100000UL, /* 1MB */
		.length = 0x1FF00000UL, /* 511MB */
		.type = E820_TYPE_RAM },

	{ /* between lowmem and PCI hole */
		.baseaddr = 0x20000000UL, /* 512MB */
		.length = 0xA0000000UL, /* 2560MB */
		.type = E820_TYPE_RESERVED },

	{ /* between PCI hole and 4GB */
		.baseaddr = 0xe0000000UL, /* 3.5GB */
		.length = 0x20000000UL, /* 512MB */
		.type = E820_TYPE_RESERVED },
};
/**
 * @pre vm != NULL
 */
void create_prelaunched_vm_e820(struct acrn_vm *vm)
{
	vm->e820_entry_num = VE820_ENTRIES_KBL_NUC_i7;
	vm->e820_entries = ve820_entry;
}

/**
 * @}
 */
