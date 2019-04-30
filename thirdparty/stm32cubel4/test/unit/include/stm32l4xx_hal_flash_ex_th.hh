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

#ifndef STM32L4XX_HAL_FLASH_EX_TH_HH_
#define STM32L4XX_HAL_FLASH_EX_TH_HH_

#include "stm32l4xx_hal_flash_ex.h"
#include "gtest.h"

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASHEx_Erase, FLASH_EraseInitTypeDef*, uint32_t*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASHEx_Erase_IT, FLASH_EraseInitTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASHEx_OBProgram, FLASH_OBProgramInitTypeDef*);
FAKE_VOID_FUNC(HAL_FLASHEx_OBGetConfig, FLASH_OBProgramInitTypeDef*);

#endif /* STM32L4XX_HAL_FLASH_EX_TH_HH_ */
