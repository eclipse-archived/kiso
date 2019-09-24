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
 * @ingroup UTILS
 *
 * @defgroup RINGBUFFER RingBuffer
 * @{
 *
 * @brief
 *      Mockup implementation for the Kiso_RingBuffer_th.hh module
 *
 * @details
 *
 * @file
 **/

/* Header definition */
#ifndef KISO_RINGBUFFER_TH_HH_
#define KISO_RINGBUFFER_TH_HH_

/* Include gtest header file */
#include "gtest.h"

#define KISO_RINGBUFFER_H
/**
 *  @brief
 *      Describes a circular buffer
 *
 */
typedef struct RingBuffer_S
{
    uint8_t *Base; /** Pointer to the base of the user-supplied buffer */
    uint8_t *Wptr; /** Write pointer. NOT to be changed by hand */
    uint8_t *Rptr; /** Read pointer. NOT to be changed by hand */
    uint32_t Size; /** Maximum number of bytes in the user-supplied buffer. Must be set during init */
} RingBuffer_T;

FAKE_VOID_FUNC(RingBuffer_Initialize, RingBuffer_T *, uint8_t *, uint32_t)
FAKE_VALUE_FUNC(uint32_t, RingBuffer_Write, RingBuffer_T *, uint8_t *, uint32_t)
FAKE_VALUE_FUNC(uint32_t, RingBuffer_Read, RingBuffer_T *, uint8_t *, uint32_t)
FAKE_VOID_FUNC(RingBuffer_Reset, RingBuffer_T *)

#endif /* KISO_RINGBUFFER_TH_HH_ */

/** ************************************************************************* */
