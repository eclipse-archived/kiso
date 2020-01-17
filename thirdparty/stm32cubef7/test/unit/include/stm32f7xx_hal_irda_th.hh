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
 * Mock-up implementation for the stm32f7xx_hal_irda module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_IRDA_TH_HH
#define STM32F7XX_HAL_IRDA_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_irda.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Init, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_DeInit, IRDA_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_IRDA_MspInit, IRDA_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_IRDA_MspDeInit, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Transmit, IRDA_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Receive, IRDA_HandleTypeDef*, uint8_t*, uint16_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Transmit_IT, IRDA_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Receive_IT, IRDA_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Transmit_DMA, IRDA_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Receive_DMA, IRDA_HandleTypeDef*, uint8_t*, uint16_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_DMAPause, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_DMAResume, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_DMAStop, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Abort, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_AbortTransmit, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_AbortReceive, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_Abort_IT, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_AbortTransmit_IT, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IRDA_AbortReceive_IT, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_IRDA_StateTypeDef, HAL_IRDA_GetState, IRDA_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_IRDA_GetError, IRDA_HandleTypeDef*);

# endif /* STM32F7XX_HAL_IRDA_TH_HH */