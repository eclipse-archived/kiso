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
 * Mock-up implementation for the stm32f7xx_hal_dac_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_DAC_EX_TH_HH
#define STM32F7XX_HAL_DAC_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_dac_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_dac_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(uint32_t, HAL_DACEx_DualGetValue, DAC_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DACEx_TriangleWaveGenerate, DAC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DACEx_NoiseWaveGenerate, DAC_HandleTypeDef*, uint32_t, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DACEx_DualSetValue, DAC_HandleTypeDef*, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(DAC_DMAConvCpltCh2, DMA_HandleTypeDef*);

FAKE_VOID_FUNC(DAC_DMAErrorCh2, DMA_HandleTypeDef*);

FAKE_VOID_FUNC(DAC_DMAHalfConvCpltCh2, DMA_HandleTypeDef*);

# endif /* STM32F7XX_HAL_DAC_EX_TH_HH */