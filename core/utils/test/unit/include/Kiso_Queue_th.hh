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

#ifndef KISO_QUEUE_TH_HH
#define KISO_QUEUE_TH_HH

#include "gtest.h"
#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "FreeRTOS_th.hh"
#include "semphr_th.hh"
#include "Kiso_Queue.h"

FAKE_VALUE_FUNC(Retcode_T, Queue_Create, Queue_T *, uint8_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Queue_Put, Queue_T *, const void *, uint32_t, const void *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Queue_Get, Queue_T *, void **, uint32_t*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, Queue_Purge, Queue_T *);
FAKE_VALUE_FUNC(uint32_t, Queue_Count, const Queue_T *);
FAKE_VOID_FUNC(Queue_Clear, Queue_T *);
FAKE_VALUE_FUNC(Retcode_T, Queue_Delete, Queue_T *);

#endif /* KISO_QUEUE_TH_HH */
