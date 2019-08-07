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

#include "BCDS_CellularModules.h"
#define BCDS_MODULE_ID BCDS_CELLULAR_MODULE_ID_ENGINE

#include "Engine.h"

#include "AtResponseParser.h"
#include "AtResponseQueue.h"

#include "AtUtils.h"
#include "AtUrc.h"
#include "Hardware.h"

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"
#include "BCDS_RingBuffer.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

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


static StaticSemaphore_t AtResponseParser_RxWakeupBuffer;   //!< Semaphore storage for rx data ready signalling
static SemaphoreHandle_t AtResponseParser_RxWakeupHandle = NULL;  //!< Handle for rx data ready semaphore

static StaticSemaphore_t CellularDriver_TxWakeupBuffer;     //!< Semaphore storage for tx data sent signalling
static SemaphoreHandle_t CellularDriver_TxWakeupHandle = NULL;  //!< Handle for tx data sent semaphore

static StaticTask_t AtResponseParser_TaskBuffer;            //!< static task allocation for Response parser task
static StackType_t  AtResponseParser_TaskStack[CELLULAR_RESP_TASK_STACK_SIZE]; //!< Stack allocation for response parser
static TaskHandle_t AtResponseParser_TaskHandle = NULL;     //!< Task handle of static response parser task


static StaticTask_t CellularDriver_TaskBuffer;            //!< static task allocation for cellular driver task
static StackType_t  CellularDriver_TaskStack[CELLULAR_DRV_TASK_STACK_SIZE];  //!< stack for cellular driver task
static TaskHandle_t CellularDriver_TaskHandle = NULL;     //!< handle to cellular driver task

static UART_T CellularSerialDevice = (UART_T) NULL;       //!< handle to the MCU essentials serial device

static bool IsFlukeFilterEnabled = false;                 //!< state of fluke filter

static StaticSemaphore_t CellularDriver_RequestBuffer;    //!< static semaphore buffer for cellular request locks
static SemaphoreHandle_t CellularDriver_RequestLock = NULL;  //!< handle for semaphore request locks

static Cellular_State_T State = CELLULAR_STATE_POWEROFF;  //!< Cellular driver state context
static Cellular_StateChanged_T OnStateChanged = NULL;     //!< dirty flag for state handling

static bool EchoModeEnabled = true;                       //!< state of modem echo mode (on/off)

static RingBuffer_T UartRxBufDescr;                       //!< RingBuffer instance for rx data reception
static uint8_t UartRxReadBuffer[CELLULAR_RX_BUFFER_SIZE]; //!< physical storage of the ring buffer
static uint8_t UartRxByte;                                //!< single byte rx buffer

char Engine_AtSendBuffer[CELLULAR_AT_SEND_BUFFER_SIZE];   //!< At engine TX buffer

static Retcode_T ReadData(uint8_t* data, uint32_t dataLength, uint32_t* dataRead)
{
    if ((NULL == data) || (NULL == dataRead))
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    *dataRead = RingBuffer_Read(&UartRxBufDescr, data, dataLength);
    return RETCODE_OK;
}

static void HandleMcuIsrCallback(UART_T uart, struct MCU_UART_Event_S event)
{
    BCDS_UNUSED(uart);
    assert(uart != 0);

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
            Retcode_RaiseErrorFromIsr(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES));
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

    uint32_t bytesRead, flukeFreeBytesRead;
    uint8_t * flukeRxReadBuffer;
    Retcode_T status = RETCODE_OK;
    uint8_t rxReadBuffer[CELLULAR_RX_READ_BUFFER_SIZE];

    while (1)
    {
        // wait for the RX IRQ to wake us up
        (void) xSemaphoreTake(AtResponseParser_RxWakeupHandle, portMAX_DELAY);

        while (1)
        {
            status = ReadData(rxReadBuffer, sizeof(rxReadBuffer), &bytesRead);
            if ((RETCODE_OK != status) || (UINT32_C(0) == bytesRead))
            {
                break;
            }
            if (IsFlukeFilterEnabled)
            {
                flukeRxReadBuffer = Utils_TrimFlukeCharacters(rxReadBuffer, bytesRead, &flukeFreeBytesRead);
            }
            else
            {
                flukeRxReadBuffer = rxReadBuffer;
                flukeFreeBytesRead = bytesRead;
            }
            //LOG_DEBUG("Cellular-COM [%d]: %.*s", (int) flukeFreeBytesRead, (int) flukeFreeBytesRead, flukeRxReadBuffer);
            (void) AtResponseParser_Parse(flukeRxReadBuffer, flukeFreeBytesRead);
        }

        if (RETCODE_OK != status)
        {
            LOG_ERROR("Error during Cellular response handling (0x%08x), resetting response parser!", (int) status);
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
        (void) Engine_HandleEvents();
        /* check if socket data bytes available for reading */
//        CellularSocket_NotifyDataReceived();
        (void) xSemaphoreGive(CellularDriver_RequestLock);
    }
}

Retcode_T Engine_HandleEvents(void)
{
    uint32_t count = AtResponseQueue_GetEventCount();
    Retcode_T retcode = Urc_HandleResponses();

    /* remove unhandled events, till next command event */
    if (0 != count && RETCODE_OK != retcode)
    {
        for (uint32_t i = 0; i < count; i++)
        {
            AtResponseQueueEntry_T* event;
#if BCDS_LOGGING
            if (RETCODE_OK == AtResponseQueue_GetEvent(0, &event))
            {
                switch (event->Type)
                {
                case AT_EVENT_TYPE_COMMAND_ECHO:
                    LOG_WARNING("Removing COMMAND_ECHO-event (%.*s) from AtResponseQueue!", event->BufferLength, event->Buffer);
                    break;
                case AT_EVENT_TYPE_COMMAND:
                    LOG_WARNING("Removing COMMAND-event (%.*s) from AtResponseQueue!", event->BufferLength, event->Buffer);
                    break;
                case AT_EVENT_TYPE_COMMAND_ARG:
                    LOG_WARNING("Removing COMMAND_ARG-event (%.*s) from AtResponseQueue!", event->BufferLength, event->Buffer);
                    break;
                case AT_EVENT_TYPE_RESPONSE_CODE:
                    LOG_WARNING("Removing RESPONSE_CODE-event (%d) from AtResponseQueue!", (int) event->ResponseCode);
                    break;
                case AT_EVENT_TYPE_MISC:
                    LOG_WARNING("Removing MISC-event (%.*s) from AtResponseQueue!", event->BufferLength, event->Buffer);
                    break;
                case AT_EVENT_TYPE_ERROR:
                    LOG_WARNING("Removing ERROR-event from AtResponseQueue!");
                    break;
                default:
                    LOG_ERROR("Unexpected event type!");
                    break;
                }
            }
            else
            {
                LOG_WARNING("Removing unknown unhandled event from AtResponseQueue!");
            }
#endif
            AtResponseQueue_MarkBufferAsUnused();

            if (RETCODE_OK != AtResponseQueue_GetEvent(0, &event)
                    || AT_EVENT_TYPE_COMMAND == event->Type)
            {
                break;
            }
        }
    }

    return retcode;
}

Retcode_T Engine_Initialize(Cellular_StateChanged_T onStateChanged)
{
    if (NULL == onStateChanged)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T status;

    LOG_DEBUG("Initializing Cellular engine.");

    // 1. Create RingBuffer for receiving
    RingBuffer_Initialize(&UartRxBufDescr, UartRxReadBuffer, sizeof(UartRxReadBuffer));

    // 2. Setup RX/TX signaling
    AtResponseParser_RxWakeupHandle = xSemaphoreCreateBinaryStatic(&AtResponseParser_RxWakeupBuffer);
    assert(NULL != AtResponseParser_RxWakeupHandle); /* due to static allocation */

    CellularDriver_TxWakeupHandle = xSemaphoreCreateBinaryStatic(&CellularDriver_TxWakeupBuffer);
    assert(NULL != CellularDriver_TxWakeupHandle); /* due to static allocation */

    // 3. Setup the hardware using the BSP
    status = Hardware_Initialize(HandleMcuIsrCallback, &UartRxByte);
    if (RETCODE_OK != status)
    {
        LOG_FATAL("Failed to initialize Hardware!");
        return status;
    }

    status = Hardware_GetCommunicationChannel(&CellularSerialDevice);
    if (RETCODE_OK != status)
    {
        LOG_FATAL("Failed to obtain communications-channel!");
        return status;
    }

    // 4. Setup the submodules (AT response parser, AT response queue, Cellular request queue)
    status = AtResponseQueue_Init();
    if (RETCODE_OK != status)
    {
        LOG_FATAL("Failed to initialize AtResponseQueue!");
        return status;
    }


    Engine_SetFlukeCharFilterEnabled(false);
    AtResponseQueue_RegisterWithResponseParser();

    CellularDriver_RequestLock = xSemaphoreCreateMutexStatic(&CellularDriver_RequestBuffer);
    assert(NULL != CellularDriver_RequestLock); /* due to static allocation */

    // 5. Setup AT response parser task and Cellular request queue task
    AtResponseParser_TaskHandle = xTaskCreateStatic(AtResponseParser_Task, "CellularResp", CELLULAR_RESP_TASK_STACK_SIZE, NULL, CELLULAR_RESP_TASK_PRIORITY, AtResponseParser_TaskStack, &AtResponseParser_TaskBuffer);
    assert(NULL != AtResponseParser_TaskHandle); /* due to static allocation */

    CellularDriver_TaskHandle = xTaskCreateStatic(CellularDriver_Task, "CellularDrv", CELLULAR_DRV_TASK_STACK_SIZE, NULL, CELLULAR_DRV_TASK_PRIORITY, CellularDriver_TaskStack, &CellularDriver_TaskBuffer);
    assert(NULL != CellularDriver_TaskHandle); /* due to static allocation */

    OnStateChanged = onStateChanged;
    State = CELLULAR_STATE_POWEROFF;
    EchoModeEnabled = true;

    return RETCODE_OK;
}

Retcode_T Engine_SendAtCommand(const uint8_t* buffer, uint32_t bufferLength)
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

    ret = MCU_UART_Send(CellularSerialDevice, (uint8_t*) buffer, bufferLength);
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
    (void) Urc_HandleResponses();

    return RETCODE_OK;
}

Retcode_T Engine_SendAtCommandWaitEcho(const uint8_t* buffer, uint32_t bufferLength, uint32_t timeout)
{
    Retcode_T retcode = Engine_SendAtCommand(buffer, bufferLength);
    if (RETCODE_OK != retcode)
    {
        LOG_ERROR("Failed to send command: %x", (unsigned int) retcode);
        return retcode;
    }

    if (EchoModeEnabled)
    {
        return AtResponseQueue_WaitForNamedCmdEcho(timeout, buffer, bufferLength - strlen(ENGINE_ATCMD_FOOTER));
    }
    else
    {
        /* There should be no echo to wait for, so return. */
        return RETCODE_OK;
    }
}

void Engine_SetFlukeCharFilterEnabled(bool value)
{
    IsFlukeFilterEnabled = value;
}

void Engine_NotifyNewState(Cellular_State_T newState, void* param, uint32_t len)
{
    assert(NULL != OnStateChanged);
    if (newState != State)
    {
        Cellular_State_T oldState = State;
        State = newState;

        OnStateChanged(oldState, newState, param, len);
    }
}

Retcode_T Engine_Dispatch(CellularRequest_CallableFunction_T function, uint32_t timeout, void* parameter, uint32_t ParameterLength)
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

void Engine_EchoModeEnabled(bool echoMode)
{
    EchoModeEnabled = echoMode;
}

Retcode_T Engine_Deinitialize(void)
{
    Retcode_T ret = RETCODE_OK;

    assert(NULL != CellularDriver_RequestLock &&
            NULL != AtResponseParser_RxWakeupHandle &&
            NULL != CellularDriver_TxWakeupHandle &&
            NULL != OnStateChanged);

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

    ret = Hardware_Deinitialize();
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

    OnStateChanged = NULL;

    return RETCODE_OK;
}
