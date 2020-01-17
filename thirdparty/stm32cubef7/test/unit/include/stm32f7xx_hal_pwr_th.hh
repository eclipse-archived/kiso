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
 * Mock-up implementation for the stm32f7xx_hal_pwr module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_HAL_PWR_TH_HH
#define STM32F7XX_HAL_PWR_TH_HH

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_hal_th.hh"

/* Include or ban the real interface header */
#include "stm32f7xx_hal_pwr.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(HAL_PWR_DeInit);

FAKE_VOID_FUNC(HAL_PWR_EnableBkUpAccess);

FAKE_VOID_FUNC(HAL_PWR_DisableBkUpAccess);

FAKE_VOID_FUNC(HAL_PWR_ConfigPVD, PWR_PVDTypeDef*);

FAKE_VOID_FUNC(HAL_PWR_EnablePVD);

FAKE_VOID_FUNC(HAL_PWR_DisablePVD);

FAKE_VOID_FUNC(HAL_PWR_EnableWakeUpPin, uint32_t);

FAKE_VOID_FUNC(HAL_PWR_DisableWakeUpPin, uint32_t);

FAKE_VOID_FUNC(HAL_PWR_EnterSTOPMode, uint32_t, uint8_t);

FAKE_VOID_FUNC(HAL_PWR_EnterSLEEPMode, uint32_t, uint8_t);

FAKE_VOID_FUNC(HAL_PWR_EnterSTANDBYMode);

FAKE_VOID_FUNC(HAL_PWR_EnableSleepOnExit);

FAKE_VOID_FUNC(HAL_PWR_DisableSleepOnExit);

FAKE_VOID_FUNC(HAL_PWR_EnableSEVOnPend);

FAKE_VOID_FUNC(HAL_PWR_DisableSEVOnPend);

# endif /* STM32F7XX_HAL_PWR_TH_HH */