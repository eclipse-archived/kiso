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
 * Mock-up implementation for the stm32f7xx_hal_dma2d module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DMA2D_TH_HH
#define STM32F7XX_HAL_DMA2D_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dma2d.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_Init, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_DeInit, DMA2D_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DMA2D_MspInit, DMA2D_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DMA2D_MspDeInit, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_Start, DMA2D_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_BlendingStart, DMA2D_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_Start_IT, DMA2D_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_BlendingStart_IT, DMA2D_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_Suspend, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_Resume, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_Abort, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_EnableCLUT, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_CLUTLoad, DMA2D_HandleTypeDef*, DMA2D_CLUTCfgTypeDef, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_CLUTLoad_IT, DMA2D_HandleTypeDef*, DMA2D_CLUTCfgTypeDef, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_CLUTLoading_Abort, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_CLUTLoading_Suspend, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_CLUTLoading_Resume, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_PollForTransfer, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_ConfigLayer, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_ConfigCLUT, DMA2D_HandleTypeDef*, DMA2D_CLUTCfgTypeDef, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_ProgramLineEvent, DMA2D_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_EnableDeadTime, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_DisableDeadTime, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DMA2D_ConfigDeadTime, DMA2D_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_DMA2D_StateTypeDef, HAL_DMA2D_GetState, DMA2D_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_DMA2D_GetError, DMA2D_HandleTypeDef*);

# endif /* STM32F7XX_HAL_DMA2D_TH_HH */