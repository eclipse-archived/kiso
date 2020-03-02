/*----------------------------------------------------------------------------*/
/*
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
 */
/*----------------------------------------------------------------------------*/

/**
 * @ingroup KISO_CELLULAR_COMMON
 * @defgroup QUEUE Queue
 * @{
 *
 * @brief Defines the interface for message queue (FIFO data structure) with variable size of elements.
 *
 * @details This queue is thread safe, so can be used with multiple senders and receivers.
 *          Though, the most efficient design should have only one receiver and one/multiple senders.
 *          There is no ISR API extension, so the queue should not be used from interrupts.
 *          A message data is copied only once, i.e. when it is added to the queue.
 *          When the message data is received from the queue with Queue_Get(), the receiver must release (dequeue) the message
 *          from the queue by Queue_Purge() when it is no longer used, this will make next message available for receiver.
 *          The message data can have two variable size parts: item and payload.
 *          The item can be fixed size header where a length of variable size payload is stored.
 *
 * @code
 * // example of usage the queue
 *
 * typedef struct
 * {
 *     uint32_t Type;
 *     uint32_t PayloadSize;
 *     uint8_t  Payload[];
 * } ItemHead_T;
 *
 * static Queue_T Queue;
 * static uint8_t QueueBuffer[128];
 *
 * void sender_task()
 * {
 *     ItemHead_T m;
 *     while(1)
 *     {
 *        // Get some data (type, payload etc.)
 *        ...
 *        m.Type = type;
 *        m.PayloadSize = payloadSize;
 *        // Send the message
 *        if (Queue_Put(&q, &m, sizeof(m), payload, payloadSize) != RETCODE_OK)
 *        {
 *             // Do error handling
 *             ...
 *        }
 *     }
 * }
 *
 * void receiver_task()
 * {
 *     ItemHead_T *m;
 *     uint32_t size;
 *
 *     while(1)
 *     {
 *         // Receive a message
 *         Queue_Get(&Queue, &m, &size, portMAX_DELAY);
 *         // Process received message
 *         if (m->type == something)
 *         {
 *             process_message(m->Payload, m->PayloadSize);
 *             ...
 *         }
 *         // Free the message
 *         Queue_Purge(&Queue);
 *     }
 * }
 *
 * void init()
 * {
 *     // Create the queue
 *     Queue_Create(&Queue, QueueBuffer, sizeof(QueueBuffer));
 *
 *     // Create sender_task() and receiver_task()
 *     ...
 * }
 *
 * @endcode
 *
 * @file
 **/

#ifndef QUEUE_H_
#define QUEUE_H_

#include "Kiso_Cellular.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#include "FreeRTOS.h"
#include "semphr.h"

/**
 * @brief The queue item.
 */
typedef struct
{
    uint8_t *Next;
    uint32_t Size;
    uint8_t Data[];
} QueueItem_T;

/**
 * @brief The queue structure.
 */
typedef struct
{
    uint8_t *Buffer;
    uint32_t BufferSize;
    uint8_t *PosRead;
    uint8_t *PosWrite;
    uint32_t Count;
    QueueItem_T *Last;
    SemaphoreHandle_t Lock;
    SemaphoreHandle_t Wakeup;
} Queue_T;

/**
 * @brief Creates a queue by initializing its structure with provided buffer where queue items to be stored.
 *
 * @param[out] Queue        The queue structure to be initialized (must not be equal to NULL).
 * @param[in] Buffer        The buffer to be used for storing queue items (must not be equal to NULL).
 * @param[in] BufferSize    The size of provided buffer (must not be equal to zero).
 *
 * @retval #RETCODE_OK               If the queue was successfully initialized
 * @retval #RETCODE_INVALID_PARAM    If one of the parameters is invalid (NULL pointer or 0 as Buffer Size)
 * @retval #RETCODE_OUT_OF_RESOURCES If the Queue associated semaphores (Lock and Wakeup, see #Queue_T) can't be created
 */
Retcode_T Queue_Create(Queue_T *Queue, uint8_t *Buffer, uint32_t BufferSize);

/**
 * @brief Deletes a queue by deinitializing its structure members.
 *
 * @param[in] Queue The queue structure to be deleted (must not be equal to NULL).
 *
 * @retval #RETCODE_OK               If the queue was successfully deleted
 * @retval #RETCODE_INVALID_PARAM    If the Queue pointer parameter is NULL
 */
Retcode_T Queue_Delete(Queue_T *Queue);

/**
 * @brief Puts an item with/without payload to the queue
 *
 * @param[in] Queue         The queue structure (must not be equal to NULL).
 * @param[in] Item          The item to be added to the queue (must not be equal to NULL)
 * @param[in] ItemSize      The size of given item (must not be equal to zero)
 * @param[in] Payload       The item payload (can be NULL) to be added to the queue
 * @param[in] PayloadSize   The payload size (can be zero)
 *
 * @retval #RETCODE_OK               If the item with payload was successfully added to the queue
 * @retval #RETCODE_OUT_OF_RESOURCES If there is no room in the queue buffer to store the item with the payload
 */
Retcode_T Queue_Put(Queue_T *Queue, const void *Item, uint32_t ItemSize, const void *Payload, uint32_t PayloadSize);

/**
 * @brief Gets an item with payload from the queue
 *
 * @param[in]  Queue    The queue structure (must not be equal to NULL).
 * @param[out] Data     The data (item including payload) received from the queue (must not be equal to NULL).
 * @param[out] DataSize The size of received data (must not be equal to NULL).
 * @param[in]  Timeout  The time to wait for a data from the queue in tick periods
 *
 * @retval #RETCODE_OK                   If the data was successfully received from the queue
 * @retval #RETCODE_SEMAPHORE_ERROR      If no data was received within the waiting time
 * @retval #RETCODE_UNEXPECTED_BEHAVIOR  If queue is empty
 */
Retcode_T Queue_Get(Queue_T *Queue, void **Data, uint32_t *DataSize, uint32_t Timeout);

/**
 * @brief Purges a room in the queue buffer (dequeues message) occupied by received data
 *
 * @param[in] Queue The queue structure (must not be equal to NULL).
 *
 * @retval #RETCODE_OK       If received data was successfully purged from the queue
 * @retval #RETCODE_FAILURE  If no data left to be purged
 */
Retcode_T Queue_Purge(Queue_T *Queue);

/**
 * @brief Gets number of messages available for receiving from the queue
 *
 * @param[in] Queue The queue structure (must not be equal to NULL).
 *
 * @return number of messages available in the queue
 */
uint32_t Queue_Count(const Queue_T *Queue);

/**
 * @brief Cleans the queue buffer from all messages
 *
 * @param[in] Queue The queue structure (must not be equal to NULL).
 */
void Queue_Clear(Queue_T *Queue);

#endif /* QUEUE_H_ */
/** @} */
