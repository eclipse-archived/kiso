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
