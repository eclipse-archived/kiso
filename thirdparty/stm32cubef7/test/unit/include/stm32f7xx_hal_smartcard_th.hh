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
 * Mock-up implementation for the stm32f7xx_hal_smartcard module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SMARTCARD_TH_HH
#define STM32F7XX_HAL_SMARTCARD_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_smartcard.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Init, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_DeInit, SMARTCARD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SMARTCARD_MspInit, SMARTCARD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SMARTCARD_MspDeInit, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Transmit, SMARTCARD_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Receive, SMARTCARD_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Transmit_IT, SMARTCARD_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Receive_IT, SMARTCARD_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Transmit_DMA, SMARTCARD_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Receive_DMA, SMARTCARD_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Abort, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_AbortTransmit, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_AbortReceive, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_Abort_IT, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_AbortTransmit_IT, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMARTCARD_AbortReceive_IT, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_SMARTCARD_StateTypeDef, HAL_SMARTCARD_GetState, SMARTCARD_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_SMARTCARD_GetError, SMARTCARD_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SMARTCARD_TH_HH */