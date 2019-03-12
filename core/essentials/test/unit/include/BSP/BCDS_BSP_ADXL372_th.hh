/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch GmbH.
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
 
#ifndef BCDS_BSP_ADXL372_TH_HH
#define BCDS_BSP_ADXL372_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_ADXL372.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_ADXL372

FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Connect, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Enable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int1Enable, int32_t, BSP_ADXL372_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int2Enable, int32_t, BSP_ADXL372_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int1Disable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Int2Disable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Disable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_Disconnect, int32_t);
FAKE_VALUE_FUNC(HAL_DeviceHandle_T, BSP_ADXL372_GetHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_SetCSLow, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_ADXL372_SetCSHigh, int32_t);


#endif /* BCDS_FEATURE_BSP_ADXL372 */

#endif /* BCDS_BSP_ADXL372_TH_HH */
