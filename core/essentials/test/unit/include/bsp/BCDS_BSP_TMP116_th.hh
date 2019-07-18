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
 
#ifndef BCDS_BSP_TMP116_TH_HH
#define BCDS_BSP_TMP116_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_TMP116.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_TMP116

FAKE_VALUE_FUNC(Retcode_T, BSP_TMP116_Connect, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP116_Enable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP116_AlertEnable, int32_t, BSP_TMP116_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP116_AlertDisable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP116_Disable, int32_t);
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP116_Disconnect, int32_t);
FAKE_VALUE_FUNC(HWHandle_T, BSP_TMP116_GetHandle, int32_t);

#endif /* BCDS_FEATURE_BSP_TMP116 */

#endif /* BCDS_BSP_TMP116_TH_HH */
