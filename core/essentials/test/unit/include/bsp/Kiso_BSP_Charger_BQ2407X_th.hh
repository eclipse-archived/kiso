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

#ifndef KISO_BSP_CHARGER_BQ2407X_TH_HH
#define KISO_BSP_CHARGER_BQ2407X_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Charger_BQ2407X.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if KISO_FEATURE_BSP_CHARGER_BQ2407X

FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Enable, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_GetStatus, BSP_ChargeState_T *)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_MeasureSignal, uint32_t, uint32_t *)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_Disconnect)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_SetSysoffHigh)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_SetSysoffLow)
FAKE_VALUE_FUNC(Retcode_T, BSP_Charger_BQ2407X_CheckStatus, BSP_ChargeState_T *, uint32_t)
#endif /* KISO_FEATURE_BSP_CHARGER_BQ2407X */
#endif /* KISO_BSP_CHARGER_BQ2407X_TH_HH */
