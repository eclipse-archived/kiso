/*******************************************************************************
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
 ******************************************************************************/

#include <gtest.h>

extern "C"
{
#include "Kiso_CellularModules.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_ATQUEUE

#include "AtResponseParser_th.hh"
#include "Queue_th.hh"

#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#include "Kiso_Logging_th.hh"

#undef KISO_MODULE_ID
#include "AtResponseQueue.c"
}

class TS_ATResponseQueue : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        RESET_FAKE(Queue_Get);
        RESET_FAKE(Queue_Delete);
    }
};

uint8_t globalBuffer[100];

Retcode_T Queue_Get_custom_emptyfunc(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    KISO_UNUSED(Queue);
    KISO_UNUSED(Timeout);
    KISO_UNUSED(DataSize);
    KISO_UNUSED(Data);
    *Data = malloc(sizeof(AtResponseQueueEntry_T));
    return RETCODE_OK;
}

Retcode_T Queue_Get_custom_echo(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    KISO_UNUSED(Queue);
    KISO_UNUSED(Timeout);
    KISO_UNUSED(DataSize);

    *Data = malloc(sizeof(AtResponseQueueEntry_T));
    AtResponseQueueEntry_T *data = (AtResponseQueueEntry_T *)*Data;
    data->Type = AT_EVENT_TYPE_COMMAND_ECHO;
    return RETCODE_OK;
}

Retcode_T Queue_Get_custom_cmd(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    KISO_UNUSED(Queue);
    KISO_UNUSED(Timeout);
    KISO_UNUSED(DataSize);

    *Data = malloc(sizeof(AtResponseQueueEntry_T));
    AtResponseQueueEntry_T *data = (AtResponseQueueEntry_T *)*Data;
    data->Type = AT_EVENT_TYPE_COMMAND;
    return RETCODE_OK;
}

Retcode_T Queue_Get_custom_cmdarg(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    KISO_UNUSED(Queue);
    KISO_UNUSED(Timeout);
    KISO_UNUSED(DataSize);
    *Data = malloc(sizeof(AtResponseQueueEntry_T) + sizeof(globalBuffer));
    AtResponseQueueEntry_T *data = (AtResponseQueueEntry_T *)*Data;
    data->Type = AT_EVENT_TYPE_COMMAND_ARG;
    data->BufferLength = 5;
    data->Buffer[0] = globalBuffer[0];
    data->Buffer[1] = globalBuffer[1];
    data->Buffer[2] = globalBuffer[2];
    data->Buffer[3] = globalBuffer[3];
    data->Buffer[4] = globalBuffer[4];
    return RETCODE_OK;
}

Retcode_T Queue_Get_custom_respcode(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    KISO_UNUSED(Queue);
    KISO_UNUSED(Timeout);
    KISO_UNUSED(DataSize);
    *Data = malloc(sizeof(AtResponseQueueEntry_T) + sizeof(globalBuffer));
    AtResponseQueueEntry_T *data = (AtResponseQueueEntry_T *)*Data;
    data->Type = AT_EVENT_TYPE_RESPONSE_CODE;
    data->BufferLength = 5;
    data->ResponseCode = AT_RESPONSE_CODE_OK;
    return RETCODE_OK;
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_Deinit_Success)
{
    Retcode_T retcode = AtResponseQueue_Init();
    EXPECT_EQ(RETCODE_OK, retcode);
    retcode = AtResponseQueue_Deinit();
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_Deinit_Fail)
{
    Retcode_T retcode = AtResponseQueue_Init();
    EXPECT_EQ(RETCODE_OK, retcode);
    Queue_Delete_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    retcode = AtResponseQueue_Deinit();
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_Reset_Success)
{
    AtResponseQueue_Reset();
    EXPECT_EQ(AtResponseParser_Reset_fake.call_count, 1U);
    EXPECT_EQ(Queue_Clear_fake.call_count, 1U);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_SetEventMask_Success)
{
    uint32_t eventMask = AT_EVENT_TYPE_OUT_OF_RANGE;
    Retcode_T retcode = AtResponseQueue_SetEventMask(eventMask);
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_GetEvent_Fail)
{
    uint32_t timeout = 10;
    AtResponseQueueEntry_T *entry;
    Queue_Get_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Retcode_T retcode = AtResponseQueue_GetEvent(timeout, &entry);
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_WaitForArbitraryCmdEcho_Success)
{
    uint32_t timeout = 10;
    uint8_t *bufferPtr;
    uint32_t bufferLen = 0;
    Queue_Get_fake.custom_fake = Queue_Get_custom_echo;
    Retcode_T retcode = AtResponseQueue_WaitForArbitraryCmdEcho(timeout, &bufferPtr, &bufferLen);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_WaitForArbitraryCmd_Success)
{
    uint32_t timeout = 10;
    uint8_t *bufferPtr;
    uint32_t bufferLen = 0;
    Queue_Get_fake.custom_fake = Queue_Get_custom_cmd;
    Retcode_T retcode = AtResponseQueue_WaitForArbitraryCmd(timeout, &bufferPtr, &bufferLen);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_WaitForNamedCmdArg_Success)
{
    uint32_t timeout = 10;
    uint32_t bufferLen = 5;
    Queue_Get_fake.custom_fake = Queue_Get_custom_cmdarg;
    Retcode_T retcode = AtResponseQueue_WaitForNamedCmdArg(timeout, globalBuffer, bufferLen);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_WaitForNamedResponseCode_Success)
{
    uint32_t timeout = 10;
    Queue_Get_fake.custom_fake = Queue_Get_custom_respcode;
    Retcode_T retcode = AtResponseQueue_WaitForNamedResponseCode(timeout, AT_RESPONSE_CODE_OK);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_WaitForNamedResponseCode_Fail1)
{
    uint32_t timeout = 10;
    Queue_Get_fake.custom_fake = Queue_Get_custom_emptyfunc;
    Retcode_T retcode = AtResponseQueue_WaitForNamedResponseCode(timeout, AT_RESPONSE_CODE_ERROR);
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_WaitForNamedResponseCode_Fail2)
{
    uint32_t timeout = 10;
    Queue_Get_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Retcode_T retcode = AtResponseQueue_WaitForNamedResponseCode(timeout, AT_RESPONSE_CODE_OK);
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_IgnoreEvent_Success)
{
    uint32_t timeout = 10;
    Queue_Get_fake.return_val = RETCODE_OK;
    ;
    Retcode_T retcode = AtResponseQueue_IgnoreEvent(timeout);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_ATResponseQueue, AtResponseQueue_IgnoreEvent_Fail)
{
    uint32_t timeout = 10;
    Queue_Get_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    ;
    Retcode_T retcode = AtResponseQueue_IgnoreEvent(timeout);
    EXPECT_NE(RETCODE_OK, retcode);
}
