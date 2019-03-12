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
 
#ifndef BCDS_BCDS_BSP_WiFi_CC3100MOD_th_HH
#define BCDS_BCDS_BSP_WiFi_CC3100MOD_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_WiFi_CC3100MOD.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_WIFI_CC3100MOD

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

#endif /* BCDS_FEATURE_BSP_WIFI_CC3100MOD */

#endif /* BCDS_BCDS_BSP_WiFi_CC3100MOD_th_HH */
