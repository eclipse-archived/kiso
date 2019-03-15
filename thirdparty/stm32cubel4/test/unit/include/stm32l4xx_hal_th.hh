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
 */
/*----------------------------------------------------------------------------*/

#ifndef STM32L4XX_HAL_TH_HH_
#define STM32L4XX_HAL_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_Init);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_DeInit);
FAKE_VOID_FUNC(HAL_MspInit);
FAKE_VOID_FUNC(HAL_MspDeInit);
FAKE_VOID_FUNC(HAL_PPP_MspInit);
FAKE_VOID_FUNC(HAL_PPP_MspDeInit);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_InitTick, uint32_t);

FAKE_VOID_FUNC(HAL_IncTick);
FAKE_VOID_FUNC(HAL_Delay, uint32_t);
FAKE_VALUE_FUNC(uint32_t, HAL_GetTick);
FAKE_VOID_FUNC(HAL_SuspendTick);
FAKE_VOID_FUNC(HAL_ResumeTick);
FAKE_VALUE_FUNC(uint32_t, HAL_GetHalVersion);
FAKE_VALUE_FUNC(uint32_t, HAL_GetREVID);
FAKE_VALUE_FUNC(uint32_t, HAL_GetDEVID);

FAKE_VOID_FUNC(HAL_DBGMCU_EnableDBGSleepMode);
FAKE_VOID_FUNC(HAL_DBGMCU_DisableDBGSleepMode);
FAKE_VOID_FUNC(HAL_DBGMCU_EnableDBGStopMode);
FAKE_VOID_FUNC(HAL_DBGMCU_DisableDBGStopMode);
FAKE_VOID_FUNC(HAL_DBGMCU_EnableDBGStandbyMode);
FAKE_VOID_FUNC(HAL_DBGMCU_DisableDBGStandbyMode);

FAKE_VOID_FUNC(HAL_SYSCFG_SRAM2Erase);
FAKE_VOID_FUNC(HAL_SYSCFG_EnableMemorySwappingBank);
FAKE_VOID_FUNC(HAL_SYSCFG_DisableMemorySwappingBank);

#if defined(VREFBUF)
FAKE_VOID_FUNC(HAL_SYSCFG_VREFBUF_VoltageScalingConfig, uint32_t);
FAKE_VOID_FUNC(HAL_SYSCFG_VREFBUF_HighImpedanceConfig, uint32_t);
FAKE_VOID_FUNC(HAL_SYSCFG_VREFBUF_TrimmingConfig, uint32_t);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SYSCFG_EnableVREFBUF);
FAKE_VOID_FUNC(HAL_SYSCFG_DisableVREFBUF);
#endif /* VREFBUF */

FAKE_VOID_FUNC(HAL_SYSCFG_EnableIOAnalogSwitchBooster);
FAKE_VOID_FUNC(HAL_SYSCFG_DisableIOAnalogSwitchBooster);

#endif /* STM32L4XX_HAL_TH_HH_ */
