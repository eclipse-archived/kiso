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

#ifndef STM32L4XX_HAL_RCC_EX_TH_HH_
#define STM32L4XX_HAL_RCC_EX_TH_HH_

/* include or ban the real interface header */
#include "stm32l4xx_hal_rcc_ex.h"

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_PeriphCLKConfig, RCC_PeriphCLKInitTypeDef*);
FAKE_VOID_FUNC(HAL_RCCEx_GetPeriphCLKConfig, RCC_PeriphCLKInitTypeDef*);
FAKE_VALUE_FUNC(uint32_t, HAL_RCCEx_GetPeriphCLKFreq, uint32_t);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_EnablePLLSAI1, RCC_PLLSAI1InitTypeDef*);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_RCCEx_DisablePLLSAI1);

FAKE_VOID_FUNC(HAL_RCCEx_WakeUpStopCLKConfig, uint32_t);
FAKE_VOID_FUNC(HAL_RCCEx_StandbyMSIRangeConfig, uint32_t);
FAKE_VOID_FUNC(HAL_RCCEx_EnableLSECSS);
FAKE_VOID_FUNC(HAL_RCCEx_DisableLSECSS);
FAKE_VOID_FUNC(HAL_RCCEx_EnableLSECSS_IT);
FAKE_VOID_FUNC(HAL_RCCEx_LSECSS_IRQHandler);
FAKE_VOID_FUNC(HAL_RCCEx_EnableLSCO, uint32_t);
FAKE_VOID_FUNC(HAL_RCCEx_DisableLSCO);
FAKE_VOID_FUNC(HAL_RCCEx_EnableMSIPLLMode);
FAKE_VOID_FUNC(HAL_RCCEx_DisableMSIPLLMode);

#if defined(CRS)
FAKE_VOID_FUNC(HAL_RCCEx_CRSConfig, RCC_CRSInitTypeDef*);
FAKE_VOID_FUNC(HAL_RCCEx_CRSSoftwareSynchronizationGenerate);
FAKE_VOID_FUNC(HAL_RCCEx_CRSGetSynchronizationInfo, RCC_CRSSynchroInfoTypeDef*);
FAKLE_VALUE_FUNC(uint32_t, HAL_RCCEx_CRSWaitSynchronization, uint32_t);
FAKE_VOID_FUNC(HAL_RCCEx_CRS_IRQHandler);
#endif /* CRS */

#endif /* STM32L4XX_HAL_RCC_EX_TH_HH_ */
