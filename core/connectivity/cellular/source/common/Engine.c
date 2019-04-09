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

/**
 * @file
 *
 * @brief The engine of the Cellular driver.
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_ENGINE

#include "CellularModule.h"
#include "BCDS_Assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"
#include "BCDS_RingBuffer.h"
#include "CellularCommon.h"
#include "CellularAtResponseParser.h"
#include "CellularResponseQueue.h"
#include "CellularUrc.h"
#include "CellularUtils.h"
#include "CellularEngine.h"
#include "CellularBsp.h"

#define LOG_LEVEL_PACKAGE   LOG_LEVEL_DEBUG
#define LOG_LEVEL_MODULE    LOG_LEVEL_DEBUG
#include "BCDS_Logging.h"

/**
 * @brief Number of bytes per chunk with which we read the receive buffer
 */
#define CELLULAR_RX_READ_BUFFER_SIZE  (UINT32_C(128))

/**
 * @brief The number of times we wait for a free RX line before sending
 */
#define CELLULAR_SEND_AT_COMMAND_WAIT_TIME     (UINT32_C(1000) / portTICK_PERIOD_MS)

/**
 * @brief The time until we check again when the last character was received
 */
#define CELLULAR_RESPONSE_QUEUE_WAIT_TIME      (UINT32_C(25) / portTICK_PERIOD_MS)

/* Modem initialization status */
CellularInitStatus_T CellularInitStatus;

static SemaphoreHandle_t AtResponseParser_RxWakeupHandle = NULL;
static SemaphoreHandle_t CellularDriver_TxWakeupHandle = NULL;
static TaskHandle_t AtResponseParser_TaskHandle = NULL;
static TaskHandle_t CellularDriver_TaskHandle = NULL;

static UART_T CellularSerialDevice = (UART_T) NULL;

static bool IsFlukeFilterEnabled = FALSE;

static xSemaphoreHandle CellularDriver_RequestLock = NULL;

/* Cellular event callback function, status and error variables */
static Cellular_CallbackFunction_T NotifyEvent = NULL;
static Cellular_DeviceStatus_T DeviceStatus = CELLULAR_DEVICESTATUS_DISCONNECTED;
static Retcode_T DeviceError = (Retcode_T) RETCODE_UNINITIALIZED;
static AtResponseParserCallback_T CustomAtResponseParser = NULL;

static RingBuffer_T UartRxBufDescr;
static uint8_t UartRxReadBuffer[CELLULAR_RX_BUFFER_SIZE];
static uint8_t UartRxByte;

static void Cellular_Callback_ISR(UART_T uart, struct MCU_UART_Event_S event)
{
    if ((UART_T) 0 == uart)
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM));
        return;
    }

    if (event.TxComplete)
    {
        /* all bytes have been transmitted, signal semaphore */
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        (void) xSemaphoreGiveFromISR(CellularDriver_TxWakeupHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    if (event.RxComplete)
    {
        uint32_t bytesWritten = RingBuffer_Write(&UartRxBufDescr, &UartRxByte, UINT32_C(1));
        if(UINT32_C(1) != bytesWritten)
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES));
            return;
        }

        if (AT_DEFAULT_S4_CHARACTER == UartRxByte)
        {
            //-- Wake up task to trigger AT command response parser
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            (void) xSemaphoreGiveFromISR(AtResponseParser_RxWakeupHandle, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

static void AtResponseParser_Task(void *param)
{
    BCDS_UNUSED(param);

    uint32_t BytesRead, FlukeFreeBytesRead;
    uint8_t * FlukeRxReadBuffer;
    Retcode_T status = RETCODE_OK;
    uint8_t rxReadBuffer[CELLULAR_RX_READ_BUFFER_SIZE];

    while (1)
    {
        // wait for the RX IRQ to wake us up
        (void) xSemaphoreTake(AtResponseParser_RxWakeupHandle, portMAX_DELAY);

        if (CustomAtResponseParser)
        {
            LOG_DEBUG("invoke custom parser: %p", CustomAtResponseParser);
            AtResponseParserArgument_T arg;
            arg.Semaphore = AtResponseParser_RxWakeupHandle;
            arg.Device = CellularSerialDevice;

            Retcode_T retcode = CustomAtResponseParser(&arg);
            if (RETCODE_OK != retcode)
            {
                LOG_ERROR("custom parser failed, retcode: %x", (unsigned int) retcode);
                AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_ERROR, NULL, 0);
            }

            CellularEngine_SetCustomAtResponseParser(NULL);
        }

        while (1)
        {
            status = Cellular_ReadData(rxReadBuffer, sizeof(rxReadBuffer), &BytesRead);

            if ((RETCODE_OK != status) || (UINT32_C(0) == BytesRead))
            {
                break;
            }

            if (IsFlukeFilterEnabled)
            {
                FlukeRxReadBuffer = CellularUtils_TrimFlukeCharacters(rxReadBuffer, BytesRead, &FlukeFreeBytesRead);
            }
            else
            {
                FlukeRxReadBuffer = rxReadBuffer;
                FlukeFreeBytesRead = BytesRead;
            }
#if defined(BCDS_LOGGING) && (LOG_LEVEL_GET_CURRENT() >= LOG_LEVEL_DEBUG)
            char hex[BytesRead * 2];
            CellularUtils_BinToHex(rxReadBuffer, hex, BytesRead);
            LOG_DEBUG("BytesRead: %d [%.*s]", (int) BytesRead, (int) BytesRead * 2, hex);
            LOG_DEBUG("parse: %.*s", (int) FlukeFreeBytesRead, FlukeRxReadBuffer);
#endif /* BCDS_LOGGING */
            (void) AtResponseParser_Parse(FlukeRxReadBuffer, FlukeFreeBytesRead);
        }

        if (RETCODE_OK != status)
        {
            LOG_ERROR("status: %d, resetting response parser", (int) status);
            AtResponseParser_Reset();
        }
    }
}

static void CellularDriver_Task(void *param)
{
    BCDS_UNUSED(param);

    while (1)
    {
        /* wait for any event */
        AtResponseQueueEntry_T *event;
        (void) AtResponseQueue_GetEvent(portMAX_DELAY, &event);

        (void) xSemaphoreTake(CellularDriver_RequestLock, portMAX_DELAY);
        /* handle urc events */
        (void) CellularEngine_HandleEvents();
        /* check if socket data bytes available for reading */
        CellularSocket_NotifyDataReceived();
        (void) xSemaphoreGive(CellularDriver_RequestLock);
    }
}

Retcode_T CellularEngine_HandleEvents(void)
{
    uint32_t count = AtResponseQueue_GetEventCount();
    Retcode_T retcode = CellularUrc_HandleResponses();

    /* remove unhandled events, till next command event */
    if (0 != count && RETCODE_OK != retcode)
    {
        for (uint32_t i = 0; i < count; i++)
        {
            LOG_WARNING("removing %d unhandled events", (int) AtResponseQueue_GetEventCount());
            AtResponseQueue_MarkBufferAsUnused();

            AtResponseQueueEntry_T *event;
            if (RETCODE_OK != AtResponseQueue_GetEvent(0, &event)
                    || AT_EVENT_TYPE_COMMAND == event->Type)
            {
                break;
            }
        }
    }

    return retcode;
}

Retcode_T CellularEngine_Init(Cellular_CallbackFunction_T EventCallback)
{
    if (NULL == EventCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    NotifyEvent = EventCallback;
    memset(&CellularInitStatus, 0, sizeof(CellularInitStatus));

    Retcode_T status;

#ifdef BCDS_LOGGING
    status = Logging_Init(Logging_AsyncRecorder, Logging_UARTAppender);
    if (RETCODE_OK != status)
    {
        return status;
    }
#endif /* BCDS_LOGGING */

    LOG_INFO("CellularEngine init");

    // 1. Create RingBuffer for receiving
    RingBuffer_Initialize(&UartRxBufDescr, UartRxReadBuffer, sizeof(UartRxReadBuffer));

    // 2. Setup RX/TX signaling
    AtResponseParser_RxWakeupHandle = xSemaphoreCreateBinary();
    if (NULL == AtResponseParser_RxWakeupHandle)
    {
        LOG_FATAL("Cellular engine init failed: AtResponseParser_RxWakeupHandle create");
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }

    CellularDriver_TxWakeupHandle = xSemaphoreCreateBinary();
    if (NULL == CellularDriver_TxWakeupHandle)
    {
        LOG_FATAL("Cellular engine init failed: CellularDriver_TxWakeupHandle create");
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }

    // 3. Setup the hardware using the BSP
    status = CellularBsp_Init(Cellular_Callback_ISR, &UartRxByte);
    if (RETCODE_OK != status)
    {
        LOG_FATAL("Cellular engine init failed: CellularBsp_Init");
        return status;
    }

    status = CellularBsp_GetCommunicationChannel(&CellularSerialDevice);
    if (RETCODE_OK != status)
    {
        LOG_FATAL("Cellular engine init failed: CellularBsp_getCommunicationChannel");
        return status;
    }

    // 4. Setup the submodules (AT response parser, AT response queue, Cellular request queue)
    status = AtResponseQueue_Init();
    if (RETCODE_OK != status)
    {
        LOG_FATAL("Cellular engine init failed: AtResponseQueue_Init");
        return status;
    }

    CellularEngine_SetFlukeCharFilterEnabled(false);
    AtResponseQueue_RegisterWithResponseParser();

    CellularDriver_RequestLock = xSemaphoreCreateMutex();
    if (NULL == CellularDriver_RequestLock)
    {
        LOG_FATAL("Cellular engine init failed: CellularDriver_RequestLock create");
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }

    // 5. Setup AT response parser task and Cellular request queue task
    status = xTaskCreate(AtResponseParser_Task, "CellularAtResponseParser", CELLULAR_RESP_TASK_STACK_SIZE, NULL, CELLULAR_RESP_TASK_PRIORITY, &AtResponseParser_TaskHandle);
    if (pdPASS != status)
    {
        LOG_FATAL("Cellular engine init failed: create AtResponseParser_Task");
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }

    status = xTaskCreate(CellularDriver_Task, "CellDrv", CELLULAR_DRV_TASK_STACK_SIZE, NULL, CELLULAR_DRV_TASK_PRIORITY, &CellularDriver_TaskHandle);
    if (pdPASS != status)
    {
        LOG_FATAL("Cellular engine init failed: create CellularDriver_Task");
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    }
    /* reset device error */
    CellularEngine_SetDeviceError(RETCODE_OK);

    LOG_INFO("CellularEngine init done");
    return RETCODE_OK;
}

Retcode_T CellularEngine_SendAtCommand(const uint8_t *buffer, uint32_t BufferLength)
{
    Retcode_T ret = RETCODE_OK;
    if (NULL == buffer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    if ((UART_T) 0 == CellularSerialDevice)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    /* ensure the semaphore is NOT signaled as we begin to send */
    (void) xSemaphoreTake(CellularDriver_TxWakeupHandle, 0);

    ret = MCU_UART_Send(CellularSerialDevice, (uint8_t *) buffer, BufferLength);

    if (RETCODE_OK != ret)
    {
        return ret;
    }

    /* wait the end of serial transfer */
    if (pdPASS != xSemaphoreTake(CellularDriver_TxWakeupHandle, CELLULAR_SEND_AT_COMMAND_WAIT_TIME))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
    }

    /* wait for any response */
    AtResponseQueueEntry_T *entry;
    (void) AtResponseQueue_GetEvent(CELLULAR_RESPONSE_QUEUE_WAIT_TIME, &entry);

    /* handle URC responses */
    (void) CellularUrc_HandleResponses();

    return RETCODE_OK;
}

Retcode_T CellularEngine_SendAtCmdWaitEcho(uint32_t timeout, const char *str)
{
    if (NULL == str)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    uint32_t len = strlen(str);
    Retcode_T retcode = CellularEngine_SendAtCommand((uint8_t *) str, len);
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("Failed to send command: %x", (unsigned int) retcode);
        return retcode;
    }

    return AtResponseQueue_WaitForNamedCmdEcho(timeout, (uint8_t *) str, len - strlen(ATCMD_AT_CRLF));
}

void CellularEngine_SetFlukeCharFilterEnabled(bool value)
{
    IsFlukeFilterEnabled = value;
}

Cellular_DeviceStatus_T CellularEngine_GetDeviceStatus(void)
{
    return DeviceStatus;
}

void CellularEngine_SetDeviceStatus(Cellular_DeviceStatus_T status, Cellular_CallbackEvent_T event, void *info)
{
    LOG_DEBUG("status: %d, event: %d", (int) status, (int) event);
    DeviceStatus = status;
    if (NotifyEvent)
    {
        NotifyEvent(event, info);
    }
}

Retcode_T CellularEngine_GetDeviceError(void)
{
    return DeviceError;
}

void CellularEngine_SetDeviceError(Retcode_T error)
{
    DeviceError = error;
}

Retcode_T CellularRequest_Dispatch(uint32_t timeout, CellularRequest_CallableFunction_T function, void* parameter, uint32_t ParameterLength)
{
    if (NULL == function)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (NULL == CellularDriver_RequestLock)
    {
        return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_UNINITIALIZED);
    }

    if (pdPASS != xSemaphoreTake(CellularDriver_RequestLock, timeout))
    {
        LOG_ERROR("Unable to get lock within %d ms, driver is busy", (int) timeout);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_DRIVER_BUSY);
    }

    Retcode_T retcode = function(parameter, ParameterLength);
    (void) xSemaphoreGive(CellularDriver_RequestLock);

    return retcode;
}

void CellularEngine_SetCustomAtResponseParser(AtResponseParserCallback_T callback)
{
    CustomAtResponseParser = callback;
}

Retcode_T CellularEngine_Deinit(void)
{
    Retcode_T ret = RETCODE_OK;

    assert(NULL != CellularDriver_RequestLock &&
            NULL != AtResponseParser_RxWakeupHandle &&
            NULL != CellularDriver_TxWakeupHandle &&
            NULL != NotifyEvent);

    vSemaphoreDelete(CellularDriver_RequestLock);
    CellularDriver_RequestLock = NULL;

    vTaskDelete(AtResponseParser_TaskHandle);
    AtResponseParser_TaskHandle = NULL;
    vTaskDelete(CellularDriver_TaskHandle);
    CellularDriver_TaskHandle = NULL;

    ret = AtResponseQueue_Deinit();
    if (RETCODE_OK != ret)
    {
        LOG_FATAL("Cellular engine deinit failed: AtResponseQueue_Deinit");
        return ret;
    }

    ret = CellularBsp_Deinit();
    if (RETCODE_OK != ret)
    {
        LOG_FATAL("Cellular engine deinit failed: CellularBsp_Deinit");
        return ret;
    }

    RingBuffer_Reset(&UartRxBufDescr);

    CellularSerialDevice = (UART_T)0;

    vSemaphoreDelete(AtResponseParser_RxWakeupHandle);
    AtResponseParser_RxWakeupHandle = NULL;

    vSemaphoreDelete(CellularDriver_TxWakeupHandle);
    CellularDriver_TxWakeupHandle = NULL;

    NotifyEvent = NULL;

    return RETCODE_OK;
}

Retcode_T Cellular_ReadData(uint8_t* data, uint32_t dataLength, uint32_t* dataRead)
{
    if((NULL == data) || (NULL == dataRead))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    *dataRead = RingBuffer_Read(&UartRxBufDescr, data, dataLength);
    return RETCODE_OK;
}
