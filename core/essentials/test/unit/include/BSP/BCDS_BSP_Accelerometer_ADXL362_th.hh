/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef BCDS_BCDS_BSP_Accel_ADXL362_TH_HH
#define BCDS_BCDS_BSP_Accel_ADXL362_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Accelerometer_ADXL362.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_ACCEL_ADXL362

FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int1Enable, BSP_Accel_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int2Enable, BSP_Accel_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int1Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Int2Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_Accel_ADXL362_GetSPIHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_SetCSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_Accel_ADXL362_SetCSHigh);


#endif /* BCDS_FEATURE_BSP_ACCEL_ADXL362 */

#endif /* BCDS_BCDS_BSP_Accel_ADXL362_TH_HH */

