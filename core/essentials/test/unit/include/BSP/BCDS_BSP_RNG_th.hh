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

#ifndef BCDS_BSP_RNG_TH_HH_
#define BCDS_BSP_RNG_TH_HH_

/* include or ban the real interface header */
#include "BCDS_BSP_RNG.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_BSP_RNG

FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Connect);
FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Enable);
FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Disable);
FAKE_VALUE_FUNC(Retcode_T, BSP_RNG_Disconnect);
FAKE_VALUE_FUNC(HWHandle_T, BSP_RNG_GetHandle);

#endif /* BCDS_FEATURE_BSP_RNG */

#endif /* BCDS_BSP_RNG_TH_HH_ */
