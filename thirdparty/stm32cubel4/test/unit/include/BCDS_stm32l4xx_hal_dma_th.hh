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
 
#ifndef BCDS_STM32L4XX_HAL_DMA_TH_HH_
#define BCDS_STM32L4XX_HAL_DMA_TH_HH_

/* ban or wrap the real interface header */
#include "BCDS_stm32l4xx_hal_dma.h"
#include "gtest.h"

/* Include the needed headers */

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(BCDS_HAL_DMA_IRQHandler, DMA_HandleTypeDef*, uint32_t);

#endif /* BCDS_STM32L4XX_HAL_DMA_TH_HH_ */
