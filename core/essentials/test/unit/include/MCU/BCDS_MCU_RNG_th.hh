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
 
#ifndef BCDS_MCU_RNG_TH_HH_
#define BCDS_MCU_RNG_TH_HH_

/* include or ban the real interface header */
#include "BCDS_MCU_RNG.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_RNG

FAKE_VALUE_FUNC(Retcode_T, MCU_RNG_Initialize, RNG_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_RNG_Deinitialize, RNG_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_RNG_Generate, RNG_T, uint8_t*, uint32_t);

#endif /* if BCDS_FEATURE_RNG */


#endif /* BCDS_MCU_RNG_TH_HH_ */
