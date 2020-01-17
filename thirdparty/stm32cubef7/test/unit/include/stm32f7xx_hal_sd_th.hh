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
 * Mock-up implementation for the stm32f7xx_hal_sd module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SD_TH_HH
#define STM32F7XX_HAL_SD_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_sd.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_Init, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_InitCard, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_DeInit, SD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SD_MspInit, SD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SD_MspDeInit, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_ReadBlocks, SD_HandleTypeDef*, uint8_t*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_WriteBlocks, SD_HandleTypeDef*, uint8_t*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_Erase, SD_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_ReadBlocks_IT, SD_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_WriteBlocks_IT, SD_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_ReadBlocks_DMA, SD_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_WriteBlocks_DMA, SD_HandleTypeDef*, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_ConfigWideBusOperation, SD_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_SendSDStatus, SD_HandleTypeDef*, uint32_t*);

FAKE_VALUE_FUNC(HAL_SD_CardStateTypeDef, HAL_SD_GetCardState, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_GetCardCID, SD_HandleTypeDef*, HAL_SD_CardCIDTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_GetCardCSD, SD_HandleTypeDef*, HAL_SD_CardCSDTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_GetCardStatus, SD_HandleTypeDef*, HAL_SD_CardStatusTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_GetCardInfo, SD_HandleTypeDef*, HAL_SD_CardInfoTypeDef*);

FAKE_VALUE_FUNC(HAL_SD_StateTypeDef, HAL_SD_GetState, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_SD_GetError, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_Abort, SD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SD_Abort_IT, SD_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SD_TH_HH */