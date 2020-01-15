/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * @addtogroup lib_utils
 *
 * @{
 */

/**
 * @file
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */
#include <types.h>

/** Roundup (x/y) to ( x/y + (x%y) ? 1 : 0) **/
#define INT_DIV_ROUNDUP(x, y) ((((x) + (y)) - 1U) / (y))

#define min(x, y) ((x) < (y)) ? (x) : (y)

#define max(x, y) ((x) < (y)) ? (y) : (x)

/** Replaces 'x' by the string "x". */
#define STRINGIFY(x) #x

/* Macro used to check if a value is aligned to the required boundary.
 * Returns TRUE if aligned; FALSE if not aligned
 * NOTE:  The required alignment must be a power of 2 (2, 4, 8, 16, 32, etc)
 */
static inline bool mem_aligned_check(uint64_t value, uint64_t req_align)
{
	return ((value & (req_align - 1UL)) == 0UL);
}

/**
 * @pre buf != NULL
 */
static inline uint8_t calculate_sum8(const void *buf, uint32_t length)
{
	uint32_t i;
	uint8_t sum = 0U;

	for (i = 0U; i < length; i++) {
		sum += *((const uint8_t *)buf + i);
	}

	return sum;
}

/**
 * @pre buf != NULL
 */
static inline uint8_t calculate_checksum8(const void *buf, uint32_t len)
{
	return (uint8_t)(0x100U - calculate_sum8(buf, len));
}

/**
 * @}
 */

#endif /* UTIL_H */
