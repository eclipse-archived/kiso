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
 * Mock-up implementation for the stm32f7xx_hal_qspi module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_QSPI_TH_HH
#define STM32F7XX_HAL_QSPI_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_qspi.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Init, QSPI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_DeInit, QSPI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_QSPI_MspInit, QSPI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_QSPI_MspDeInit, QSPI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Command, QSPI_HandleTypeDef*, QSPI_CommandTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Transmit, QSPI_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Receive, QSPI_HandleTypeDef*, uint8_t*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Command_IT, QSPI_HandleTypeDef*, QSPI_CommandTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Transmit_IT, QSPI_HandleTypeDef*, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Receive_IT, QSPI_HandleTypeDef*, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Transmit_DMA, QSPI_HandleTypeDef*, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Receive_DMA, QSPI_HandleTypeDef*, uint8_t*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_AutoPolling, QSPI_HandleTypeDef*, QSPI_CommandTypeDef*, QSPI_AutoPollingTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_AutoPolling_IT, QSPI_HandleTypeDef*, QSPI_CommandTypeDef*, QSPI_AutoPollingTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_MemoryMapped, QSPI_HandleTypeDef*, QSPI_CommandTypeDef*, QSPI_MemoryMappedTypeDef*);

FAKE_VALUE_FUNC(HAL_QSPI_StateTypeDef, HAL_QSPI_GetState, QSPI_HandleTypeDef*);

FAKE_VALUE_FUNC(uint32_t, HAL_QSPI_GetError, QSPI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Abort, QSPI_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_Abort_IT, QSPI_HandleTypeDef*);

FAKE_VOID_FUNC(HAL_QSPI_SetTimeout, QSPI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_QSPI_SetFifoThreshold, QSPI_HandleTypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, HAL_QSPI_GetFifoThreshold, QSPI_HandleTypeDef*);

# endif /* STM32F7XX_HAL_QSPI_TH_HH */