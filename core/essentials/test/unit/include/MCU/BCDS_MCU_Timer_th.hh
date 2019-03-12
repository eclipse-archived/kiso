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

#ifndef BCDS_MCU_Timer_TH_HH
#define BCDS_MCU_Timer_TH_HH

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

/* wrap the real interface header */
#include "BCDS_MCU_Timer.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
#if BCDS_FEATURE_TIMER

FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_Initialize, Timer_T, MCU_TIMER_Callback_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_Deinitialize, Timer_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_SetCompareValue, Timer_T, uint32_t, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_GetCompareValue, Timer_T, uint32_t, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_GetCountValue, Timer_T, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_GetMaxCount, Timer_T, uint32_t*);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_Start, Timer_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_Stop, Timer_T);
FAKE_VALUE_FUNC(MCU_Timer_Status_T, MCU_Timer_GetStatus, Timer_T);
FAKE_VALUE_FUNC(Retcode_T, MCU_Timer_GetClockFrequency, Timer_T,uint32_t *);
FAKE_VALUE_FUNC(HWHandle_T, MCU_Timer_GetRtcHandle);

#endif /* #if BCDS_FEATURE_TIMER  */
#endif /* BCDS_MCU_Timer_TH_HH */

