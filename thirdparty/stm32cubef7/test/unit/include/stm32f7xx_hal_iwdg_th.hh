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
 * Mock-up implementation for the stm32f7xx_hal_iwdg module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_IWDG_TH_HH
#define STM32F7XX_HAL_IWDG_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_iwdg.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided macros */

#undef __HAL_IWDG_START
FAKE_VOID_FUNC(__HAL_IWDG_START, IWDG_HandleTypeDef*);

#undef __HAL_IWDG_RELOAD_COUNTER
FAKE_VOID_FUNC(__HAL_IWDG_RELOAD_COUNTER, IWDG_HandleTypeDef*);

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IWDG_Init, IWDG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IWDG_Refresh, IWDG_HandleTypeDef*);

# endif /* STM32F7XX_HAL_IWDG_TH_HH */
