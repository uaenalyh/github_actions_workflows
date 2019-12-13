/*
 * Copyright (C) 2018 Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <types.h>
#include <rtl.h>
#include <logmsg.h>

/**
 * @defgroup lib lib
 * @brief {TBD brief description}
 *
 * {TBD detailed description, including purposes, designed usages, usage remarks and dependency justification}
 */

/**
 * @defgroup lib_utils lib.utils
 * @ingroup lib
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

/**
 *
 *    strnlen_s
 *
 * description:
 *    The function calculates the length of the string pointed
 *    to by str.
 *
 *
 * input:
 *    str      pointer to the null-terminated string to be examined.
 *
 *    dmax      maximum number of characer to examine.
 *
 * return value:
 *    string length, excluding the null character.
 *    will return 0 if str is null.
 */
size_t strnlen_s(const char *str_arg, size_t maxlen_arg)
{
	const char *str = str_arg;
	size_t count = 0U;

	if (str != NULL) {
		size_t maxlen = maxlen_arg;
		while ((*str) != '\0') {
			if (maxlen == 0U) {
				break;
			}

			count++;
			maxlen--;
			str++;
		}
	}

	return count;
}

/**
 * @pre n_arg > 0
 */
int32_t strncmp(const char *s1_arg, const char *s2_arg, size_t n_arg)
{
	const char *str1 = s1_arg;
	const char *str2 = s2_arg;
	size_t n = n_arg;
	int32_t ret = 0;

	if (n > 0U) {
		while (((n - 1) != 0U) && ((*str1) != '\0') && ((*str2) != '\0') && ((*str1) == (*str2))) {
			str1++;
			str2++;
			n--;
		}
		ret = (int32_t)(*str1 - *str2);
	}

	return ret;
}

/**
 * @}
 */
