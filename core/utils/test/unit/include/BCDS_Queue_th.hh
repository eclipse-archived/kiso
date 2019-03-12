/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

#ifndef BCDS_QUEUE_TH_HH
#define BCDS_QUEUE_TH_HH

#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "FreeRTOS_th.hh"
#include "semphr_th.hh"
#include "BCDS_Queue.h"

FAKE_VALUE_FUNC(Retcode_T, Queue_Create, Queue_T *, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Queue_Put, Queue_T *, const void *, uint32_t, const void *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Queue_Get, Queue_T *, void **, uint32_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Queue_Purge, Queue_T *);
FAKE_VALUE_FUNC(uint32_t, Queue_Count, const Queue_T *);
FAKE_VOID_FUNC(Queue_Clear, Queue_T *);
FAKE_VALUE_FUNC(Retcode_T, Queue_Delete, Queue_T *);

#endif /* BCDS_QUEUE_TH_HH */
