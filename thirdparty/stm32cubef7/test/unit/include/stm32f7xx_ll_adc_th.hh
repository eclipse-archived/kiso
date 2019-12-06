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
 * Mock-up implementation for the stm32f7xx_ll_adc module.
 */
/*----------------------------------------------------------------------------*/

#ifndef STM32F7XX_LL_ADC_TH_HH
#define STM32F7XX_LL_ADC_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_ADC_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Exposed types */

/**
  * @brief  Structure definition of some features of ADC common parameters
  *         and multimode
  *         (all ADC instances belonging to the same ADC common instance).
  * @note   The setting of these parameters by function @ref LL_ADC_CommonInit()
  *         is conditioned to ADC instances state (all ADC instances
  *         sharing the same ADC common instance):
  *         All ADC instances sharing the same ADC common instance must be
  *         disabled.
  */
typedef struct
{
  uint32_t CommonClock;                 /*!< Set parameter common to several ADC: Clock source and prescaler.
                                             This parameter can be a value of @ref ADC_LL_EC_COMMON_CLOCK_SOURCE

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetCommonClock(). */

  uint32_t Multimode;                   /*!< Set ADC multimode configuration to operate in independent mode or multimode (for devices with several ADC instances).
                                             This parameter can be a value of @ref ADC_LL_EC_MULTI_MODE

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetMultimode(). */

  uint32_t MultiDMATransfer;            /*!< Set ADC multimode conversion data transfer: no transfer or transfer by DMA.
                                             This parameter can be a value of @ref ADC_LL_EC_MULTI_DMA_TRANSFER

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetMultiDMATransfer(). */

  uint32_t MultiTwoSamplingDelay;       /*!< Set ADC multimode delay between 2 sampling phases.
                                             This parameter can be a value of @ref ADC_LL_EC_MULTI_TWOSMP_DELAY

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetMultiTwoSamplingDelay(). */

} LL_ADC_CommonInitTypeDef;

/**
  * @brief  Structure definition of some features of ADC instance.
  * @note   These parameters have an impact on ADC scope: ADC instance.
  *         Affects both group regular and group injected (availability
  *         of ADC group injected depends on STM32 families).
  *         Refer to corresponding unitary functions into
  *         @ref ADC_LL_EF_Configuration_ADC_Instance .
  * @note   The setting of these parameters by function @ref LL_ADC_Init()
  *         is conditioned to ADC state:
  *         ADC instance must be disabled.
  *         This condition is applied to all ADC features, for efficiency
  *         and compatibility over all STM32 families. However, the different
  *         features can be set under different ADC state conditions
  *         (setting possible with ADC enabled without conversion on going,
  *         ADC enabled with conversion on going, ...)
  *         Each feature can be updated afterwards with a unitary function
  *         and potentially with ADC in a different state than disabled,
  *         refer to description of each function for setting
  *         conditioned to ADC state.
  */
typedef struct
{
  uint32_t Resolution;                  /*!< Set ADC resolution.
                                             This parameter can be a value of @ref ADC_LL_EC_RESOLUTION

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetResolution(). */

  uint32_t DataAlignment;               /*!< Set ADC conversion data alignment.
                                             This parameter can be a value of @ref ADC_LL_EC_DATA_ALIGN

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetDataAlignment(). */

  uint32_t SequencersScanMode;          /*!< Set ADC scan selection.
                                             This parameter can be a value of @ref ADC_LL_EC_SCAN_SELECTION

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_SetSequencersScanMode(). */

} LL_ADC_InitTypeDef;

/**
  * @brief  Structure definition of some features of ADC group regular.
  * @note   These parameters have an impact on ADC scope: ADC group regular.
  *         Refer to corresponding unitary functions into
  *         @ref ADC_LL_EF_Configuration_ADC_Group_Regular
  *         (functions with prefix "REG").
  * @note   The setting of these parameters by function @ref LL_ADC_REG_Init()
  *         is conditioned to ADC state:
  *         ADC instance must be disabled.
  *         This condition is applied to all ADC features, for efficiency
  *         and compatibility over all STM32 families. However, the different
  *         features can be set under different ADC state conditions
  *         (setting possible with ADC enabled without conversion on going,
  *         ADC enabled with conversion on going, ...)
  *         Each feature can be updated afterwards with a unitary function
  *         and potentially with ADC in a different state than disabled,
  *         refer to description of each function for setting
  *         conditioned to ADC state.
  */
typedef struct
{
  uint32_t TriggerSource;               /*!< Set ADC group regular conversion trigger source: internal (SW start) or from external IP (timer event, external interrupt line).
                                             This parameter can be a value of @ref ADC_LL_EC_REG_TRIGGER_SOURCE
                                             @note On this STM32 serie, setting of external trigger edge is performed
                                                   using function @ref LL_ADC_REG_StartConversionExtTrig().

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_REG_SetTriggerSource(). */

  uint32_t SequencerLength;             /*!< Set ADC group regular sequencer length.
                                             This parameter can be a value of @ref ADC_LL_EC_REG_SEQ_SCAN_LENGTH
                                             @note This parameter is discarded if scan mode is disabled (refer to parameter 'ADC_SequencersScanMode').

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_REG_SetSequencerLength(). */

  uint32_t SequencerDiscont;            /*!< Set ADC group regular sequencer discontinuous mode: sequence subdivided and scan conversions interrupted every selected number of ranks.
                                             This parameter can be a value of @ref ADC_LL_EC_REG_SEQ_DISCONT_MODE
                                             @note This parameter has an effect only if group regular sequencer is enabled
                                                   (scan length of 2 ranks or more).

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_REG_SetSequencerDiscont(). */

  uint32_t ContinuousMode;              /*!< Set ADC continuous conversion mode on ADC group regular, whether ADC conversions are performed in single mode (one conversion per trigger) or in continuous mode (after the first trigger, following conversions launched successively automatically).
                                             This parameter can be a value of @ref ADC_LL_EC_REG_CONTINUOUS_MODE
                                             Note: It is not possible to enable both ADC group regular continuous mode and discontinuous mode.

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_REG_SetContinuousMode(). */

  uint32_t DMATransfer;                 /*!< Set ADC group regular conversion data transfer: no transfer or transfer by DMA, and DMA requests mode.
                                             This parameter can be a value of @ref ADC_LL_EC_REG_DMA_TRANSFER

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_REG_SetDMATransfer(). */

} LL_ADC_REG_InitTypeDef;

/**
  * @brief  Structure definition of some features of ADC group injected.
  * @note   These parameters have an impact on ADC scope: ADC group injected.
  *         Refer to corresponding unitary functions into
  *         @ref ADC_LL_EF_Configuration_ADC_Group_Regular
  *         (functions with prefix "INJ").
  * @note   The setting of these parameters by function @ref LL_ADC_INJ_Init()
  *         is conditioned to ADC state:
  *         ADC instance must be disabled.
  *         This condition is applied to all ADC features, for efficiency
  *         and compatibility over all STM32 families. However, the different
  *         features can be set under different ADC state conditions
  *         (setting possible with ADC enabled without conversion on going,
  *         ADC enabled with conversion on going, ...)
  *         Each feature can be updated afterwards with a unitary function
  *         and potentially with ADC in a different state than disabled,
  *         refer to description of each function for setting
  *         conditioned to ADC state.
  */
typedef struct
{
  uint32_t TriggerSource;               /*!< Set ADC group injected conversion trigger source: internal (SW start) or from external IP (timer event, external interrupt line).
                                             This parameter can be a value of @ref ADC_LL_EC_INJ_TRIGGER_SOURCE
                                             @note On this STM32 serie, setting of external trigger edge is performed
                                                   using function @ref LL_ADC_INJ_StartConversionExtTrig().

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_INJ_SetTriggerSource(). */

  uint32_t SequencerLength;             /*!< Set ADC group injected sequencer length.
                                             This parameter can be a value of @ref ADC_LL_EC_INJ_SEQ_SCAN_LENGTH
                                             @note This parameter is discarded if scan mode is disabled (refer to parameter 'ADC_SequencersScanMode').

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_INJ_SetSequencerLength(). */

  uint32_t SequencerDiscont;            /*!< Set ADC group injected sequencer discontinuous mode: sequence subdivided and scan conversions interrupted every selected number of ranks.
                                             This parameter can be a value of @ref ADC_LL_EC_INJ_SEQ_DISCONT_MODE
                                             @note This parameter has an effect only if group injected sequencer is enabled
                                                   (scan length of 2 ranks or more).

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_INJ_SetSequencerDiscont(). */

  uint32_t TrigAuto;                    /*!< Set ADC group injected conversion trigger: independent or from ADC group regular.
                                             This parameter can be a value of @ref ADC_LL_EC_INJ_TRIG_AUTO
                                             Note: This parameter must be set to set to independent trigger if injected trigger source is set to an external trigger.

                                             This feature can be modified afterwards using unitary function @ref LL_ADC_INJ_SetTrigAuto(). */

} LL_ADC_INJ_InitTypeDef;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(uint32_t, LL_ADC_DMA_GetRegAddr, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_SetCommonClock, ADC_Common_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetCommonClock, ADC_Common_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetCommonPathInternalCh, ADC_Common_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetCommonPathInternalCh, ADC_Common_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetResolution, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetResolution, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetDataAlignment, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetDataAlignment, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetSequencersScanMode, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetSequencersScanMode, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_SetTriggerSource, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetTriggerSource, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_IsTriggerSourceSWStart, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetTriggerEdge, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_SetSequencerLength, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetSequencerLength, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_SetSequencerDiscont, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetSequencerDiscont, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_SetSequencerRanks, ADC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetSequencerRanks, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_REG_SetContinuousMode, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetContinuousMode, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_SetDMATransfer, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetDMATransfer, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_SetFlagEndOfConversion, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_GetFlagEndOfConversion, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_SetTriggerSource, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetTriggerSource, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_IsTriggerSourceSWStart, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetTriggerEdge, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_SetSequencerLength, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetSequencerLength, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_SetSequencerDiscont, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetSequencerDiscont, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_SetSequencerRanks, ADC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetSequencerRanks, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_INJ_SetTrigAuto, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetTrigAuto, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_SetOffset, ADC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_GetOffset, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_SetChannelSamplingTime, ADC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetChannelSamplingTime, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_SetAnalogWDMonitChannels, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetAnalogWDMonitChannels, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetAnalogWDThresholds, ADC_TypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetAnalogWDThresholds, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_SetMultimode, ADC_Common_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetMultimode, ADC_Common_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetMultiDMATransfer, ADC_Common_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetMultiDMATransfer, ADC_Common_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_SetMultiTwoSamplingDelay, ADC_Common_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_GetMultiTwoSamplingDelay, ADC_Common_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_Enable, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_Disable, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsEnabled, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_StartConversionSWStart, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_StartConversionExtTrig, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_REG_StopConversionExtTrig, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_ReadConversionData32, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint16_t, LL_ADC_REG_ReadConversionData12, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint16_t, LL_ADC_REG_ReadConversionData10, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint8_t, LL_ADC_REG_ReadConversionData8, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint8_t, LL_ADC_REG_ReadConversionData6, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_REG_ReadMultiConversionData32, ADC_Common_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_INJ_StartConversionSWStart, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_StartConversionExtTrig, ADC_TypeDef*, uint32_t);

FAKE_VOID_FUNC(LL_ADC_INJ_StopConversionExtTrig, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_INJ_ReadConversionData32, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint16_t, LL_ADC_INJ_ReadConversionData12, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint16_t, LL_ADC_INJ_ReadConversionData10, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint8_t, LL_ADC_INJ_ReadConversionData8, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint8_t, LL_ADC_INJ_ReadConversionData6, ADC_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_EOCS, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_OVR, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_JEOS, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_AWD1, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_ClearFlag_EOCS, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_ClearFlag_OVR, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_ClearFlag_JEOS, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_ClearFlag_AWD1, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_MST_EOCS, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV1_EOCS, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV2_EOCS, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_MST_OVR, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV1_OVR, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV2_OVR, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_MST_JEOS, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV1_JEOS, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV2_JEOS, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_MST_AWD1, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV1_AWD1, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsActiveFlag_SLV2_AWD1, ADC_Common_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_EnableIT_EOCS, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_EnableIT_OVR, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_EnableIT_JEOS, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_EnableIT_AWD1, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_DisableIT_EOCS, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_DisableIT_OVR, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_DisableIT_JEOS, ADC_TypeDef*);

FAKE_VOID_FUNC(LL_ADC_DisableIT_AWD1, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsEnabledIT_EOCS, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsEnabledIT_OVR, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsEnabledIT_JEOS, ADC_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_ADC_IsEnabledIT_AWD1, ADC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_ADC_CommonDeInit, ADC_Common_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_ADC_CommonInit, ADC_Common_TypeDef*, LL_ADC_CommonInitTypeDef*);

FAKE_VOID_FUNC(LL_ADC_CommonStructInit, LL_ADC_CommonInitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_ADC_DeInit, ADC_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_ADC_Init, ADC_TypeDef*, LL_ADC_InitTypeDef*);

FAKE_VOID_FUNC(LL_ADC_StructInit, LL_ADC_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_ADC_REG_Init, ADC_TypeDef*, LL_ADC_REG_InitTypeDef*);

FAKE_VOID_FUNC(LL_ADC_REG_StructInit, LL_ADC_REG_InitTypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_ADC_INJ_Init, ADC_TypeDef*, LL_ADC_INJ_InitTypeDef*);

FAKE_VOID_FUNC(LL_ADC_INJ_StructInit, LL_ADC_INJ_InitTypeDef*);

# endif /* STM32F7XX_LL_ADC_TH_HH */
