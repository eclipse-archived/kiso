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

#ifndef KISO_BSP_MAX44009_TH_HH
#define KISO_BSP_MAX44009_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_MAX44009.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_MAX44009

FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Connect, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Enable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_IntEnable, int32_t, BSP_MAX44009_InterruptCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_IntDisable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Disable, int32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_MAX44009_Disconnect, int32_t)
FAKE_VALUE_FUNC(HWHandle_T, BSP_MAX44009_GetHandle, int32_t)
FAKE_VALUE_FUNC(void *, BSP_MAX44009_GetInitParams, int32_t)

#endif /* KISO_FEATURE_BSP_MAX44009 */

#endif /* KISO_BSP_MAX44009_TH_HH */
