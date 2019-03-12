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

#ifndef BCDS_MCU_Sleep_th_HH
#define BCDS_MCU_Sleep_th_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_MCU_Sleep.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_SLEEP

FAKE_VALUE_FUNC(Retcode_T, MCU_Sleep_Initialize);
FAKE_VALUE_FUNC(Retcode_T, MCU_Sleep_BlockSleepMode, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_Sleep_UnblockSleepMode, uint32_t);
FAKE_VALUE_FUNC(uint32_t, MCU_Sleep_GetLowestSleepMode);
FAKE_VOID_FUNC(MCU_Sleep_EnterSleep, uint32_t);

#endif /* BCDS_FEATURE_SLEEP */
#endif /* BCDS_MCU_Sleep_th_HH */
