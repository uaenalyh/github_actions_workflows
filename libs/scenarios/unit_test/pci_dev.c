/*
 * Copyright (C) 2019 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <vm_config.h>
#include <pci_devices.h>
#include <vpci.h>

struct acrn_vm_pci_dev_config vm0_pci_devs[VM0_CONFIG_PCI_DEV_NUM] = {
	{
		.emu_type = PCI_DEV_TYPE_HVEMUL,
		.vbdf.bits = { .b = 0x00U, .d = 0x00U, .f = 0x00U },
		.vdev_ops = &vhostbridge_ops,
	},
};

struct acrn_vm_pci_dev_config vm1_pci_devs[VM1_CONFIG_PCI_DEV_NUM] = {
	{
		.emu_type = PCI_DEV_TYPE_HVEMUL,
		.vbdf.bits = { .b = 0x00U, .d = 0x00U, .f = 0x00U },
		.vdev_ops = &vhostbridge_ops,
	},
};
