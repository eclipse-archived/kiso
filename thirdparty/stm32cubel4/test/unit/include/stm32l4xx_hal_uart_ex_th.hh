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

#ifndef STM32L4XX_HAL_UART_EX_TH_HH_
#define STM32L4XX_HAL_UART_EX_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_uart_ex.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RS485Ex_Init, UART_HandleTypeDef*, uint32_t, uint32_t, uint32_t)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UARTEx_StopModeWakeUpSourceConfig, UART_HandleTypeDef*, UART_WakeUpTypeDef)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UARTEx_EnableStopMode, UART_HandleTypeDef*)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UARTEx_DisableStopMode, UART_HandleTypeDef*)
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MultiProcessorEx_AddressLength_Set, UART_HandleTypeDef*, uint32_t)
FAKE_VOID_FUNC(HAL_UARTEx_WakeupCallback, UART_HandleTypeDef*)

#endif /* STM32L4XX_HAL_UART_EX_TH_HH_ */
