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
#ifndef BCDS_EVENTHUB_TH_HH_
#define BCDS_EVENTHUB_TH_HH_

#include "BCDS_Retcode_th.hh"
#include "BCDS_EventHub.h"

FAKE_VALUE_FUNC(Retcode_T, EventHub_Initialize, EventHub_T*);
FAKE_VALUE_FUNC(Retcode_T, EventHub_Observe, EventHub_T*, EventHandler_T, TaskEvent_T);
FAKE_VALUE_FUNC(Retcode_T, EventHub_ObserveAll, EventHub_T*, EventHandler_T);
FAKE_VALUE_FUNC(Retcode_T, EventHub_Notify, EventHub_T*, TaskEvent_T, void*);

#endif /* BCDS_EVENTHUB_TH_HH_ */

/** ************************************************************************* */
