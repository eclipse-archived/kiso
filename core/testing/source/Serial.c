/**********************************************************************************************************************
 * Copyright (c) 2010#2019 Robert Bosch GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl#2.0.
 *
 * SPDX#License#Identifier: EPL#2.0
 *
 * Contributors:
 *    Robert Bosch GmbH # initial contribution
 *
 **********************************************************************************************************************/

/**
 * @file
 *
 * @brief
 * todo add brief description Implements the following functionalities specified in template.h
 */
/*###################### INCLUDED HEADERS ###########################################################################*/

#include "Kiso_Testing.h"
#include "Kiso_Testing_Config.h"
#include "Kiso_BSP_TestInterface.h"
#include "Kiso_MCU_UART.h"
#include "Kiso_GuardedTask.h"
#include "Kiso_RingBuffer.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "SerialMsgTransceiver.h"
#include "SerialCChannel.h"

/*###################### MACROS DEFINITION ##########################################################################*/
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TESTING_SERIAL

#define WAIT_TIME_FOR_SINGLE_UART_TRANSMISSION UINT32_C(500)

#define MAX_BUFFER_SIZE UINT16_C(300)

#ifndef SERIAL_TASK_PRIO
#define SERIAL_TASK_PRIO (1UL)
#endif /* SERiAL_TASK_PRIO */

#ifndef SERIAL_TASK_STACK_DEPTH
#define SERIAL_TASK_STACK_DEPTH (128UL)
#endif /* SERIAL_TASK_STACK_DEPTH */

/*###################### LOCAL_TYPES DEFINITION #####################################################################*/

/*###################### LOCAL FUNCTIONS DECLARATION ################################################################*/

static void uartEventsCallbackFunc(UART_T uart, struct MCU_UART_Event_S event);

/*###################### VARIABLES DECLARATION ######################################################################*/

static GuardedTask_T serialGuardedTask;
volatile uint32_t serialReceivedCnt = 0;
static SemaphoreHandle_t TransmitDataSemaphoreHandle = NULL;
static RingBuffer_T serialRingBuffer;
static uint8_t serialBuffer[MAX_BUFFER_SIZE];
static uint8_t RxBuffer;
static UART_T TestInterfaceUart;
#ifndef NDEBUG
volatile uint32_t TestUartErrorCount = 0; /* number of errors post mortem */
#endif                                    /* NDEBUG */

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ###########################################################*/

/* The description is defined at function declaration */
Retcode_T Serial_Initialize(void)
{
    Retcode_T rc = RETCODE_OK;

    rc = BSP_TestInterface_Connect();
    if (RETCODE_OK == rc)
    {
        TestInterfaceUart = (UART_T)BSP_TestInterface_GetUARTHandle();
        if (NULL == TestInterfaceUart)
        {
            rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        }
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_UART_Initialize(TestInterfaceUart, uartEventsCallbackFunc);
    }

    if (RETCODE_OK == rc)
    {
        rc = BSP_TestInterface_Enable();
    }

    if (RETCODE_OK == rc)
    {
        rc = GuardedTask_Initialize(&serialGuardedTask, SerialMsgTransceiver_Receive, "SERIAL_TASK", SERIAL_TASK_PRIO, SERIAL_TASK_STACK_DEPTH);
    }

    if (RETCODE_OK == rc)
    {
        TransmitDataSemaphoreHandle = xSemaphoreCreateBinary();
        if (NULL == TransmitDataSemaphoreHandle)
        {
            rc = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_OUT_OF_RESOURCES);
        }
    }

    if (RETCODE_OK == rc)
    {
        RingBuffer_Initialize(&serialRingBuffer, serialBuffer, MAX_BUFFER_SIZE);
    }

    if (RETCODE_OK == rc)
    {
        /* start the receive process, this will enable UART interrupts and trigger a callback on receive */
        rc = MCU_UART_Receive(TestInterfaceUart, &RxBuffer, 1UL);
    }

    return (rc);
}

Retcode_T Serial_Deinitialize(void)
{
    Retcode_T rc = MCU_UART_Deinitialize(TestInterfaceUart);
    if (RETCODE_OK == rc)
    {
        rc = BSP_TestInterface_Disable();
    }

    if (RETCODE_OK == rc)
    {
        rc = BSP_TestInterface_Disconnect();
    }

    if (RETCODE_OK == rc)
    {
        rc = GuardedTask_Deinitialize(&serialGuardedTask);
    }
    return rc;
}

Retcode_T Serial_Send(void *data, uint32_t len)
{
    Retcode_T retCode;
    retCode = MCU_UART_Send(TestInterfaceUart, (uint8_t *)data, len);

    if (RETCODE_OK == retCode)
    {
        /* Waiting here for the transmit complete event */
        if (pdTRUE != xSemaphoreTake(TransmitDataSemaphoreHandle, (WAIT_TIME_FOR_SINGLE_UART_TRANSMISSION / portTICK_RATE_MS)))
        {
            retCode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_SEMAPHORE_ERROR);
        }
    }

    return (retCode);
}

Retcode_T Serial_Receive(void *data, uint32_t len)
{
    uint32_t numberOfReadBytes = 0;
    Retcode_T retcode = RETCODE_OK;

    numberOfReadBytes = RingBuffer_Read(&serialRingBuffer, (uint8_t *)data, len);

    if (numberOfReadBytes != len)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    return retcode;
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION #############################################################*/

static void uartEventsCallbackFunc(UART_T uart, struct MCU_UART_Event_S event)
{
    KISO_UNUSED(uart); /* not used in One-Byte-Mode */

    Retcode_T retcode = RETCODE_OK;

    /* Signal the guarded task to indicate that the receive is complete */
    if (event.RxComplete)
    {
        serialReceivedCnt++;

        if (1UL == RingBuffer_Write(&serialRingBuffer, (uint8_t *)&RxBuffer, 1UL))
        {
            (void)GuardedTask_SignalFromIsr(&serialGuardedTask);
        }
    }
    else if (event.TxComplete)
    {
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

        if (pdTRUE == xSemaphoreGiveFromISR(TransmitDataSemaphoreHandle, &xHigherPriorityTaskWoken))
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        else
        {
            /* ignore... semaphore has already been given */
        }
    }
    else if (event.RxError)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    else if (event.TxError)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseErrorFromIsr(retcode);
    }
}