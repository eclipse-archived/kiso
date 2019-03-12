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

#ifndef BCDS_BCDS_UART_th_HH_
#define BCDS_BCDS_UART_th_HH_

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_MCU_UART.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_UART

FAKE_VALUE_FUNC(Retcode_T, MCU_UART_Initialize, UART_T, MCU_UART_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_UART_Deinitialize, UART_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_UART_Send, UART_T, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_UART_Receive, UART_T, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_UART_GetRxCount, UART_T, uint32_t*);

#endif /* #if BCDS_FEATURE_UART && UART_COUNT */
#endif /* BCDS_BCDS_UART_th_HH_ */
