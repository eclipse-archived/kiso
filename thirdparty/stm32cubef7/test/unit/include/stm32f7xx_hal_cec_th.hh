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
 * Mock-up implementation for the stm32f7xx_hal_cec module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_CEC_TH_HH
#define STM32F7XX_HAL_CEC_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_cec.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#if defined (CEC)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CEC_Init, CEC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CEC_DeInit, CEC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CEC_SetDeviceAddress, CEC_HandleTypeDef*, uint16_t);

FAKE_VOID_FUNC(HAL_CEC_MspInit, CEC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CEC_MspDeInit, CEC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CEC_Transmit_IT, CEC_HandleTypeDef*, uint8_t, uint8_t, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_CEC_GetLastReceivedFrameSize, CEC_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_CEC_ChangeRxBuffer, CEC_HandleTypeDef*, uint8_t*);

FAKE_VALUE_FUNC(HAL_CEC_StateTypeDef, HAL_CEC_GetState, CEC_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_CEC_GetError, CEC_HandleTypeDef*);

#endif /* CEC */

# endif /* STM32F7XX_HAL_CEC_TH_HH */
