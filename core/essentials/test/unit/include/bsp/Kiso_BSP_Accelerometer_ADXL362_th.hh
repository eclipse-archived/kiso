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

#ifndef KISO_KISO_BSP_Accel_ADXL362_TH_HH
#define KISO_KISO_BSP_Accel_ADXL362_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_Accelerometer_ADXL362.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_ACCEL_ADXL362

FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Enable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int1Enable, BSP_Accel_InterruptCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int2Enable, BSP_Accel_InterruptCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int1Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int2Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Disconnect)
FAKE_VALUE_FUNC(HWHandle_T, BSP_Accel_ADXL362_GetSPIHandle)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_SetCSLow)
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_SetCSHigh)

#endif /* KISO_FEATURE_BSP_ACCEL_ADXL362 */

#endif /* KISO_KISO_BSP_Accel_ADXL362_TH_HH */
