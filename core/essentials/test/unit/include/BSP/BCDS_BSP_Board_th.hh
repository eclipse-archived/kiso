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
