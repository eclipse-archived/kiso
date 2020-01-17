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
 * Mock-up implementation for the stm32f7xx_hal_flash module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_FLASH_TH_HH
#define STM32F7XX_HAL_FLASH_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_flash.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Program, uint32_t, uint32_t, uint64_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Program_IT, uint32_t, uint32_t, uint64_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Unlock);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Lock);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_OB_Unlock);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_OB_Lock);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_OB_Launch);

FAKE_VALUE_FUNC(uint32_t, HAL_FLASH_GetError);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FLASH_WaitForLastOperation, uint32_t);

# endif /* STM32F7XX_HAL_FLASH_TH_HH */
