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
 * Mock-up implementation for the stm32f7xx_hal_pcd_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_PCD_EX_TH_HH
#define STM32F7XX_HAL_PCD_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_pcd_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_pcd_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCDEx_SetTxFiFo, PCD_HandleTypeDef*, uint8_t, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCDEx_SetRxFiFo, PCD_HandleTypeDef*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCDEx_ActivateLPM, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCDEx_DeActivateLPM, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCDEx_ActivateBCD, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCDEx_DeActivateBCD, PCD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_PCDEx_BCD_VBUSDetect, PCD_HandleTypeDef*);

# endif /* STM32F7XX_HAL_PCD_EX_TH_HH */