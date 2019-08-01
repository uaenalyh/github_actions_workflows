/*
 * Copyright (C) 2018 Intel Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <types.h>
#include <errno.h>
#include <bits.h>
#include <atomic.h>
#include <irq.h>
#include <cpu.h>
#include <per_cpu.h>
#include <lapic.h>

static uint32_t notification_irq = IRQ_INVALID;

static uint64_t smp_call_mask = 0UL;

/* run in interrupt context */
static void kick_notification(__unused uint32_t irq, __unused void *data)
{
	/* Notification vector is used to kick taget cpu out of non-root mode.
	 * And it also serves for smp call.
	 */
	uint16_t pcpu_id = get_pcpu_id();

	if (bitmap_test(pcpu_id, &smp_call_mask)) {
		struct smp_call_info_data *smp_call =
			&per_cpu(smp_call_info, pcpu_id);

		if (smp_call->func != NULL) {
			smp_call->func(smp_call->data);
		}
		bitmap_clear_lock(pcpu_id, &smp_call_mask);
	}
}

static int32_t request_notification_irq(irq_action_t func, void *data)
{
	int32_t retval;

	if (notification_irq != IRQ_INVALID) {
		pr_info("%s, Notification vector already allocated on this CPU", __func__);
		retval = -EBUSY;
	} else {
		/* all cpu register the same notification vector */
		retval = request_irq(NOTIFY_IRQ, func, data, IRQF_NONE);
		if (retval < 0) {
			pr_err("Failed to add notify isr");
			retval = -ENODEV;
		} else {
			notification_irq = (uint32_t)retval;
		}
	}

	return retval;
}

/*
 * @pre be called only by BSP initialization process
 */
void setup_notification(void)
{
	/* support IPI notification, SOS_VM will register all CPU */
	if (request_notification_irq(kick_notification, NULL) < 0) {
		pr_err("Failed to setup notification");
	}

	dev_dbg(ACRN_DBG_PTIRQ, "NOTIFY: irq[%d] setup vector %x",
		notification_irq, irq_to_vector(notification_irq));
}

static void posted_intr_notification(__unused uint32_t irq, __unused void *data)
{
	/* Dummy IRQ handler for case that Posted-Interrupt Notification
	 * is sent to vCPU in root mode(isn't running),interrupt will be
	 * picked up in next vmentry,do nothine here.
	 */
}

/*pre-conditon: be called only by BSP initialization proccess*/
void setup_posted_intr_notification(void)
{
	if (request_irq(POSTED_INTR_NOTIFY_IRQ,
			posted_intr_notification,
			NULL, IRQF_NONE) < 0) {
		pr_err("Failed to setup posted-intr notification");
	}
}
