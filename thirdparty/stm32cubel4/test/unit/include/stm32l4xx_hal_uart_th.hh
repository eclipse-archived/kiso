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

#ifndef STM32L4XX_HAL_UART_TH_HH_
#define STM32L4XX_HAL_UART_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_uart.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
//UART_HandleTypeDef*
/* Mock-ups for the provided interfaces */
#undef __HAL_UART_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_UART_RESET_HANDLE_STATE, UART_HandleTypeDef*);
#undef __HAL_UART_FLUSH_DRREGISTER
FAKE_VOID_FUNC(__HAL_UART_FLUSH_DRREGISTER, UART_HandleTypeDef*);
#undef __HAL_UART_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_UART_CLEAR_FLAG, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_CLEAR_PEFLAG
FAKE_VOID_FUNC(__HAL_UART_CLEAR_PEFLAG, UART_HandleTypeDef*);
#undef __HAL_UART_CLEAR_FEFLAG
FAKE_VOID_FUNC(__HAL_UART_CLEAR_FEFLAG, UART_HandleTypeDef*);
#undef __HAL_UART_CLEAR_NEFLAG
FAKE_VOID_FUNC(__HAL_UART_CLEAR_NEFLAG, UART_HandleTypeDef*);
#undef __HAL_UART_CLEAR_OREFLAG
FAKE_VOID_FUNC(__HAL_UART_CLEAR_OREFLAG, UART_HandleTypeDef*);
#undef __HAL_UART_CLEAR_IDLEFLAG
FAKE_VOID_FUNC(__HAL_UART_CLEAR_IDLEFLAG, UART_HandleTypeDef*);
#undef __HAL_UART_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_UART_GET_FLAG, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_ENABLE_IT
FAKE_VOID_FUNC(__HAL_UART_ENABLE_IT, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_DISABLE_IT
FAKE_VOID_FUNC(__HAL_UART_DISABLE_IT, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_GET_IT
FAKE_VALUE_FUNC(bool, __HAL_UART_GET_IT, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_CLEAR_IT
FAKE_VOID_FUNC(__HAL_UART_CLEAR_IT, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_SEND_REQ
FAKE_VOID_FUNC(__HAL_UART_SEND_REQ, UART_HandleTypeDef*, uint32_t);
#undef __HAL_UART_ONE_BIT_SAMPLE_ENABLE
FAKE_VOID_FUNC(__HAL_UART_ONE_BIT_SAMPLE_ENABLE, UART_HandleTypeDef*);
#undef __HAL_UART_ONE_BIT_SAMPLE_DISABLE
FAKE_VOID_FUNC(__HAL_UART_ONE_BIT_SAMPLE_DISABLE, UART_HandleTypeDef*);
#undef __HAL_UART_ENABLE
FAKE_VOID_FUNC(__HAL_UART_ENABLE, UART_HandleTypeDef*);
#undef __HAL_UART_DISABLE
FAKE_VOID_FUNC(__HAL_UART_DISABLE, UART_HandleTypeDef*);
#undef __HAL_UART_HWCONTROL_CTS_ENABLE
FAKE_VOID_FUNC(__HAL_UART_HWCONTROL_CTS_ENABLE, UART_HandleTypeDef*);
#undef __HAL_UART_HWCONTROL_CTS_DISABLE
FAKE_VOID_FUNC(__HAL_UART_HWCONTROL_CTS_DISABLE, UART_HandleTypeDef*);
#undef __HAL_UART_HWCONTROL_RTS_ENABLE
FAKE_VOID_FUNC(__HAL_UART_HWCONTROL_RTS_ENABLE, UART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Init, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HalfDuplex_Init, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LIN_Init, UART_HandleTypeDef *, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MultiProcessor_Init, UART_HandleTypeDef*, uint8_t, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_DeInit, UART_HandleTypeDef*);
FAKE_VOID_FUNC(HAL_UART_MspInit, UART_HandleTypeDef*);
FAKE_VOID_FUNC(HAL_UART_MspDeInit, UART_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Transmit, UART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Receive, UART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Transmit_IT, UART_HandleTypeDef *, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Receive_IT, UART_HandleTypeDef *, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Transmit_DMA, UART_HandleTypeDef *, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Receive_DMA, UART_HandleTypeDef *, uint8_t *, uint16_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_DMAPause, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_DMAResume, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_DMAStop, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Abort, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_AbortTransmit, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_AbortReceive, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_Abort_IT, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_AbortTransmit_IT, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UART_AbortReceive_IT, UART_HandleTypeDef *);

FAKE_VOID_FUNC(HAL_UART_IRQHandler, UART_HandleTypeDef *);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LIN_SendBreak, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MultiProcessor_EnableMuteMode, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MultiProcessor_DisableMuteMode, UART_HandleTypeDef *);
FAKE_VOID_FUNC(HAL_MultiProcessor_EnterMuteMode, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HalfDuplex_EnableTransmitter, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HalfDuplex_EnableReceiver, UART_HandleTypeDef *);

FAKE_VALUE_FUNC(HAL_UART_StateTypeDef, HAL_UART_GetState, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(uint32_t, HAL_UART_GetError, UART_HandleTypeDef *);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, UART_SetConfig, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, UART_CheckIdleState, UART_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, UART_WaitOnFlagUntilTimeout, UART_HandleTypeDef *, uint32_t, FlagStatus, uint32_t, uint32_t);
FAKE_VOID_FUNC(UART_AdvFeatureConfig, UART_HandleTypeDef *);

#endif /* STM32L4XX_HAL_UART_TH_HH_ */
