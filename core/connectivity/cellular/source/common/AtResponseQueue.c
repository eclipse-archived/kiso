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

#include "BCDS_CellularModules.h"
#define BCDS_MODULE_ID BCDS_CELLULAR_MODULE_ID_ATQUEUE

#include "AtResponseParser.h"
#include "AtResponseQueue.h"

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"

#include "BCDS_Queue.h"
#include "BCDS_Logging.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "queue.h"
#include "task.h"

/* Response event queue with dedicated buffer */
static Queue_T EventQueue;
static uint8_t EventQueueBuffer[AT_RESPONSE_QUEUE_BUFFER_SIZE];

/*response queue event mask*/
static AtEventType_T AtEventMask=0;


static Retcode_T AtResponseQueue_WaitForEntry(uint32_t timeout, AtEventType_T EventType, AtResponseQueueEntry_T **entry)
{
    Retcode_T retcode = Queue_Get(&EventQueue, (void **) entry, NULL, timeout);

    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_AT_RESPONSE_QUEUE_TIMEOUT);
    }

    if (EventType == (*entry)->Type)
    {
        retcode = RETCODE_OK;
    }
    else if (AT_EVENT_TYPE_ERROR == (*entry)->Type)
    {
        (void) Queue_Purge(&EventQueue);
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT);
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT);
    }

    return retcode;
}

/**
 * @brief Waits for a specific event and copies its payload to the given buffer parameters
 *
 * @param[in] timeout The time to wait (in milliseconds) for an event to happen on the queue
 * @param[in] EventType The type of event to wait for
 * @param[out] BufferOutPtr The event uint8_tacter payload address
 * @param[out] BufferOutLen The event uint8_tacter payload length in bytes
 *
 * @return RETCODE_OK If an event of the required time was received in time
 * @return RETCODE_AT_RESPONSE_QUEUE_TIMEOUT No event was received in the given time
 * @return RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @return RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type
 */
static Retcode_T AtResponseQueue_WaitFor(uint32_t timeout, AtEventType_T EventType, uint8_t **BufferOutPtr, uint32_t *BufferOutLen)
{
    AtResponseQueueEntry_T *entry;
    Retcode_T retcode = AtResponseQueue_WaitForEntry(timeout, EventType, &entry);

    if (RETCODE_OK == retcode)
    {
        *BufferOutPtr = entry->Buffer;
        *BufferOutLen = entry->BufferLength;
    }
    return retcode;
}

/**
 * @brief Waits for a specific event with a specified content. This function takes care of marking the buffer
 * 		  as unused upon comparison.
 *
 * @param[in] timeout The time to wait for an event to happen on the queue
 * @param[in] EventType The type of event to wait for
 * @param[in] content The content to compare the event payload with
 *
 * @return RETCODE_OK If an event of the required time was received in time
 * @return RETCODE_AT_RESPONSE_QUEUE_TIMEOUT No event was received in the given time
 * @return RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT We received an error event ... please reset the event queue
 * @return RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT The event received was of the wrong type
 */
static Retcode_T AtResponseQueue_WaitForContent(uint32_t timeout, AtEventType_T EventType, const uint8_t *buffer, uint32_t BufferLength)
{
    AtResponseQueueEntry_T *entry;
    Retcode_T retcode = AtResponseQueue_WaitForEntry(timeout, EventType, &entry);

    /*
     *  Check if the entry received matches what we expect.
     *  The entry.BufferLength - 1 is because the BufferLength is the string length plus the byte for the zero termination.
     */
    if (RETCODE_OK == retcode)
    {
        if (BufferLength == entry->BufferLength
                && 0 == memcmp(entry->Buffer, buffer, entry->BufferLength))
        {
            (void) Queue_Purge(&EventQueue);
        }
        else
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE);
        }
    }

    return retcode;
}

void AtResponseQueue_EnqueueEvent(AtEventType_T EventType, uint8_t *arg, uint32_t len)
{
    AtResponseQueueEntry_T entry = {
            .Type = EventType,
            .ResponseCode = AT_RESPONSE_CODE_OK,
            .BufferLength = len
    };

    if (AtEventMask&EventType)
    {

        Retcode_T retcode = Queue_Put(&EventQueue, &entry, sizeof(entry), arg, len);

        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }
    }
}

static void AtResponseQueue_CallbackCmdEcho(uint8_t *cmd, uint32_t len)
{
    AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_COMMAND_ECHO, cmd, len);
}

static void AtResponseQueue_CallbackCmd(uint8_t *cmd, uint32_t len)
{
    AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_COMMAND, cmd, len);
}

static void AtResponseQueue_CallbackCmdArg(uint8_t *cmd, uint32_t len)
{
    AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_COMMAND_ARG, cmd, len);
}

static void AtResponseQueue_CallbackMiscContent(uint8_t *cmd, uint32_t len)
{
    AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_MISC, cmd, len);
}

static void AtResponseQueue_CallbackError(void)
{
    AtResponseQueue_Clear();
    AtResponseQueue_EnqueueEvent(AT_EVENT_TYPE_ERROR, NULL, 0);
}

static void AtResponseQueue_CallbackResponseCode(AtResponseCode_T response)
{
    AtResponseQueueEntry_T entry = {
            .Type = AT_EVENT_TYPE_RESPONSE_CODE,
            .ResponseCode = response
    };

    Retcode_T retcode = Queue_Put(&EventQueue, &entry, sizeof(entry), NULL, 0);

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
}

Retcode_T AtResponseQueue_Init(void)
{
	AtResponseQueue_SetEventMask(AT_EVENT_TYPE_ALL-AT_EVENT_TYPE_MISC);

    return Queue_Create(&EventQueue, EventQueueBuffer, sizeof(EventQueueBuffer));
}

void AtResponseQueue_RegisterWithResponseParser(void)
{
    AtResponseParser_RegisterResponseCodeCallback(AtResponseQueue_CallbackResponseCode);
    AtResponseParser_RegisterErrorCallback(AtResponseQueue_CallbackError);
    AtResponseParser_RegisterCmdEchoCallback(AtResponseQueue_CallbackCmdEcho);
    AtResponseParser_RegisterCmdCallback(AtResponseQueue_CallbackCmd);
    AtResponseParser_RegisterCmdArgCallback(AtResponseQueue_CallbackCmdArg);
    AtResponseParser_RegisterMiscCallback(AtResponseQueue_CallbackMiscContent);
}

Retcode_T AtResponseQueue_SetEventMask(uint32_t eventMask)
{
    Retcode_T retcode = RETCODE_OK;

    /* check if the mask is in the possible range*/
    if (eventMask<AT_EVENT_TYPE_OUT_OF_RANGE)
    {
        retcode = RETCODE_OK;
        AtEventMask=eventMask;
    }
    else
    {
        /* input value eventMask is out of range */
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    return retcode;

}

uint32_t AtResponseQueue_GetEventMask(uint32_t eventMask)
{
    return AtEventMask;
}

void AtResponseQueue_Reset(void)
{
    AtResponseParser_Reset();
    AtResponseQueue_Clear();
}

Retcode_T AtResponseQueue_WaitForNamedCmdEcho(uint32_t timeout, const uint8_t *buffer, uint32_t BufferLength)
{
    return AtResponseQueue_WaitForContent(timeout, AT_EVENT_TYPE_COMMAND_ECHO, buffer, BufferLength);
}

Retcode_T AtResponseQueue_WaitForArbitraryCmdEcho(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen)
{
    return AtResponseQueue_WaitFor(timeout, AT_EVENT_TYPE_COMMAND_ECHO, BufferPtr, BufferLen);
}

Retcode_T AtResponseQueue_WaitForNamedCmd(uint32_t timeout, const uint8_t *buffer, uint32_t BufferLength)
{
    return AtResponseQueue_WaitForContent(timeout, AT_EVENT_TYPE_COMMAND, buffer, BufferLength);
}

Retcode_T AtResponseQueue_WaitForArbitraryCmd(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen)
{
    return AtResponseQueue_WaitFor(timeout, AT_EVENT_TYPE_COMMAND, BufferPtr, BufferLen);
}

Retcode_T AtResponseQueue_WaitForNamedCmdArg(uint32_t timeout, const uint8_t *buffer, uint32_t BufferLength)
{
    return AtResponseQueue_WaitForContent(timeout, AT_EVENT_TYPE_COMMAND_ARG, buffer, BufferLength);
}

Retcode_T AtResponseQueue_WaitForArbitraryCmdArg(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen)
{
    return AtResponseQueue_WaitFor(timeout, AT_EVENT_TYPE_COMMAND_ARG, BufferPtr, BufferLen);
}

Retcode_T AtResponseQueue_WaitForNamedResponseCode(uint32_t timeout, AtResponseCode_T response)
{
    AtResponseQueueEntry_T *entry;
    Retcode_T retcode = AtResponseQueue_WaitForEntry(timeout, AT_EVENT_TYPE_RESPONSE_CODE, &entry);

    if (RETCODE_OK == retcode)
    {
        if (entry->ResponseCode != response)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE);
        }
        else
        {
            (void) Queue_Purge(&EventQueue);
        }
    }
    return retcode;
}

Retcode_T AtResponseQueue_WaitForArbitraryResponseCode(uint32_t timeout, AtResponseCode_T *response)
{
    AtResponseQueueEntry_T *entry;
    Retcode_T retcode = AtResponseQueue_WaitForEntry(timeout, AT_EVENT_TYPE_RESPONSE_CODE, &entry);

    if (RETCODE_OK == retcode)
    {
        *response = entry->ResponseCode;
        (void) Queue_Purge(&EventQueue);
    }

    return retcode;
}

Retcode_T AtResponseQueue_WaitForMiscContent(uint32_t timeout, uint8_t **BufferPtr, uint32_t *BufferLen)
{
    return AtResponseQueue_WaitFor(timeout, AT_EVENT_TYPE_MISC, BufferPtr, BufferLen);
}

Retcode_T AtResponseQueue_IgnoreEvent(uint32_t timeout)
{
    AtResponseQueueEntry_T *entry;
    Retcode_T retcode = Queue_Get(&EventQueue, (void **) &entry, NULL, timeout);

    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_TIMEOUT);
    }
    return Queue_Purge(&EventQueue);
}

Retcode_T AtResponseQueue_GetEvent(uint32_t timeout, AtResponseQueueEntry_T **entry)
{
    Retcode_T retcode = Queue_Get(&EventQueue, (void **) entry, NULL, timeout);
    if (RETCODE_OK != retcode)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_AT_RESPONSE_QUEUE_TIMEOUT);
    }

    return retcode;
}

void AtResponseQueue_MarkBufferAsUnused(void)
{
    /* Don't care if message is already purged */
    (void) Queue_Purge(&EventQueue);
    return;
}

uint32_t AtResponseQueue_GetEventCount(void)
{
    return Queue_Count(&EventQueue);
}

void AtResponseQueue_Clear(void)
{
    Queue_Clear(&EventQueue);
}

Retcode_T AtResponseQueue_Deinit(void)
{
    Retcode_T ret = RETCODE_OK;

    ret = Queue_Delete(&EventQueue);

    if (RETCODE_OK == ret)
    {
        memset(EventQueueBuffer, UINT8_C(0), sizeof(EventQueueBuffer));

        AtResponseParser_RegisterResponseCodeCallback(NULL);
        AtResponseParser_RegisterErrorCallback(NULL);
        AtResponseParser_RegisterCmdEchoCallback(NULL);
        AtResponseParser_RegisterCmdCallback(NULL);
        AtResponseParser_RegisterCmdArgCallback(NULL);
        AtResponseParser_RegisterMiscCallback(NULL);
    }

    return ret;
}

