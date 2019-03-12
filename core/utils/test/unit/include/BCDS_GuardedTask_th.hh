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
 *  Mockup implementation for the GuardedTask_th module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_GUARDEDTASK_TH_HH_
#define BCDS_GUARDEDTASK_TH_HH_

/* include system header files */
#include <gtest.h>
#include <stdlib.h>
#include <stdint.h>

#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "semphr_th.hh"

/* wrap the real interface header */
#include "BCDS_GuardedTask.h"

/* faked variables needs to be initialized by the test fixture */
typedef void (*GuardedTask_Function_T)(void);

/**
 * mock-ups for the provided interfaces
 * Deprecated functions will be removed as part of PFM-659
 * Deprecated since vXXX (TODO: replace XXX with next version)
 */
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_initialize, GuardedTask_T*, GuardedTask_Function_T, const char*, uint32_t, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_deinitialize, GuardedTask_T*);
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_signal, GuardedTask_T*);
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_signalFromIsr, GuardedTask_T*);

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_Initialize, GuardedTask_T*, GuardedTask_Function_T, const char*, uint32_t, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_Deinitialize, GuardedTask_T*);
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_Signal, GuardedTask_T*);
FAKE_VALUE_FUNC(Retcode_T, GuardedTask_SignalFromIsr, GuardedTask_T*);

#endif /* BCDS_GUARDEDTASK_TH_HH_ */

/** ************************************************************************* */
