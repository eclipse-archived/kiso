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
 * Mock-up implementation for the stm32f7xx_hal_usart module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_USART_TH_HH
#define STM32F7XX_HAL_USART_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_usart.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Init, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DeInit, USART_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_USART_MspInit, USART_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_USART_MspDeInit, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_CheckIdleState, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Transmit, USART_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Receive, USART_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_TransmitReceive, USART_HandleTypeDef*, uint8_t*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Transmit_IT, USART_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Receive_IT, USART_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_TransmitReceive_IT, USART_HandleTypeDef*, uint8_t*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Transmit_DMA, USART_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Receive_DMA, USART_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_TransmitReceive_DMA, USART_HandleTypeDef*, uint8_t*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DMAPause, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DMAResume, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DMAStop, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_USART_StateTypeDef, HAL_USART_GetState, USART_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_USART_GetError, USART_HandleTypeDef*);

# endif /* STM32F7XX_HAL_USART_TH_HH */