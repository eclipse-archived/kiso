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
 * Mock-up implementation for the stm32f7xx_ll_fmc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_FMC_TH_HH
#define STM32F7XX_LL_FMC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_ll_fmc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NORSRAM_Init, FMC_NORSRAM_TypeDef*, FMC_NORSRAM_InitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NORSRAM_Timing_Init, FMC_NORSRAM_TypeDef*, FMC_NORSRAM_TimingTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NORSRAM_Extended_Timing_Init, FMC_NORSRAM_EXTENDED_TypeDef*, FMC_NORSRAM_TimingTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NORSRAM_DeInit, FMC_NORSRAM_TypeDef*, FMC_NORSRAM_EXTENDED_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NORSRAM_WriteOperation_Enable, FMC_NORSRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NORSRAM_WriteOperation_Disable, FMC_NORSRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_Init, FMC_NAND_TypeDef*, FMC_NAND_InitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_CommonSpace_Timing_Init, FMC_NAND_TypeDef*, FMC_NAND_PCC_TimingTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_AttributeSpace_Timing_Init, FMC_NAND_TypeDef*, FMC_NAND_PCC_TimingTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_DeInit, FMC_NAND_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_ECC_Enable, FMC_NAND_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_ECC_Disable, FMC_NAND_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_NAND_GetECC, FMC_NAND_TypeDef*, uint32_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_Init, FMC_SDRAM_TypeDef*, FMC_SDRAM_InitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_Timing_Init, FMC_SDRAM_TypeDef*, FMC_SDRAM_TimingTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_DeInit, FMC_SDRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_WriteProtection_Enable, FMC_SDRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_WriteProtection_Disable, FMC_SDRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_SendCommand, FMC_SDRAM_TypeDef*, FMC_SDRAM_CommandTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_ProgramRefreshRate, FMC_SDRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, FMC_SDRAM_SetAutoRefreshNumber, FMC_SDRAM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, FMC_SDRAM_GetModeStatus, FMC_SDRAM_TypeDef*, uint32_t);

# endif /* STM32F7XX_LL_FMC_TH_HH */