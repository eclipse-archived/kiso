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
 * Mock-up implementation for the stm32f7xx_ll_cortex module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_CORTEX_TH_HH
#define STM32F7XX_LL_CORTEX_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_CORTEX_H

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(uint32_t, LL_SYSTICK_IsActiveCounterFlag);

FAKE_VOID_FUNC(LL_SYSTICK_SetClkSource, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SYSTICK_GetClkSource);

FAKE_VOID_FUNC(LL_SYSTICK_EnableIT);

FAKE_VOID_FUNC(LL_SYSTICK_DisableIT);

FAKE_VALUE_FUNC(uint32_t, LL_SYSTICK_IsEnabledIT);

FAKE_VOID_FUNC(LL_LPM_EnableSleep);

FAKE_VOID_FUNC(LL_LPM_EnableDeepSleep);

FAKE_VOID_FUNC(LL_LPM_EnableSleepOnExit);

FAKE_VOID_FUNC(LL_LPM_DisableSleepOnExit);

FAKE_VOID_FUNC(LL_LPM_EnableEventOnPend);

FAKE_VOID_FUNC(LL_LPM_DisableEventOnPend);

FAKE_VOID_FUNC(LL_HANDLER_EnableFault, uint32_t);

FAKE_VOID_FUNC(LL_HANDLER_DisableFault, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_CPUID_GetImplementer);

FAKE_VALUE_FUNC(uint32_t, LL_CPUID_GetVariant);

FAKE_VALUE_FUNC(uint32_t, LL_CPUID_GetConstant);

FAKE_VALUE_FUNC(uint32_t, LL_CPUID_GetParNo);

FAKE_VALUE_FUNC(uint32_t, LL_CPUID_GetRevision);

FAKE_VOID_FUNC(LL_MPU_Enable, uint32_t);

FAKE_VOID_FUNC(LL_MPU_Disable);

FAKE_VALUE_FUNC(uint32_t, LL_MPU_IsEnabled);

FAKE_VOID_FUNC(LL_MPU_EnableRegion, uint32_t);

FAKE_VOID_FUNC(LL_MPU_ConfigRegion, uint32_t, uint32_t, uint32_t, uint32_t);

FAKE_VOID_FUNC(LL_MPU_DisableRegion, uint32_t);

# endif /* STM32F7XX_LL_CORTEX_TH_HH */
