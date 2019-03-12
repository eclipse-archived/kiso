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

#ifndef BCDS_BCDS_MCU_Watchdog_th_HH
#define BCDS_BCDS_MCU_Watchdog_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_MCU_Watchdog.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_WATCHDOG

FAKE_VALUE_FUNC(Retcode_T, MCU_Watchdog_Init,WdgHandle_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_Watchdog_Enable);
FAKE_VALUE_FUNC(Retcode_T, MCU_Watchdog_Feed);
FAKE_VALUE_FUNC(bool, MCU_Watchdog_IsResetCausedByWatchdog);

#endif /* BCDS_FEATURE_WATCHDOG */
#endif /* BCDS_BCDS_MCU_Watchdog_th_HH */
