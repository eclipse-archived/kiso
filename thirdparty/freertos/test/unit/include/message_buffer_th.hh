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
 *  Mockup implementation for the message-buffer module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef KISO_MESSAGE_BUFFER_TH_HH_
#define KISO_MESSAGE_BUFFER_TH_HH_

/* include system header files */
#include <gtest.h>

#include "FreeRTOS_th.hh"

/* ban the real queue.h header */
#define MESSAGE_BUFFER_H

/* mock-ups for the provided interfaces */
typedef void *MessageBufferHandle_t;

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(MessageBufferHandle_t, xMessageBufferCreate, size_t)
FAKE_VALUE_FUNC(MessageBufferHandle_t, xMessageBufferCreateStatic, size_t, uint8_t *, StaticMessageBuffer_t *)
FAKE_VALUE_FUNC(size_t, xMessageBufferSend, MessageBufferHandle_t, const void *, size_t, TickType_t)
FAKE_VALUE_FUNC(size_t, xMessageBufferSendFromISR, MessageBufferHandle_t, const void *, size_t, BaseType_t *)
FAKE_VALUE_FUNC(size_t, xMessageBufferReceive, MessageBufferHandle_t, void *, size_t, TickType_t)
FAKE_VALUE_FUNC(size_t, xMessageBufferReceiveFromISR, MessageBufferHandle_t, void *, size_t, BaseType_t *);
FAKE_VOID_FUNC(vMessageBufferDelete, MessageBufferHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xMessageBufferIsFull, MessageBufferHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xMessageBufferIsEmpty, MessageBufferHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xMessageBufferReset, MessageBufferHandle_t)
FAKE_VALUE_FUNC(size_t, xMessageBufferSpacesAvailable, MessageBufferHandle_t)
FAKE_VALUE_FUNC(BaseType_t, xMessageBufferSendCompletedFromISR, MessageBufferHandle_t, BaseType_t *)
FAKE_VALUE_FUNC(BaseType_t, xMessageBufferReceiveCompletedFromISR, MessageBufferHandle_t, BaseType_t *)

#endif /* KISO_MESSAGE_BUFFER_TH_HH_ */

/** ************************************************************************* */
