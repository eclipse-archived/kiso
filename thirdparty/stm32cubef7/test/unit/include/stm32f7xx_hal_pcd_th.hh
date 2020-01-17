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
 * Mock-up implementation for the stm32f7xx_hal_pcd module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_PCD_TH_HH
#define STM32F7XX_HAL_PCD_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_pcd.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_Init, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_DeInit, PCD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_PCD_MspInit, PCD_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_PCD_MspDeInit, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_Start, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_Stop, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_DevConnect, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_DevDisconnect, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_SetAddress, PCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_Open, PCD_HandleTypeDef*, uint8_t, uint16_t, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_Close, PCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_Receive, PCD_HandleTypeDef*, uint8_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_Transmit, PCD_HandleTypeDef*, uint8_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(uint16_t, HAL_PCD_EP_GetRxCount, PCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_SetStall, PCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_ClrStall, PCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_EP_Flush, PCD_HandleTypeDef*, uint8_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_ActivateRemoteWakeup, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PCD_DeActivateRemoteWakeup, PCD_HandleTypeDef*);

FAKE_VALUE_FUNC(PCD_StateTypeDef, HAL_PCD_GetState, PCD_HandleTypeDef*);

# endif /* STM32F7XX_HAL_PCD_TH_HH */