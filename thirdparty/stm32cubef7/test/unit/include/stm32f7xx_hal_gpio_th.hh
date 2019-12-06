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
 * Mock-up implementation for the stm32f7xx_hal_gpio module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_GPIO_TH_HH
#define STM32F7XX_HAL_GPIO_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_gpio.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(HAL_GPIO_Init, GPIO_TypeDef*, GPIO_InitTypeDef*);

FAKE_VOID_FUNC(HAL_GPIO_DeInit, GPIO_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(GPIO_PinState, HAL_GPIO_ReadPin, GPIO_TypeDef*, uint16_t);

FAKE_VOID_FUNC(HAL_GPIO_WritePin, GPIO_TypeDef*, uint16_t, GPIO_PinState);

FAKE_VOID_FUNC(HAL_GPIO_TogglePin, GPIO_TypeDef*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_GPIO_LockPin, GPIO_TypeDef*, uint16_t);

# endif /* STM32F7XX_HAL_GPIO_TH_HH */