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
 * Mock-up implementation for the stm32f7xx_hal_rcc_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_RCC_EX_TH_HH
#define STM32F7XX_HAL_RCC_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_rcc_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_rcc_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_PeriphCLKConfig, RCC_PeriphCLKInitTypeDef*);

FAKE_VOID_FUNC(HAL_RCCEx_GetPeriphCLKConfig, RCC_PeriphCLKInitTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_RCCEx_GetPeriphCLKFreq, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_EnablePLLI2S, RCC_PLLI2SInitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_DisablePLLI2S);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_EnablePLLSAI, RCC_PLLSAIInitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_DisablePLLSAI);

# endif /* STM32F7XX_HAL_RCC_EX_TH_HH */