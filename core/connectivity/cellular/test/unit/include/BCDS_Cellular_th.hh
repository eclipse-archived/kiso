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

#ifndef BCDS_CELLULAR_TH_HH
#define BCDS_CELLULAR_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Cellular.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, Cellular_Initialize, Cellular_CallbackFunction_T);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Uninitialize);
FAKE_VALUE_FUNC(Cellular_DeviceStatus_T, Cellular_GetDeviceStatus);
FAKE_VALUE_FUNC(Retcode_T, Cellular_QueryModem, Cellular_QueryInfo_T, void*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Control, Cellular_Control_T, void*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_NetworkInit);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Connect);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, Cellular_PowerControl, Cellular_PowerState_T);

#endif /* BCDS_CELLULAR_TH_HH */
