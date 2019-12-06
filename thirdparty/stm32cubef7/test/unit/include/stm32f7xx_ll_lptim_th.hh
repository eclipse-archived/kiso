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
 * Mock-up implementation for the stm32f7xx_ll_lptim module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_LPTIM_TH_HH
#define STM32F7XX_LL_LPTIM_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_LPTIM_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  LPTIM Init structure definition
  */
typedef struct
{
  uint32_t ClockSource;    /*!< Specifies the source of the clock used by the LPTIM instance.
                                This parameter can be a value of @ref LPTIM_LL_EC_CLK_SOURCE.

                                This feature can be modified afterwards using unitary function @ref LL_LPTIM_SetClockSource().*/

  uint32_t Prescaler;      /*!< Specifies the prescaler division ratio.
                                This parameter can be a value of @ref LPTIM_LL_EC_PRESCALER.

                                This feature can be modified afterwards using using unitary function @ref LL_LPTIM_SetPrescaler().*/

  uint32_t Waveform;       /*!< Specifies the waveform shape.
                                This parameter can be a value of @ref LPTIM_LL_EC_OUTPUT_WAVEFORM.

                                This feature can be modified afterwards using unitary function @ref LL_LPTIM_ConfigOutput().*/

  uint32_t Polarity;       /*!< Specifies waveform polarity.
                                This parameter can be a value of @ref LPTIM_LL_EC_OUTPUT_POLARITY.

                                This feature can be modified afterwards using unitary function @ref LL_LPTIM_ConfigOutput().*/
} LL_LPTIM_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_LPTIM_Enable, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_Disable, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabled, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_StartCounter, LPTIM_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_LPTIM_SetUpdateMode, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetUpdateMode, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetAutoReload, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetAutoReload, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetCompare, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetCompare, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetCounter, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetCounterMode, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetCounterMode, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ConfigOutput, LPTIM_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_LPTIM_SetWaveform, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetWaveform, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetPolarity, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetPolarity, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetPrescaler, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetPrescaler, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableTimeout, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableTimeout, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledTimeout, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_TrigSw, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ConfigTrigger, LPTIM_TypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetTriggerSource, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetTriggerFilter, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetTriggerPolarity, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetClockSource, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetClockSource, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ConfigClock, LPTIM_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetClockPolarity, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetClockFilter, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_SetEncoderMode, LPTIM_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_GetEncoderMode, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableEncoderMode, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableEncoderMode, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledEncoderMode, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFLAG_CMPM, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_CMPM, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFLAG_ARRM, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_ARRM, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFlag_EXTTRIG, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_EXTTRIG, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFlag_CMPOK, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_CMPOK, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFlag_ARROK, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_ARROK, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFlag_UP, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_UP, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_ClearFlag_DOWN, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsActiveFlag_DOWN, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_CMPM, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_CMPM, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_CMPM, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_ARRM, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_ARRM, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_ARRM, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_EXTTRIG, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_EXTTRIG, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_EXTTRIG, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_CMPOK, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_CMPOK, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_CMPOK, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_ARROK, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_ARROK, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_ARROK, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_UP, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_UP, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_UP, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_EnableIT_DOWN, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_DisableIT_DOWN, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_LPTIM_IsEnabledIT_DOWN, LPTIM_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_LPTIM_DeInit, LPTIM_TypeDef*);

FAKE_VOID_FUNC(LL_LPTIM_StructInit, LL_LPTIM_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_LPTIM_Init, LPTIM_TypeDef*, LL_LPTIM_InitTypeDef*);

# endif /* STM32F7XX_LL_LPTIM_TH_HH */
