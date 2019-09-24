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

#ifndef STM32L4XX_HAL_GPIO_TH_HH_
#define STM32L4XX_HAL_GPIO_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_gpio.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#undef __HAL_GPIO_EXTI_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_GPIO_EXTI_GET_FLAG, uint32_t)
#undef __HAL_GPIO_EXTI_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_GPIO_EXTI_CLEAR_FLAG, uint32_t)
#undef __HAL_GPIO_EXTI_GET_IT
FAKE_VALUE_FUNC(bool, __HAL_GPIO_EXTI_GET_IT, uint32_t)
#undef __HAL_GPIO_EXTI_CLEAR_IT
FAKE_VOID_FUNC(__HAL_GPIO_EXTI_CLEAR_IT, uint32_t)
#undef __HAL_GPIO_EXTI_GENERATE_SWIT
FAKE_VOID_FUNC(__HAL_GPIO_EXTI_GENERATE_SWIT, uint32_t)

FAKE_VOID_FUNC(HAL_GPIO_Init, GPIO_TypeDef *, GPIO_InitTypeDef *)
FAKE_VOID_FUNC(HAL_GPIO_DeInit, GPIO_TypeDef *, uint32_t)

FAKE_VALUE_FUNC(GPIO_PinState, HAL_GPIO_ReadPin, GPIO_TypeDef *, uint16_t)
FAKE_VOID_FUNC(HAL_GPIO_WritePin, GPIO_TypeDef *, uint16_t, GPIO_PinState)
FAKE_VOID_FUNC(HAL_GPIO_TogglePin, GPIO_TypeDef *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_GPIO_LockPin, GPIO_TypeDef *, uint16_t)
FAKE_VOID_FUNC(HAL_GPIO_EXTI_IRQHandler, uint16_t)

#endif /* STM32L4XX_HAL_GPIO_TH_HH_ */
