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
 * Mock-up implementation for the stm32f7xx_hal_mmc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_MMC_TH_HH
#define STM32F7XX_HAL_MMC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_mmc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_Init, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_InitCard, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_DeInit, MMC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_MMC_MspInit, MMC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_MMC_MspDeInit, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_ReadBlocks, MMC_HandleTypeDef*, uint8_t*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_WriteBlocks, MMC_HandleTypeDef*, uint8_t*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_Erase, MMC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_ReadBlocks_IT, MMC_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_WriteBlocks_IT, MMC_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_ReadBlocks_DMA, MMC_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_WriteBlocks_DMA, MMC_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_ConfigWideBusOperation, MMC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_MMC_CardStateTypeDef, HAL_MMC_GetCardState, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_GetCardCID, MMC_HandleTypeDef*, HAL_MMC_CardCIDTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_GetCardCSD, MMC_HandleTypeDef*, HAL_MMC_CardCSDTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_GetCardInfo, MMC_HandleTypeDef*, HAL_MMC_CardInfoTypeDef*);

FAKE_VALUE_FUNC(HAL_MMC_StateTypeDef, HAL_MMC_GetState, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_MMC_GetError, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_Abort, MMC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MMC_Abort_IT, MMC_HandleTypeDef*);

# endif /* STM32F7XX_HAL_MMC_TH_HH */