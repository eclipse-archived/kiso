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
 *
 * @file
 *
 * Mock-up implementation for the stm32f7xx_hal_dsi module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DSI_TH_HH
#define STM32F7XX_HAL_DSI_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dsi.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if defined (STM32F769xx) || defined (STM32F779xx)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_Init, DSI_HandleTypeDef*, DSI_PLLInitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_DeInit, DSI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DSI_MspInit, DSI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DSI_MspDeInit, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetGenericVCID, DSI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigVideoMode, DSI_HandleTypeDef*, DSI_VidCfgTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigAdaptedCommandMode, DSI_HandleTypeDef*, DSI_CmdCfgTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigCommand, DSI_HandleTypeDef*, DSI_LPCmdTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigFlowControl, DSI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigPhyTimer, DSI_HandleTypeDef*, DSI_PHY_TimerTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigHostTimeouts, DSI_HandleTypeDef*, DSI_HOST_TimeoutTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_Start, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_Stop, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_Refresh, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ColorMode, DSI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_Shutdown, DSI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ShortWrite, DSI_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_LongWrite, DSI_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_Read, DSI_HandleTypeDef*, uint32_t, uint8_t*, uint32_t, uint32_t, uint32_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_EnterULPMData, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ExitULPMData, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_EnterULPM, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ExitULPM, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_PatternGeneratorStart, DSI_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_PatternGeneratorStop, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetSlewRateAndDelayTuning, DSI_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetLowPowerRXFilter, DSI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetSDD, DSI_HandleTypeDef*, FunctionalState);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetLanePinsConfiguration, DSI_HandleTypeDef*, uint32_t, uint32_t, FunctionalState);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetPHYTimings, DSI_HandleTypeDef*, uint32_t, FunctionalState, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ForceTXStopMode, DSI_HandleTypeDef*, uint32_t, FunctionalState);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ForceRXLowPower, DSI_HandleTypeDef*, FunctionalState);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ForceDataLanesInRX, DSI_HandleTypeDef*, FunctionalState);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetPullDown, DSI_HandleTypeDef*, FunctionalState);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_SetContentionDetectionOff, DSI_HandleTypeDef*, FunctionalState);

FAKE_VALUE_FUNC(uint32_t, HAL_DSI_GetError, DSI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DSI_ConfigErrorMonitor, DSI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_DSI_StateTypeDef, HAL_DSI_GetState, DSI_HandleTypeDef*);

#endif /*STM32F769xx | STM32F779xx */

# endif /* STM32F7XX_HAL_DSI_TH_HH */
