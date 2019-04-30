/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
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
