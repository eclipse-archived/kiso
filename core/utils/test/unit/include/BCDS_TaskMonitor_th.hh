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

 
/* header definition ******************************************************** */
#ifndef BCDS_TASKMONITOR_TH_HH_
#define BCDS_TASKMONITOR_TH_HH_

/* include system header files */

/* wrap the real  interface header */
#include "BCDS_TaskMonitor.h"

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, TaskMonitor_Initialize);
FAKE_VALUE_FUNC(bool, TaskMonitor_Check);
FAKE_VALUE_FUNC(Retcode_T, TaskMonitor_Register, TaskHandle_t, uint32_t);

#endif /* BCDS_TASKMONITOR_TH_HH_ */

/** ************************************************************************* */
