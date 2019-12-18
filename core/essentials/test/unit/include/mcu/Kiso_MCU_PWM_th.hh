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

#ifndef KISO_MCU_PWM_TH_HH
#define KISO_MCU_PWM_TH_HH

#include "gtest.h"

/* Include the needed headers */
#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_PWM

#if defined KISO_TARGET_PLATFORM && (stm32 == KISO_TARGET_PLATFORM)
#include "stm32l4xx_th.hh"
#endif /* stm32 */

#endif /* KISO_FEATURE_PWM */

/* wrap the real interface header */
#include "Kiso_MCU_PWM.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if KISO_FEATURE_PWM

FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_TimerInitialize, TIM_TypeDef *)
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_TimerChannelInitialize, TIM_TypeDef *, uint32_t)
FAKE_VALUE_FUNC(uint32_t, MCU_PWM_GetFrequency_Hz, TIM_TypeDef *)
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_SetFrequency_Hz, TIM_TypeDef *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_SetFrequency_ScaleChannels, TIM_TypeDef *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_SetDutyCycle, TIM_TypeDef *, uint32_t, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_StartChannel, TIM_TypeDef *, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, MCU_PWM_StopChannel, TIM_TypeDef *, uint32_t)
FAKE_VALUE_FUNC(uint32_t, MCU_PWM_GetDutyCycle, TIM_TypeDef *, uint32_t)

#endif /* KISO_FEATURE_PWM */
#endif /* KISO_MCU_PWM_TH_HH */
