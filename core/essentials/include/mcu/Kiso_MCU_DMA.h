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
 * @defgroup    KISO_HAL_MCU_DMA  MCU DMA
 * @ingroup     KISO_HAL_MCU_IF
 * @{
 *
 * @brief       DMA typedefs for DMA handles and channels.
 *
 * @details
 */

#ifndef MCU_KISO_MCU_DMA_H_
#define MCU_KISO_MCU_DMA_H_

#include "Kiso_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_DMA

/**
 * @brief       Typedef for the DMA handle which is used to identify the DMA controller's HW context to use the
 *              underlying DMA Controller structures are defined individually in the MCU specific HAL layer
 */
typedef HWHandle_T DMA_T;
/**
 * @brief       Typedef for the DMA Channel handle which is used to identify a controller's DMA channel the underlying
 *              DMA channel structures are defined individually in the MUC specific HAL layer
 */
typedef HWHandle_T DMA_Channel_T;

#endif /* KISO_FEATURE_DMA */
#endif /* MCU_KISO_MCU_DMA_H_ */
/** @} */
