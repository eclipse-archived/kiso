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
 
#ifndef BCDS_BCDS_UMTS_LisaU2_th_HH
#define BCDS_BCDS_UMTS_LisaU2_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_UMTS_LisaU2.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_UMTS_LISAU2

FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_Disconnect);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_Reset);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_GetCTS);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_SetRTSLow);
FAKE_VALUE_FUNC(Retcode_T, BSP_UMTS_LisaU2_SetRTSHigh);
FAKE_VALUE_FUNC(HWHandle_T, BSP_UMTS_LisaU2_GetUARTHandle);
FAKE_VALUE_FUNC(HWHandle_T, BSP_UMTS_LisaU2_GetSPIHandle);

#endif /* BCDS_FEATURE_BSP_UMTS_LISAU2 */

#endif /* BCDS_BCDS_UMTS_LisaU2_th_HH */
