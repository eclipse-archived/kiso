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
 * Mock-up implementation for the stm32f7xx_hal_dcmi module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DCMI_TH_HH
#define STM32F7XX_HAL_DCMI_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dcmi.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_Init, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_DeInit, DCMI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DCMI_MspInit, DCMI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_DCMI_MspDeInit, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_Start_DMA, DCMI_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_Stop, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_Suspend, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_Resume, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_ConfigCrop, DCMI_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_EnableCrop, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DCMI_DisableCrop, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_DCMI_StateTypeDef, HAL_DCMI_GetState, DCMI_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_DCMI_GetError, DCMI_HandleTypeDef*);

# endif /* STM32F7XX_HAL_DCMI_TH_HH */