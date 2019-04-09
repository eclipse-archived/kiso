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
 * @defgroup BSP_XDK110_SD_CARD SD_CARD
 * @ingroup BCDS_HAL_BSP_IMP
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for SD Card related interrupts
 *
 */

#ifndef SDCard_H
#define SDCard_H

#include "BCDS_BSP_SD_Card.h"

#if BCDS_FEATURE_BSP_SD_CARD

/**
 * @brief SPI receive interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void SDCard_SPIRxISR(void);
/**
 * @brief SPI transmit interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void SDCard_SPITxISR(void);

/**
 * @brief SPI rx dma interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void SDCard_SPI_RxDMAComplete(uint32_t channel, bool primary, void * user);

/**
 * @brief SPI tx dma interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void SDCard_SPI_TxDMAComplete(uint32_t channel, bool primary, void * user);


#endif /* BCDS_FEATURE_BSP_SD_CARD */

#endif /* SDCard_H */

/**@}*/
