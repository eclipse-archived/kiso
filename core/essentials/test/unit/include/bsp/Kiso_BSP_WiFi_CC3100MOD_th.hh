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

#ifndef KISO_BSP_WIFI_CC3100MOD_TH_HH
#define KISO_BSP_WIFI_CC3100MOD_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

/* wrap the real interface header */
#include "Kiso_BSP_WiFi_CC3100MOD.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_WIFI_CC3100MOD

FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Enable, BSP_WiFi_CC3100MOD_InterruptCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_WiFi_CC3100MOD_GetSPIHandle);
FAKE_VALUE_FUNC(HWHandle_T, BSP_WiFi_CC3100MOD_GetUartHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Wakeup);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Hibernate);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_Reset);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_SetCSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_WiFi_CC3100MOD_SetCSHigh);

#endif /* KISO_FEATURE_BSP_WIFI_CC3100MOD */

#endif /* KISO_BSP_WIFI_CC3100MOD_TH_HH */
