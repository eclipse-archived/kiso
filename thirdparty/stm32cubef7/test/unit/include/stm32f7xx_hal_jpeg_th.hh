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
 * Mock-up implementation for the stm32f7xx_hal_jpeg module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_JPEG_TH_HH
#define STM32F7XX_HAL_JPEG_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_jpeg.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Init, JPEG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_DeInit, JPEG_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_JPEG_MspInit, JPEG_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_JPEG_MspDeInit, JPEG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_ConfigEncoding, JPEG_HandleTypeDef*, JPEG_ConfTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_GetInfo, JPEG_HandleTypeDef*, JPEG_ConfTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_EnableHeaderParsing, JPEG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_DisableHeaderParsing, JPEG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_SetUserQuantTables, JPEG_HandleTypeDef*, uint8_t*, uint8_t*, uint8_t*, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Encode, JPEG_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Decode, JPEG_HandleTypeDef*, uint8_t*, uint32_t, uint8_t *, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Encode_IT, JPEG_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Decode_IT, JPEG_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Encode_DMA, JPEG_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Decode_DMA, JPEG_HandleTypeDef*, uint8_t*, uint32_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Pause, JPEG_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Resume, JPEG_HandleTypeDef*, uint32_t);

FAKE_VOID_FUNC(HAL_JPEG_ConfigInputBuffer, JPEG_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VOID_FUNC(HAL_JPEG_ConfigOutputBuffer, JPEG_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_JPEG_Abort, JPEG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_JPEG_STATETypeDef, HAL_JPEG_GetState, JPEG_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_JPEG_GetError, JPEG_HandleTypeDef*);

#endif /* STM32F767xx ||  STM32F769xx ||  STM32F777xx ||  STM32F779xx */

# endif /* STM32F7XX_HAL_JPEG_TH_HH */
