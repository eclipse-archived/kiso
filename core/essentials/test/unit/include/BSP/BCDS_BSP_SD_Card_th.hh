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
