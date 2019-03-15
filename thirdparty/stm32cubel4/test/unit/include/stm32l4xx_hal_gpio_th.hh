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

#ifndef STM32L4XX_HAL_GPIO_TH_HH_
#define STM32L4XX_HAL_GPIO_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_gpio.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#undef __HAL_GPIO_EXTI_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_GPIO_EXTI_GET_FLAG, uint32_t);
#undef __HAL_GPIO_EXTI_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_GPIO_EXTI_CLEAR_FLAG, uint32_t);
#undef __HAL_GPIO_EXTI_GET_IT
FAKE_VALUE_FUNC(bool, __HAL_GPIO_EXTI_GET_IT, uint32_t);
#undef __HAL_GPIO_EXTI_CLEAR_IT
FAKE_VOID_FUNC(__HAL_GPIO_EXTI_CLEAR_IT, uint32_t);
#undef __HAL_GPIO_EXTI_GENERATE_SWIT
FAKE_VOID_FUNC(__HAL_GPIO_EXTI_GENERATE_SWIT, uint32_t);

FAKE_VOID_FUNC(HAL_GPIO_Init, GPIO_TypeDef*, GPIO_InitTypeDef*);
FAKE_VOID_FUNC(HAL_GPIO_DeInit, GPIO_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(GPIO_PinState, HAL_GPIO_ReadPin, GPIO_TypeDef*, uint16_t);
FAKE_VOID_FUNC(HAL_GPIO_WritePin, GPIO_TypeDef*, uint16_t, GPIO_PinState);
FAKE_VOID_FUNC(HAL_GPIO_TogglePin, GPIO_TypeDef*, uint16_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_GPIO_LockPin, GPIO_TypeDef*, uint16_t);
FAKE_VOID_FUNC(HAL_GPIO_EXTI_IRQHandler, uint16_t);

#endif /* STM32L4XX_HAL_GPIO_TH_HH_ */
