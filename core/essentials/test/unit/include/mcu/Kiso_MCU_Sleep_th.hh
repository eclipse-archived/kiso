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

#ifndef KISO_MCU_SLEEP_TH_HH
#define KISO_MCU_SLEEP_TH_HH

#include "gtest.h"

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_MCU_Sleep.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_SLEEP

FAKE_VALUE_FUNC(Retcode_T, MCU_Sleep_Initialize)
FAKE_VALUE_FUNC(Retcode_T, MCU_Sleep_BlockSleepMode, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_Sleep_UnblockSleepMode, uint32_t)
FAKE_VALUE_FUNC(uint32_t, MCU_Sleep_GetLowestSleepMode)
FAKE_VOID_FUNC(MCU_Sleep_EnterSleep, uint32_t)

#endif /* KISO_FEATURE_SLEEP */
#endif /* KISO_MCU_SLEEP_TH_HH */
