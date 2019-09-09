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
 
#ifndef KISO_HAL_CRITICALSECTION_TH_HH_
#define KISO_HAL_CRITICALSECTION_TH_HH_

/* include or ban the real interface header */
#include "Kiso_HAL_CriticalSection.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, HAL_CriticalSection_SetHooks, HAL_CriticalSection_Hook_T, HAL_CriticalSection_Hook_T);
FAKE_VALUE_FUNC(Retcode_T, HAL_CriticalSection_Enter, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, HAL_CriticalSection_Leave, uint32_t*);

#endif /* KISO_HAL_CRITICALSECTION_TH_HH_ */
