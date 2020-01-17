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
 * Mock-up implementation for the stm32f7xx_hal_rng module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_RNG_TH_HH
#define STM32F7XX_HAL_RNG_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_rng.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RNG_Init, RNG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RNG_DeInit, RNG_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_RNG_MspInit, RNG_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_RNG_MspDeInit, RNG_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_RNG_GetRandomNumber, RNG_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_RNG_GetRandomNumber_IT, RNG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RNG_GenerateRandomNumber, RNG_HandleTypeDef*, uint32_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RNG_GenerateRandomNumber_IT, RNG_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_RNG_ReadLastRandomNumber, RNG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_RNG_StateTypeDef, HAL_RNG_GetState, RNG_HandleTypeDef*);

# endif /* STM32F7XX_HAL_RNG_TH_HH */
