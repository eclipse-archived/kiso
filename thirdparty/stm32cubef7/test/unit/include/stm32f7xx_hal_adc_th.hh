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
 * Mock-up implementation for the stm32f7xx_hal_adc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_ADC_TH_HH
#define STM32F7XX_HAL_ADC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_adc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Init, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_DeInit, ADC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_ADC_MspInit, ADC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_ADC_MspDeInit, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Start, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Stop, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_PollForConversion, ADC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_PollForEvent, ADC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Start_IT, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Stop_IT, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Start_DMA, ADC_HandleTypeDef*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_Stop_DMA, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_ADC_GetValue, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_ConfigChannel, ADC_HandleTypeDef*, ADC_ChannelConfTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADC_AnalogWDGConfig, ADC_HandleTypeDef*, ADC_AnalogWDGConfTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_ADC_GetState, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_ADC_GetError, ADC_HandleTypeDef*);

# endif /* STM32F7XX_HAL_ADC_TH_HH */
