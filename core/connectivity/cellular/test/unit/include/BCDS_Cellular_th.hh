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

#ifndef BCDS_CELLULAR_TH_HH_
#define BCDS_CELLULAR_TH_HH_

#include <gtest.h>
#include "BCDS_Cellular.h"

FAKE_VALUE_FUNC(Retcode_T, Cellular_Initialize, Cellular_StateChanged_T);
FAKE_VALUE_FUNC(Retcode_T, Cellular_PowerOn, const Cellular_PowerUpParameters_T*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Reset, const Cellular_PowerUpParameters_T*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_RegisterOnNetwork, const Cellular_NetworkParameters_T*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_DisconnectFromNetwork);
FAKE_VALUE_FUNC(Retcode_T, Cellular_SetAirPlaneMode, bool);
FAKE_VALUE_FUNC(Retcode_T, Cellular_QueryIccid, char*, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_QueryNetworkInfo, Cellular_NetworkInfo_T*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_ConfigureDataContext, uint32_t, const Cellular_DataContextParameters_T*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_ActivateDataContext, uint32_t, const Cellular_DataContext_T**);
FAKE_VALUE_FUNC(Retcode_T, Cellular_DeactivateDataContext, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Cellular_PowerOff);
FAKE_VALUE_FUNC(Retcode_T, Cellular_IsPoweredOn, bool*);
FAKE_VALUE_FUNC(Retcode_T, Cellular_Deinitialize);
FAKE_VALUE_FUNC(Cellular_State_T, Cellular_GetState);

#endif /* BCDS_CELLULAR_TH_HH_ */
