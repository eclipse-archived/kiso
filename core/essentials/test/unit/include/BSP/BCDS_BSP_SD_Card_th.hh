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
 
#ifndef BCDS_BCDS_BSP_SD_Card_th_HH
#define BCDS_BCDS_BSP_SD_Card_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

#if BCDS_FEATURE_BSP_SD_CARD

//#include "BSP_Board_th.hh"

#endif /* BCDS_BSP_SD_Card_H */

/* wrap the real interface header */
#include "BCDS_BSP_SD_Card.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_SD_CARD

FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Enable, BSP_SDCardAutoDetectCallback_T);
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_SDCard_GetSPIHandle);
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_SetCSHigh);
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_SetCSLow);

#endif /* BCDS_BSP_SD_Card_H */

#endif /* BCDS_BCDS_BSP_SD_Card_th_HH */
