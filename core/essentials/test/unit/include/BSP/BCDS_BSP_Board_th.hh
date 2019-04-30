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

#ifndef BCDS_BCDS_BSP_Board_th_HH
#define BCDS_BCDS_BSP_Board_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Board.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(Retcode_T, BSP_Board_Initialize, uint32_t, void*);
FAKE_VALUE_FUNC(Retcode_T, BSP_Board_SwitchPowerMode, uint32_t);
FAKE_VALUE_FUNC(uint32_t, BSP_Board_GetWakeupSource);
FAKE_VALUE_FUNC(uint32_t, BSP_Board_DetectPowerSupply);
FAKE_VALUE_FUNC(uint32_t, BSP_Board_GetBoardRevision);
FAKE_VALUE_FUNC(Retcode_T, BSP_Board_ControlPowerRail, uint32_t);
FAKE_VOID_FUNC(BSP_Board_OSTickInitialize, BSP_Systick_Callback_T, BSP_Systick_Callback_T);
FAKE_VOID_FUNC(BSP_Board_Delay, uint32_t);
FAKE_VOID_FUNC(BSP_Board_SetOStoBSPDelayFunc, OStoBSPDelayFunc_T);
FAKE_VOID_FUNC(BSP_Board_DisableSysTick);
FAKE_VOID_FUNC(BSP_Board_SoftReset);

#endif /* BCDS_BCDS_BSP_Board_th_HH */
