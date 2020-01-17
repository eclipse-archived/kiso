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
 * Mock-up implementation for the stm32f7xx_hal module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_TH_HH
#define STM32F7XX_HAL_TH_HH

/* Include or ban the real interface header */
#include "stm32f7xx_hal.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_Init);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DeInit);

FAKE_VOID_FUNC(HAL_MspInit);

FAKE_VOID_FUNC(HAL_MspDeInit);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_InitTick, uint32_t);

FAKE_VOID_FUNC(HAL_IncTick);

FAKE_VOID_FUNC(HAL_Delay, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_GetTick);

FAKE_VALUE_FUNC(uint32_t, HAL_GetTickPrio);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SetTickFreq, HAL_TickFreqTypeDef);

FAKE_VALUE_FUNC(HAL_TickFreqTypeDef, HAL_GetTickFreq);

FAKE_VOID_FUNC(HAL_SuspendTick);

FAKE_VOID_FUNC(HAL_ResumeTick);

FAKE_VALUE_FUNC(uint32_t, HAL_GetHalVersion);

FAKE_VALUE_FUNC(uint32_t, HAL_GetREVID);

FAKE_VALUE_FUNC(uint32_t, HAL_GetDEVID);

FAKE_VALUE_FUNC(uint32_t, HAL_GetUIDw0);

FAKE_VALUE_FUNC(uint32_t, HAL_GetUIDw1);

FAKE_VALUE_FUNC(uint32_t, HAL_GetUIDw2);

FAKE_VOID_FUNC(HAL_DBGMCU_EnableDBGSleepMode);

FAKE_VOID_FUNC(HAL_DBGMCU_DisableDBGSleepMode);

FAKE_VOID_FUNC(HAL_DBGMCU_EnableDBGStopMode);

FAKE_VOID_FUNC(HAL_DBGMCU_DisableDBGStopMode);

FAKE_VOID_FUNC(HAL_DBGMCU_EnableDBGStandbyMode);

FAKE_VOID_FUNC(HAL_DBGMCU_DisableDBGStandbyMode);

FAKE_VOID_FUNC(HAL_EnableCompensationCell);

FAKE_VOID_FUNC(HAL_DisableCompensationCell);

FAKE_VOID_FUNC(HAL_EnableFMCMemorySwapping);

FAKE_VOID_FUNC(HAL_DisableFMCMemorySwapping);

#if defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)

FAKE_VOID_FUNC(HAL_EnableMemorySwappingBank);

FAKE_VOID_FUNC(HAL_DisableMemorySwappingBank);

#endif /* STM32F765xx || STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx */

# endif /* STM32F7XX_HAL_TH_HH */
