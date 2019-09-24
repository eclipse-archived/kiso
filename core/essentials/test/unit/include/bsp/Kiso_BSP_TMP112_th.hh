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

#ifndef KISO_BSP_TMP112_TH_HH_
#define KISO_BSP_TMP112_TH_HH_

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_TMP112.h"
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_TMP112

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Connect, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Enable, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Disable, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_Disconnect, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_InterruptEnable, uint32_t, BSP_TMP112_InterruptCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_TMP112_InterruptDisable, uint32_t)
FAKE_VALUE_FUNC(HWHandle_T, BSP_TMP112_GetHandle)

#endif /* KISO_FEATURE_TMP112DRIVER */

#endif /* KISO_BSP_TMP112_TH_HH_ */
