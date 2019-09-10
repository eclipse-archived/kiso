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
 * Mock-up implementation for the stm32l4xx_hal_pwr module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32L4XX_HAL_PWR_TH_HH
#define STM32L4XX_HAL_PWR_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32l4xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32l4xx_hal_pwr.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(HAL_PWR_DeInit)

FAKE_VOID_FUNC(HAL_PWR_EnableBkUpAccess)

FAKE_VOID_FUNC(HAL_PWR_DisableBkUpAccess)

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_PWR_ConfigPVD, PWR_PVDTypeDef*)

FAKE_VOID_FUNC(HAL_PWR_EnablePVD)

FAKE_VOID_FUNC(HAL_PWR_DisablePVD)

FAKE_VOID_FUNC(HAL_PWR_EnableWakeUpPin, uint32_t)

FAKE_VOID_FUNC(HAL_PWR_DisableWakeUpPin, uint32_t)

FAKE_VOID_FUNC(HAL_PWR_EnterSTOPMode, uint32_t, uint8_t)

FAKE_VOID_FUNC(HAL_PWR_EnterSLEEPMode, uint32_t, uint8_t)

FAKE_VOID_FUNC(HAL_PWR_EnterSTANDBYMode)

FAKE_VOID_FUNC(HAL_PWR_EnableSleepOnExit)

FAKE_VOID_FUNC(HAL_PWR_DisableSleepOnExit)

FAKE_VOID_FUNC(HAL_PWR_EnableSEVOnPend)

FAKE_VOID_FUNC(HAL_PWR_DisableSEVOnPend)

# endif /* STM32L4XX_HAL_PWR_TH_HH */