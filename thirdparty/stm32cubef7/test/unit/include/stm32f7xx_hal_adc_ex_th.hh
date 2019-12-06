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
 * Mock-up implementation for the stm32f7xx_hal_adc_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_ADC_EX_TH_HH
#define STM32F7XX_HAL_ADC_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include"stm32f7xx_hal_adc_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_adc_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_InjectedStart, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_InjectedStop, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_InjectedPollForConversion, ADC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_InjectedStart_IT, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_InjectedStop_IT, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_ADCEx_InjectedGetValue, ADC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_MultiModeStart_DMA, ADC_HandleTypeDef*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_MultiModeStop_DMA, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_ADCEx_MultiModeGetValue, ADC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_InjectedConfigChannel, ADC_HandleTypeDef*, ADC_InjectionConfTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ADCEx_MultiModeConfigChannel, ADC_HandleTypeDef*, ADC_MultiModeTypeDef*);

# endif /* STM32F7XX_HAL_ADC_EX_TH_HH */
