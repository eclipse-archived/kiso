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

 * @ingroup BSP_BT_MODULE
 * @{
 *  @file
 *  @brief Handling routines provided for Bluetooth module related interrupts.
 */

#ifndef BT_EM9301_H
#define BT_EM9301_H

#include "BCDS_BSP_BT_EM9301.h"

#if BCDS_FEATURE_BSP_BT_EM9301

#include "BCDS_stm32l4xx_hal_dma.h"

/**
 * @brief UART interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void BT_EM9301_UARTISR(void);

void BT_EM9301_UARTTxIsr(void);

void BT_EM9301_UARTRxIsr(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E BT_EM9301_getState(void);

#endif /* BCDS_FEATURE_BSP_BT_EM9301 */
#endif /* BT_EM9301_H */
/**@}*//* @ingroup BSP_BT_MODULE */
