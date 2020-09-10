/*******************************************************************************
 * Copyright (c) 2010-2020 Robert Bosch GmbH
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
 ******************************************************************************/

/**
 * @file
 *
 * @brief
 *      Implements test cases for uart comminication verification
 */
#include "TestSuiteUart.h"
#include "Kiso_Basics.h"
#include "Kiso_Testing.h"
#include "Kiso_CmdProcessor.h"
#include "Kiso_MCU_UART.h"
#include "Kiso_BSP_GenericUart.h"
#include "FreeRTOS.h"
#include "semphr.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID 0

#define UART_BUFFER_LEN (5)
#define DATA_TRANSFER_TIMEOUT_MS UINT32_C(200)
#define UART_DEVICE UINT32_C(1)
#define MSG_BUFFER_SIZE (32)

enum TestSuiteUart_TestCases_E
{
    TEST_CASE_FUNCTIONAL_TEST_ID = 1
};

static Retcode_T TestCase_FctTest_Setup(CCMsg_T *ccmsg);
static void TestCase_FctTest_Run(CCMsg_T *ccmsg);
static Retcode_T TestCase_FctTest_Teardown(CCMsg_T *ccmsg);
static void UartISRCallback(UART_T uart, struct MCU_UART_Event_S event);

static UART_T UartHdl = 0;
static xSemaphoreHandle TxSignal = 0;
static xSemaphoreHandle RxSignal = 0;

Retcode_T TestSuiteUart_Initialize(uint8_t sId)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_RegisterTestSuite(sId, NULL, NULL);

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(sId, TEST_CASE_FUNCTIONAL_TEST_ID, TestCase_FctTest_Setup, TestCase_FctTest_Run, TestCase_FctTest_Teardown);
    }
    return retcode;
}

/**
 * @brief           Performs the setup operation of the functional test of uart in interrupt mode
 * @details         This function initializes the uart interface in interrupt mode and creates the necessary
 *                  synchronisation ressources.
 */
static Retcode_T TestCase_FctTest_Setup(CCMsg_T *ccmsg)
{
    KISO_UNUSED(ccmsg);
    Retcode_T retcode = RETCODE_OK;

    UartHdl = (UART_T)BSP_GenericUart_GetHandle(UART_DEVICE);
    if (NULL == UartHdl)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    if (RETCODE_OK == retcode)
    {
        TxSignal = xSemaphoreCreateBinary();
        if (NULL == TxSignal)
        {
            return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_SEMAPHORE_ERROR);
        }
    }
    if (RETCODE_OK == retcode)
    {
        RxSignal = xSemaphoreCreateBinary();
        if (NULL == RxSignal)
        {
            return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_SEMAPHORE_ERROR);
        }
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_GenericUart_Connect(UART_DEVICE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = MCU_UART_Initialize(UartHdl, UartISRCallback);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_GenericUart_Enable(UART_DEVICE);
    }
    return retcode;
}

/**
 * @brief           Deinitializes the uart interface
 */
static Retcode_T TestCase_FctTest_Teardown(CCMsg_T *ccmsg)
{
    KISO_UNUSED(ccmsg);
    Retcode_T retcode;
    uint8_t dummy[0];

    /* cancel receive */
    (void)MCU_UART_Receive(UartHdl, dummy, 0);

    retcode = MCU_UART_Deinitialize(UartHdl);
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_GenericUart_Disable(UART_DEVICE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_GenericUart_Disconnect(UART_DEVICE);
    }
    if (RETCODE_OK == retcode)
    {
        vSemaphoreDelete(TxSignal);
    }
    if (RETCODE_OK == retcode)
    {
        vSemaphoreDelete(RxSignal);
    }
    return retcode;
}

/**
 * This Test will put the uart receiver into receive mode and send data via the transmitter the data will be
 * looped back to the receiver at hardware level (e.g. wiring TX line to RX line)
 * the test will succede if the transmit operation succeeded and if the received data matches the transmitted data
 */
static void TestCase_FctTest_Run(CCMsg_T *ccmsg)
{
    KISO_UNUSED(ccmsg);

    Retcode_T retcode;
    uint8_t dataOut[UART_BUFFER_LEN];
    uint8_t dataIn[UART_BUFFER_LEN] = {0};
    char msg[MSG_BUFFER_SIZE] = "SUCCESS";

    for (uint8_t i = 0; i < UART_BUFFER_LEN; i++)
    {
        dataOut[i] = i;
    }

    retcode = MCU_UART_Receive(UartHdl, dataIn, UART_BUFFER_LEN);

    if (RETCODE_OK == retcode)
    {
        retcode = MCU_UART_Send(UartHdl, dataOut, UART_BUFFER_LEN);
    }
    if (RETCODE_OK == retcode)
    {
        if (pdTRUE != xSemaphoreTake(TxSignal, DATA_TRANSFER_TIMEOUT_MS))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
            strcpy(msg, "FAIL");
        }
    }

    if (RETCODE_OK == retcode)
    {
        if (pdTRUE == xSemaphoreTake(RxSignal, DATA_TRANSFER_TIMEOUT_MS))
        {
            uint8_t tries = 1;
            // we received something... now wait till we receive EVERYTHING.
            while (pdTRUE == xSemaphoreTake(RxSignal, DATA_TRANSFER_TIMEOUT_MS))
            {
                /* Semaphore should signal at most UART_BUFFER_LEN times (also
                 * counting the first semaphore signal above) */
                tries++;
                if (tries > UART_BUFFER_LEN)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
                    strcpy(msg, "FAIL");
                    break;
                }
            }

            for (uint8_t i = 0; RETCODE_OK == retcode && i < UART_BUFFER_LEN; i++)
            {
                if (dataIn[i] != dataOut[i])
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
                    strcpy(msg, "FAIL");
                }
            }
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
            strcpy(msg, "FAIL");
        }
    }
    Tests_SendReport(Retcode_GetCode(retcode), msg);
}

static void UartISRCallback(UART_T uart, struct MCU_UART_Event_S event)
{
    KISO_UNUSED(uart);
    Retcode_T Rc = RETCODE_OK;

    if (UINT8_C(1) == event.TxComplete)
    {
        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != TxSignal)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(TxSignal, &higherPriorityTaskWoken))
                {
                    portYIELD_FROM_ISR(higherPriorityTaskWoken);
                }
                else
                {
                    /* ignore... semaphore has already been given */
                }
            }
            else
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
            }
        }
    }

    if (UINT8_C(1) == event.RxComplete)
    {
        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != RxSignal)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(RxSignal, &higherPriorityTaskWoken))
                {
                    portYIELD_FROM_ISR(higherPriorityTaskWoken);
                }
                else
                {
                    /* ignore... semaphore has already been given */
                }
            }
            else
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
            }
        }
    }

    if (UINT8_C(1) == event.TxError)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if (RETCODE_OK != Rc)
    {
        Retcode_RaiseErrorFromIsr(Rc);
    }
}
