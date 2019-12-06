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
 * Mock-up implementation for the stm32f7xx_ll_wwdg module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_WWDG_TH_HH
#define STM32F7XX_LL_WWDG_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_WWDG_H

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_WWDG_Enable, WWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_WWDG_IsEnabled, WWDG_TypeDef*);

FAKE_VOID_FUNC(LL_WWDG_SetCounter, WWDG_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_WWDG_GetCounter, WWDG_TypeDef*);

FAKE_VOID_FUNC(LL_WWDG_SetPrescaler, WWDG_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_WWDG_GetPrescaler, WWDG_TypeDef*);

FAKE_VOID_FUNC(LL_WWDG_SetWindow, WWDG_TypeDef*, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_WWDG_GetWindow, WWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_WWDG_IsActiveFlag_EWKUP, WWDG_TypeDef*);

FAKE_VOID_FUNC(LL_WWDG_ClearFlag_EWKUP, WWDG_TypeDef*);

FAKE_VOID_FUNC(LL_WWDG_EnableIT_EWKUP, WWDG_TypeDef*);

FAKE_VALUE_FUNC(uint32_t, LL_WWDG_IsEnabledIT_EWKUP, WWDG_TypeDef*);

# endif /* STM32F7XX_LL_WWDG_TH_HH */
