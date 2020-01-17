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
 * Mock-up implementation for the stm32f7xx_hal_nor module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_NOR_TH_HH
#define STM32F7XX_HAL_NOR_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_nor.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Init, NOR_HandleTypeDef*, FMC_NORSRAM_TimingTypeDef*, FMC_NORSRAM_TimingTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_DeInit, NOR_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_NOR_MspInit, NOR_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_NOR_MspDeInit, NOR_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_NOR_MspWait, NOR_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Read_ID, NOR_HandleTypeDef*, NOR_IDTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_ReturnToReadMode, NOR_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Read, NOR_HandleTypeDef*, uint32_t*, uint16_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Program, NOR_HandleTypeDef*, uint32_t*, uint16_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_ReadBuffer, NOR_HandleTypeDef*, uint32_t, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_ProgramBuffer, NOR_HandleTypeDef*, uint32_t, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Erase_Block, NOR_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Erase_Chip, NOR_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_Read_CFI, NOR_HandleTypeDef*, NOR_CFITypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_WriteOperation_Enable, NOR_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_NOR_WriteOperation_Disable, NOR_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_NOR_StateTypeDef, HAL_NOR_GetState, NOR_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_NOR_StatusTypeDef, HAL_NOR_GetStatus, NOR_HandleTypeDef*, uint32_t, uint32_t);

# endif /* STM32F7XX_HAL_NOR_TH_HH */