/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/
/**
 * @file
 *
 * Mock-up implementation for the stm32l4xx_hal_pwr_ex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32L4XX_HAL_PWR_EX_TH_HH
#define STM32L4XX_HAL_PWR_EX_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32l4xx_hal_pwr_th.hh"

/* Include or ban the real interface header */
#include "stm32l4xx_hal_pwr_ex.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(uint32_t, HAL_PWREx_GetVoltageRange)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWREx_ControlVoltageScaling, uint32_t)

FAKE_VOID_FUNC(HAL_PWREx_EnableFlashPowerDown)

FAKE_VOID_FUNC(HAL_PWREx_DisableFlashPowerDown)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWREx_EnableBkUpReg)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWREx_DisableBkUpReg)

FAKE_VOID_FUNC(HAL_PWREx_EnableMainRegulatorLowVoltage)

FAKE_VOID_FUNC(HAL_PWREx_DisableMainRegulatorLowVoltage)

FAKE_VOID_FUNC(HAL_PWREx_EnableLowRegulatorLowVoltage)

FAKE_VOID_FUNC(HAL_PWREx_DisableLowRegulatorLowVoltage)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWREx_EnableOverDrive)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWREx_DisableOverDrive)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWREx_EnterUnderDriveSTOPMode, uint32_t, uint8_t)

# endif /* STM32L4XX_HAL_PWR_EX_TH_HH */