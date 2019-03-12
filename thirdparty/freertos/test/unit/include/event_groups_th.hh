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
 *  Mockup implementation for the event_groups module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_EVENT_GROUPS_TH_HH_
#define BCDS_EVENT_GROUPS_TH_HH_

/* include system header files */
#include <gtest.h>

/* wrap the real event_groups.h header */
#include "event_groups.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(EventGroupHandle_t, xEventGroupCreate);
FAKE_VALUE_FUNC(EventBits_t, xEventGroupClearBits, EventGroupHandle_t, EventBits_t);
FAKE_VALUE_FUNC(EventBits_t, xEventGroupSetBits, EventGroupHandle_t, EventBits_t);
FAKE_VOID_FUNC(vEventGroupSetBitsCallback, void*, uint32_t);
FAKE_VALUE_FUNC(BaseType_t, xTimerPendFunctionCallFromISR, PendedFunction_t, void*, uint32_t, BaseType_t*);
FAKE_VALUE_FUNC(EventBits_t, xEventGroupWaitBits, EventGroupHandle_t, EventBits_t, BaseType_t, BaseType_t, TickType_t)

#endif /* BCDS_EVENT_GROUPS_TH_HH_ */

/** ************************************************************************* */
