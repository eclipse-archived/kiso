/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/
/**
 * @file
 *
 *
 *  Mockup implementation for the queue module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef KISO_QUEUE_TH_HH_
#define KISO_QUEUE_TH_HH_

/* include system header files */
#include <gtest.h>
#include "FreeRTOS_th.hh"

/* ban the real queue.h header */
#define QUEUE_H

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
typedef void* QueueHandle_t;
typedef void* QueueSetHandle_t;
typedef void* QueueSetMemberHandle_t;

FAKE_VALUE_FUNC(QueueHandle_t, xQueueCreate, UBaseType_t, UBaseType_t)

FAKE_VALUE_FUNC(BaseType_t, xQueueSendToFront, QueueHandle_t, void*, TickType_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueSendToBack, QueueHandle_t, void*, TickType_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueSend, QueueHandle_t, void*, TickType_t)

FAKE_VALUE_FUNC(BaseType_t, xQueueOverwrite, QueueHandle_t, void*)

FAKE_VALUE_FUNC(BaseType_t, xQueueGenericSend, QueueHandle_t, void*, TickType_t, BaseType_t)

FAKE_VALUE_FUNC(BaseType_t, xQueuePeek, QueueHandle_t, void*, TickType_t)
FAKE_VALUE_FUNC(BaseType_t, xQueuePeekFromISR, QueueHandle_t, void*)

FAKE_VALUE_FUNC(BaseType_t, xQueueReceive, QueueHandle_t, void*, TickType_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueGenericReceive, QueueHandle_t, void*, TickType_t, BaseType_t)

FAKE_VALUE_FUNC(BaseType_t, uxQueueMessagesWaiting, QueueHandle_t)
FAKE_VALUE_FUNC(UBaseType_t, uxQueueSpacesAvailable, QueueHandle_t)

FAKE_VOID_FUNC(vQueueDelete, QueueHandle_t)

FAKE_VALUE_FUNC(BaseType_t, xQueueSendToFrontFromISR, QueueHandle_t, void*, BaseType_t*)
FAKE_VALUE_FUNC(BaseType_t, xQueueSendToBackFromISR, QueueHandle_t, void*, BaseType_t*)

FAKE_VALUE_FUNC(BaseType_t, xQueueOverwriteFromISR, QueueHandle_t, void*, BaseType_t*)

FAKE_VALUE_FUNC(BaseType_t, xQueueSendFromISR, QueueHandle_t, void*, BaseType_t*)

FAKE_VALUE_FUNC(BaseType_t, xQueueGenericSendFromISR, QueueHandle_t, void*, BaseType_t*, BaseType_t)

FAKE_VALUE_FUNC(BaseType_t, xQueueGiveFromISR, QueueHandle_t, BaseType_t*)

FAKE_VALUE_FUNC(BaseType_t, xQueueReceiveFromISR, QueueHandle_t, void*, BaseType_t*)

FAKE_VALUE_FUNC(BaseType_t, xQueueIsQueueEmptyFromISR, QueueHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueIsQueueFullFromISR, QueueHandle_t)
FAKE_VALUE_FUNC(UBaseType_t, uxQueueMessagesWaitingFromISR, QueueHandle_t)

FAKE_VALUE_FUNC(QueueHandle_t, xQueueCreateMutex, uint8_t)
FAKE_VALUE_FUNC(QueueHandle_t, xQueueCreateCountingSemaphore, UBaseType_t, UBaseType_t)
FAKE_VALUE_FUNC(void*, xQueueGetMutexHolder, QueueHandle_t)

FAKE_VALUE_FUNC(BaseType_t, xQueueTakeMutexRecursive, QueueHandle_t, TickType_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueGiveMutexRecursive, QueueHandle_t)

FAKE_VALUE_FUNC(BaseType_t, xQueueReset, QueueHandle_t)

FAKE_VALUE_FUNC(QueueHandle_t, xQueueGenericCreate, UBaseType_t, UBaseType_t, uint8_t)

FAKE_VALUE_FUNC(QueueSetHandle_t, xQueueCreateSet, UBaseType_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueAddToSet, QueueSetMemberHandle_t, QueueSetHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xQueueRemoveFromSet, QueueSetMemberHandle_t, QueueSetHandle_t)

FAKE_VALUE_FUNC(QueueSetMemberHandle_t, xQueueSelectFromSet, QueueSetHandle_t, TickType_t)
FAKE_VALUE_FUNC(QueueSetMemberHandle_t, xQueueSelectFromSetFromISR, QueueSetHandle_t)

#endif /* KISO_QUEUE_TH_HH_ */

/** ************************************************************************* */
