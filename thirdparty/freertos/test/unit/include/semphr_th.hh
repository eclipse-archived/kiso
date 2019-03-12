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
 *
 *  Mockup implementation for the semphr module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_SEMPHR_TH_HH_
#define BCDS_SEMPHR_TH_HH_

/* include system header files */
#include <gtest.h>
#include "queue_th.hh"

/* ban the real semphr.h header */
#define SEMAPHORE_H

/* mock-ups for the provided interfaces */
typedef QueueHandle_t SemaphoreHandle_t;

/* @note : This is a work around for a bug that was conflicting
 * with the in built gcov's __gcov.vSemaphoreDelete variable
 * resulting in a multiple definition error - mta5cob */
#define vSemaphoreDelete( xSemaphore ) vQueueDelete( ( QueueHandle_t ) ( xSemaphore ) );

FAKE_VOID_FUNC(vSemaphoreCreateBinary, SemaphoreHandle_t);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateBinary);

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreTake, SemaphoreHandle_t, TickType_t);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreTakeRecursive, SemaphoreHandle_t, TickType_t);

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGive, SemaphoreHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGiveRecursive, SemaphoreHandle_t);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGiveFromISR, SemaphoreHandle_t, BaseType_t*);
FAKE_VALUE_FUNC(BaseType_t, xSemaphoreTakeFromISR, SemaphoreHandle_t, BaseType_t*);

FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateMutex);
FAKE_VALUE_FUNC(SemaphoreHandle_t, xSemaphoreCreateRecursiveMutex);

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreCreateCounting, UBaseType_t, UBaseType_t);

FAKE_VALUE_FUNC(BaseType_t, xSemaphoreGetMutexHolder, SemaphoreHandle_t);

#endif /* BCDS_SEMPHR_TH_HH_ */

/** ************************************************************************* */
