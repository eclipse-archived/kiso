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
 * Mock-up implementation for the stm32f7xx_ll_dac module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_DAC_TH_HH
#define STM32F7XX_LL_DAC_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_DAC_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  Structure definition of some features of DAC instance.
  */
typedef struct
{
  uint32_t TriggerSource;               /*!< Set the conversion trigger source for the selected DAC channel: internal (SW start) or from external IP (timer event, external interrupt line).
                                             This parameter can be a value of @ref DAC_LL_EC_TRIGGER_SOURCE

                                             This feature can be modified afterwards using unitary function @ref LL_DAC_SetTriggerSource(). */

  uint32_t WaveAutoGeneration;          /*!< Set the waveform automatic generation mode for the selected DAC channel.
                                             This parameter can be a value of @ref DAC_LL_EC_WAVE_AUTO_GENERATION_MODE

                                             This feature can be modified afterwards using unitary function @ref LL_DAC_SetWaveAutoGeneration(). */

  uint32_t WaveAutoGenerationConfig;    /*!< Set the waveform automatic generation mode for the selected DAC channel.
                                             If waveform automatic generation mode is set to noise, this parameter can be a value of @ref DAC_LL_EC_WAVE_NOISE_LFSR_UNMASK_BITS
                                             If waveform automatic generation mode is set to triangle, this parameter can be a value of @ref DAC_LL_EC_WAVE_TRIANGLE_AMPLITUDE
                                             @note If waveform automatic generation mode is disabled, this parameter is discarded.

                                             This feature can be modified afterwards using unitary function @ref LL_DAC_SetWaveNoiseLFSR() or @ref LL_DAC_SetWaveTriangleAmplitude(), depending on the wave automatic generation selected. */

  uint32_t OutputBuffer;                /*!< Set the output buffer for the selected DAC channel.
                                             This parameter can be a value of @ref DAC_LL_EC_OUTPUT_BUFFER

                                             This feature can be modified afterwards using unitary function @ref LL_DAC_SetOutputBuffer(). */

} LL_DAC_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_DAC_SetTriggerSource, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_GetTriggerSource, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_SetWaveAutoGeneration, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_GetWaveAutoGeneration, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_SetWaveNoiseLFSR, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_GetWaveNoiseLFSR, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_SetWaveTriangleAmplitude, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_GetWaveTriangleAmplitude, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_SetOutputBuffer, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_GetOutputBuffer, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_EnableDMAReq, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_DisableDMAReq, DAC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsDMAReqEnabled, DAC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_DMA_GetRegAddr, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DAC_Enable, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_Disable, DAC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsEnabled, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_EnableTrigger, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_DisableTrigger, DAC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsTriggerEnabled, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_TrigSWConversion, DAC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_DAC_ConvertData12RightAligned, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DAC_ConvertData12LeftAligned, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DAC_ConvertData8RightAligned, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DAC_ConvertDualData12RightAligned, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DAC_ConvertDualData12LeftAligned, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_DAC_ConvertDualData8RightAligned, DAC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_RetrieveOutputData, DAC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsActiveFlag_DMAUDR1, DAC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsActiveFlag_DMAUDR2, DAC_TypeDef*);

FAKE_VOID_FUNC(LL_DAC_ClearFlag_DMAUDR1, DAC_TypeDef*);

FAKE_VOID_FUNC(LL_DAC_ClearFlag_DMAUDR2, DAC_TypeDef*);

FAKE_VOID_FUNC(LL_DAC_EnableIT_DMAUDR1, DAC_TypeDef*);

FAKE_VOID_FUNC(LL_DAC_EnableIT_DMAUDR2, DAC_TypeDef*);

FAKE_VOID_FUNC(LL_DAC_DisableIT_DMAUDR1, DAC_TypeDef*);

FAKE_VOID_FUNC(LL_DAC_DisableIT_DMAUDR2, DAC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsEnabledIT_DMAUDR1, DAC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_DAC_IsEnabledIT_DMAUDR2, DAC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_DAC_DeInit, DAC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_DAC_Init, DAC_TypeDef*, uint32_t, LL_DAC_InitTypeDef*);

FAKE_VOID_FUNC(LL_DAC_StructInit, LL_DAC_InitTypeDef*);

# endif /* STM32F7XX_LL_DAC_TH_HH */
