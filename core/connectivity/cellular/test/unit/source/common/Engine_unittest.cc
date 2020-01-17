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

#include <gtest.h>

FFF_DEFINITION_BLOCK_START

extern "C"
{
#include "Kiso_CellularModules.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_ENGINE

#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"

#include "AtUtils_th.hh"
#include "AtUrc_th.hh"
#include "Hardware_th.hh"
#include "Kiso_MCU_UART_th.hh"

#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "Kiso_RingBuffer_th.hh"
#include "Kiso_Logging_th.hh"
#undef LOG_DEBUG
#define LOG_DEBUG(...) \
    do                 \
    {                  \
    } while (0)

#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "semphr_th.hh"
#include "portmacro_th.hh"

#undef KISO_MODULE_ID
#include "Engine.c"
}

#include <array>

FFF_DEFINITION_BLOCK_END

static SemaphoreHandle_t Custom_xSemaphoreCreateBinaryStatic(StaticSemaphore_t *staticSemphr)
{
    return (SemaphoreHandle_t)staticSemphr;
}

static SemaphoreHandle_t Custom_xSemaphoreCreateMutexStatic(StaticSemaphore_t *staticSemphr)
{
    return (SemaphoreHandle_t)staticSemphr;
}

static TaskHandle_t Custom_xTaskCreateStatic(TaskFunction_t fun, const char *, uint32_t, void *, UBaseType_t, StackType_t *, StaticTask_t *)
{
    return (TaskHandle_t)fun;
}

class TS_Engine_Tasks : public testing::Test
{
protected:
    virtual void SetUp()
    {
        srand(time(NULL));

        FFF_RESET_HISTORY();

        RESET_FAKE(RingBuffer_Read);

        IsFlukeFilterEnabled = false;
    }
};

TEST_F(TS_Engine_Tasks, ReadData_ZeroLength)
{
    RingBuffer_Read_fake.return_val = 0;
    AtResponseParser_TaskLoop();
}

TEST_F(TS_Engine_Tasks, ReadData_OneByte)
{
    uint32_t readReturns[2] = {1, 0};
    SET_RETURN_SEQ(RingBuffer_Read, readReturns, 2);
    AtResponseParser_TaskLoop();
}

TEST_F(TS_Engine_Tasks, ReadData_OneByteFlukeFilter)
{
    uint32_t readReturns[2] = {1, 0};
    SET_RETURN_SEQ(RingBuffer_Read, readReturns, 2);
    IsFlukeFilterEnabled = true;
    AtResponseParser_TaskLoop();
}

class TS_Engine_Initialize : public testing::Test
{
protected:
    virtual void SetUp()
    {
        srand(time(NULL));

        FFF_RESET_HISTORY();
        RESET_FAKE(RingBuffer_Initialize);
        RESET_FAKE(xSemaphoreCreateBinaryStatic);
        RESET_FAKE(Hardware_Initialize);
        RESET_FAKE(Hardware_GetCommunicationChannel);
        RESET_FAKE(AtResponseQueue_Init);
        RESET_FAKE(AtResponseQueue_RegisterWithResponseParser);
        RESET_FAKE(xSemaphoreCreateMutexStatic);
        RESET_FAKE(xTaskCreateStatic);
        RESET_FAKE(AtResponseQueue_Deinit);
        RESET_FAKE(Hardware_Deinitialize);

        xSemaphoreCreateBinaryStatic_fake.custom_fake = Custom_xSemaphoreCreateBinaryStatic;
        xSemaphoreCreateMutexStatic_fake.custom_fake = Custom_xSemaphoreCreateMutexStatic;
        xTaskCreateStatic_fake.custom_fake = Custom_xTaskCreateStatic;

        OnStateChanged = NULL;
        AtResponseParser_RxWakeupHandle = NULL;
        CellularDriver_TxWakeupHandle = NULL;
        CellularDriver_RequestLock = NULL;
        AtResponseParser_TaskHandle = NULL;
        CellularDriver_TaskHandle = NULL;
        State = CELLULAR_STATE_MAX;
        EchoModeEnabled = false;
    }
};

TEST_F(TS_Engine_Initialize, Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Cellular_StateChanged_T excpStateChanged = (Cellular_StateChanged_T)123;

    rc = Engine_Initialize(excpStateChanged);

    EXPECT_EQ(RETCODE_OK, rc);

    EXPECT_EQ(1U, RingBuffer_Initialize_fake.call_count);
    EXPECT_EQ(&UartRxBufDescr, RingBuffer_Initialize_fake.arg0_val);

    EXPECT_EQ(2U, xSemaphoreCreateBinaryStatic_fake.call_count);
    EXPECT_EQ(&AtResponseParser_RxWakeupBuffer, AtResponseParser_RxWakeupHandle);
    EXPECT_EQ(&CellularDriver_TxWakeupBuffer, CellularDriver_TxWakeupHandle);

    EXPECT_EQ(1U, Hardware_Initialize_fake.call_count);
    EXPECT_EQ((void *)HandleMcuIsrCallback, Hardware_Initialize_fake.arg0_val);
    EXPECT_EQ(&UartRxByte, Hardware_Initialize_fake.arg1_val);
    EXPECT_EQ(1U, Hardware_GetCommunicationChannel_fake.call_count);
    EXPECT_EQ(&CellularSerialDevice, Hardware_GetCommunicationChannel_fake.arg0_val);

    EXPECT_EQ(1U, AtResponseQueue_Init_fake.call_count);

    EXPECT_EQ(1U, AtResponseQueue_RegisterWithResponseParser_fake.call_count);

    EXPECT_EQ(1U, xSemaphoreCreateMutexStatic_fake.call_count);
    EXPECT_EQ(&CellularDriver_RequestBuffer, CellularDriver_RequestLock);

    EXPECT_EQ(2U, xTaskCreateStatic_fake.call_count);
    EXPECT_EQ((void *)AtResponseParser_Task, xTaskCreateStatic_fake.arg0_history[0]);
    EXPECT_EQ((void *)AtResponseParser_Task, AtResponseParser_TaskHandle);
    EXPECT_EQ((void *)CellularDriver_Task, xTaskCreateStatic_fake.arg0_history[1]);
    EXPECT_EQ((void *)CellularDriver_Task, CellularDriver_TaskHandle);

    EXPECT_EQ(excpStateChanged, OnStateChanged);
    EXPECT_EQ(CELLULAR_STATE_POWEROFF, State);
    EXPECT_TRUE(EchoModeEnabled);
}

TEST_F(TS_Engine_Initialize, StateChangedParamNull_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Cellular_StateChanged_T excpStateChanged = NULL;

    rc = Engine_Initialize(excpStateChanged);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    EXPECT_EQ(0U, RingBuffer_Initialize_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreCreateBinaryStatic_fake.call_count);
    EXPECT_EQ(0U, Hardware_Initialize_fake.call_count);
    EXPECT_EQ(0U, Hardware_GetCommunicationChannel_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_Init_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_RegisterWithResponseParser_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreCreateMutexStatic_fake.call_count);
    EXPECT_EQ(0U, xTaskCreateStatic_fake.call_count);

    EXPECT_EQ(excpStateChanged, OnStateChanged);
    EXPECT_EQ(CELLULAR_STATE_MAX, State);
    EXPECT_FALSE(EchoModeEnabled);
}

TEST_F(TS_Engine_Initialize, HwInit_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Cellular_StateChanged_T excpStateChanged = (Cellular_StateChanged_T)123;
    Hardware_Initialize_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    rc = Engine_Initialize(excpStateChanged);

    EXPECT_EQ(Hardware_Initialize_fake.return_val, rc);

    EXPECT_EQ(1U, RingBuffer_Initialize_fake.call_count);
    EXPECT_EQ(2U, xSemaphoreCreateBinaryStatic_fake.call_count);
    EXPECT_EQ(1U, Hardware_Initialize_fake.call_count);

    EXPECT_EQ(0U, Hardware_GetCommunicationChannel_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_Init_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_RegisterWithResponseParser_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreCreateMutexStatic_fake.call_count);
    EXPECT_EQ(0U, xTaskCreateStatic_fake.call_count);

    EXPECT_EQ(NULL, OnStateChanged);
    EXPECT_EQ(CELLULAR_STATE_MAX, State);
    EXPECT_FALSE(EchoModeEnabled);
}

TEST_F(TS_Engine_Initialize, HwComInit_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Cellular_StateChanged_T excpStateChanged = (Cellular_StateChanged_T)123;
    Hardware_GetCommunicationChannel_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    rc = Engine_Initialize(excpStateChanged);

    EXPECT_EQ(Hardware_GetCommunicationChannel_fake.return_val, rc);

    EXPECT_EQ(1U, RingBuffer_Initialize_fake.call_count);
    EXPECT_EQ(2U, xSemaphoreCreateBinaryStatic_fake.call_count);
    EXPECT_EQ(1U, Hardware_Initialize_fake.call_count);
    EXPECT_EQ(1U, Hardware_GetCommunicationChannel_fake.call_count);

    EXPECT_EQ(0U, AtResponseQueue_Init_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_RegisterWithResponseParser_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreCreateMutexStatic_fake.call_count);
    EXPECT_EQ(0U, xTaskCreateStatic_fake.call_count);

    EXPECT_EQ(NULL, OnStateChanged);
    EXPECT_EQ(CELLULAR_STATE_MAX, State);
    EXPECT_FALSE(EchoModeEnabled);
}

TEST_F(TS_Engine_Initialize, AtQueueInit_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Cellular_StateChanged_T excpStateChanged = (Cellular_StateChanged_T)123;
    AtResponseQueue_Init_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    rc = Engine_Initialize(excpStateChanged);

    EXPECT_EQ(AtResponseQueue_Init_fake.return_val, rc);

    EXPECT_EQ(1U, RingBuffer_Initialize_fake.call_count);
    EXPECT_EQ(2U, xSemaphoreCreateBinaryStatic_fake.call_count);
    EXPECT_EQ(1U, Hardware_Initialize_fake.call_count);
    EXPECT_EQ(1U, Hardware_GetCommunicationChannel_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_Init_fake.call_count);

    EXPECT_EQ(0U, AtResponseQueue_RegisterWithResponseParser_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreCreateMutexStatic_fake.call_count);
    EXPECT_EQ(0U, xTaskCreateStatic_fake.call_count);

    EXPECT_EQ(NULL, OnStateChanged);
    EXPECT_EQ(CELLULAR_STATE_MAX, State);
    EXPECT_FALSE(EchoModeEnabled);
}

TEST_F(TS_Engine_Initialize, Engine_Deinitialize_Sucess)
{
    Retcode_T rc = Engine_Deinitialize();
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Engine_Initialize, Engine_Deinitialize_AtQueueDeinit_Fail)
{
    AtResponseQueue_Deinit_fake.return_val = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_QUEUE_ERROR);
    Retcode_T rc = Engine_Deinitialize();
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_QUEUE_ERROR), rc);
}

TEST_F(TS_Engine_Initialize, Engine_Deinitialize_HardwareDeinitialize_Fail)
{
    Hardware_Deinitialize_fake.return_val = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_QUEUE_ERROR);
    Retcode_T rc = Engine_Deinitialize();
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_QUEUE_ERROR), rc);
}

class TS_Engine_EchoModeEnabled : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        EchoModeEnabled = false;
    }
};

TEST_F(TS_Engine_EchoModeEnabled, Success)
{
    bool expEchoMode = true;

    Engine_EchoModeEnabled(expEchoMode);

    EXPECT_EQ(expEchoMode, EchoModeEnabled);
}

class TS_Engine_SendAtCommand : public testing::Test
{
private:
    std::array<BaseType_t, 2> takeRetVals = {{pdFAIL, pdPASS}};

protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(xSemaphoreTake);
        RESET_FAKE(MCU_UART_Send);
        RESET_FAKE(AtResponseQueue_GetEvent);
        RESET_FAKE(Urc_HandleResponses);

        SET_RETURN_SEQ(xSemaphoreTake, takeRetVals.data(), takeRetVals.size());

        CellularSerialDevice = (UART_T)123;
        CellularDriver_TxWakeupHandle = (SemaphoreHandle_t)&CellularDriver_TxWakeupBuffer;
    }
};

TEST_F(TS_Engine_SendAtCommand, Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint8_t buffer[128];

    rc = Engine_SendAtCommand(buffer, sizeof(buffer));

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(2U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[0]);
    EXPECT_EQ(0U, xSemaphoreTake_fake.arg1_history[0]);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[1]);
    EXPECT_EQ(CELLULAR_SEND_AT_COMMAND_WAIT_TIME, xSemaphoreTake_fake.arg1_history[1]);
    EXPECT_EQ(1U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(buffer, MCU_UART_Send_fake.arg1_val);
    EXPECT_EQ(sizeof(buffer), MCU_UART_Send_fake.arg2_val);
    EXPECT_EQ(1U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(1U, Urc_HandleResponses_fake.call_count);
}

TEST_F(TS_Engine_SendAtCommand, BufferNull_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    rc = Engine_SendAtCommand(NULL, 123);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    EXPECT_EQ(0U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(0U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(0U, Urc_HandleResponses_fake.call_count);
}

TEST_F(TS_Engine_SendAtCommand, ComDeviceNonInit_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint8_t buffer[128];
    CellularSerialDevice = (UART_T)0;

    rc = Engine_SendAtCommand(buffer, sizeof(buffer));

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
    EXPECT_EQ(0U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(0U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(0U, Urc_HandleResponses_fake.call_count);
}

TEST_F(TS_Engine_SendAtCommand, Send_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint8_t buffer[128];

    MCU_UART_Send_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    rc = Engine_SendAtCommand(buffer, sizeof(buffer));

    EXPECT_EQ(MCU_UART_Send_fake.return_val, rc);
    EXPECT_EQ(1U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[0]);
    EXPECT_EQ(0U, xSemaphoreTake_fake.arg1_history[0]);
    EXPECT_EQ(1U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(buffer, MCU_UART_Send_fake.arg1_val);
    EXPECT_EQ(sizeof(buffer), MCU_UART_Send_fake.arg2_val);
    EXPECT_EQ(0U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(0U, Urc_HandleResponses_fake.call_count);
}

TEST_F(TS_Engine_SendAtCommand, TimeoutAfterSend_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint8_t buffer[128];

    // TODO: Use takeRetVals in the fixture
    std::array<BaseType_t, 2> retVals = {pdFAIL, pdFAIL};
    SET_RETURN_SEQ(xSemaphoreTake, retVals.data(), retVals.size());

    rc = Engine_SendAtCommand(buffer, sizeof(buffer));

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR), rc);
    EXPECT_EQ(2U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[0]);
    EXPECT_EQ(0U, xSemaphoreTake_fake.arg1_history[0]);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[1]);
    EXPECT_EQ(CELLULAR_SEND_AT_COMMAND_WAIT_TIME, xSemaphoreTake_fake.arg1_history[1]);
    EXPECT_EQ(1U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(buffer, MCU_UART_Send_fake.arg1_val);
    EXPECT_EQ(sizeof(buffer), MCU_UART_Send_fake.arg2_val);
    EXPECT_EQ(0U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(0U, Urc_HandleResponses_fake.call_count);
}

class TS_Engine_SetFlukeCharFilterEnabled : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        IsFlukeFilterEnabled = false;
    }
};

TEST_F(TS_Engine_SetFlukeCharFilterEnabled, Success)
{
    bool expFlukeFilterEnabled = true;

    Engine_SetFlukeCharFilterEnabled(expFlukeFilterEnabled);

    EXPECT_EQ(expFlukeFilterEnabled, IsFlukeFilterEnabled);
}

class TS_Engine_SendAtCommandWaitEcho : public TS_Engine_SendAtCommand
{
protected:
    virtual void SetUp() override
    {
        TS_Engine_SendAtCommand::SetUp();

        RESET_FAKE(AtResponseQueue_WaitForNamedCmdEcho);

        EchoModeEnabled = true;
    }
};

TEST_F(TS_Engine_SendAtCommandWaitEcho, EchoEnabled_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint8_t buffer[128];
    size_t footerLen = strlen(ENGINE_ATCMD_FOOTER);
    for (size_t i = 0; i < sizeof(buffer) - 1 - footerLen; ++i)
    {
        buffer[i] = (uint8_t)((rand() % ('Z' - 'A')) + 'A'); // generate some random A-Z string
    }
    memcpy(buffer + sizeof(buffer) - 1 - footerLen, ENGINE_ATCMD_FOOTER, footerLen);
    buffer[sizeof(buffer) - 1] = '\0';
    uint32_t expTimeout = rand();
    EchoModeEnabled = true;

    rc = Engine_SendAtCommandWaitEcho(buffer, strlen((const char *)buffer), expTimeout);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(2U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[0]);
    EXPECT_EQ(0U, xSemaphoreTake_fake.arg1_history[0]);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[1]);
    EXPECT_EQ(CELLULAR_SEND_AT_COMMAND_WAIT_TIME, xSemaphoreTake_fake.arg1_history[1]);
    EXPECT_EQ(1U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(buffer, MCU_UART_Send_fake.arg1_val);
    EXPECT_EQ(strlen((const char *)buffer), MCU_UART_Send_fake.arg2_val);
    EXPECT_EQ(1U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(1U, Urc_HandleResponses_fake.call_count);
    EXPECT_EQ(1U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(expTimeout, AtResponseQueue_WaitForNamedCmdEcho_fake.arg0_val);
    EXPECT_STREQ((const char *)buffer, (const char *)AtResponseQueue_WaitForNamedCmdEcho_fake.arg1_val);
    EXPECT_EQ(strlen((const char *)buffer) - footerLen, AtResponseQueue_WaitForNamedCmdEcho_fake.arg2_val);
}

TEST_F(TS_Engine_SendAtCommandWaitEcho, EchoDisabled_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint8_t buffer[128];
    size_t footerLen = strlen(ENGINE_ATCMD_FOOTER);
    for (size_t i = 0; i < sizeof(buffer) - 1 - footerLen; ++i)
    {
        buffer[i] = (uint8_t)((rand() % ('Z' - 'A')) + 'A'); // generate some random A-Z string
    }
    memcpy(buffer + sizeof(buffer) - 1 - footerLen, ENGINE_ATCMD_FOOTER, footerLen);
    buffer[sizeof(buffer) - 1] = '\0';
    uint32_t expTimeout = rand();
    EchoModeEnabled = false;

    rc = Engine_SendAtCommandWaitEcho(buffer, strlen((const char *)buffer), expTimeout);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(2U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[0]);
    EXPECT_EQ(0U, xSemaphoreTake_fake.arg1_history[0]);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreTake_fake.arg0_history[1]);
    EXPECT_EQ(CELLULAR_SEND_AT_COMMAND_WAIT_TIME, xSemaphoreTake_fake.arg1_history[1]);
    EXPECT_EQ(1U, MCU_UART_Send_fake.call_count);
    EXPECT_EQ(buffer, MCU_UART_Send_fake.arg1_val);
    EXPECT_EQ(strlen((const char *)buffer), MCU_UART_Send_fake.arg2_val);
    EXPECT_EQ(1U, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(1U, Urc_HandleResponses_fake.call_count);
    EXPECT_EQ(0U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
}

TEST_F(TS_Engine_SendAtCommandWaitEcho, EngineSendAtCommand_Fail)
{
    Retcode_T rc = RETCODE_OK;
    uint8_t *buffer = NULL;
    uint32_t expTimeout = rand();
    EchoModeEnabled = false;

    rc = Engine_SendAtCommandWaitEcho(buffer, 0, expTimeout);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

FAKE_VOID_FUNC(TS_Engine_NotifyNewState_DummyCallback, Cellular_State_T, Cellular_State_T, void *, uint32_t)

class TS_Engine_NotifyNewState : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(TS_Engine_NotifyNewState_DummyCallback)

        OnStateChanged = TS_Engine_NotifyNewState_DummyCallback;
        State = CELLULAR_STATE_MAX;
    }
};

TEST_F(TS_Engine_NotifyNewState, NewState_Success)
{
    Cellular_State_T expState = CELLULAR_STATE_POWERON;
    char expParam[] = "Hello World!";

    Engine_NotifyNewState(expState, (void *)expParam, strlen(expParam));

    EXPECT_EQ(1U, TS_Engine_NotifyNewState_DummyCallback_fake.call_count);
    EXPECT_EQ(CELLULAR_STATE_MAX, TS_Engine_NotifyNewState_DummyCallback_fake.arg0_val);
    EXPECT_EQ(expState, TS_Engine_NotifyNewState_DummyCallback_fake.arg1_val);
    EXPECT_STREQ(expParam, (const char *)TS_Engine_NotifyNewState_DummyCallback_fake.arg2_val);
    EXPECT_EQ(strlen(expParam), TS_Engine_NotifyNewState_DummyCallback_fake.arg3_val);
    EXPECT_EQ(expState, State);
}

TEST_F(TS_Engine_NotifyNewState, SameState_Success)
{
    Cellular_State_T expState = CELLULAR_STATE_POWERON;
    char expParam[] = "Hello World!";
    State = expState;

    Engine_NotifyNewState(expState, (void *)expParam, strlen(expParam));

    EXPECT_EQ(0U, TS_Engine_NotifyNewState_DummyCallback_fake.call_count);
    EXPECT_EQ(expState, State);
}

FAKE_VALUE_FUNC(Retcode_T, TS_Engine_Dispatch_DummyCallback, void *, uint32_t)

class TS_Engine_Dispatch : public testing::Test
{
protected:
    virtual void SetUp()
    {
        srand(time(NULL));

        FFF_RESET_HISTORY();

        RESET_FAKE(xSemaphoreTake);
        RESET_FAKE(TS_Engine_Dispatch_DummyCallback);
        RESET_FAKE(xSemaphoreGive);

        xSemaphoreTake_fake.return_val = pdPASS;

        CellularDriver_RequestLock = (SemaphoreHandle_t)123;
    }
};

TEST_F(TS_Engine_Dispatch, Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint32_t expTimeout = rand();
    char expParam[] = "Hello World!";

    rc = Engine_Dispatch(TS_Engine_Dispatch_DummyCallback, expTimeout, (void *)expParam, strlen(expParam));

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(1U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(1U, TS_Engine_Dispatch_DummyCallback_fake.call_count);
    EXPECT_EQ(1U, xSemaphoreGive_fake.call_count);
}

TEST_F(TS_Engine_Dispatch, InvalidFunc_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint32_t expTimeout = rand();
    char expParam[] = "Hello World!";

    rc = Engine_Dispatch(NULL, expTimeout, (void *)expParam, strlen(expParam));

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
    EXPECT_EQ(0U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(0U, TS_Engine_Dispatch_DummyCallback_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreGive_fake.call_count);
}

TEST_F(TS_Engine_Dispatch, LockNotInit_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint32_t expTimeout = rand();
    char expParam[] = "Hello World!";

    CellularDriver_RequestLock = NULL;

    rc = Engine_Dispatch(TS_Engine_Dispatch_DummyCallback, expTimeout, (void *)expParam, strlen(expParam));

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_UNINITIALIZED), rc);
    EXPECT_EQ(0U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(0U, TS_Engine_Dispatch_DummyCallback_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreGive_fake.call_count);
}

TEST_F(TS_Engine_Dispatch, LockTimeout_Failure)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    uint32_t expTimeout = rand();
    char expParam[] = "Hello World!";

    xSemaphoreTake_fake.return_val = pdFAIL;

    rc = Engine_Dispatch(TS_Engine_Dispatch_DummyCallback, expTimeout, (void *)expParam, strlen(expParam));

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_DRIVER_BUSY), rc);
    EXPECT_EQ(1U, xSemaphoreTake_fake.call_count);
    EXPECT_EQ(0U, TS_Engine_Dispatch_DummyCallback_fake.call_count);
    EXPECT_EQ(0U, xSemaphoreGive_fake.call_count);
}

std::array<AtResponseQueueEntry_T *, 7> TS_SkipEventsUntilCommand_GetEventVals;
size_t TS_SkipEventsUntilCommand_GetEventVals_Index = 0;

void TS_SkipEventsUntilCommand_DummyMarkUnused(void)
{
    TS_SkipEventsUntilCommand_GetEventVals_Index = (TS_SkipEventsUntilCommand_GetEventVals_Index + 1) % TS_SkipEventsUntilCommand_GetEventVals.size();
}

Retcode_T TS_SkipEventsUntilCommand_DummyGetEvent(uint32_t, AtResponseQueueEntry_T **entry)
{
    *entry = TS_SkipEventsUntilCommand_GetEventVals[TS_SkipEventsUntilCommand_GetEventVals_Index];

    return RETCODE_OK;
}

class TS_SkipEventsUntilCommand : public testing::Test
{
protected:
    void CreateQueueEntry(AtResponseQueueEntry_T **entry, AtEventType_T type, AtResponseCode_T code, size_t len)
    {
        if (AT_EVENT_TYPE_OUT_OF_RANGE < type)
        {
            std::cerr << "Error during test-setup: Invalid EventType '" << type << "'" << std::endl;
            exit(1);
        }

        *entry = (AtResponseQueueEntry_T *)malloc(sizeof(AtResponseQueueEntry_T) + len + 1);
        for (size_t i = 0; i < len; ++i)
        {
            (*entry)->Buffer[i] = (char)((rand() % ('Z' - 'A')) + 'A'); // generate some random A-Z string
        }
        (*entry)->Buffer[len] = '\0';
        (*entry)->Type = type;
        (*entry)->ResponseCode = code;
        (*entry)->BufferLength = len;
    }
    virtual void SetUp()
    {
        srand(time(NULL));
        FFF_RESET_HISTORY();

        RESET_FAKE(AtResponseQueue_GetEventCount);
        RESET_FAKE(Urc_HandleResponses);
        RESET_FAKE(AtResponseQueue_GetEvent);
        RESET_FAKE(AtResponseQueue_MarkBufferAsUnused);

        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[0], AT_EVENT_TYPE_COMMAND_ECHO, (AtResponseCode_T)666, std::min(rand() % 16, 2));
        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[1], AT_EVENT_TYPE_COMMAND_ARG, (AtResponseCode_T)667, std::min(rand() % 16, 2));
        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[2], AT_EVENT_TYPE_ERROR, (AtResponseCode_T)669, 0);
        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[3], AT_EVENT_TYPE_MISC, (AtResponseCode_T)670, std::min(rand() % 32, 2));
        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[4], AT_EVENT_TYPE_RESPONSE_CODE, AT_RESPONSE_CODE_OK, 0);
        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[5], AT_EVENT_TYPE_OUT_OF_RANGE, (AtResponseCode_T)668, std::min(rand() % 16, 2));
        CreateQueueEntry(&TS_SkipEventsUntilCommand_GetEventVals[6], AT_EVENT_TYPE_COMMAND, (AtResponseCode_T)668, std::min(rand() % 16, 2));

        TS_SkipEventsUntilCommand_GetEventVals_Index = 0;

        AtResponseQueue_GetEventCount_fake.return_val = TS_SkipEventsUntilCommand_GetEventVals.size();
        AtResponseQueue_GetEvent_fake.custom_fake = TS_SkipEventsUntilCommand_DummyGetEvent;
        AtResponseQueue_MarkBufferAsUnused_fake.custom_fake = TS_SkipEventsUntilCommand_DummyMarkUnused;
        Urc_HandleResponses_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_CELLULAR_URC_NOT_PRESENT);
    }
    virtual void TearDown()
    {
        for (auto *val : TS_SkipEventsUntilCommand_GetEventVals)
        {
            free(val);
            val = nullptr;
        }
    }
};

TEST_F(TS_SkipEventsUntilCommand, Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    rc = SkipEventsUntilCommand();

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(AtResponseQueue_GetEventCount_fake.return_val, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(AtResponseQueue_GetEventCount_fake.return_val - 1, AtResponseQueue_MarkBufferAsUnused_fake.call_count);
}

TEST_F(TS_SkipEventsUntilCommand, AtResonsequeue_Timeout)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    AtResponseQueue_GetEvent_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_TIMEOUT);

    rc = SkipEventsUntilCommand();

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(AtResponseQueue_GetEventCount_fake.return_val, AtResponseQueue_GetEvent_fake.call_count);
    EXPECT_EQ(AtResponseQueue_GetEventCount_fake.return_val - 1, AtResponseQueue_MarkBufferAsUnused_fake.call_count);
}

class TS_HandleMcuIsrCallback : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(Retcode_RaiseErrorFromIsr);
        RESET_FAKE(xSemaphoreGiveFromISR);
        RESET_FAKE(RingBuffer_Write);

        xSemaphoreGiveFromISR_fake.return_val = pdPASS;
        RingBuffer_Write_fake.return_val = 1U;
    }
};

TEST_F(TS_HandleMcuIsrCallback, Tx_Success)
{
    struct MCU_UART_Event_S event;
    memset(&event, 0U, sizeof(event));
    event.TxComplete = 1;
    UART_T uart = (UART_T)1;

    HandleMcuIsrCallback(uart, event);

    EXPECT_EQ(1U, xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(CellularDriver_TxWakeupHandle, xSemaphoreGiveFromISR_fake.arg0_val);
    EXPECT_EQ(0U, RingBuffer_Write_fake.call_count);
    EXPECT_EQ(0U, Retcode_RaiseErrorFromIsr_fake.call_count);
}

TEST_F(TS_HandleMcuIsrCallback, RxS4Char_Success)
{
    struct MCU_UART_Event_S event;
    memset(&event, 0U, sizeof(event));
    event.RxComplete = 1;
    UART_T uart = (UART_T)1;
    UartRxByte = AT_DEFAULT_S4_CHARACTER;

    HandleMcuIsrCallback(uart, event);

    EXPECT_EQ(1U, xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(AtResponseParser_RxWakeupHandle, xSemaphoreGiveFromISR_fake.arg0_val);
    EXPECT_EQ(1U, RingBuffer_Write_fake.call_count);
    EXPECT_EQ(1U, RingBuffer_Write_fake.arg2_val);
    EXPECT_EQ(0U, Retcode_RaiseErrorFromIsr_fake.call_count);
}

TEST_F(TS_HandleMcuIsrCallback, RxNonS4Char_Success)
{
    struct MCU_UART_Event_S event;
    memset(&event, 0U, sizeof(event));
    event.RxComplete = 1;
    UART_T uart = (UART_T)1;
    UartRxByte = '0';

    HandleMcuIsrCallback(uart, event);

    EXPECT_EQ(0U, xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(1U, RingBuffer_Write_fake.call_count);
    EXPECT_EQ(1U, RingBuffer_Write_fake.arg2_val);
    EXPECT_EQ(0U, Retcode_RaiseErrorFromIsr_fake.call_count);
}

TEST_F(TS_HandleMcuIsrCallback, Ringbuffer_Write_Fail)
{
    struct MCU_UART_Event_S event;
    memset(&event, 0U, sizeof(event));
    event.RxComplete = 1;
    UART_T uart = (UART_T)1;
    UartRxByte = '0';

    RingBuffer_Write_fake.return_val = 0;

    HandleMcuIsrCallback(uart, event);

    EXPECT_EQ(0U, xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(1U, RingBuffer_Write_fake.call_count);
    EXPECT_EQ(1U, RingBuffer_Write_fake.arg2_val);
    EXPECT_EQ(1U, Retcode_RaiseErrorFromIsr_fake.call_count);
}
