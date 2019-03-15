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

#include <gtest.h>

FFF_DEFINITION_BLOCK_START

extern "C"
{
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif /* __STDC_FORMAT_MACROS */

#define portNUM_CONFIGURABLE_REGIONS    1
#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_ENGINE

#include "CellularModule.h"
#include "BCDS_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "queue_th.hh"
#include "semphr_th.hh"
#include "BCDS_Ringbuffer_th.hh"
#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"
#include "Control_th.hh"
#include "Socket_th.hh"
#include "Network_th.hh"
#include "Utils_th.hh"
#include "Log_th.hh"

#include "BCDS_MCU_UART_th.hh"
#include "Bsp_th.hh"

#define CellularSocket_NotifyDataReceived() break

#undef RETCODE
#define RETCODE(severity,code) ((Retcode_T) code)

#undef BCDS_MODULE_ID
#include "Urc.c"

#undef BCDS_MODULE_ID
#include "Engine.c"

Retcode_T ParseCallback(AtResponseParserArgument_T * callback)
{
    BCDS_UNUSED(callback);
    return RETCODE_OK;
}

static void EventCb(Cellular_CallbackEvent_T event, void *notified)
{
    BCDS_UNUSED(event);

    if (notified)
    {
        *(bool *) notified = true;
    }
}

static Retcode_T UartReceiveCustom(UART_T uart, uint8_t* buffer, uint32_t size)
{
    BCDS_UNUSED(uart);
    BCDS_UNUSED(size);

    *buffer = AT_DEFAULT_S4_CHARACTER;

    return RETCODE_OK;
}

static Retcode_T CellularBsp_GetCommunicationChannelCustom(UART_T* uartHandle)
{
    *uartHandle = (UART_T)10;

    return RETCODE_OK;
}

static uint32_t SemphrPass = 0;
static uint32_t SemphrCount = 0;

static SemaphoreHandle_t SemphrCreateBinaryHandle(void)
{
    SemphrCount = (SemphrCount + 1) % 2;

    switch (SemphrPass)
    {
    case 0:
        return (SemaphoreHandle_t) 1;
    case 1:
        return SemphrCount == 1 ? (SemaphoreHandle_t) 1 : NULL;
    case 2:
        return SemphrCount == 1 ? NULL : (SemaphoreHandle_t) 1;
    default:
        return NULL;
    }
}

static uint8_t TaskFailNr = 0;
static uint8_t TaskCount = 0;

static BaseType_t TaskCreate(TaskFunction_t pvTaskCode,
        const char * const pcName,
        uint16_t usStackDepth,
        void *pvParameters,
        UBaseType_t uxPriority,
        TaskHandle_t *pvCreatedTask)
{
    BCDS_UNUSED(pvTaskCode);
    BCDS_UNUSED(pcName);
    BCDS_UNUSED(usStackDepth);
    BCDS_UNUSED(pvParameters);
    BCDS_UNUSED(uxPriority);
    BCDS_UNUSED(pvCreatedTask);

    TaskCount = (TaskCount + 1) % 2;

    return ((TaskCount + 1) ^ TaskFailNr) > 0 ? pdPASS : pdFAIL;
}

static Retcode_T AtrspqMiscContentReturnVal = RETCODE_OK;
static uint8_t AtrspqArgNbr = 0;

static Retcode_T AtrspqMiscContent(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen)
{
    BCDS_UNUSED(timeout);
    BCDS_UNUSED(BufferLen);

    const char* arguments[6] =
            {
                    "RDY", "NORMAL POWER DOWN", "POWERED DOWN", "READY", "SMS DONE", "PB DONE"
            };

    *BufferPtr = (uint8_t *) arguments[AtrspqArgNbr];
    return AtrspqMiscContentReturnVal;

}

} //-- extern "C"

FFF_DEFINITION_BLOCK_END

class CellularEngine: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(xSemaphoreCreateBinary);
        RESET_FAKE(xSemaphoreTake);
        RESET_FAKE(xSemaphoreGive);
        RESET_FAKE(xSemaphoreGiveFromISR);
        RESET_FAKE(xSemaphoreCreateMutex);

        RESET_FAKE(xTaskCreate);
        RESET_FAKE(vTaskDelete);

        RESET_FAKE(AtResponseQueue_Init);
        RESET_FAKE(AtResponseQueue_Deinit);
        RESET_FAKE(AtResponseQueue_WaitForNamedCmd);
        RESET_FAKE(AtResponseQueue_EnqueueEvent);
        RESET_FAKE(AtResponseQueue_GetEvent);

        RESET_FAKE(CellularSocketUrc_UUSOCL);
        RESET_FAKE(CellularSocketUrc_UUSORD);
        RESET_FAKE(CellularSocketUrc_UUSORF);
        RESET_FAKE(CellularSocketUrc_QIURC);

        RESET_FAKE(CellularNetworkUrc_UUPSDA);
        RESET_FAKE(CellularNetworkUrc_UUPSDD);
        RESET_FAKE(CellularNetworkUrc_CREG);

        RESET_FAKE(CellularBsp_Init);
        RESET_FAKE(CellularBsp_GetCommunicationChannel);
        RESET_FAKE(CellularBsp_Deinit);

        RESET_FAKE(MCU_UART_Receive);
        RESET_FAKE(MCU_UART_GetRxCount);
        RESET_FAKE(MCU_UART_Send);

        RESET_FAKE(Retcode_RaiseError);

        CellularBsp_GetCommunicationChannel_fake.custom_fake = CellularBsp_GetCommunicationChannelCustom;
        xSemaphoreCreateBinary_fake.custom_fake = SemphrCreateBinaryHandle;
        xTaskCreate_fake.custom_fake = TaskCreate;
        AtResponseQueue_WaitForMiscContent_fake.custom_fake = AtrspqMiscContent;

        IsFlukeFilterEnabled = false;

    }

    virtual void TearDown()
    {
        ;
    }
};

/* test cases ******************************************************* */

TEST_F(CellularEngine, TestCellularDriverTask)
{
    CellularDriver_Task(NULL);
    EXPECT_EQ(1U,AtResponseQueue_GetEvent_fake.call_count);
}

TEST_F(CellularEngine, TestCellularEngineInit)
{
    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularEngine_Init(NULL));
    SemphrPass = 1;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));
    SemphrPass = 2;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));
    SemphrPass = 0;

    CellularBsp_Init_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularEngine_Init(EventCb));

    CellularBsp_Init_fake.return_val = RETCODE_OK;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));

    AtResponseQueue_Init_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));

    AtResponseQueue_Init_fake.return_val = RETCODE_OK;
    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 0;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));

    xSemaphoreCreateMutex_fake.return_val = (SemaphoreHandle_t) 1;

    TaskFailNr = 1;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));
    TaskFailNr = 2;
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, CellularEngine_Init(EventCb));

    /* test passing scenario */
    TaskFailNr = 0;
    EXPECT_EQ(RETCODE_OK, CellularEngine_Init(EventCb));
}

TEST_F(CellularEngine, TestCellularEngineSendAtCommand)
{
    uint8_t buffer[1];

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularEngine_SendAtCommand(NULL, 1));
    CellularBsp_Init_fake.return_val = RETCODE_FAILURE;
    CellularEngine_Init(EventCb);
    EXPECT_NE(RETCODE_OK, CellularEngine_SendAtCommand(buffer, sizeof(buffer)));
    xSemaphoreTake_fake.return_val = pdFAIL;
    EXPECT_EQ(RETCODE_SEMAPHORE_ERROR, CellularEngine_SendAtCommand(buffer, sizeof(buffer)));

    xSemaphoreTake_fake.return_val = pdPASS;
    MCU_UART_Send_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularEngine_SendAtCommand(buffer, sizeof(buffer)));

    /* test passing scenario */
    MCU_UART_Send_fake.return_val = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 1;
    xSemaphoreTake_fake.return_val = pdPASS;
    EXPECT_EQ(RETCODE_OK, CellularEngine_SendAtCommand(buffer, sizeof(buffer)));
}

TEST_F(CellularEngine, TestCellularEngineSendAtCmdWaitEcho)
{
    const char *buffer = "a";

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularEngine_SendAtCmdWaitEcho(0,NULL));

    MCU_UART_Send_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularEngine_SendAtCmdWaitEcho(0,buffer));

    /* test passing scenario */
    MCU_UART_Send_fake.return_val = RETCODE_OK;
    xTaskGetTickCount_fake.return_val = 1;
    xSemaphoreTake_fake.return_val = pdPASS;
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_OK;
    EXPECT_EQ(RETCODE_OK, CellularEngine_SendAtCmdWaitEcho(0,buffer));
}

static Retcode_T TestFunction(void* buffer, uint32_t SizeBuffer)
{
    BCDS_UNUSED(buffer);
    BCDS_UNUSED(SizeBuffer);
    return RETCODE_OK;
}

TEST_F(CellularEngine, TestCellularRequestDispatch)
{
    uint8_t buffer[1];

    CellularDriver_RequestLock = (void*)1;
    EXPECT_EQ(RETCODE_NULL_POINTER, CellularRequest_Dispatch(0, NULL, (void*) buffer, sizeof(buffer)));

    xSemaphoreTake_fake.return_val = pdFAIL;
    EXPECT_EQ(RETCODE_CELLULAR_DRIVER_BUSY, CellularRequest_Dispatch(0, TestFunction, (void*) buffer,
            sizeof(buffer)));

    xSemaphoreTake_fake.return_val = pdPASS;
    EXPECT_EQ(RETCODE_OK, CellularRequest_Dispatch(0, TestFunction, (void*) buffer, sizeof(buffer)));
}

TEST_F(CellularEngine, TestCellularCellularCallBack_Error)
{
    UART_T uart = (UART_T) 0;
    struct MCU_UART_Event_S event;

    memset((void*)&event, UINT32_C(0), sizeof(MCU_UART_Event_S));

    Cellular_Callback_ISR(uart, event);
    EXPECT_EQ(0U, xSemaphoreGiveFromISR_fake.call_count);
}

TEST_F(CellularEngine, TestCellularCellularTxCallBack_ISR)
{
    UART_T uart = (UART_T) 1;
    struct MCU_UART_Event_S event;

    memset((void*)&event, UINT32_C(0), sizeof(MCU_UART_Event_S));

    event.TxComplete = UINT32_C(1);

    Cellular_Callback_ISR(uart, event);
    EXPECT_EQ(1U, xSemaphoreGiveFromISR_fake.call_count);

    Cellular_Callback_ISR(uart, event);
    EXPECT_EQ(2U, xSemaphoreGiveFromISR_fake.call_count);
}

TEST_F(CellularEngine, TestCellularCellularRxCallBack_ISR_Error)
{
    UART_T uart = (UART_T) 1;
    struct MCU_UART_Event_S event;

    memset((void*)&event, UINT32_C(0), sizeof(MCU_UART_Event_S));

    event.RxComplete = UINT32_C(1);

    MCU_UART_Receive_fake.return_val = RETCODE_FAILURE;
    Cellular_Callback_ISR(uart, event);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreGiveFromISR_fake.call_count);

    RESET_FAKE(Retcode_RaiseError);

    MCU_UART_Receive_fake.return_val = RETCODE_OK;
    RingBuffer_Write_fake.return_val = UINT32_C(0);
    Cellular_Callback_ISR(uart, event);
    EXPECT_EQ(1U, Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreGiveFromISR_fake.call_count);

    RESET_FAKE(MCU_UART_Receive);
    UartRxByte = AT_DEFAULT_S4_CHARACTER;

    MCU_UART_Receive_fake.custom_fake = UartReceiveCustom;
    RingBuffer_Write_fake.return_val = UINT32_C(1);
    Cellular_Callback_ISR(uart, event);
    EXPECT_EQ(1U, xSemaphoreGiveFromISR_fake.call_count);
}

TEST_F(CellularEngine, TestCellularDeviceStatus)
{
    bool notified = false;

    CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_CONNECTED, CELLULAR_EVENT_CONNECTED, &notified);
    EXPECT_EQ(CELLULAR_DEVICESTATUS_CONNECTED, CellularEngine_GetDeviceStatus());
    EXPECT_EQ(true, notified);

    CellularEngine_SetDeviceStatus(CELLULAR_DEVICESTATUS_DISCONNECTED, CELLULAR_EVENT_DISCONNECTED, NULL);
    EXPECT_EQ(CELLULAR_DEVICESTATUS_DISCONNECTED, CellularEngine_GetDeviceStatus());
}

TEST_F(CellularEngine, TestCellularDeviceError)
{
    CellularEngine_SetDeviceError(RETCODE_INVALID_PARAM);
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularEngine_GetDeviceError());
    CellularEngine_SetDeviceError(RETCODE_OK);
    EXPECT_EQ(RETCODE_OK, CellularEngine_GetDeviceError());
}

TEST_F(CellularEngine, TestCellularUrcCallHandler)
{
    bool arg = false;

    /* test failing scenarios */
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUrc_CallHandler(NULL, RETCODE_OK, &arg));
    EXPECT_EQ(RETCODE_INVALID_PARAM, CellularUrc_CallHandler(CellularNetworkUrc_CREG, RETCODE_OK, NULL));
    EXPECT_EQ(0U, CellularNetworkUrc_CREG_fake.call_count);

    /* test passing scenario */
    EXPECT_EQ(RETCODE_OK, CellularUrc_CallHandler(CellularNetworkUrc_CREG, RETCODE_OK, &arg));
    EXPECT_EQ(1U, CellularNetworkUrc_CREG_fake.call_count);
    EXPECT_EQ(true, arg);
}

TEST_F(CellularEngine, TestCellularUrcMiscellaneous)
{
    /* test failing scenarios */
    AtrspqMiscContentReturnVal = RETCODE_FAILURE;
    AtResponseQueue_WaitForArbitraryCmdArg_fake.return_val = RETCODE_FAILURE;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularUrc_Miscellaneous());

    /* test passing scenarios */
    AtrspqMiscContentReturnVal = RETCODE_OK;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_OK;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());

    AtResponseQueue_WaitForArbitraryCmdArg_fake.return_val = RETCODE_OK;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());

    AtrspqArgNbr = 1;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());
    AtrspqArgNbr = 2;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());
    AtrspqArgNbr = 3;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());
    AtrspqArgNbr = 4;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());
    AtrspqArgNbr = 5;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());

    CellularUtils_StrtolBounds_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_OK, CellularUrc_Miscellaneous());

    CellularUtils_StrtolBounds_fake.return_val = RETCODE_OK;

}

TEST_F(CellularEngine, TestCellularUrcHandleResponses)
{
    /* test handled scenario */
    EXPECT_EQ(RETCODE_OK, CellularUrc_HandleResponses());

    /* test unhandled scenario */
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;

    CellularSocketUrc_UUSOCL_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;
    CellularSocketUrc_UUSORD_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;
    CellularSocketUrc_UUSORF_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;
    CellularNetworkUrc_UUPSDA_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;
    CellularNetworkUrc_UUPSDD_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;
    CellularSocketUrc_QIURC_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;
    CellularNetworkUrc_CREG_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE;

    EXPECT_EQ(RETCODE_CELLULAR_URC_NOT_PRESENT, CellularUrc_HandleResponses());
}

TEST_F(CellularEngine, TestCellularEngineHandleEvents)
{
    /* test passing scenarios */
    AtResponseQueue_GetEventCount_fake.return_val = 0;
    EXPECT_EQ(RETCODE_OK, CellularEngine_HandleEvents());

    AtResponseQueue_GetEventCount_fake.return_val = 8;
    EXPECT_EQ(RETCODE_OK, CellularEngine_HandleEvents());

    AtResponseQueue_GetEvent_fake.return_val = RETCODE_OK;

    /* test failing scenarios */
    CellularSocketUrc_UUSOCL_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    CellularSocketUrc_UUSORD_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    CellularSocketUrc_UUSORF_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    CellularNetworkUrc_UUPSDA_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    CellularNetworkUrc_UUPSDD_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    CellularSocketUrc_QIURC_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    CellularNetworkUrc_CREG_fake.return_val = RETCODE_CELLULAR_URC_NOT_PRESENT;
    EXPECT_EQ(RETCODE_OK, CellularEngine_HandleEvents());

    AtResponseQueue_GetEvent_fake.return_val = RETCODE_AT_RESPONSE_QUEUE_TIMEOUT;
    EXPECT_EQ(RETCODE_OK, CellularEngine_HandleEvents());
    AtResponseQueue_Clear();
}

TEST_F(CellularEngine, TestCellularEngineDeinit)
{
    CellularDriver_RequestLock = (xSemaphoreHandle)1;
    AtResponseParser_RxWakeupHandle = (SemaphoreHandle_t)1;
    CellularDriver_TxWakeupHandle = (xSemaphoreHandle)1;
    NotifyEvent = (Cellular_CallbackFunction_T)1;

    AtResponseQueue_Deinit_fake.return_val = RETCODE_FAILURE;
    CellularBsp_Deinit_fake.return_val = RETCODE_OK;

    EXPECT_EQ(RETCODE_FAILURE, CellularEngine_Deinit());
    EXPECT_EQ(UINT32_C(1), AtResponseQueue_Deinit_fake.call_count);
    EXPECT_EQ(UINT32_C(0), CellularBsp_Deinit_fake.call_count);

    AtResponseQueue_Deinit_fake.return_val = RETCODE_OK;
    CellularBsp_Deinit_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, CellularEngine_Deinit());
    EXPECT_EQ(UINT32_C(1), CellularBsp_Deinit_fake.call_count);
    EXPECT_EQ(UINT32_C(0), RingBuffer_Reset_fake.call_count);

    CellularBsp_Deinit_fake.return_val = RETCODE_OK;
    EXPECT_EQ(RETCODE_OK, CellularEngine_Deinit());
    EXPECT_EQ(UINT32_C(1), RingBuffer_Reset_fake.call_count);
}

TEST_F(CellularEngine, TestCellular_ReadData)
{
    Retcode_T retcode = RETCODE_OK;
    RingBuffer_Read_fake.return_val = UINT32_C(2);

    uint8_t data = AT_DEFAULT_S4_CHARACTER;
    uint32_t dataLength = UINT32_C(1);
    uint32_t dataRead = UINT32_C(0);

    /* test failing scenario */
    retcode = Cellular_ReadData(NULL, dataLength, &dataRead);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retcode);
    EXPECT_EQ(UINT32_C(0), RingBuffer_Read_fake.call_count);

    /* test passing scenarios */
    retcode = Cellular_ReadData(&data, dataLength, &dataRead);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(UINT32_C(1), RingBuffer_Read_fake.call_count);
}

TEST_F(CellularEngine, SetCustomAtResponseParserTest)
{
    CustomAtResponseParser = ParseCallback;
    CellularEngine_SetCustomAtResponseParser(CustomAtResponseParser);
}

TEST_F(CellularEngine, DispatchTest)
{
    uint8_t buffer[1] = {0};
    Retcode_T retcode = RETCODE_OK;
    CellularDriver_RequestLock = NULL;
    retcode = CellularRequest_Dispatch(0, TestFunction, (void*) buffer, sizeof(buffer));
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_UNINITIALIZED), retcode);
}
TEST_F(CellularEngine, SendAtCommandTest)
{
    uint8_t buffer[1] = {0};
    CellularSerialDevice = (UART_T) 0;
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), CellularEngine_SendAtCommand(buffer, sizeof(buffer)));
}

