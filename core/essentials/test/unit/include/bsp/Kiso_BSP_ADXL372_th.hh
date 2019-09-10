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
 
#ifndef KISO_BSP_ADXL372_TH_HH
#define KISO_BSP_ADXL372_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_ADXL372.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_ADXL372

FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Connect, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Enable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int1Enable, int32_t, BSP_ADXL372_InterruptCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int2Enable, int32_t, BSP_ADXL372_InterruptCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int1Disable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int2Disable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Disable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Disconnect, int32_t)
FAKE_VALUE_FUNC(HAL_DeviceHandle_T, BSP_ADXL372_GetHandle)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_SetCSLow, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_SetCSHigh, int32_t)


#endif /* KISO_FEATURE_BSP_ADXL372 */

#endif /* KISO_BSP_ADXL372_TH_HH */
