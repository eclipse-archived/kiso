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
 */
/*----------------------------------------------------------------------------*/

#ifndef STM32L4XX_HAL_UART_EX_TH_HH_
#define STM32L4XX_HAL_UART_EX_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_uart_ex.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RS485Ex_Init, UART_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UARTEx_StopModeWakeUpSourceConfig, UART_HandleTypeDef*, UART_WakeUpTypeDef);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UARTEx_EnableStopMode, UART_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_UARTEx_DisableStopMode, UART_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MultiProcessorEx_AddressLength_Set, UART_HandleTypeDef*, uint32_t);
FAKE_VOID_FUNC(HAL_UARTEx_WakeupCallback, UART_HandleTypeDef*);

#endif /* STM32L4XX_HAL_UART_EX_TH_HH_ */
