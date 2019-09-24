/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/

#ifndef STM32L4XX_HAL_USART_TH_HH_
#define STM32L4XX_HAL_USART_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_usart.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#undef __HAL_USART_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_USART_RESET_HANDLE_STATE, USART_HandleTypeDef *)
#undef __HAL_USART_RESET_HANDLE_STATE
FAKE_VALUE_FUNC(bool, __HAL_USART_RESET_HANDLE_STATE, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_USART_CLEAR_FLAG, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_CLEAR_PEFLAG
FAKE_VOID_FUNC(__HAL_USART_CLEAR_PEFLAG, USART_HandleTypeDef *)
#undef __HAL_USART_CLEAR_NEFLAG
FAKE_VOID_FUNC(__HAL_USART_CLEAR_NEFLAG, USART_HandleTypeDef *)
#undef __HAL_USART_CLEAR_OREFLAG
FAKE_VOID_FUNC(__HAL_USART_CLEAR_OREFLAG, USART_HandleTypeDef *)
#undef __HAL_USART_CLEAR_IDLEFLAG
FAKE_VOID_FUNC(__HAL_USART_CLEAR_IDLEFLAG, USART_HandleTypeDef *)
#undef __HAL_USART_ENABLE_IT
FAKE_VOID_FUNC(__HAL_USART_ENABLE_IT, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_DISABLE_IT
FAKE_VOID_FUNC(__HAL_USART_DISABLE_IT, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_GET_IT
FAKE_VALUE_FUNC(bool, __HAL_USART_GET_IT, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_GET_IT_SOURCE
FAKE_VALUE_FUNC(bool, __HAL_USART_GET_IT_SOURCE, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_CLEAR_IT
FAKE_VOID_FUNC(__HAL_USART_CLEAR_IT, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_SEND_REQ
FAKE_VOID_FUNC(__HAL_USART_SEND_REQ, USART_HandleTypeDef *, uint32_t)
#undef __HAL_USART_ONE_BIT_SAMPLE_ENABLE
FAKE_VOID_FUNC(__HAL_USART_ONE_BIT_SAMPLE_ENABLE, USART_HandleTypeDef *)
#undef __HAL_USART_ONE_BIT_SAMPLE_DISABLE
FAKE_VOID_FUNC(__HAL_USART_ONE_BIT_SAMPLE_DISABLE, USART_HandleTypeDef *)
#undef __HAL_USART_ENABLE
FAKE_VOID_FUNC(__HAL_USART_ENABLE, USART_HandleTypeDef *)
#undef __HAL_USART_DISABLE
FAKE_VOID_FUNC(__HAL_USART_DISABLE, USART_HandleTypeDef *)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Init, USART_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DeInit, USART_HandleTypeDef *)
FAKE_VOID_FUNC(HAL_USART_MspInit, USART_HandleTypeDef *)
FAKE_VOID_FUNC(HAL_USART_MspDeInit, USART_HandleTypeDef *)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Transmit, USART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Receive, USART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_TransmitReceive, USART_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t, uint32_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Transmit_IT, USART_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Receive_IT, USART_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_TransmitReceive_IT, USART_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Transmit_DMA, USART_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Receive_DMA, USART_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_TransmitReceive_DMA, USART_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DMAPause, USART_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DMAResume, USART_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_DMAStop, USART_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Abort, USART_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_USART_Abort_IT, USART_HandleTypeDef *)

FAKE_VOID_FUNC(HAL_USART_IRQHandler, USART_HandleTypeDef *)

FAKE_VALUE_FUNC(HAL_USART_StateTypeDef, HAL_USART_GetState, USART_HandleTypeDef *)
FAKE_VALUE_FUNC(uint32_t, HAL_USART_GetError, USART_HandleTypeDef *)

#endif /* STM32L4XX_HAL_USART_TH_HH_ */
