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
 * Mock-up implementation for the stm32f7xx_hal_dac module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DAC_TH_HH
#define STM32F7XX_HAL_DAC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dac.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_Init, DAC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_DeInit, DAC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DAC_MspInit, DAC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DAC_MspDeInit, DAC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_Start, DAC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_Stop, DAC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_Start_DMA, DAC_HandleTypeDef*, uint32_t, uint32_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_Stop_DMA, DAC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_DAC_GetValue, DAC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_ConfigChannel, DAC_HandleTypeDef*, DAC_ChannelConfTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DAC_SetValue, DAC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_DAC_StateTypeDef, HAL_DAC_GetState, DAC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_DAC_GetError, DAC_HandleTypeDef*);

# endif /* STM32F7XX_HAL_DAC_TH_HH */