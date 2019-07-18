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

#ifndef STM32L4XX_HAL_DMA_TH_HH_
#define STM32L4XX_HAL_DMA_TH_HH_

/* Include the needed headers */
#include "gtest.h"
#include "stm32l4xx_hal_th.hh"

/* include or ban the real interface header */
#include "stm32l4xx_hal_iwdg.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided macros */

#undef __HAL_IWDG_START
FAKE_VOID_FUNC(__HAL_IWDG_START, IWDG_HandleTypeDef*);

#undef __HAL_IWDG_RELOAD_COUNTER
FAKE_VOID_FUNC(__HAL_IWDG_RELOAD_COUNTER, IWDG_HandleTypeDef*);

/* Mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IWDG_Init, IWDG_HandleTypeDef*);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_IWDG_Refresh, IWDG_HandleTypeDef*);

#endif /* STM32L4XX_HAL_DMA_TH_HH_ */
