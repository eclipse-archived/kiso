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
 
#ifndef STM32L4XX_HAL_CORTEX_TH_HH_
#define STM32L4XX_HAL_CORTEX_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_cortex.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VOID_FUNC(HAL_NVIC_SetPriorityGrouping, uint32_t)
FAKE_VOID_FUNC(HAL_NVIC_SetPriority, IRQn_Type, uint32_t, uint32_t)
FAKE_VOID_FUNC(HAL_NVIC_EnableIRQ, IRQn_Type)
FAKE_VOID_FUNC(HAL_NVIC_DisableIRQ, IRQn_Type)
FAKE_VOID_FUNC(HAL_NVIC_SystemReset)
FAKE_VALUE_FUNC(uint32_t, HAL_SYSTICK_Config, uint32_t)

FAKE_VALUE_FUNC(uint32_t, HAL_NVIC_GetPriorityGrouping)
FAKE_VOID_FUNC(HAL_NVIC_GetPriority, IRQn_Type, uint32_t, uint32_t*, uint32_t*)
FAKE_VALUE_FUNC(uint32_t, HAL_NVIC_GetPendingIRQ, IRQn_Type)
FAKE_VOID_FUNC(HAL_NVIC_SetPendingIRQ, IRQn_Type)
FAKE_VOID_FUNC(HAL_NVIC_ClearPendingIRQ, IRQn_Type)
FAKE_VALUE_FUNC(uint32_t, HAL_NVIC_GetActive, IRQn_Type)
FAKE_VOID_FUNC(HAL_SYSTICK_CLKSourceConfig, uint32_t)
FAKE_VOID_FUNC(HAL_SYSTICK_IRQHandler)
FAKE_VOID_FUNC(HAL_SYSTICK_Callback)

#endif /* STM32L4XX_HAL_CORTEX_TH_HH_ */
