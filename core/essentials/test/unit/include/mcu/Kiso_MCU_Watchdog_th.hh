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

#ifndef KISO_MCU_WATCHDOG_TH_HH
#define KISO_MCU_WATCHDOG_TH_HH

#include "gtest.h"

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_MCU_Watchdog.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_WATCHDOG

FAKE_VALUE_FUNC(Retcode_T, MCU_Watchdog_Init,WdgHandle_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_Watchdog_Enable);
FAKE_VALUE_FUNC(Retcode_T, MCU_Watchdog_Feed);
FAKE_VALUE_FUNC(bool, MCU_Watchdog_IsResetCausedByWatchdog);

#endif /* KISO_FEATURE_WATCHDOG */
#endif /* KISO_MCU_WATCHDOG_TH_HH */
