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
 * Mock-up implementation for the stm32f7xx_hal_hcd module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_HCD_TH_HH
#define STM32F7XX_HAL_HCD_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_hcd.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_Init, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_DeInit, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_HC_Init, HCD_HandleTypeDef*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_HC_Halt, HCD_HandleTypeDef*, uint8_t);

FAKE_VOID_FUNC(HAL_HCD_MspInit, HCD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_HCD_MspDeInit, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_HC_SubmitRequest, HCD_HandleTypeDef*, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t*, uint16_t, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_ResetPort, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_Start, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_HCD_Stop, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HCD_StateTypeDef, HAL_HCD_GetState, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HCD_URBStateTypeDef, HAL_HCD_HC_GetURBState, HCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, HAL_HCD_HC_GetXferCount, HCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HCD_HCStateTypeDef, HAL_HCD_HC_GetState, HCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(uint32_t, HAL_HCD_GetCurrentFrame, HCD_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_HCD_GetCurrentSpeed, HCD_HandleTypeDef*);

# endif /* STM32F7XX_HAL_HCD_TH_HH */