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

#ifndef ATRESPONSEQUEUE_TH_HH_
#define ATRESPONSEQUEUE_TH_HH_

/* Include all headers which are needed by this file. */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "CellularResponseQueue.h"

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
