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
 * @defgroup BSP_XDK110_WIFI_CC3100MOD WIFI_CC3100MOD
 * @ingroup BCDS_HAL_BSP_IMP
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for WiFi module related interrupts
 *
 */

#ifndef WiFi_cc3100mod_H
#define WiFi_cc3100mod_H

#include "BCDS_BSP_WiFi_CC3100MOD.h"

#if BCDS_FEATURE_BSP_WIFI_CC3100MOD

/**
 * @brief SPI receive interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void WiFi_CC3100MOD_SPIRxISR(void);
/**
 * @brief SPI transmit interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void WiFi_CC3100MOD_SPITxISR(void);


/**
 * @brief SPI rx dma interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void WiFi_CC3100MOD_SPI_RxDMAComplete(uint32_t channel, bool primary, void * user);

/**
 * @brief SPI tx dma interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void WiFi_CC3100MOD_SPI_TxDMAComplete(uint32_t channel, bool primary, void * user);

#endif /* BCDS_FEATURE_BSP_WIFI_CC3100MOD */

#endif /* WiFi_cc3100mod_H */

/**@}*/
