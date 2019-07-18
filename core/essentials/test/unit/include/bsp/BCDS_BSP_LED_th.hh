/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/
 
#ifndef BCDS_BCDS_BSP_LED_th_HH
#define BCDS_BCDS_BSP_LED_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_LED.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(Retcode_T, BSP_LED_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_Enable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_EnableAll);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_Switch, uint32_t, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_SwitchAll, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_Disable, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_DisableAll);
FAKE_VALUE_FUNC(Retcode_T, BSP_LED_Disconnect);

#endif /* BCDS_BCDS_BSP_LED_th_HH */
