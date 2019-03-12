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

#ifndef BCDS_BSP_LED_LM2755_TH_HH_
#define BCDS_BSP_LED_LM2755_TH_HH_

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_LED_LM2755.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_LED_LM2755

FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_LM2755_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_LED_LM2755_GetI2CHandle );

#endif /* BCDS_FEATURE_BSP_LED_LM2755 */

#endif /* BCDS_BSP_LED_LM2755_TH_HH_ */
