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
 
#ifndef BCDS_BSP_MAX44009_TH_HH
#define BCDS_BSP_MAX44009_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_MAX44009.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_MAX44009

FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Connect, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Enable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_IntEnable, int32_t, BSP_MAX44009_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_IntDisable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Disable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Disconnect, int32_t);
FAKE_VALUE_FUNC(HWHandle_T, BSP_MAX44009_GetHandle, int32_t);
FAKE_VALUE_FUNC(void*, BSP_MAX44009_GetInitParams, int32_t);


#endif /* BCDS_FEATURE_BSP_MAX44009 */

#endif /* BCDS_BSP_MAX44009_TH_HH */
