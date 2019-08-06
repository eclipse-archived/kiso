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

 
/* header definition ******************************************************** */
#ifndef BCDS_TASKMONITOR_TH_HH_
#define BCDS_TASKMONITOR_TH_HH_

/* include system header files */
#include "gtest.h"

/* wrap the real  interface header */
#include "BCDS_TaskMonitor.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, TaskMonitor_Initialize);
FAKE_VALUE_FUNC(bool, TaskMonitor_Check);
FAKE_VALUE_FUNC(Retcode_T, TaskMonitor_Register, TaskHandle_t, uint32_t);

#endif /* BCDS_TASKMONITOR_TH_HH_ */

/** ************************************************************************* */
