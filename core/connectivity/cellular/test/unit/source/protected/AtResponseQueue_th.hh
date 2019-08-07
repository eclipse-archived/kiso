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

#ifndef ATRESPONSEQUEUE_TH_HH_
#define ATRESPONSEQUEUE_TH_HH_

/* Include all headers which are needed by this file. */
#include <gtest.h>

#include "AtResponseQueue.h"

FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_Init);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_Deinit);
FAKE_VOID_FUNC (AtResponseQueue_RegisterWithResponseParser);
FAKE_VOID_FUNC (AtResponseQueue_Reset);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForNamedCmdEcho,uint32_t, const uint8_t *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForArbitraryCmdEcho,uint32_t,uint8_t **,uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForNamedCmd,uint32_t, const uint8_t *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForArbitraryCmd,uint32_t, uint8_t **, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForNamedCmdArg,uint32_t, const uint8_t *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForArbitraryCmdArg,uint32_t, uint8_t **, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForNamedResponseCode,uint32_t, AtResponseCode_T);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForArbitraryResponseCode,uint32_t, AtResponseCode_T *);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_WaitForMiscContent,uint32_t, uint8_t **, uint32_t *);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_IgnoreEvent,uint32_t);
FAKE_VALUE_FUNC(Retcode_T, AtResponseQueue_GetEvent,uint32_t, AtResponseQueueEntry_T **);
FAKE_VOID_FUNC(AtResponseQueue_MarkBufferAsUnused);
FAKE_VALUE_FUNC(uint32_t, AtResponseQueue_GetEventCount);
FAKE_VOID_FUNC(AtResponseQueue_EnqueueEvent, AtEventType_T, uint8_t *, uint32_t);
FAKE_VOID_FUNC(AtResponseQueue_Clear);

#endif /* ATRESPONSEQUEUE_TH_HH_ */
