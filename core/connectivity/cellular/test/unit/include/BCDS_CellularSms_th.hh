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

#ifndef BCDS_CELLULARSMS_TH_HH
#define BCDS_CELLULARSMS_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"

#include "BCDS_Cellular_Sms.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, CellularSms_Initialize, Sms_CallbackFunction_T);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_Deinitialize);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_SendText, uint8_t*, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_SetServiceCenter, uint8_t*);
FAKE_VALUE_FUNC(Retcode_T, CellularSms_GetServiceCenter, uint8_t*, uint8_t*);

#endif /* BCDS_CELLULARSMS_TH_HH */
