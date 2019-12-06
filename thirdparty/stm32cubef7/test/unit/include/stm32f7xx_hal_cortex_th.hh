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
 * Mock-up implementation for the stm32f7xx_hal_cortex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_CORTEX_TH_HH
#define STM32F7XX_HAL_CORTEX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_cortex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(HAL_NVIC_SetPriorityGrouping, uint32_t);

FAKE_VOID_FUNC(HAL_NVIC_SetPriority, IRQn_Type, uint32_t, uint32_t);

FAKE_VOID_FUNC(HAL_NVIC_EnableIRQ, IRQn_Type);

FAKE_VOID_FUNC(HAL_NVIC_DisableIRQ, IRQn_Type);

FAKE_VOID_FUNC(HAL_NVIC_SystemReset);

FAKE_VALUE_FUNC(uint32_t, HAL_SYSTICK_Config, uint32_t);

FAKE_VOID_FUNC(HAL_MPU_Enable, uint32_t);

FAKE_VOID_FUNC(HAL_MPU_Disable);

FAKE_VOID_FUNC(HAL_SYSTICK_IRQHandler);

FAKE_VOID_FUNC(HAL_MPU_ConfigRegion, MPU_Region_InitTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_NVIC_GetPriorityGrouping);

FAKE_VOID_FUNC(HAL_NVIC_GetPriority, IRQn_Type, uint32_t, uint32_t*, uint32_t*);

FAKE_VALUE_FUNC(uint32_t, HAL_NVIC_GetPendingIRQ, IRQn_Type);

FAKE_VOID_FUNC(HAL_NVIC_SetPendingIRQ, IRQn_Type);

FAKE_VOID_FUNC(HAL_NVIC_ClearPendingIRQ, IRQn_Type);

FAKE_VALUE_FUNC(uint32_t, HAL_NVIC_GetActive, IRQn_Type);

FAKE_VOID_FUNC(HAL_SYSTICK_CLKSourceConfig, uint32_t);

# endif /* STM32F7XX_HAL_CORTEX_TH_HH */