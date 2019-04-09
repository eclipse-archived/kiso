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


/*
*
* @brief
* Defines MCU DMA handles
*
* @details
* MCU functions do always have at least an integer parameter of type DMA_T.
* This integer is casted by MCU to a pointer to MCU_DMA_handle_T.
* The data pointed to by USART_TypeDef* is managed in BSP.
*/

#ifndef MCU_EFM32_BCDS_MCU_DMA_HANDLE_H_
#define MCU_EFM32_BCDS_MCU_DMA_HANDLE_H_

#include "BCDS_HAL.h"
#include "em_dma.h"


struct MCU_DMA_Controller_S
{
	DMA_TypeDef * DmaInstance;
	void * DmaDescriptors;
};

struct MCU_DMA_Channel_S
{
	uint32_t ChannelId;                    /**< an ID that represents the channel        */
	struct DMA_Controller_S * Controller;  /**< pointer to the associated DMA controller */
	DMA_CfgChannel_TypeDef    Config;      /**< channel configuration */
	DMA_CB_TypeDef            CallBack;    /**< interrupt callback function structure*/
	void * Link1;                          /**< general component link1 */
	void * Link2;                          /**< general component link2 */
};

#endif /* MCU_EFM32_BCDS_MCU_DMA_HANDLE_H_ */
