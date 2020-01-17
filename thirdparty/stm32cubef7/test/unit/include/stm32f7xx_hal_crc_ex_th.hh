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
 * Mock-up implementation for the stm32f7xx_hal_crc_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_CRC_EX_TH_HH
#define STM32F7XX_HAL_CRC_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_crc_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_crc_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided macros */

/**
  * @brief  Set CRC output reversal
  * @param  __HANDLE__     CRC handle
  * @retval None.
  */
#undef  __HAL_CRC_OUTPUTREVERSAL_ENABLE
FAKE_VOID_FUNC(__HAL_CRC_OUTPUTREVERSAL_ENABLE, CRC_HandleTypeDef*);

/**
  * @brief  Unset CRC output reversal
  * @param  __HANDLE__     CRC handle
  * @retval None.
  */
#undef __HAL_CRC_OUTPUTREVERSAL_DISABLE
FAKE_VOID_FUNC(__HAL_CRC_OUTPUTREVERSAL_DISABLE, CRC_HandleTypeDef*);

/**
  * @brief  Set CRC non-default polynomial
  * @param  __HANDLE__     CRC handle
  * @param  __POLYNOMIAL__ 7, 8, 16 or 32-bit polynomial
  * @retval None.
  */
#undef __HAL_CRC_POLYNOMIAL_CONFIG
FAKE_VOID_FUNC(__HAL_CRC_POLYNOMIAL_CONFIG, CRC_HandleTypeDef*, uint32_t)

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRCEx_Polynomial_Set, CRC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRCEx_Input_Data_Reverse, CRC_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CRCEx_Output_Data_Reverse, CRC_HandleTypeDef*, uint32_t);

# endif /* STM32F7XX_HAL_CRC_EX_TH_HH */
