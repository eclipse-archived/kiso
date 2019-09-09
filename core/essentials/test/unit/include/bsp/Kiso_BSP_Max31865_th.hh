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

#ifndef KISO_BSP_MAX31865_TH_HH
#define KISO_BSP_MAX31865_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Max31865.h"

#if KISO_FEATURE_BSP_MAX31865

FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Disable);
FAKE_VALUE_FUNC(HWHandle_T, BSP_Max31865_GetSPIHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_SetCSHigh);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_SetCSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_GetStatus, uint8_t *);

#endif /* KISO_FEATURE_BSP_MAX31865 */
#endif /* KISO_BSP_MAX31865_TH_HH */
