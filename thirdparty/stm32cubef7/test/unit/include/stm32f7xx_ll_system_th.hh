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
 * Mock-up implementation for the stm32f7xx_ll_system module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_LL_SYSTEM_TH_HH
#define STM32F7XX_LL_SYSTEM_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_LL_SYSTEM_H

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(LL_SYSCFG_EnableFMCMemorySwapping);

FAKE_VOID_FUNC(LL_SYSCFG_DisableFMCMemorySwapping);

FAKE_VOID_FUNC(LL_SYSCFG_EnableCompensationCell);

FAKE_VOID_FUNC(LL_SYSCFG_DisableCompensationCell);

FAKE_VALUE_FUNC(uint32_t, LL_SYSCFG_IsActiveFlag_CMPCR);

FAKE_VALUE_FUNC(uint32_t, LL_SYSCFG_GetRemapMemoryBoot);

FAKE_VOID_FUNC(LL_SYSCFG_SetPHYInterface, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SYSCFG_GetPHYInterface);

FAKE_VOID_FUNC(LL_SYSCFG_SetFlashBankMode, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SYSCFG_GetFlashBankMode);

FAKE_VOID_FUNC(LL_SYSCFG_EnableFastModePlus, uint32_t);

FAKE_VOID_FUNC(LL_SYSCFG_DisableFastModePlus, uint32_t);

FAKE_VOID_FUNC(LL_SYSCFG_SetEXTISource, uint32_t, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SYSCFG_GetEXTISource, uint32_t);

FAKE_VOID_FUNC(LL_SYSCFG_SetTIMBreakInputs, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_SYSCFG_GetTIMBreakInputs);

FAKE_VALUE_FUNC(uint32_t, LL_DBGMCU_GetDeviceID);

FAKE_VALUE_FUNC(uint32_t, LL_DBGMCU_GetRevisionID);

FAKE_VOID_FUNC(LL_DBGMCU_EnableDBGSleepMode);

FAKE_VOID_FUNC(LL_DBGMCU_DisableDBGSleepMode);

FAKE_VOID_FUNC(LL_DBGMCU_EnableDBGStopMode);

FAKE_VOID_FUNC(LL_DBGMCU_DisableDBGStopMode);

FAKE_VOID_FUNC(LL_DBGMCU_EnableDBGStandbyMode);

FAKE_VOID_FUNC(LL_DBGMCU_DisableDBGStandbyMode);

FAKE_VOID_FUNC(LL_DBGMCU_SetTracePinAssignment, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_DBGMCU_GetTracePinAssignment);

FAKE_VOID_FUNC(LL_DBGMCU_APB1_GRP1_FreezePeriph, uint32_t);

FAKE_VOID_FUNC(LL_DBGMCU_APB1_GRP1_UnFreezePeriph, uint32_t);

FAKE_VOID_FUNC(LL_DBGMCU_APB2_GRP1_FreezePeriph, uint32_t);

FAKE_VOID_FUNC(LL_DBGMCU_APB2_GRP1_UnFreezePeriph, uint32_t);

FAKE_VOID_FUNC(LL_FLASH_SetLatency, uint32_t);

FAKE_VALUE_FUNC(uint32_t, LL_FLASH_GetLatency);

FAKE_VOID_FUNC(LL_FLASH_EnablePrefetch);

FAKE_VOID_FUNC(LL_FLASH_DisablePrefetch);

FAKE_VALUE_FUNC(uint32_t, LL_FLASH_IsPrefetchEnabled);

FAKE_VOID_FUNC(LL_FLASH_EnableART);

FAKE_VOID_FUNC(LL_FLASH_DisableART);

FAKE_VOID_FUNC(LL_FLASH_EnableARTReset);

FAKE_VOID_FUNC(LL_FLASH_DisableARTReset);

# endif /* STM32F7XX_LL_SYSTEM_TH_HH */
