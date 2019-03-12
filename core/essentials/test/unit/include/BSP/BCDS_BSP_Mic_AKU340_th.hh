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
 
#ifndef BCDS_BCDS_BSP_Mic_AKU340_th_HH
#define BCDS_BCDS_BSP_Mic_AKU340_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_BSP_Mic_AKU340.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_BSP_MIC_AKU340

FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Enable);
FAKE_VALUE_FUNC(int32_t, BSP_Mic_AKU340_Sense);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_Mic_AKU340_Disconnect);

#endif /* BCDS_FEATURE_BSP_MIC_AKU340 */

#endif /* BCDS_BCDS_BSP_Mic_AKU340_th_HH */
