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
 *
 * @file
 *
 * Mock-up implementation for the stm32f7xx_hal_tim module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_TIM_TH_HH
#define STM32F7XX_HAL_TIM_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_tim.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided macros */

#undef __HAL_TIM_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_TIM_RESET_HANDLE_STATE, TIM_TypeDef*)

#undef __HAL_TIM_ENABLE
FAKE_VOID_FUNC(__HAL_TIM_ENABLE, TIM_TypeDef*)

#undef __HAL_TIM_URS_ENABLE
FAKE_VOID_FUNC(__HAL_TIM_URS_ENABLE, TIM_TypeDef*)

#undef __HAL_TIM_MOE_ENABLE
FAKE_VOID_FUNC(__HAL_TIM_MOE_ENABLE, TIM_TypeDef*)

#undef __HAL_TIM_DISABLE
FAKE_VOID_FUNC(__HAL_TIM_DISABLE, TIM_TypeDef*)

#undef __HAL_TIM_URS_DISABLE
FAKE_VOID_FUNC(__HAL_TIM_URS_DISABLE, TIM_TypeDef*)

#undef __HAL_TIM_MOE_DISABLE
FAKE_VOID_FUNC(__HAL_TIM_MOE_DISABLE, TIM_TypeDef*)

#undef __HAL_TIM_ENABLE_IT
FAKE_VOID_FUNC(__HAL_TIM_ENABLE_IT, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_DISABLE_IT
FAKE_VOID_FUNC(__HAL_TIM_DISABLE_IT, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_ENABLE_DMA
FAKE_VOID_FUNC(__HAL_TIM_ENABLE_DMA, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_DISABLE_DMA
FAKE_VOID_FUNC(__HAL_TIM_DISABLE_DMA, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_TIM_GET_FLAG, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_CLEAR_FLAG
FAKE_VALUE_FUNC(bool, __HAL_TIM_CLEAR_FLAG, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_GET_IT_SOURCE
FAKE_VALUE_FUNC(ITStatus, __HAL_TIM_GET_IT_SOURCE, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_CLEAR_IT
FAKE_VOID_FUNC(__HAL_TIM_CLEAR_IT, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_IS_TIM_COUNTING_DOWN
FAKE_VALUE_FUNC(bool, __HAL_TIM_IS_TIM_COUNTING_DOWN, TIM_TypeDef*)

#undef __HAL_TIM_SET_PRESCALER
FAKE_VOID_FUNC(__HAL_TIM_SET_PRESCALER, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_SET_COUNTER
FAKE_VOID_FUNC(__HAL_TIM_SET_COUNTER, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_GET_COUNTER
FAKE_VALUE_FUNC(uint32_t, __HAL_TIM_GET_COUNTER, TIM_TypeDef*)

#undef __HAL_TIM_SET_AUTORELOAD
FAKE_VALUE_FUNC(uint32_t, __HAL_TIM_SET_AUTORELOAD, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_GET_AUTORELOAD
FAKE_VOID_FUNC(__HAL_TIM_GET_AUTORELOAD, TIM_TypeDef*)

#undef __HAL_TIM_SET_CLOCKDIVISION
FAKE_VOID_FUNC(__HAL_TIM_SET_CLOCKDIVISION, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_GET_CLOCKDIVISION
FAKE_VALUE_FUNC(uint32_t, __HAL_TIM_GET_CLOCKDIVISION, TIM_TypeDef*)

#undef __HAL_TIM_SET_ICPRESCALER
FAKE_VOID_FUNC(__HAL_TIM_SET_ICPRESCALER, TIM_TypeDef*, uint32_t, uint32_t)

#undef __HAL_TIM_GET_ICPRESCALER
FAKE_VALUE_FUNC(uint32_t, __HAL_TIM_GET_ICPRESCALER, TIM_TypeDef*, uint32_t)

#undef __HAL_TIM_SET_CAPTUREPOLARITY
FAKE_VALUE_FUNC(uint32_t, __HAL_TIM_SET_CAPTUREPOLARITY, TIM_TypeDef*, uint32_t, uint32_t)

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(HAL_TIM_IRQHandler, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Init, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_Base_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_Base_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Start, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Stop, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Start_IT, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Stop_IT, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Start_DMA, TIM_HandleTypeDef*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Base_Stop_DMA, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Init, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_OC_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_OC_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Start_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_Stop_DMA, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Init, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_PWM_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_PWM_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Start_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_Stop_DMA, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Init, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_IC_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_IC_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Start_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_Stop_DMA, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_Init, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_OnePulse_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_OnePulse_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Init, TIM_HandleTypeDef*, TIM_Encoder_InitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_Encoder_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_Encoder_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Start_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_Encoder_Stop_DMA, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OC_ConfigChannel, TIM_HandleTypeDef*, TIM_OC_InitTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_PWM_ConfigChannel, TIM_HandleTypeDef*, TIM_OC_InitTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_IC_ConfigChannel, TIM_HandleTypeDef*, TIM_IC_InitTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_OnePulse_ConfigChannel, TIM_HandleTypeDef*, TIM_OnePulse_InitTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_ConfigOCrefClear, TIM_HandleTypeDef*, TIM_ClearInputConfigTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_ConfigClockSource, TIM_HandleTypeDef*, TIM_ClockConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_ConfigTI1Input, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_SlaveConfigSynchronization, TIM_HandleTypeDef*, TIM_SlaveConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_SlaveConfigSynchronization_IT, TIM_HandleTypeDef*, TIM_SlaveConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_DMABurst_WriteStart, TIM_HandleTypeDef*, uint32_t, uint32_t, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_DMABurst_WriteStop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_DMABurst_ReadStart, TIM_HandleTypeDef*, uint32_t, uint32_t, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_DMABurst_ReadStop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_GenerateEvent, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_TIM_ReadCapturedValue, TIM_HandleTypeDef*, uint32_t);

#if (USE_HAL_TIM_REGISTER_CALLBACKS == 1)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_RegisterCallback, TIM_HandleTypeDef*, HAL_TIM_CallbackIDTypeDef, pTIM_CallbackTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIM_UnRegisterCallback, TIM_HandleTypeDef*, HAL_TIM_CallbackIDTypeDef);

#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIM_Base_GetState, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIM_OC_GetState, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIM_PWM_GetState, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIM_IC_GetState, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIM_OnePulse_GetState, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIM_Encoder_GetState, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(TIM_Base_SetConfig, TIM_TypeDef*, TIM_Base_InitTypeDef*);

FAKE_VOID_FUNC(TIM_TI1_SetConfig, TIM_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(TIM_OC1_SetConfig, TIM_TypeDef*, TIM_OC_InitTypeDef*);

FAKE_VOID_FUNC(TIM_OC2_SetConfig, TIM_TypeDef*, TIM_OC_InitTypeDef*);

FAKE_VOID_FUNC(TIM_OC3_SetConfig, TIM_TypeDef*, TIM_OC_InitTypeDef*);

FAKE_VOID_FUNC(TIM_OC4_SetConfig, TIM_TypeDef*, TIM_OC_InitTypeDef*);

FAKE_VOID_FUNC(TIM_ETR_SetConfig, TIM_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(HAL_TIM_DMADelayPulseCplt, DMA_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_DMAError, DMA_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIM_DMACaptureCplt, DMA_HandleTypeDef*);

FAKE_VOID_FUNC(TIM_CCxChannelCmd, TIM_TypeDef*, uint32_t, uint32_t);

# endif /* STM32F7XX_HAL_TIM_TH_HH */
