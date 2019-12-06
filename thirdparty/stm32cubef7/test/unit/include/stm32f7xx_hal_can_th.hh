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
 * Mock-up implementation for the stm32f7xx_hal_can module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_CAN_TH_HH
#define STM32F7XX_HAL_CAN_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_can.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (CAN1)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_Init, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_DeInit, CAN_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CAN_MspInit, CAN_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CAN_MspDeInit, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_ConfigFilter, CAN_HandleTypeDef*, CAN_FilterTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_Start, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_Stop, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_RequestSleep, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_WakeUp, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_CAN_IsSleepActive, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_AddTxMessage, CAN_HandleTypeDef*, CAN_TxHeaderTypeDef*, uint8_t*, uint32_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_AbortTxRequest, CAN_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_CAN_GetTxMailboxesFreeLevel, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_CAN_IsTxMessagePending, CAN_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_CAN_GetTxTimestamp, CAN_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_GetRxMessage, CAN_HandleTypeDef*, uint32_t, CAN_RxHeaderTypeDef*, uint8_t*);

FAKE_VALUE_FUNC(uint32_t, HAL_CAN_GetRxFifoFillLevel, CAN_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_ActivateNotification, CAN_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_DeactivateNotification, CAN_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_CAN_StateTypeDef, HAL_CAN_GetState, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_CAN_GetError, CAN_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_ResetError, CAN_HandleTypeDef*);

#endif /* CAN1 */

# endif /* STM32F7XX_HAL_CAN_TH_HH */
