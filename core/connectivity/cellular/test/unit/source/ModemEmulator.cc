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

/** LCOV_EXCL_START : start of section where code coverage is ignored. */

#include "Kiso_Basics.h"

#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#include "FreeRTOS_th.hh"
#include "semphr_th.hh"
#include "task_th.hh"

#include "Engine_th.hh"
#include "Queue_th.hh"

#include "AtResponseParser.h"

#undef KISO_MODULE_ID
#include "AtResponseParser.c"
#undef KISO_MODULE_ID
#include "AtResponseQueue.c"

#include <stdarg.h>
#include <stdio.h>
#include <gtest.h>

#ifndef TEST_VERBOSE_MODEM_EMULATOR
#define TEST_VERBOSE_MODEM_EMULATOR 0
#endif

struct FakeAnswers_S
{
    const char *Trigger;
    const char *Answer;
    struct FakeAnswers_S *_Next;
};

static struct FakeAnswers_S *RootAnswer = NULL;
static struct FakeAnswers_S *CurrentAnswer = RootAnswer;

bool ModemEmulator_EnableEcho = true;

/* *** TEST HELPER ********************************************************** */
#if TEST_VERBOSE_MODEM_EMULATOR

void _printf_mask_cr_lf_(const char *fmt, ...)
{
    /* Make this buffer large enough to potentially  hold any AT echo/command*/
    char buffer[CELLULAR_AT_SEND_BUFFER_SIZE * 2];
    va_list args;
    va_start(args, fmt);
    int len = vsprintf(buffer, fmt, args);
    va_end(args);

    if (len > (int)sizeof(buffer))
        exit(1);

    for (int i = 0; i < len; i++)
    {
        if (buffer[i] == '\r')
        {
            printf("<CR>");
        }
        else if (buffer[i] == '\n')
        {
            printf("<LF>");
        }
        else
        {
            putchar(buffer[i]);
        }
    }
}

#define TEST_PRINTF(fmt, ...)                  \
    do                                         \
    {                                          \
        printf("\tMODEM-EMU|%s|", __func__);   \
        _printf_mask_cr_lf_(fmt, __VA_ARGS__); \
        puts("");                              \
    } while (0)

#else

#define TEST_PRINTF(fmt, ...) \
    do                        \
    {                         \
    } while (0)

#endif

/* *** FAKE ENGINE IMPLEMENTATION ******************************************* */
Retcode_T Custom_Engine_SendAtCommand(const uint8_t *buffer, uint32_t bufferLength)
{
    Retcode_T retcode = RETCODE_OK;

    if (ModemEmulator_EnableEcho)
    {
        /* Echo */
        TEST_PRINTF("Echo: %.*s", bufferLength, buffer);
        (void)AtResponseParser_Parse((const uint8_t *)buffer, bufferLength);
    }

    if (NULL != CurrentAnswer)
    {
        if (0 == strncmp(CurrentAnswer->Trigger, (const char *)buffer, bufferLength))
        {
            TEST_PRINTF("Answer: %s", CurrentAnswer->Answer);
            (void)AtResponseParser_Parse((const uint8_t *)CurrentAnswer->Answer, strlen(CurrentAnswer->Answer));
        }
        CurrentAnswer = CurrentAnswer->_Next;
    }

    return retcode;
}

Retcode_T Custom_Engine_SendAtCommandWaitEcho(const uint8_t *buffer, uint32_t bufferLength, uint32_t timeout)
{
    if (NULL == buffer)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Retcode_T retcode = Custom_Engine_SendAtCommand(buffer, bufferLength);
    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    if (ModemEmulator_EnableEcho)
    {
        return AtResponseQueue_WaitForNamedCmdEcho(timeout, buffer, bufferLength - strlen(ENGINE_ATCMD_FOOTER));
    }
    else
    {
        return RETCODE_OK;
    }
}

/* *** FAKE QUEUE IMPLEMENTATION ******************************************** */
Retcode_T Custom_Queue_Create(Queue_T *Queue, uint8_t *Buffer, uint32_t BufferSize)
{
    if (NULL == Queue || NULL == Buffer || 0 == BufferSize)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Queue->Buffer = Buffer;
    Queue->BufferSize = BufferSize;
    Queue->PosRead = Queue->PosWrite = Buffer;
    Queue->Count = 0;
    Queue->Last = NULL;

    return RETCODE_OK;
}

Retcode_T Custom_Queue_Delete(Queue_T *Queue)
{
    if (NULL == Queue)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    Queue->Lock = Queue->Wakeup = (xSemaphoreHandle)NULL;

    return RETCODE_OK;
}

Retcode_T Custom_Queue_Put(Queue_T *Queue, const void *Item, uint32_t ItemSize, const void *payload, uint32_t PayloadSize)
{
    QueueItem_T m;
    m.Next = NULL;
    m.Size = ItemSize + PayloadSize;

    uint32_t TotalSize = m.Size + sizeof(QueueItem_T);
    uint8_t *pos;

    if ((Queue->PosWrite > Queue->PosRead) || 0 == Queue->Count)
    {
        if (TotalSize <= (Queue->BufferSize - (uint32_t)(Queue->PosWrite - Queue->Buffer)))
        {
            /* Write to back */
            pos = Queue->PosWrite;
        }
        else if (TotalSize <= (uint32_t)(Queue->PosRead - Queue->Buffer))
        {
            /* Write in front */
            pos = Queue->Buffer;
        }
        else
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }
    else if (Queue->PosWrite < Queue->PosRead && TotalSize <= (uint32_t)(Queue->PosRead - Queue->PosWrite))
    {
        pos = Queue->PosWrite;
    }
    else
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    /* Add a node */
    if (Queue->Last)
    {
        Queue->Last->Next = pos;
    }

    Queue->Last = (QueueItem_T *)pos;

    /* Add a message to queue */
    memcpy(pos, &m, sizeof(QueueItem_T));
    pos += sizeof(QueueItem_T);
    memcpy(pos, Item, ItemSize);
    pos += ItemSize;

    if (NULL != payload)
    {
        memcpy(pos, payload, PayloadSize);
        pos += PayloadSize;
    }

    Queue->PosWrite = pos;
    Queue->Count++;

    return RETCODE_OK;
}

Retcode_T Custom_Queue_Get(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    KISO_UNUSED(Timeout);
    if (0 == Queue->Count)
    {
        /* Insure that a message is available */
        if (0 == Queue->Count)
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
    }

    QueueItem_T *m = (QueueItem_T *)Queue->PosRead;
    *Data = Queue->PosRead + sizeof(QueueItem_T);
    if (DataSize)
    {
        *DataSize = m->Size;
    }

    return RETCODE_OK;
}

Retcode_T Custom_Queue_Purge(Queue_T *Queue)
{
    if (0 == Queue->Count)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    QueueItem_T *m = (QueueItem_T *)Queue->PosRead;
    Queue->PosRead = m->Next;
    Queue->Count--;

    if (NULL == Queue->PosRead)
    {
        Queue->PosRead = Queue->PosWrite = Queue->Buffer;
        Queue->Last = NULL;
    }

    return RETCODE_OK;
}

uint32_t Custom_Queue_Count(const Queue_T *Queue)
{
    return Queue->Count;
}

void Custom_Queue_Clear(Queue_T *Queue)
{
    Queue->PosRead = Queue->PosWrite = Queue->Buffer;
    Queue->Count = 0;
    Queue->Last = NULL;
}

/* *** SETUP/TEARDOWN HELPER ********************************************************* */

void ConnectFakeModem(void)
{
    ModemEmulator_EnableEcho = true;

    Engine_SendAtCommand_fake.custom_fake = Custom_Engine_SendAtCommand;
    Engine_SendAtCommandWaitEcho_fake.custom_fake = Custom_Engine_SendAtCommandWaitEcho;

    Queue_Clear_fake.custom_fake = Custom_Queue_Clear;
    Queue_Count_fake.custom_fake = Custom_Queue_Count;
    Queue_Create_fake.custom_fake = Custom_Queue_Create;
    Queue_Delete_fake.custom_fake = Custom_Queue_Delete;
    Queue_Get_fake.custom_fake = Custom_Queue_Get;
    Queue_Purge_fake.custom_fake = Custom_Queue_Purge;
    Queue_Put_fake.custom_fake = Custom_Queue_Put;

    AtResponseQueue_Init();
    AtResponseQueue_RegisterWithResponseParser();
    AtResponseParser_Reset();
}

void DeleteFakeAnswers(void)
{
    struct FakeAnswers_S *cur = RootAnswer;
    RootAnswer = NULL;
    CurrentAnswer = NULL;
    while (NULL != cur)
    {
        struct FakeAnswers_S *next = cur->_Next;
        free(cur);
        cur = next;
    }
}

void DisconnectFakeModem(void)
{
    Engine_SendAtCommand_fake.custom_fake = NULL;
    Engine_SendAtCommandWaitEcho_fake.custom_fake = NULL;

    Queue_Clear_fake.custom_fake = NULL;
    Queue_Count_fake.custom_fake = NULL;
    Queue_Create_fake.custom_fake = NULL;
    Queue_Delete_fake.custom_fake = NULL;
    Queue_Get_fake.custom_fake = NULL;
    Queue_Purge_fake.custom_fake = NULL;
    Queue_Put_fake.custom_fake = NULL;

    DeleteFakeAnswers();
}

void AddFakeAnswer(const char *trigger, const char *answer)
{
    struct FakeAnswers_S *newFakeAnswer = (struct FakeAnswers_S *)malloc(sizeof(struct FakeAnswers_S));
    if (NULL == newFakeAnswer)
    {
        exit(1);
    }

    newFakeAnswer->Trigger = trigger;
    newFakeAnswer->Answer = answer;
    newFakeAnswer->_Next = NULL;

    if (RootAnswer == NULL)
    {
        RootAnswer = newFakeAnswer;
        CurrentAnswer = newFakeAnswer;
    }
    else
    {
        struct FakeAnswers_S *cur = RootAnswer;
        while (NULL != cur->_Next)
        {
            cur = cur->_Next;
        }
        cur->_Next = newFakeAnswer;
    }
}

class TS_ModemTest : public testing::Test
{
protected:
    char *Trigger;
    char *Answer;

    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        ConnectFakeModem();

        Trigger = NULL;
        Answer = NULL;
    }

    virtual void TearDown()
    {
        DisconnectFakeModem();

        if (NULL != Trigger)
        {
            free(Trigger);
            Trigger = NULL;
        }

        if (NULL != Answer)
        {
            free(Answer);
            Answer = NULL;
        }
    }

    virtual const char *FormatIntoNewBuffer(char **buffer, const char *fmt, ...)
    {
        if (NULL != *buffer)
            exit(1);

        va_list args;
        va_start(args, fmt);
        int len = vsnprintf(NULL, 0, fmt, args);
        va_end(args);
        if (len <= 0)
            exit(1);

        va_start(args, fmt);
        *buffer = (char *)malloc(len + 1);
        if (NULL == *buffer)
            exit(1);
        vsnprintf(*buffer, len + 1, fmt, args);
        va_end(args);

        return *buffer;
    }
};

/** LCOV_EXCL_STOP : end of section where code coverage is ignored. */
