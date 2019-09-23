/*******************************************************************************
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
 ******************************************************************************/

#ifndef KISO_BSP_CELLULAR_SARAR4N4_TH_HH_
#define KISO_BSP_CELLULAR_SARAR4N4_TH_HH_

#include <fff.h>

#include "Kiso_HAL_th.hh"

#include "Kiso_BSP_Cellular_SaraR4N4.h"

#if KISO_FEATURE_BSP_CELLULAR_SARAR4N4
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_Cellular_SaraR4N4_GetUARTHandle);
FAKE_VALUE_FUNC(HWHandle_T, BSP_Cellular_SaraR4N4_GetSPIHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_Reset);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_Control, uint32_t, void *);
FAKE_VALUE_FUNC(uint32_t, BSP_Cellular_SaraR4N4_GetCTS);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_SetRTSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_Cellular_SaraR4N4_SetRTSHigh);
#endif

#endif
