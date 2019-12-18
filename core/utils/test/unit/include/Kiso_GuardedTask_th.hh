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
 * @ingroup UTILS
 *
 * @defgroup GUARDEDTASK_TESTS GuardedTask Unit Tests
 * @{
 *
 * @brief
 *      Mockup implementation for the @ref GUARDEDTASK module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_GUARDEDTASK_TH_HH_
#define KISO_GUARDEDTASK_TH_HH_

/* Include gtest file */
#include <gtest.h>

/* Include system header files */
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS header files */
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "semphr_th.hh"

/* Wrap Kiso_GuardedTask interface header */
#include "Kiso_GuardedTask.h"

/* Faked variables needs to be initialized by the test fixture */
typedef void (*GuardedTask_Function_T)(void);

/**
 * Mock-ups for the provided interfaces
 * Deprecated functions will be removed as part of PFM-659
 * Deprecated since vXXX (\todo: replace XXX with next version)
 */
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_initialize, GuardedTask_T *, GuardedTask_Function_T, const char *, uint32_t, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_deinitialize, GuardedTask_T *)
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_signal, GuardedTask_T *)
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_signalFromIsr, GuardedTask_T *)

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_Initialize, GuardedTask_T *, GuardedTask_Function_T, const char *, uint32_t, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_Deinitialize, GuardedTask_T *)
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_Signal, GuardedTask_T *)
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_SignalFromIsr, GuardedTask_T *)

#endif /* KISO_GUARDEDTASK_TH_HH_ */

/** ************************************************************************* */
