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

#ifndef STM32L4XX_HAL_SPI_TH_HH_
#define STM32L4XX_HAL_SPI_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_spi.h"

/* Include the needed headers */
#include "gtest.h"
/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#undef __HAL_SPI_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_SPI_RESET_HANDLE_STATE, SPI_HandleTypeDef*)
#undef __HAL_SPI_ENABLE_IT
FAKE_VOID_FUNC(__HAL_SPI_ENABLE_IT, SPI_HandleTypeDef*)
#undef __HAL_SPI_DISABLE_IT
FAKE_VOID_FUNC(__HAL_SPI_DISABLE_IT, SPI_HandleTypeDef*)
#undef __HAL_SPI_GET_IT_SOURCE
FAKE_VALUE_FUNC(bool, __HAL_SPI_GET_IT_SOURCE, SPI_HandleTypeDef*, uint32_t)
#undef __HAL_SPI_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_SPI_GET_FLAG, SPI_HandleTypeDef*, uint32_t)
#undef __HAL_SPI_CLEAR_CRCERRFLAG
FAKE_VOID_FUNC(__HAL_SPI_CLEAR_CRCERRFLAG, SPI_HandleTypeDef*)
#undef __HAL_SPI_CLEAR_MODFFLAG
FAKE_VOID_FUNC(__HAL_SPI_CLEAR_MODFFLAG, SPI_HandleTypeDef*)
#undef __HAL_SPI_CLEAR_OVRFLAG
FAKE_VOID_FUNC(__HAL_SPI_CLEAR_OVRFLAG, SPI_HandleTypeDef*)
#undef __HAL_SPI_CLEAR_FREFLAG
FAKE_VOID_FUNC(__HAL_SPI_CLEAR_FREFLAG, SPI_HandleTypeDef*)
#undef __HAL_SPI_ENABLE
FAKE_VOID_FUNC(__HAL_SPI_ENABLE, SPI_HandleTypeDef*)
#undef __HAL_SPI_DISABLE
FAKE_VOID_FUNC(__HAL_SPI_DISABLE, SPI_HandleTypeDef*)

#undef SPI_1LINE_TX
FAKE_VOID_FUNC(SPI_1LINE_TX, SPI_HandleTypeDef*)
#undef SPI_1LINE_RX
FAKE_VOID_FUNC(SPI_1LINE_RX, SPI_HandleTypeDef*)
#undef SPI_RESET_CRC
FAKE_VOID_FUNC(SPI_RESET_CRC, SPI_HandleTypeDef*)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Init, SPI_HandleTypeDef*)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_DeInit, SPI_HandleTypeDef*)
FAKE_VOID_FUNC(HAL_SPI_MspInit, SPI_HandleTypeDef*)
FAKE_VOID_FUNC(HAL_SPI_MspDeInit, SPI_HandleTypeDef*)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Transmit, SPI_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Receive, SPI_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_TransmitReceive, SPI_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t, uint32_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Transmit_IT, SPI_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Receive_IT, SPI_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_TransmitReceive_IT, SPI_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Transmit_DMA, SPI_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Receive_DMA, SPI_HandleTypeDef *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_TransmitReceive_DMA, SPI_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_DMAPause, SPI_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_DMAResume, SPI_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_DMAStop, SPI_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Abort, SPI_HandleTypeDef *)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Abort_IT, SPI_HandleTypeDef *)

FAKE_VOID_FUNC(HAL_SPI_IRQHandler, SPI_HandleTypeDef *)

FAKE_VALUE_FUNC(HAL_SPI_StateTypeDef, HAL_SPI_GetState, SPI_HandleTypeDef*)
FAKE_VALUE_FUNC(uint32_t, HAL_SPI_GetError, SPI_HandleTypeDef*)

#endif /* STM32L4XX_HAL_SPI_TH_HH_ */
