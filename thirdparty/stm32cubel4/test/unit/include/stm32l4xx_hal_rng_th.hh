/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32L4XX_HAL_RNG_TH_HH_
#define STM32L4XX_HAL_RNG_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_rng.h"

/* Include the needed headers */
#include "gtest.h"

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
FAKE_VOID_FUNC(HAL_RNG_IRQHandler, RNG_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_RNG_StateTypeDef, HAL_RNG_GetState, RNG_HandleTypeDef*);

#endif /* STM32L4XX_HAL_RNG_TH_HH_ */
