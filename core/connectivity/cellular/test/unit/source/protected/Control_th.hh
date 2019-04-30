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

#ifndef CELLULAR_CONTROL_TH_HH
#define CELLULAR_CONTROL_TH_HH

/* Include all headers which are needed by this file. */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Control.h"

/* Put the function declarations here */
FAKE_VALUE_FUNC(Retcode_T, Cellular_Startup);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Shutdown);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Reset);
FAKE_VALUE_FUNC(Retcode_T, Cellular_GetPowerStatus, Cellular_PowerStatus_T *);
FAKE_VALUE_FUNC(Retcode_T, Cellular_IsSimCardInserted, bool *);
FAKE_VALUE_FUNC(Retcode_T, Cellular_GetIccid, uint8_t *, uint32_t, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_EnableEcho, bool);

#endif /* CELLULAR_CONTROL_TH_HH */
