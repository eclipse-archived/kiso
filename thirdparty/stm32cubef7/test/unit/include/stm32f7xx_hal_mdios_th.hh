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
 * Mock-up implementation for the stm32f7xx_hal_mdios module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_MDIOS_TH_HH
#define STM32F7XX_HAL_MDIOS_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_mdios.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_Init, MDIOS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_DeInit, MDIOS_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_MDIOS_MspInit, MDIOS_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_MDIOS_MspDeInit, MDIOS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_WriteReg, MDIOS_HandleTypeDef*, uint32_t, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_ReadReg, MDIOS_HandleTypeDef*, uint32_t, uint16_t*);

FAKE_VALUE_FUNC(uint32_t, HAL_MDIOS_GetWrittenRegAddress, MDIOS_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_MDIOS_GetReadRegAddress, MDIOS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_ClearWriteRegAddress, MDIOS_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_ClearReadRegAddress, MDIOS_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_MDIOS_EnableEvents, MDIOS_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_MDIOS_GetError, MDIOS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_MDIOS_StateTypeDef, HAL_MDIOS_GetState, MDIOS_HandleTypeDef*);

# endif /* STM32F7XX_HAL_MDIOS_TH_HH */