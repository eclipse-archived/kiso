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
 * @defgroup TASKMONITOR_TESTS TaskMonitor Unit Tests
 * @{
 *
 * @brief
 *      Mockup implementation for the @ref TASKMONITOR module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_TASKMONITOR_TH_HH_
#define KISO_TASKMONITOR_TH_HH_

/* Include gtest header file */
#include "gtest.h"

/* Wrap Kiso_TaskMonitor interface header */
#include "Kiso_TaskMonitor.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, TaskMonitor_Initialize)
FAKE_VALUE_FUNC(bool, TaskMonitor_Check)
FAKE_VALUE_FUNC(Retcode_T, TaskMonitor_Register, TaskHandle_t, uint32_t)

#endif /* KISO_TASKMONITOR_TH_HH_ */

/** ************************************************************************* */
