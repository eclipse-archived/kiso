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

#ifndef CELLULAR_ENGINE_TH_HH
#define CELLULAR_ENGINE_TH_HH

/* Include all headers which are needed by this file. */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Semphr_th.hh"
#include "BCDS_MCU_UART_th.hh"
#include "CellularEngine.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularEngine_Init, Cellular_CallbackFunction_T);
FAKE_VALUE_FUNC(Retcode_T, CellularEngine_Deinit);
FAKE_VOID_FUNC(CellularEngine_SetFlukeCharFilterEnabled, bool);
FAKE_VALUE_FUNC(Retcode_T, CellularEngine_SendAtCommand, const uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularEngine_SendAtCmdWaitEcho, uint32_t, const char *);
FAKE_VALUE_FUNC(Cellular_DeviceStatus_T, CellularEngine_GetDeviceStatus);
FAKE_VOID_FUNC(CellularEngine_SetDeviceStatus, Cellular_DeviceStatus_T, Cellular_CallbackEvent_T, void *);
FAKE_VALUE_FUNC(Retcode_T, CellularEngine_GetDeviceError);
FAKE_VOID_FUNC(CellularEngine_SetDeviceError, Retcode_T);
FAKE_VALUE_FUNC(Retcode_T, CellularRequest_Dispatch, uint32_t, CellularRequest_CallableFunction_T, void *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularEngine_HandleEvents);
FAKE_VOID_FUNC(CellularEngine_SetCustomAtResponseParser, AtResponseParserCallback_T);
FAKE_VALUE_FUNC(Retcode_T, Cellular_ReadData, uint8_t*, uint32_t, uint32_t*);

#endif /* CELLULAR_ENGINE_TH_HH */
