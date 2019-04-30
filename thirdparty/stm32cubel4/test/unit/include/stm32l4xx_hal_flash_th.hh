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

#ifndef STM32L4XX_HAL_FLASH_TH_HH_
#define STM32L4XX_HAL_FLASH_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_flash.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
/* Overwritten because of direct memory access via hard-coded address */
#undef FLASH_SIZE
#if defined (STM32L486xx)
#define FLASH_SIZE  ((uint32_t)0x100000)
#elif defined (STM32L476xx)
#define FLASH_SIZE  ((uint32_t)0x080000)
#else
#error "Unknown chip family! Please add the flash size definition for your MCU."
#endif

/* Mock-ups for the provided interfaces */
#undef __HAL_FLASH_SET_LATENCY
FAKE_VOID_FUNC(__HAL_FLASH_SET_LATENCY, uint32_t);
#undef __HAL_FLASH_GET_LATENCY
FAKE_VALUE_FUNC(uint32_t, __HAL_FLASH_GET_LATENCY);
#undef __HAL_FLASH_PREFETCH_BUFFER_ENABLE
FAKE_VOID_FUNC(__HAL_FLASH_PREFETCH_BUFFER_ENABLE);
#undef __HAL_FLASH_PREFETCH_BUFFER_DISABLE
FAKE_VOID_FUNC(__HAL_FLASH_PREFETCH_BUFFER_DISABLE);
#undef __HAL_FLASH_INSTRUCTION_CACHE_ENABLE
FAKE_VOID_FUNC(__HAL_FLASH_INSTRUCTION_CACHE_ENABLE);
#undef __HAL_FLASH_INSTRUCTION_CACHE_DISABLE
FAKE_VOID_FUNC(__HAL_FLASH_INSTRUCTION_CACHE_DISABLE);
#undef __HAL_FLASH_DATA_CACHE_ENABLE
FAKE_VOID_FUNC(__HAL_FLASH_DATA_CACHE_ENABLE);
#undef __HAL_FLASH_DATA_CACHE_DISABLE
FAKE_VOID_FUNC(__HAL_FLASH_DATA_CACHE_DISABLE);
#undef __HAL_FLASH_INSTRUCTION_CACHE_RESET
FAKE_VOID_FUNC(__HAL_FLASH_INSTRUCTION_CACHE_RESET);
#undef __HAL_FLASH_DATA_CACHE_RESET
FAKE_VOID_FUNC(__HAL_FLASH_DATA_CACHE_RESET);
#undef __HAL_FLASH_POWER_DOWN_ENABLE
FAKE_VOID_FUNC(__HAL_FLASH_POWER_DOWN_ENABLE);
#undef __HAL_FLASH_SLEEP_POWERDOWN_DISABLE
FAKE_VOID_FUNC(__HAL_FLASH_SLEEP_POWERDOWN_DISABLE);
#undef __HAL_FLASH_ENABLE_IT
FAKE_VOID_FUNC(__HAL_FLASH_ENABLE_IT, uint32_t);
#undef __HAL_FLASH_DISABLE_IT
FAKE_VOID_FUNC(__HAL_FLASH_DISABLE_IT, uint32_t);
#undef __HAL_FLASH_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_FLASH_GET_FLAG, uint32_t);
#undef __HAL_FLASH_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_FLASH_CLEAR_FLAG, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Program, uint32_t, uint32_t, uint64_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Program_IT, uint32_t, uint32_t, uint64_t);
FAKE_VOID_FUNC(HAL_FLASH_IRQHandler);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Unlock);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_Lock);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_OB_Unlock);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_OB_Lock);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASH_OB_Launch);

FAKE_VALUE_FUNC(uint32_t, HAL_FLASH_GetError);

#endif /* STM32L4XX_HAL_FLASH_TH_HH_ */
