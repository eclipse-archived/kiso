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
 * Mock-up implementation for the stm32f7xx_hal_sai module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SAI_TH_HH
#define STM32F7XX_HAL_SAI_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_sai.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_InitProtocol, SAI_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Init, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_DeInit, SAI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SAI_MspInit, SAI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SAI_MspDeInit, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Transmit, SAI_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Receive, SAI_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Transmit_IT, SAI_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Receive_IT, SAI_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Transmit_DMA, SAI_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Receive_DMA, SAI_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_DMAPause, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_DMAResume, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_DMAStop, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_Abort, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_EnableTxMuteMode, SAI_HandleTypeDef*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_DisableTxMuteMode, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_EnableRxMuteMode, SAI_HandleTypeDef*, SAIcallback, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SAI_DisableRxMuteMode, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_SAI_StateTypeDef, HAL_SAI_GetState, SAI_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_SAI_GetError, SAI_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SAI_TH_HH */