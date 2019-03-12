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

#ifndef BCDS_BSP_MAX31865_TH_HH
#define BCDS_BSP_MAX31865_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Max31865.h"

#if BCDS_FEATURE_BSP_MAX31865

FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_Disable);
FAKE_VALUE_FUNC(HWHandle_T, BSP_Max31865_GetSPIHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_SetCSHigh);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_SetCSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_Max31865_GetStatus, uint8_t *);

#endif /* BCDS_FEATURE_BSP_MAX31865 */
#endif /* BCDS_BSP_MAX31865_TH_HH */
