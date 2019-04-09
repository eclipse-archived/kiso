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
 * @defgroup BSP_XDK110_BT_MODULE BT MODULE 9301
 * @ingroup BCDS_HAL_BSP_IMP
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for Bluetooth module related interrupts
 *
 */

#ifndef BT_EM9301_H
#define BT_EM9301_H

#include "BCDS_BSP_BT_EM9301.h"

#if BCDS_FEATURE_BSP_BT_EM9301

/**
 * @brief UART transmit interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void BT_EM9301_UART_TxISR(void);

/**
 * @brief UART receive interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void BT_EM9301_UART_RxISR(void);
/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void BT_EM9301_UART_DMARxISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void BT_EM9301_UART_DMATxISR(void);

#endif /* BCDS_FEATURE_BSP_BT_EM9301 */

#endif /* BT_EM9301_H */

/**@}*/

