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
 * Mock-up implementation for the stm32f7xx_ll_rng module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_RNG_TH_HH
#define STM32F7XX_LL_RNG_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_RNG_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_RNG_Enable, RNG_TypeDef*);

FAKE_VOID_FUNC(LL_RNG_Disable, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsEnabled, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsActiveFlag_DRDY, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsActiveFlag_CECS, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsActiveFlag_SECS, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsActiveFlag_CEIS, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsActiveFlag_SEIS, RNG_TypeDef*);

FAKE_VOID_FUNC(LL_RNG_ClearFlag_CEIS, RNG_TypeDef*);

FAKE_VOID_FUNC(LL_RNG_ClearFlag_SEIS, RNG_TypeDef*);

FAKE_VOID_FUNC(LL_RNG_EnableIT, RNG_TypeDef*);

FAKE_VOID_FUNC(LL_RNG_DisableIT, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_IsEnabledIT, RNG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_RNG_ReadRandData32, RNG_TypeDef*);

FAKE_VALUE_FUNC(ErrorStatus, LL_RNG_DeInit, RNG_TypeDef*);

# endif /* STM32F7XX_LL_RNG_TH_HH */
