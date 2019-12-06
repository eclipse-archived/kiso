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
 * Mock-up implementation for the stm32f7xx_ll_bus module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_BUS_TH_HH
#define STM32F7XX_LL_BUS_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_BUS_H

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_AHB1_GRP1_EnableClock, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_AHB1_GRP1_IsEnabledClock, uint32_t);

FAKE_VOID_FUNC(LL_AHB1_GRP1_DisableClock, uint32_t);

FAKE_VOID_FUNC(LL_AHB1_GRP1_ForceReset, uint32_t);

FAKE_VOID_FUNC(LL_AHB1_GRP1_ReleaseReset, uint32_t);

FAKE_VOID_FUNC(LL_AHB1_GRP1_EnableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_AHB1_GRP1_DisableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_AHB2_GRP1_EnableClock, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_AHB2_GRP1_IsEnabledClock, uint32_t);

FAKE_VOID_FUNC(LL_AHB2_GRP1_DisableClock, uint32_t);

FAKE_VOID_FUNC(LL_AHB2_GRP1_ForceReset, uint32_t);

FAKE_VOID_FUNC(LL_AHB2_GRP1_ReleaseReset, uint32_t);

FAKE_VOID_FUNC(LL_AHB2_GRP1_EnableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_AHB2_GRP1_DisableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_AHB3_GRP1_EnableClock, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_AHB3_GRP1_IsEnabledClock, uint32_t);

FAKE_VOID_FUNC(LL_AHB3_GRP1_DisableClock, uint32_t);

FAKE_VOID_FUNC(LL_AHB3_GRP1_ForceReset, uint32_t);

FAKE_VOID_FUNC(LL_AHB3_GRP1_ReleaseReset, uint32_t);

FAKE_VOID_FUNC(LL_AHB3_GRP1_EnableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_AHB3_GRP1_DisableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_APB1_GRP1_EnableClock, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_APB1_GRP1_IsEnabledClock, uint32_t);

FAKE_VOID_FUNC(LL_APB1_GRP1_DisableClock, uint32_t);

FAKE_VOID_FUNC(LL_APB1_GRP1_ForceReset, uint32_t);

FAKE_VOID_FUNC(LL_APB1_GRP1_ReleaseReset, uint32_t);

FAKE_VOID_FUNC(LL_APB1_GRP1_EnableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_APB1_GRP1_DisableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_APB2_GRP1_EnableClock, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_APB2_GRP1_IsEnabledClock, uint32_t);

FAKE_VOID_FUNC(LL_APB2_GRP1_DisableClock, uint32_t);

FAKE_VOID_FUNC(LL_APB2_GRP1_ForceReset, uint32_t);

FAKE_VOID_FUNC(LL_APB2_GRP1_ReleaseReset, uint32_t);

FAKE_VOID_FUNC(LL_APB2_GRP1_EnableClockLowPower, uint32_t);

FAKE_VOID_FUNC(LL_APB2_GRP1_DisableClockLowPower, uint32_t);

# endif /* STM32F7XX_LL_BUS_TH_HH */
