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
 * Mock-up implementation for the stm32f7xx_hal_dfsdm module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DFSDM_TH_HH
#define STM32F7XX_HAL_DFSDM_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dfsdm.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelInit, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelDeInit, DFSDM_Channel_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DFSDM_ChannelMspInit, DFSDM_Channel_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DFSDM_ChannelMspDeInit, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelCkabStart, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelCkabStart_IT, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelCkabStop, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelCkabStop_IT, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelScdStart, DFSDM_Channel_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelScdStart_IT, DFSDM_Channel_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelScdStop, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelScdStop_IT, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(int16_t, HAL_DFSDM_ChannelGetAwdValue, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelModifyOffset, DFSDM_Channel_HandleTypeDef*, int32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelPollForCkab, DFSDM_Channel_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_ChannelPollForScd, DFSDM_Channel_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_DFSDM_Channel_StateTypeDef, HAL_DFSDM_ChannelGetState, DFSDM_Channel_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInit, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterDeInit, DFSDM_Filter_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DFSDM_FilterMspInit, DFSDM_Filter_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DFSDM_FilterMspDeInit, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularStart, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularStart_IT, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularStart_DMA, DFSDM_Filter_HandleTypeDef*, int32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularMsbStart_DMA, DFSDM_Filter_HandleTypeDef*, int16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularStop, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularStop_IT, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterRegularStop_DMA, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedStart, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedStart_IT, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedStart_DMA, DFSDM_Filter_HandleTypeDef*, int32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedMsbStart_DMA, DFSDM_Filter_HandleTypeDef*, int16_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedStop, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedStop_IT, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterInjectedStop_DMA, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterAwdStart_IT, DFSDM_Filter_HandleTypeDef*, DFSDM_Filter_AwdParamTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterAwdStop_IT, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterExdStart, DFSDM_Filter_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterExdStop, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(int32_t, HAL_DFSDM_FilterGetRegularValue, DFSDM_Filter_HandleTypeDef*, uint32_t*);

FAKE_VALUE_FUNC(int32_t, HAL_DFSDM_FilterGetInjectedValue, DFSDM_Filter_HandleTypeDef*, uint32_t*);

FAKE_VALUE_FUNC(int32_t, HAL_DFSDM_FilterGetExdMaxValue, DFSDM_Filter_HandleTypeDef*, uint32_t*);

FAKE_VALUE_FUNC(int32_t, HAL_DFSDM_FilterGetExdMinValue, DFSDM_Filter_HandleTypeDef*, uint32_t*);

FAKE_VALUE_FUNC(uint32_t, HAL_DFSDM_FilterGetConvTimeValue, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterPollForRegConversion, DFSDM_Filter_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DFSDM_FilterPollForInjConversion, DFSDM_Filter_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_DFSDM_Filter_StateTypeDef, HAL_DFSDM_FilterGetState, DFSDM_Filter_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_DFSDM_FilterGetError, DFSDM_Filter_HandleTypeDef*);

# endif /* STM32F7XX_HAL_DFSDM_TH_HH */