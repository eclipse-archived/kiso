/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch.
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
 
#ifndef BCDS_BSP_TMP112_TH_HH_
#define BCDS_BSP_TMP112_TH_HH_

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_TMP112.h"
/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_TMP112

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Connect, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Enable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Disable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Disconnect, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_InterruptEnable, uint32_t, BSP_TMP112_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_InterruptDisable, uint32_t);
FAKE_VALUE_FUNC(HWHandle_T, BSP_TMP112_GetHandle);

#endif /* BCDS_FEATURE_TMP112DRIVER */

#endif /* BCDS_BSP_TMP112_TH_HH_ */
