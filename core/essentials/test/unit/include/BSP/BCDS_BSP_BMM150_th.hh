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
 
#ifndef BCDS_BSP_BMM150_TH_HH
#define BCDS_BSP_BMM150_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_BMM150.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_BMM150

FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_Connect, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_Enable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_DrdyEnable, int32_t, BSP_BMM150_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_IntEnable, int32_t, BSP_BMM150_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_DrdyDisable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_IntDisable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_Disable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_Disconnect, int32_t);
FAKE_VALUE_FUNC(HAL_DeviceHandle_T, BSP_BMM150_GetHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_SetCSLow, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_BMM150_SetCSHigh, int32_t);


#endif /* BCDS_FEATURE_BSP_BMM150 */

#endif /* BCDS_BSP_BMM150_TH_HH */
