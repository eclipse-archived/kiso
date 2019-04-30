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
 *  Mockup implementation for the stream-buffer module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef CDDK_STREAM_BUFFER_TH_HH_
#define CDDK_STREAM_BUFFER_TH_HH_

/* include system header files */
#include <gtest.h>

#include "FreeRTOS_th.hh"

/* ban the real queue.h header */
#define STREAM_BUFFER_H

/* mock-ups for the provided interfaces */
typedef void * StreamBufferHandle_t;

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(StreamBufferHandle_t, xStreamBufferCreate, size_t, size_t);
FAKE_VALUE_FUNC(StreamBufferHandle_t, xStreamBufferCreateStatic, size_t, size_t, uint8_t *, StaticStreamBuffer_t *);
FAKE_VALUE_FUNC(size_t, xStreamBufferSend, StreamBufferHandle_t , const void *, size_t , TickType_t);
FAKE_VALUE_FUNC(size_t, xStreamBufferSendFromISR, StreamBufferHandle_t , const void *, size_t , BaseType_t *);
FAKE_VALUE_FUNC(size_t, xStreamBufferReceive, StreamBufferHandle_t , void *, size_t, TickType_t);
FAKE_VALUE_FUNC(size_t, xStreamBufferReceiveFromISR, StreamBufferHandle_t , void *, size_t, BaseType_t *) ;
FAKE_VOID_FUNC(vStreamBufferDelete, StreamBufferHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xStreamBufferIsFull, StreamBufferHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xStreamBufferIsEmpty, StreamBufferHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xStreamBufferReset, StreamBufferHandle_t);
FAKE_VALUE_FUNC(size_t, xStreamBufferSpacesAvailable, StreamBufferHandle_t);
FAKE_VALUE_FUNC(size_t, xStreamBufferBytesAvailable, StreamBufferHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xStreamBufferSetTriggerLevel, StreamBufferHandle_t, size_t);
FAKE_VALUE_FUNC(BaseType_t, xStreamBufferSendCompletedFromISR, StreamBufferHandle_t, BaseType_t *);
FAKE_VALUE_FUNC(BaseType_t, xStreamBufferReceiveCompletedFromISR, StreamBufferHandle_t, BaseType_t *);

FAKE_VALUE_FUNC(StreamBufferHandle_t, xStreamBufferGenericCreate, size_t, size_t, BaseType_t);
FAKE_VALUE_FUNC(StreamBufferHandle_t, xStreamBufferGenericCreateStatic, size_t, size_t, BaseType_t, uint8_t *, StaticStreamBuffer_t *);


#endif /* CDDK_STREAM_BUFFER_TH_HH_ */

/** ************************************************************************* */
