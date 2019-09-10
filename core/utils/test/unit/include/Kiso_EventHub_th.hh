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
#ifndef KISO_EVENTHUB_TH_HH_
#define KISO_EVENTHUB_TH_HH_

#include "gtest.h"

#include "Kiso_Retcode_th.hh"
#include "Kiso_EventHub.h"

FAKE_VALUE_FUNC(Retcode_T, EventHub_Initialize, EventHub_T*)
FAKE_VALUE_FUNC(Retcode_T, EventHub_Observe, EventHub_T*, EventHandler_T, TaskEvent_T)
FAKE_VALUE_FUNC(Retcode_T, EventHub_ObserveAll, EventHub_T*, EventHandler_T)
FAKE_VALUE_FUNC(Retcode_T, EventHub_Notify, EventHub_T*, TaskEvent_T, void*)

#endif /* KISO_EVENTHUB_TH_HH_ */

/** ************************************************************************* */
