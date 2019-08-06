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

#ifndef BCDS_BCDS_MCU_PWM_th_HH
#define BCDS_BCDS_MCU_PWM_th_HH

#include "gtest.h"

/* Include the needed headers */
#include "BCDS_HAL_th.hh"

#if BCDS_FEATURE_PWM

#if defined BCDS_TARGET_PLATFORM && (stm32 == BCDS_TARGET_PLATFORM)
#include "stm32l4xx_th.hh"
#endif /* stm32 */

#endif /* BCDS_FEATURE_PWM */

/* wrap the real interface header */
#include "BCDS_MCU_PWM.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if BCDS_FEATURE_PWM

FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_TimerInitialize, TIM_TypeDef*);
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_TimerChannelInitialize, TIM_TypeDef*, uint32_t);
FAKE_VALUE_FUNC(uint32_t, MCU_PWM_GetFrequency_Hz, TIM_TypeDef*);
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_SetFrequency_Hz, TIM_TypeDef*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_SetFrequency_ScaleChannels, TIM_TypeDef*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_SetDutyCycle, TIM_TypeDef*, uint32_t, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_StartChannel, TIM_TypeDef*, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_StopChannel, TIM_TypeDef*, uint32_t);
FAKE_VALUE_FUNC(uint32_t, MCU_PWM_GetDutyCycle, TIM_TypeDef*, uint32_t);

#endif /* BCDS_FEATURE_PWM */
#endif /* BCDS_BCDS_MCU_PWM_th_HH */
