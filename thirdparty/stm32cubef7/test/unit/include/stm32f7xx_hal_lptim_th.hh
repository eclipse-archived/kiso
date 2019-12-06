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
 * Mock-up implementation for the stm32f7xx_hal_lptim module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_LPTIM_TH_HH
#define STM32F7XX_HAL_LPTIM_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_lptim.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Init, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_DeInit, LPTIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_LPTIM_MspInit, LPTIM_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_LPTIM_MspDeInit, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_PWM_Start, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_PWM_Stop, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_PWM_Start_IT, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_PWM_Stop_IT, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_OnePulse_Start, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_OnePulse_Stop, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_OnePulse_Start_IT, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_OnePulse_Stop_IT, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_SetOnce_Start, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_SetOnce_Stop, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_SetOnce_Start_IT, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_SetOnce_Stop_IT, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Encoder_Start, LPTIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Encoder_Stop, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Encoder_Start_IT, LPTIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Encoder_Stop_IT, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_TimeOut_Start, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_TimeOut_Stop, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_TimeOut_Start_IT, LPTIM_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_TimeOut_Stop_IT, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Counter_Start, LPTIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Counter_Stop, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Counter_Start_IT, LPTIM_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_Counter_Stop_IT, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_LPTIM_ReadCounter, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_LPTIM_ReadAutoReload, LPTIM_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_LPTIM_ReadCompare, LPTIM_HandleTypeDef*);

#if (USE_HAL_LPTIM_REGISTER_CALLBACKS == 1)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_RegisterCallback, LPTIM_HandleTypeDef*, HAL_LPTIM_CallbackIDTypeDef, pLPTIM_CallbackTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_LPTIM_UnRegisterCallback, LPTIM_HandleTypeDef*, HAL_LPTIM_CallbackIDTypeDef);

#endif /* USE_HAL_LPTIM_REGISTER_CALLBACKS */

FAKE_VALUE_FUNC(HAL_LPTIM_StateTypeDef, HAL_LPTIM_GetState, LPTIM_HandleTypeDef*);

# endif /* STM32F7XX_HAL_LPTIM_TH_HH */
