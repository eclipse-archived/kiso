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
 *  Mockup implementation for the semphr module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_SEMPHR_TH_HH_
#define BCDS_SEMPHR_TH_HH_

/* include system header files */
#include <gtest.h>
#include "queue_th.hh"

/* ban the real semphr.h header */
#define SEMAPHORE_H

/* mock-ups for the provided interfaces */
typedef QueueHandle_t SemaphoreHandle_t;

/* @note : This is a work around for a bug that was conflicting
 * with the in built gcov's __gcov.vSemaphoreDelete variable
 * resulting in a multiple definition error - mta5cob */
#define vSemaphoreDelete( xSemaphore ) vQueueDelete( ( QueueHandle_t ) ( xSemaphore ) );

#if (configSUPPORT_STATIC_ALLOCATION == 1)
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateBinaryStatic, StaticSemaphore_t *);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateMutexStatic, StaticSemaphore_t *);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateRecursiveMutexStatic, StaticSemaphore_t *);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateCountingStatic, UBaseType_t, UBaseType_t, StaticSemaphore_t *);
#endif /* configSUPPORT_STATIC_ALLOCATION */

#if (configSUPPORT_DYNAMIC_ALLOCATION == 1)
FAKE_VOID_FUNC(vSemaphoreCreateBinary, SemaphoreHandle_t);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateBinary);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateMutex);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateRecursiveMutex);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreCreateCounting, UBaseType_t, UBaseType_t);
#endif /* configSUPPORT_DYNAMIC_ALLOCATION */

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreTake, SemaphoreHandle_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreTakeRecursive, SemaphoreHandle_t, TickType_t);

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGive, SemaphoreHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGiveRecursive, SemaphoreHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGiveFromISR, SemaphoreHandle_t, BaseType_t*);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreTakeFromISR, SemaphoreHandle_t, BaseType_t*);

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGetMutexHolder, SemaphoreHandle_t);

#endif /* BCDS_SEMPHR_TH_HH_ */

/** ************************************************************************* */
