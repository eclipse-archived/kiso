/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/
 
#ifndef BCDS_BCDS_BSP_Accelerometer_H3LIS_th_HH
#define BCDS_BCDS_BSP_Accelerometer_H3LIS_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Accelerometer_H3LIS.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_ACCEL_H3LIS

FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_IntEnable, BSP_Accel_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_IntDisable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_Accel_H3LIS_GetSPIHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_SetCSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_H3LIS_SetCSHigh);

#endif /* BCDS_FEATURE_BSP_ACCEL_H3LIS */

#endif /* BCDS_BCDS_BSP_Accelerometer_H3LIS_th_HH */

