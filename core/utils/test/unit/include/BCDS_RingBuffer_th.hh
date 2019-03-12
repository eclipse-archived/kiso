/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Mockup implementation for the BCDS_RingBuffer module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_RINGBUFFER_TH_HH_
#define BCDS_RINGBUFFER_TH_HH_

/* include system header files */

#define BCDS_RINGBUFFER_H
/**
 *  @brief
 *      Describes a circular buffer
 *
 */
typedef struct RingBuffer_S
{
    uint8_t * Base; /** Pointer to the base of the user-supplied buffer */
    uint8_t * Wptr; /** Write pointer. NOT to be changed by hand */
    uint8_t * Rptr; /** Read pointer. NOT to be changed by hand */
    uint32_t Size; /** Maximum number of bytes in the user-supplied buffer. Must be set during init */
} RingBuffer_T;

FAKE_VOID_FUNC(RingBuffer_Initialize, RingBuffer_T *, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(uint32_t,RingBuffer_Write, RingBuffer_T*, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(uint32_t,RingBuffer_Read, RingBuffer_T*, uint8_t*, uint32_t);
FAKE_VOID_FUNC(RingBuffer_Reset, RingBuffer_T*);

#endif /* BCDS_RINGBUFFER_TH_HH_ */

/** ************************************************************************* */
