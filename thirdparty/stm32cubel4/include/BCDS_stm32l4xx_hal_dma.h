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

/**
 * @file 
 * @brief This file has custom implementations of the stm32cube's DMA related changes.
 *
 * @details This file is a lean implementation of the stm32l4xx_hal_dma.c file.
 * Not all the API's are not re-factored, yet.
 */

#ifndef BCDS_STM32L4XX_HAL_DMA_H_
#define BCDS_STM32L4XX_HAL_DMA_H_

/* Include all headers which are needed by this file. */
#include "stm32l4xx.h"
#include "stm32l4xx_hal_dma.h"

/**
 * @brief  Handle DMA interrupt request.
 * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
 *               the configuration information for the specified DMA Channel.
 * @param  channel: the DMA channel for which the interrupt request is to be serviced.
 */
void BCDS_HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma, uint32_t channel);

#endif /* BCDS_STM32L4XX_HAL_DMA_H_ */

