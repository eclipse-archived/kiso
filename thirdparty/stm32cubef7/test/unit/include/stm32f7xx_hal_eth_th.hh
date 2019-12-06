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
 * Mock-up implementation for the stm32f7xx_hal_eth module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_ETH_TH_HH
#define STM32F7XX_HAL_ETH_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_eth.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_Init, ETH_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_DeInit, ETH_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_ETH_MspInit, ETH_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_ETH_MspDeInit, ETH_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_DMATxDescListInit, ETH_HandleTypeDef*, ETH_DMADescTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_DMARxDescListInit, ETH_HandleTypeDef*, ETH_DMADescTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_TransmitFrame, ETH_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_GetReceivedFrame, ETH_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_ReadPHYRegister, ETH_HandleTypeDef*, uint16_t, uint32_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_WritePHYRegister, ETH_HandleTypeDef*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_GetReceivedFrame_IT, ETH_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_Start, ETH_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_Stop, ETH_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_ConfigMAC, ETH_HandleTypeDef*, ETH_MACInitTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_ETH_ConfigDMA, ETH_HandleTypeDef*, ETH_DMAInitTypeDef*);

FAKE_VALUE_FUNC(HAL_ETH_StateTypeDef, HAL_ETH_GetState, ETH_HandleTypeDef*);

# endif /* STM32F7XX_HAL_ETH_TH_HH */