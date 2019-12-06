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
 * Mock-up implementation for the stm32f7xx_hal_sdram module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SDRAM_TH_HH
#define STM32F7XX_HAL_SDRAM_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_sdram.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Init, SDRAM_HandleTypeDef*, FMC_SDRAM_TimingTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_DeInit, SDRAM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SDRAM_MspInit, SDRAM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SDRAM_MspDeInit, SDRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Read_8b, SDRAM_HandleTypeDef*, uint32_t*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Write_8b, SDRAM_HandleTypeDef*, uint32_t*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Read_16b, SDRAM_HandleTypeDef*, uint32_t*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Write_16b, SDRAM_HandleTypeDef*, uint32_t*, uint16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Read_32b, SDRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Write_32b, SDRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Read_DMA, SDRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_Write_DMA, SDRAM_HandleTypeDef*, uint32_t*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_WriteProtection_Enable, SDRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_WriteProtection_Disable, SDRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_SendCommand, SDRAM_HandleTypeDef*, FMC_SDRAM_CommandTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_ProgramRefreshRate, SDRAM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SDRAM_SetAutoRefreshNumber, SDRAM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_SDRAM_GetModeStatus, SDRAM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_SDRAM_StateTypeDef, HAL_SDRAM_GetState, SDRAM_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SDRAM_TH_HH */