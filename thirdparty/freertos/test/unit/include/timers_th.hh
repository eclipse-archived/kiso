/****************************************************************************/
/**
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 *
 *  @file
 *
 *
 *  Mockup implementation for the timers module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_TIMERS_TH_HH_
#define BCDS_TIMERS_TH_HH_

/* include system header files */
#include <gtest.h>

#include "FreeRTOS_th.hh"
#include "task_th.hh"

/* ban the real timers.h header */
#define TIMERS_H

/* mock-ups for the provided interfaces */
typedef void* TimerHandle_t;
typedef void (*TimerCallbackFunction_t)(TimerHandle_t xTimer);
typedef void (*PendedFunction_t)(void*, uint32_t);

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(TimerHandle_t, xTimerCreate, const char*, TickType_t, UBaseType_t, void*, TimerCallbackFunction_t);
FAKE_VALUE_FUNC(void*, pvTimerGetTimerID, TimerHandle_t);
FAKE_VOID_FUNC(vTimerSetTimerID, TimerHandle_t, void*);
FAKE_VALUE_FUNC(BaseType_t, xTimerIsTimerActive, TimerHandle_t);
FAKE_VALUE_FUNC(TaskHandle_t, xTimerGetTimerDaemonTaskHandle);
FAKE_VALUE_FUNC(BaseType_t, xTimerStart, TimerHandle_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerStop, TimerHandle_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerChangePeriod, TimerHandle_t, TickType_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerDelete, TimerHandle_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerReset, TimerHandle_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerStartFromISR, TimerHandle_t, BaseType_t*);
FAKE_VALUE_FUNC(BaseType_t, xTimerStopFromISR, TimerHandle_t, BaseType_t*);
FAKE_VALUE_FUNC(BaseType_t, xTimerChangePeriodFromISR, TimerHandle_t, TickType_t, BaseType_t*)
FAKE_VALUE_FUNC(BaseType_t, xTimerResetFromISR, TimerHandle_t, BaseType_t*);
FAKE_VALUE_FUNC(BaseType_t, xTimerPendFunctionCallFromISR, PendedFunction_t, void*, uint32_t, BaseType_t*);
FAKE_VALUE_FUNC(BaseType_t, xTimerPendFunctionCall, PendedFunction_t, void*, uint32_t, TickType_t);
FAKE_VALUE_FUNC(const char*, pcTimerGetTimerName, TimerHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerCreateTimerTask);
FAKE_VALUE_FUNC(BaseType_t, xTimerGenericCommand, TimerHandle_t, BaseType_t, TickType_t, BaseType_t*, TickType_t);

#endif /* BCDS_TIMERS_TH_HH_ */

/** ************************************************************************* */
