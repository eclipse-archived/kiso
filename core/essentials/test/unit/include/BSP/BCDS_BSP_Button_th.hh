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

#ifndef BCDS_BCDS_BSP_Button_th_HH
#define BCDS_BCDS_BSP_Button_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Button.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_BUTTON

FAKE_VALUE_FUNC(Retcode_T, BSP_Button_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Button_Enable, uint32_t, BSP_Button_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_Button_Disable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_Button_Disconnect);
FAKE_VALUE_FUNC(uint32_t, BSP_Button_GetState, uint32_t);

#endif /* BCDS_FEATURE_BSP_BUTTON */
#endif /* BCDS_BCDS_BSP_Button_th_HH */
