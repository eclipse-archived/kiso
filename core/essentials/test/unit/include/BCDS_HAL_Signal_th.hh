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

#ifndef BCDS_BCDS_HAL_Signal_th_HH
#define BCDS_BCDS_HAL_Signal_th_HH

/* Include the needed headers */
#include "BCDS_HAL_Signal.h"

#include "gtest.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, HAL_Signal_SetHooks, HAL_Signal_WaitHook_T, HAL_Signal_NotifyHook_T);
FAKE_VALUE_FUNC(Retcode_T, HAL_Signal_Wait, HAL_SignalHandle_T*, uint32_t );
FAKE_VALUE_FUNC(Retcode_T, HAL_Signal_Notify, HAL_SignalHandle_T*);

#endif /* BCDS_BCDS_HAL_Signal_th_HH */
