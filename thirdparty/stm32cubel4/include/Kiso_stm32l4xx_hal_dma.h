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

/**
 * @file 
 * @brief This file has custom implementations of the stm32cube's DMA related changes.
 *
 * @details This file is a lean implementation of the stm32l4xx_hal_dma.c file.
 * Not all the API's are not re-factored, yet.
 */

#ifndef KISO_STM32L4XX_HAL_DMA_H_
#define KISO_STM32L4XX_HAL_DMA_H_

/* Include all headers which are needed by this file. */
#include "stm32l4xx.h"
#include "stm32l4xx_hal_dma.h"

/**
 * @brief  Handle DMA interrupt request.
 * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA Channel.
 * @param  channel: the DMA channel for which the interrupt request is to be serviced.
 */
void KISO_HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma, uint32_t channel);

#endif /* KISO_STM32L4XX_HAL_DMA_H_ */
