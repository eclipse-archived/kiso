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
 * Mock-up implementation for the stm32f7xx_hal_dma module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DMA_TH_HH
#define STM32F7XX_HAL_DMA_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dma.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided macros */

#undef __HAL_DMA_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_DMA_RESET_HANDLE_STATE, DMA_HandleTypeDef*);

#undef __HAL_DMA_GET_FS
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_FS, DMA_HandleTypeDef*);

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

#undef __HAL_DMA_GET_FE_FLAG_INDEX
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_FE_FLAG_INDEX, DMA_HandleTypeDef*);

#undef __HAL_DMA_GET_DME_FLAG_INDEX
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_DME_FLAG_INDEX, DMA_HandleTypeDef*);

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

#undef __HAL_DMA_SET_COUNTER
FAKE_VOID_FUNC(__HAL_DMA_SET_COUNTER, DMA_HandleTypeDef*, uint16_t);

#undef __HAL_DMA_GET_COUNTER
FAKE_VALUE_FUNC(uint32_t, __HAL_DMA_GET_COUNTER, DMA_HandleTypeDef*);

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Init, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_DeInit, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Start, DMA_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Start_IT, DMA_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Abort, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_Abort_IT, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_PollForTransfer, DMA_HandleTypeDef*, HAL_DMA_LevelCompleteTypeDef, uint32_t);

FAKE_VOID_FUNC(HAL_DMA_IRQHandler, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA_UnRegisterCallback, DMA_HandleTypeDef*, HAL_DMA_CallbackIDTypeDef);

FAKE_VALUE_FUNC(HAL_DMA_StateTypeDef, HAL_DMA_GetState, DMA_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_DMA_GetError, DMA_HandleTypeDef*);

# endif /* STM32F7XX_HAL_DMA_TH_HH */
