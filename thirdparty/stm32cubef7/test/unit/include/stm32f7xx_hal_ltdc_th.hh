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
 * Mock-up implementation for the stm32f7xx_hal_ltdc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_LTDC_TH_HH
#define STM32F7XX_HAL_LTDC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_ltdc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_Init, LTDC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_DeInit, LTDC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_LTDC_MspInit, LTDC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_LTDC_MspDeInit, LTDC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_ConfigLayer, LTDC_HandleTypeDef*, LTDC_LayerCfgTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetWindowSize, LTDC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetWindowPosition, LTDC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetPixelFormat, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetAlpha, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetAddress, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetPitch, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_ConfigColorKeying, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_ConfigCLUT, LTDC_HandleTypeDef*, uint32_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_EnableColorKeying, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_DisableColorKeying, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_EnableCLUT, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_DisableCLUT, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_ProgramLineEvent, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_EnableDither, LTDC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_DisableDither, LTDC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_Reload, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_ConfigLayer_NoReload, LTDC_HandleTypeDef*, LTDC_LayerCfgTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetWindowSize_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetWindowPosition_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetPixelFormat_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetAlpha_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetAddress_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_SetPitch_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_ConfigColorKeying_NoReload, LTDC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_EnableColorKeying_NoReload, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_DisableColorKeying_NoReload, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_EnableCLUT_NoReload, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LTDC_DisableCLUT_NoReload, LTDC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_LTDC_StateTypeDef, HAL_LTDC_GetState, LTDC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_LTDC_GetError, LTDC_HandleTypeDef*);

# endif /* STM32F7XX_HAL_LTDC_TH_HH */
