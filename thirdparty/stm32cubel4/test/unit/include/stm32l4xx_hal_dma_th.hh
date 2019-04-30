/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

#ifndef STM32L4XX_HAL_DMA_TH_HH_
#define STM32L4XX_HAL_DMA_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_dma.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
#undef __HAL_DMA_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_DMA_RESET_HANDLE_STATE, DMA_HandleTypeDef*);
#undef __HAL_DMA_ENABLE
FAKE_VOID_FUNC(__HAL_DMA_ENABLE, DMA_HandleTypeDef*);
#undef __HAL_DMA_DISABLE
FAKE_VOID_FUNC(__HAL_DMA_DISABLE, DMA_HandleTypeDef*);
#undef __HAL_DMA_GET_TC_FLAG_INDEX
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_TC_FLAG_INDEX, DMA_HandleTypeDef*);
#undef __HAL_DMA_GET_HT_FLAG_INDEX
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_HT_FLAG_INDEX, DMA_HandleTypeDef*);
#undef __HAL_DMA_GET_TE_FLAG_INDEX
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_TE_FLAG_INDEX, DMA_HandleTypeDef*);
#undef __HAL_DMA_GET_GI_FLAG_INDEX
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_GI_FLAG_INDEX, DMA_HandleTypeDef*);
#undef __HAL_DMA_GET_FLAG
FAKE_VALUE_FUNC(bool, __HAL_DMA_GET_FLAG, DMA_HandleTypeDef*, uint32_t);
#undef __HAL_DMA_CLEAR_FLAG
FAKE_VOID_FUNC(__HAL_DMA_CLEAR_FLAG, DMA_HandleTypeDef*, uint32_t);
#undef __HAL_DMA_ENABLE_IT
FAKE_VOID_FUNC(__HAL_DMA_ENABLE_IT, DMA_HandleTypeDef*, uint32_t);
#undef __HAL_DMA_DISABLE_IT
FAKE_VOID_FUNC(__HAL_DMA_DISABLE_IT, DMA_HandleTypeDef*, uint32_t);
#undef __HAL_DMA_GET_IT_SOURCE
FAKE_VALUE_FUNC(bool, __HAL_DMA_GET_IT_SOURCE, DMA_HandleTypeDef*, uint32_t);
#undef __HAL_DMA_GET_COUNTER
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_COUNTER, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Init, DMA_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_DeInit, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Start, DMA_HandleTypeDef*, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Start_IT, DMA_HandleTypeDef*, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Abort, DMA_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Abort_IT, DMA_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_PollForTransfer, DMA_HandleTypeDef*, uint32_t, uint32_t);
FAKE_VOID_FUNC(HAL_DMA_IRQHandler, DMA_HandleTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_UnRegisterCallback, DMA_HandleTypeDef*, HAL_DMA_CallbackIDTypeDef);

FAKE_VALUE_FUNC(HAL_DMA_StateTypeDef, HAL_DMA_GetState, DMA_HandleTypeDef*);
FAKE_VALUE_FUNC(uint32_t, HAL_DMA_GetError, DMA_HandleTypeDef*);

/* Mock-ups for the provided interfaces */

#endif /* STM32L4XX_HAL_DMA_TH_HH_ */
