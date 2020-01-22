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
 *
 * @brief
 *      This module represents optimized for speed circular buffer implementation,
 *      intended to be used as transport for data between ISR and user code.
 *      Read and Write functions may handle data only partially, depending of fill-level
 *      of the buffer, so corresponding calls may need to be retried.
 *
 * @details
 *      This source file implements following features:
 *      - RingBuffer_Initialize()
 *      - RingBuffer_Write()
 *      - RingBuffer_Read()
 *      - RingBuffer_Reset()
  @note
 *      For optimization purposes, error handling is minimized and responsibility
 *      for parameter correctness is transfered to user code. Also some code constructions
 *      may not follow expected patterns.
 * 
 * @file
 **/

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_RINGBUFFER

/* Include Kiso_RingBuffer interface header */
#include "Kiso_RingBuffer.h"

#include "Kiso_Basics.h"

#if KISO_FEATURE_RINGBUFFER

/* KISO basics header files */
#include "Kiso_Basics.h"
#include "Kiso_Assert.h"

static KISO_INLINE void Add(RingBuffer_T *ringBuffer, uint8_t element)
{
    /* Load the writeIndex to avoid false intermediate values on the real write index */
    register uint32_t index = ringBuffer->WriteIndex;

    /* Write the element and increment the index */
    ringBuffer->Base[index++] = element;

    /* Update the index */
    ringBuffer->WriteIndex = (ringBuffer->Size != index) ? index : 0UL;
    /* Now it is safe to read from the "old" index and write to the new index */
}

static KISO_INLINE uint8_t Get(RingBuffer_T *ringBuffer)
{
    /* Use a copy of the ReadIndex to avoid false intermediate values on the real read index
     * since it is also used in the write process (add) */
    register uint32_t index = ringBuffer->ReadIndex;

    /* Read the data from the buffer and increment the index */
    uint8_t element = ringBuffer->Base[index++];

    /* Update the index */
    ringBuffer->ReadIndex = (ringBuffer->Size != index) ? index : 0UL;
    /* Now it is safe to write to this index again */
    return element;
}

static KISO_INLINE bool RingBuffer_IsFull(RingBuffer_T *ringBuffer)
{
    /* Incrementing the WriteIndex would hit the ReadIndex => full */
    return ((ringBuffer->WriteIndex + 1UL) % (ringBuffer->Size)) != ringBuffer->ReadIndex ? false : true;
}

static KISO_INLINE bool RingBuffer_IsEmpty(RingBuffer_T *ringBuffer)
{
    /* The ReadIndex equals the WriteIndex => empty */
    return ringBuffer->ReadIndex == ringBuffer->WriteIndex ? true : false;
}

/*  The description of the function is available in Kiso_RingBuffer.h */
void RingBuffer_Initialize(RingBuffer_T *ringBuffer, uint8_t *bufferSpace, uint32_t size)
{
    if ((NULL == ringBuffer) || (NULL == bufferSpace) || (0UL == size))
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM));
    }
    else
    {
        /* Initialize the ring-buffer structure:buffer, index r/w, buffer size */
        ringBuffer->Base = bufferSpace;
        ringBuffer->ReadIndex = 0;
        ringBuffer->WriteIndex = 0;
        ringBuffer->Size = size;
    }
}

/*  The description of the function is available in Kiso_RingBuffer.h */
uint32_t RingBuffer_Write(RingBuffer_T *ringBuffer, uint8_t *data, uint32_t length)
{
    uint32_t actualLength = 0UL;

    while (!RingBuffer_IsFull(ringBuffer) && length--)
    {
        /* Now it is safe to write to the buffer when it is not empty, since no one else is writing to it.
         * It could only get even "more empty" since some read processes can have consumed (read) data from the buffer
         */
        Add(ringBuffer, data[actualLength++]);
    }
    return actualLength;
}

/*  The description of the function is available in Kiso_RingBuffer.h */
uint32_t RingBuffer_Read(RingBuffer_T *ringBuffer, uint8_t *data, uint32_t length)
{
    uint32_t actualLength = 0UL;
    if ((NULL != ringBuffer) && (NULL != data))
    {
        /* Read data while the buffer is not empty for the amount of requested data */
        while (!RingBuffer_IsEmpty(ringBuffer) && length--)
        {
            data[actualLength++] = Get(ringBuffer);
        }
    }
    /* Report the number of bytes read */
    return actualLength;
}

/*  The description of the function is available in Kiso_RingBuffer.h */
void RingBuffer_Reset(RingBuffer_T *ringBuffer)
{
    if (NULL != ringBuffer)
    {
        /* Reset the ring-buffer structure:buffer, index r/w */
        ringBuffer->ReadIndex = 0;
        ringBuffer->WriteIndex = 0;
        (void)memset(ringBuffer->Base, 0x00, ringBuffer->Size);
    }
    else
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM));
    }
}

#endif /* if KISO_FEATURE_RINGBUFFER */
