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
 * @brief Implements the interface for message queue with variable size of elements.
 *
 */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_QUEUE

#include "BCDS_Queue.h"

#if BCDS_FEATURE_QUEUE

#include <string.h>
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"
#include "BCDS_Retcode.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"
#include "task.h"

Retcode_T Queue_Create(Queue_T *Queue, uint8_t *Buffer, uint32_t BufferSize)
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

    Queue->Lock = xSemaphoreCreateMutex();
    if (NULL == Queue->Lock)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    Queue->Wakeup = xSemaphoreCreateBinary();
    if (NULL == Queue->Wakeup)
    {
        vSemaphoreDelete(Queue->Lock);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    return RETCODE_OK;
}

Retcode_T Queue_Delete(Queue_T *Queue)
{
    if (NULL == Queue)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    assert(NULL != Queue->Lock && NULL != Queue->Wakeup);
    vSemaphoreDelete(Queue->Lock);
    vSemaphoreDelete(Queue->Wakeup);
    Queue->Lock = Queue->Wakeup = (xSemaphoreHandle) NULL;

    return RETCODE_OK;
}

Retcode_T Queue_Put(Queue_T *Queue, const void *Item, uint32_t ItemSize, const void *payload, uint32_t PayloadSize)
{
    assert(NULL != Queue && NULL != Item && 0 != ItemSize); /*lint -e613 -e668 */
    (void) xSemaphoreTake(Queue->Lock, portMAX_DELAY);

    QueueItem_T m;
    m.Next = NULL;
    m.Size = ItemSize + PayloadSize;

    uint32_t TotalSize = m.Size + sizeof(QueueItem_T);
    uint8_t *pos;

    if ((Queue->PosWrite > Queue->PosRead) || 0 == Queue->Count)
    {
        if (TotalSize <= (Queue->BufferSize - (uint32_t) (Queue->PosWrite - Queue->Buffer)))
        {
            /* Write to back */
            pos = Queue->PosWrite;
        }
        else if (TotalSize <= (uint32_t) (Queue->PosRead - Queue->Buffer))
        {
            /* Write in front */
            pos = Queue->Buffer;
        }
        else
        {
            (void) xSemaphoreGive(Queue->Lock);
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }
    else if (Queue->PosWrite < Queue->PosRead && TotalSize <= (uint32_t) (Queue->PosRead - Queue->PosWrite))
    {
        pos = Queue->PosWrite;
    }
    else
    {
        (void) xSemaphoreGive(Queue->Lock);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    /* Add a node */
    if (Queue->Last)
    {
        Queue->Last->Next = pos;
    }

    Queue->Last = (QueueItem_T*) pos;

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

    if (1 == Queue->Count)
    {
        /* Notify any waiting task */
        (void) xSemaphoreGive(Queue->Wakeup);
    }

    (void) xSemaphoreGive(Queue->Lock);
    return RETCODE_OK;
}

Retcode_T Queue_Get(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout)
{
    assert(NULL != Queue && NULL != Data);

    (void) xSemaphoreTake(Queue->Lock, portMAX_DELAY);
    if (0 == Queue->Count)
    {
        /* reset the wakeup */
        (void) xSemaphoreTake(Queue->Wakeup, 0);
        (void) xSemaphoreGive(Queue->Lock);
        /* wait for a message */
        if (pdPASS != xSemaphoreTake(Queue->Wakeup, Timeout))
        {
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        }
        (void) xSemaphoreTake(Queue->Lock, portMAX_DELAY);
        (void) xSemaphoreGive(Queue->Wakeup);
        /* Insure that a message is available */
        if (0 == Queue->Count)
        {
            (void) xSemaphoreGive(Queue->Lock);
            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
        }
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    QueueItem_T *m = (QueueItem_T*) Queue->PosRead;
    *Data = m->Data;
    if (DataSize)
    {
        *DataSize = m->Size;
    }

    (void) xSemaphoreGive(Queue->Lock);
    return RETCODE_OK;
}

Retcode_T Queue_Purge(Queue_T *Queue)
{
    assert(NULL != Queue);
    (void) xSemaphoreTake(Queue->Lock, portMAX_DELAY);

    if (0 == Queue->Count)
    {
        (void) xSemaphoreGive(Queue->Lock);
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    QueueItem_T *m = (QueueItem_T*) Queue->PosRead;
    Queue->PosRead = m->Next;
    Queue->Count--;

    if (NULL == Queue->PosRead)
    {
        Queue->PosRead = Queue->PosWrite = Queue->Buffer;
        Queue->Last = NULL;
    }

    (void) xSemaphoreGive(Queue->Lock);
    return RETCODE_OK;
}

uint32_t Queue_Count(const Queue_T *Queue)
{
    assert(NULL != Queue);
    return Queue->Count;
}

void Queue_Clear(Queue_T *Queue)
{
    assert(NULL != Queue);
    (void) xSemaphoreTake(Queue->Lock, portMAX_DELAY);
    Queue->PosRead = Queue->PosWrite = Queue->Buffer;
    Queue->Count = 0;
    Queue->Last = NULL;
    (void) xSemaphoreGive(Queue->Lock);
}

#endif /* if BCDS_FEATURE_QUEUE */
