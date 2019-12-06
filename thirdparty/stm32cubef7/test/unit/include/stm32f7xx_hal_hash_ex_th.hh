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
 * Mock-up implementation for the stm32f7xx_hal_hash_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_HASH_EX_TH_HH
#define STM32F7XX_HAL_HASH_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include"stm32f7xx_hal_hash_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_hash_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (STM32F756xx) || defined (STM32F777xx) || defined (STM32F779xx)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA224_Start, HASH_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA256_Start, HASH_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA224_Accumulate, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA256_Accumulate, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HMACEx_SHA224_Start, HASH_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HMACEx_SHA256_Start, HASH_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA224_Start_IT, HASH_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA256_Start_IT, HASH_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA224_Start_DMA, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA224_Finish, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA256_Start_DMA, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HASHEx_SHA256_Finish, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HMACEx_SHA224_Start_DMA, HASH_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HMACEx_SHA256_Start_DMA, HASH_HandleTypeDef*, uint8_t*, uint32_t);

#endif /* STM32F756xx || STM32F777xx || STM32F779xx */

# endif /* STM32F7XX_HAL_HASH_EX_TH_HH */
