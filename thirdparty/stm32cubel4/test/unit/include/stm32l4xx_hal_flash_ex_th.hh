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

#ifndef STM32L4XX_HAL_FLASH_EX_TH_HH_
#define STM32L4XX_HAL_FLASH_EX_TH_HH_

#include "stm32l4xx_hal_flash_ex.h"
#include "gtest.h"

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASHEx_Erase, FLASH_EraseInitTypeDef*, uint32_t*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASHEx_Erase_IT, FLASH_EraseInitTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_FLASHEx_OBProgram, FLASH_OBProgramInitTypeDef*);
FAKE_VOID_FUNC(HAL_FLASHEx_OBGetConfig, FLASH_OBProgramInitTypeDef*);

#endif /* STM32L4XX_HAL_FLASH_EX_TH_HH_ */
