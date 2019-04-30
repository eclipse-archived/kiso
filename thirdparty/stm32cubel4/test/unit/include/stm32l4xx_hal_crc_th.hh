/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/
/**
 * @file
 *
 * Mock-up implementation for the stm32l4xx_hal_crc module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32L4XX_HAL_CRC_TH_HH
#define STM32L4XX_HAL_CRC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32l4xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32l4xx_hal_crc.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided macros */

/** @brief Reset CRC handle state
  * @param  __HANDLE__ CRC handle.
  * @retval None
  */
#undef __HAL_CRC_RESET_HANDLE_STATE
FAKE_VOID_FUNC(__HAL_CRC_RESET_HANDLE_STATE, CRC_HandleTypeDef*);

/**
  * @brief  Reset CRC Data Register.
  * @param  __HANDLE__ CRC handle
  * @retval None.
  */
#undef __HAL_CRC_DR_RESET
FAKE_VOID_FUNC(__HAL_CRC_DR_RESET, CRC_HandleTypeDef*);

/**
  * @brief  Set CRC INIT non-default value
  * @param  __HANDLE__     CRC handle
  * @param  __INIT__       32-bit initial value
  * @retval None.
  */
#undef __HAL_CRC_INITIALCRCVALUE_CONFIG
FAKE_VOID_FUNC(__HAL_CRC_INITIALCRCVALUE_CONFIG, CRC_HandleTypeDef*, uint32_t);

/**
  * @brief Stores a 8-bit data in the Independent Data(ID) register.
  * @param __HANDLE__ CRC handle
  * @param __VALUE__ 8-bit value to be stored in the ID register
  * @retval None
  */
#undef __HAL_CRC_SET_IDR
FAKE_VOID_FUNC(__HAL_CRC_SET_IDR, CRC_HandleTypeDef*, uint8_t);

/**
  * @brief Returns the 8-bit data stored in the Independent Data(ID) register.
  * @param __HANDLE__ CRC handle
  * @retval 8-bit value of the ID register
  */
#undef __HAL_CRC_GET_IDR
FAKE_VALUE_FUNC(uint8_t, __HAL_CRC_GET_IDR, CRC_HandleTypeDef*);

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRC_Init, CRC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRC_DeInit, CRC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CRC_MspInit, CRC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CRC_MspDeInit, CRC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_CRC_Accumulate, CRC_HandleTypeDef*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_CRC_Calculate, CRC_HandleTypeDef*, uint32_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_CRC_StateTypeDef, HAL_CRC_GetState, CRC_HandleTypeDef*);

# endif /* STM32L4XX_HAL_CRC_TH_HH */
