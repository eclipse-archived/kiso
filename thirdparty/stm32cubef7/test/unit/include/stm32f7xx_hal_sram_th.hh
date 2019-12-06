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
 * Mock-up implementation for the stm32f7xx_hal_sram module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SRAM_TH_HH
#define STM32F7XX_HAL_SRAM_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_sram.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Init, SRAM_HandleTypeDef*, FMC_NORSRAM_TimingTypeDef*, FMC_NORSRAM_TimingTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_DeInit, SRAM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SRAM_MspInit, SRAM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SRAM_MspDeInit, SRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Read_8b, SRAM_HandleTypeDef*, uint32_t*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Write_8b, SRAM_HandleTypeDef*, uint32_t*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Read_16b, SRAM_HandleTypeDef*, uint32_t*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Write_16b, SRAM_HandleTypeDef*, uint32_t*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Read_32b, SRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Write_32b, SRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Read_DMA, SRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_Write_DMA, SRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_WriteOperation_Enable, SRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SRAM_WriteOperation_Disable, SRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_SRAM_StateTypeDef, HAL_SRAM_GetState, SRAM_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SRAM_TH_HH */
