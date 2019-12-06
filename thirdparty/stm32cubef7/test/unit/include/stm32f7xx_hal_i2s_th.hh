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
 * Mock-up implementation for the stm32f7xx_hal_i2s module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_I2S_TH_HH
#define STM32F7XX_HAL_I2S_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_i2s.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Init, I2S_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_DeInit, I2S_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_I2S_MspInit, I2S_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_I2S_MspDeInit, I2S_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Transmit, I2S_HandleTypeDef*, uint16_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Receive, I2S_HandleTypeDef*, uint16_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Transmit_IT, I2S_HandleTypeDef*, uint16_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Receive_IT, I2S_HandleTypeDef*, uint16_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Transmit_DMA, I2S_HandleTypeDef*, uint16_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_Receive_DMA, I2S_HandleTypeDef*, uint16_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_DMAPause, I2S_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_DMAResume, I2S_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_I2S_DMAStop, I2S_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_I2S_StateTypeDef, HAL_I2S_GetState, I2S_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_I2S_GetError, I2S_HandleTypeDef*);

# endif /* STM32F7XX_HAL_I2S_TH_HH */