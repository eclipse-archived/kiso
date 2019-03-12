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

#ifndef BCDS_BCDS_MCU_PWM_th_HH
#define BCDS_BCDS_MCU_PWM_th_HH

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
