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
 * @defgroup EVENTHUB_TESTS EventHub Unit Tests
 * @{
 *
 * @brief
 *      Mockup implementation for the @ref EVENTHUB module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_EVENTHUB_TH_HH_
#define KISO_EVENTHUB_TH_HH_

/* Include gtest header file */
#include "gtest.h"

/* KISO header files */
#include "Kiso_Retcode_th.hh"
#include "Kiso_EventHub.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, EventHub_Initialize, EventHub_T *)
FAKE_VALUE_FUNC(Retcode_T, EventHub_Observe, EventHub_T *, EventHandler_T, TaskEvent_T)
FAKE_VALUE_FUNC(Retcode_T, EventHub_ObserveAll, EventHub_T *, EventHandler_T)
FAKE_VALUE_FUNC(Retcode_T, EventHub_Notify, EventHub_T *, TaskEvent_T, void *)

#endif /* KISO_EVENTHUB_TH_HH_ */

/** ************************************************************************* */
