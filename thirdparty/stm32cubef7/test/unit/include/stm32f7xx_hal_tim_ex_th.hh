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
 * Mock-up implementation for the stm32f7xx_hal_tim_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_TIM_EX_TH_HH
#define STM32F7XX_HAL_TIM_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_tim_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_tim_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Init, TIM_HandleTypeDef*, TIM_HallSensor_InitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_DeInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIMEx_HallSensor_MspInit, TIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_TIMEx_HallSensor_MspDeInit, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Start, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Stop, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Start_IT, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Stop_IT, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Start_DMA, TIM_HandleTypeDef*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_HallSensor_Stop_DMA, TIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OCN_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OCN_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OCN_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OCN_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OCN_Start_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OCN_Stop_DMA, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_PWMN_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_PWMN_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_PWMN_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_PWMN_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_PWMN_Start_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_PWMN_Stop_DMA, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OnePulseN_Start, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OnePulseN_Stop, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OnePulseN_Start_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_OnePulseN_Stop_IT, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_ConfigCommutationEvent, TIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_ConfigCommutationEvent_IT, TIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_ConfigCommutationEvent_DMA, TIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_MasterConfigSynchronization, TIM_HandleTypeDef*, TIM_MasterConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_ConfigBreakDeadTime, TIM_HandleTypeDef*, TIM_BreakDeadTimeConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_ConfigBreakInput, TIM_HandleTypeDef*, uint32_t, TIMEx_BreakInputConfigTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_RemapConfig, TIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_TIMEx_GroupChannel5, TIM_HandleTypeDef*, uint32_t);

FAKE_VOID_FUNC(HAL_TIMEx_DMACommutationCplt, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_TIM_StateTypeDef, HAL_TIMEx_HallSensor_GetState, TIM_HandleTypeDef*);

# endif /* STM32F7XX_HAL_TIM_EX_TH_HH */