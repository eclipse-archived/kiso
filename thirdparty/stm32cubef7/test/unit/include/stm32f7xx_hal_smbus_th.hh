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
 * Mock-up implementation for the stm32f7xx_hal_smbus module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_SMBUS_TH_HH
#define STM32F7XX_HAL_SMBUS_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_smbus.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_Init, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_DeInit, SMBUS_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SMBUS_MspInit, SMBUS_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_SMBUS_MspDeInit, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_ConfigAnalogFilter, SMBUS_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_ConfigDigitalFilter, SMBUS_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_IsDeviceReady, SMBUS_HandleTypeDef*, uint16_t, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_Master_Transmit_IT, SMBUS_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_Master_Receive_IT, SMBUS_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_Master_Abort_IT, SMBUS_HandleTypeDef*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_Slave_Transmit_IT, SMBUS_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_Slave_Receive_IT, SMBUS_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_EnableAlert_IT, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_DisableAlert_IT, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_EnableListen_IT, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SMBUS_DisableListen_IT, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_SMBUS_GetState, SMBUS_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_SMBUS_GetError, SMBUS_HandleTypeDef*);

# endif /* STM32F7XX_HAL_SMBUS_TH_HH */