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

#ifndef BCDS_BCDS_BSP_PWM_LED_th_HH
#define BCDS_BCDS_BSP_PWM_LED_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_PWM_LED.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_PWM_LED

FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_Connect );
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_Enable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_EnableAll );
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_Switch, uint32_t, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_SwitchAll, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_Disable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_DisableAll );
FAKE_VALUE_FUNC(Retcode_T, BSP_PWM_LED_Disconnect );

#endif /* BCDS_FEATURE_BSP_PWM_LED */
#endif /* BCDS_BCDS_BSP_PWM_LED_th_HH */
