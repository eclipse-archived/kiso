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
 * Mock-up implementation for the stm32f7xx_ll_iwdg module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_IWDG_TH_HH
#define STM32F7XX_LL_IWDG_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_IWDG_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_IWDG_Enable, IWDG_TypeDef*);

FAKE_VOID_FUNC(LL_IWDG_ReloadCounter, IWDG_TypeDef*);

FAKE_VOID_FUNC(LL_IWDG_EnableWriteAccess, IWDG_TypeDef*);

FAKE_VOID_FUNC(LL_IWDG_DisableWriteAccess, IWDG_TypeDef*);

FAKE_VOID_FUNC(LL_IWDG_SetPrescaler, IWDG_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_GetPrescaler, IWDG_TypeDef*);

FAKE_VOID_FUNC(LL_IWDG_SetReloadCounter, IWDG_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_GetReloadCounter, IWDG_TypeDef*);

FAKE_VOID_FUNC(LL_IWDG_SetWindow, IWDG_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_GetWindow, IWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_IsActiveFlag_PVU, IWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_IsActiveFlag_RVU, IWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_IsActiveFlag_WVU, IWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_IWDG_IsReady, IWDG_TypeDef*);

# endif /* STM32F7XX_LL_IWDG_TH_HH */
