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

#ifndef KISO_BSP_SD_CARD_TH_HH
#define KISO_BSP_SD_CARD_TH_HH

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_BSP_SD_CARD

//#include "BSP_Board_th.hh"

#endif /* KISO_BSP_SD_Card_H */

/* wrap the real interface header */
#include "Kiso_BSP_SD_Card.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_BSP_SD_CARD

FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Connect)
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Enable, BSP_SDCardAutoDetectCallback_T)
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Disable)
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_Disconnect)
FAKE_VALUE_FUNC(HWHandle_T, BSP_SDCard_GetSPIHandle)
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_SetCSHigh)
FAKE_VALUE_FUNC(Retcode_T, BSP_SDCard_SetCSLow)

#endif /* KISO_BSP_SD_Card_H */

#endif /* KISO_BSP_SD_CARD_TH_HH */
