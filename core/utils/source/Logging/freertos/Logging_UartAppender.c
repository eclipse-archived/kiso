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

/*---------------------- INCLUDED HEADERS ---------------------------------------------------------------------------*/

/**
 *
 * @brief
 *      Implements log appender over Rtt Jlink for stm32 target.
 * 
 * @details
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LOGGING_APPENDER_UART

/* Include the real interface header */
#include "Kiso_Logging.h"

#if KISO_FEATURE_LOGGING && KISO_UART_APPENDER

/* Additional interface header files */
#include "Kiso_BSP_TestInterface.h"
#include "Kiso_MCU_UART.h"
#include "Kiso_UARTTransceiver.h"

#if !KISO_FEATURE_BSP_TEST_INTERFACE
#error "KISO_Logginng module needs KISO_FEATURE_BSP_TEST_INTERFACE feature to be implemented and enabled."
#endif

#if !KISO_FEATURE_UART
#error "KISO_Logginng module needs KISO_FEATURE_UART feature to be implemented and enabled."
#endif

#if !KISO_FEATURE_UARTTRANSCEIVER
#error "KISO_Logginng module needs KISO_FEATURE_UARTTRANSCEIVER feature to be implemented and enabled."
#endif
/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
static Retcode_T UartAppenderInit(void *init);
static Retcode_T UartAppenderWrite(const char *message, uint32_t length);
static void UartCallback(UART_T uart, struct MCU_UART_Event_S event);
static bool LogCheckEndFrameFunc(uint8_t lastByte);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/
static const LogAppender_T UartAppender =
    {
        .Init = UartAppenderInit,
        .Write = UartAppenderWrite};
/**
 * Exported
 */
const LogAppender_T *Logging_UARTAppender = &UartAppender;

// HWHandle_T uartHandle;

UARTTransceiver_T LogTransceiver;

uint8_t buffer[LOG_BUFFER_SIZE];

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

static Retcode_T UartAppenderInit(void *init)
{
    (void)init;
    HWHandle_T uartHandle = NULL;
    Retcode_T retcode = BSP_TestInterface_Connect();
    if (RETCODE_OK == retcode)
    {
        uartHandle = BSP_TestInterface_GetUARTHandle();
        retcode = MCU_UART_Initialize(uartHandle, UartCallback);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = UARTTransceiver_Initialize(&LogTransceiver, uartHandle, buffer, LOG_BUFFER_SIZE, UART_TRANSCEIVER_UART_TYPE_UART);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_TestInterface_Enable();
    }
    if (RETCODE_OK == retcode)
    {
        retcode = UARTTransceiver_Start(&LogTransceiver, LogCheckEndFrameFunc);
    }
    return retcode;
}

static Retcode_T UartAppenderWrite(const char *message, uint32_t length)
{
    return UARTTransceiver_WriteData(&LogTransceiver, (const uint8_t *)message, length, LOG_APPENDER_TIMEOUT);
}

static void UartCallback(UART_T uart, struct MCU_UART_Event_S event)
{
    (void)uart;
    UARTTransceiver_LoopCallback(&LogTransceiver, event);
}

static bool LogCheckEndFrameFunc(uint8_t lastByte)
{
    (void)lastByte;
    return false;
}

#endif
