/*
 * Copyright (c) 2011 NetApp, Inc.
 * Copyright (c) 2018 Intel Corporation
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

#include <vm.h>
#include "vpci_priv.h"
#include <ept.h>
#include <logmsg.h>

/**
 * @addtogroup vp-dm_vperipheral
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
 * @pre vdev != NULL
 */
uint32_t pci_vdev_read_cfg(const struct pci_vdev *vdev, uint32_t offset, uint32_t bytes)
{
	uint32_t val;

	switch (bytes) {
	case 1U:
		val = pci_vdev_read_cfg_u8(vdev, offset);
		break;
	case 2U:
		val = pci_vdev_read_cfg_u16(vdev, offset);
		break;
	default:
		val = pci_vdev_read_cfg_u32(vdev, offset);
		break;
	}

	return val;
}

/**
 * @pre vdev != NULL
 */
void pci_vdev_write_cfg(struct pci_vdev *vdev, uint32_t offset, uint32_t bytes, uint32_t val)
{
	switch (bytes) {
	case 1U:
		pci_vdev_write_cfg_u8(vdev, offset, (uint8_t)val);
		break;
	case 2U:
		pci_vdev_write_cfg_u16(vdev, offset, (uint16_t)val);
		break;
	default:
		pci_vdev_write_cfg_u32(vdev, offset, val);
		break;
	}
}

/**
 * @pre vpci != NULL
 * @pre vpci->pci_vdev_cnt <= CONFIG_MAX_PCI_DEV_NUM
 */
struct pci_vdev *pci_find_vdev(struct acrn_vpci *vpci, union pci_bdf vbdf)
{
	struct pci_vdev *vdev, *tmp;
	uint32_t i;

	vdev = NULL;
	for (i = 0U; i < vpci->pci_vdev_cnt; i++) {
		tmp = &(vpci->pci_vdevs[i]);

		if (tmp->bdf.value == vbdf.value) {
			vdev = tmp;
			break;
		}
	}

	return vdev;
}

uint32_t pci_vdev_read_bar(const struct pci_vdev *vdev, uint32_t idx)
{
	uint32_t bar, offset;

	offset = pci_bar_offset(idx);
	bar = pci_vdev_read_cfg_u32(vdev, offset);
	/* Sizing BAR */
	if (bar == ~0U) {
		bar = vdev->bar[idx].mask;
	}
	return bar;
}

/* check the limitation for BAR range */
#define PCI_VBAR_BASE_LIMIT 0xC0000000UL
#define PCI_VBAR_TOP_LIMIT  0xE0000000UL
static bool vbar_base_is_valid(uint64_t bar_base, uint64_t size)
{
	bool result = true;

	if ((bar_base < PCI_VBAR_BASE_LIMIT) || ((bar_base + size) > PCI_VBAR_TOP_LIMIT)) {
		result = false;
	}

	return result;
}

static void pci_vdev_update_bar_base(struct pci_vdev *vdev, uint32_t idx)
{
	struct pci_bar *vbar;
	enum pci_bar_type type;
	uint64_t base = 0UL;
	uint32_t lo, hi, offset;

	vbar = &vdev->bar[idx];
	offset = pci_bar_offset(idx);
	lo = pci_vdev_read_cfg_u32(vdev, offset);
	if ((vbar->type != PCIBAR_NONE) && (lo != ~0U)) {
		type = vbar->type;
		base = lo & vbar->mask;

		if (vbar->type == PCIBAR_MEM64) {
			vbar = &vdev->bar[idx + 1U];
			hi = pci_vdev_read_cfg_u32(vdev, offset + 4U);
			if (hi != ~0U) {
				hi &= vbar->mask;
				base |= ((uint64_t)hi << 32U);
			} else {
				base = 0UL;
			}
		}
		if (type == PCIBAR_IO_SPACE) {
			base &= 0xffffUL;
		}
	}

	if ((base != 0UL) && !vbar_base_is_valid(base, vdev->bar[idx].size)) {
		pr_fatal("%s, %x:%x.%x set invalid bar[%d] base: 0x%lx, size: 0x%lx\n", __func__, vdev->bdf.bits.b,
			vdev->bdf.bits.d, vdev->bdf.bits.f, idx, base, vdev->bar[idx].size);
		/* If guest set a invalid GPA, ignore it temporarily */
		base = 0UL;
	}

	vdev->bar[idx].base = base;
}

void pci_vdev_write_bar(struct pci_vdev *vdev, uint32_t idx, uint32_t val)
{
	struct pci_bar *vbar;
	uint32_t bar, offset;
	uint32_t update_idx = idx;

	vbar = &vdev->bar[idx];
	bar = val & vbar->mask;
	bar |= vbar->fixed;
	offset = pci_bar_offset(idx);
	pci_vdev_write_cfg_u32(vdev, offset, bar);

	if (vbar->type == PCIBAR_MEM64HI) {
		update_idx -= 1U;
	}

	pci_vdev_update_bar_base(vdev, update_idx);
}

/**
 * @}
 */
