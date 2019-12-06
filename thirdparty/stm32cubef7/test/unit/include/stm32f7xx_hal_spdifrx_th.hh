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
 * Mock-up implementation for the stm32f7xx_hal_spdifrx module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SPDIFRX_TH_HH
#define STM32F7XX_HAL_SPDIFRX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_spdifrx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_Init, SPDIFRX_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_DeInit, SPDIFRX_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SPDIFRX_MspInit, SPDIFRX_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SPDIFRX_MspDeInit, SPDIFRX_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_SetDataFormat, SPDIFRX_HandleTypeDef*, SPDIFRX_SetDataFormatTypeDef);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_ReceiveDataFlow, SPDIFRX_HandleTypeDef*, uint32_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_ReceiveControlFlow, SPDIFRX_HandleTypeDef*, uint32_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_ReceiveControlFlow_IT, SPDIFRX_HandleTypeDef*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_ReceiveDataFlow_IT, SPDIFRX_HandleTypeDef*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_ReceiveControlFlow_DMA, SPDIFRX_HandleTypeDef*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_ReceiveDataFlow_DMA, SPDIFRX_HandleTypeDef*, uint32_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPDIFRX_DMAStop, SPDIFRX_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_SPDIFRX_StateTypeDef, HAL_SPDIFRX_GetState, SPDIFRX_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_SPDIFRX_GetError, SPDIFRX_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SPDIFRX_TH_HH */