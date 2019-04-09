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
 * @ingroup BSP_GNSS_MODULE
 * @{
 *  @file
 *  @brief Handling routines provided for GNSS module related interrupts.
 */

#ifndef GNSS_G7020_H_
#define GNSS_G7020_H_

#include "BCDS_BSP_GNSS_G7020.h"

#if BCDS_FEATURE_BSP_GNSS_G7020

/**
 * @brief UART interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void GNSS_G7020_UARTISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void GNSS_G7020_UART_DMARxISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void GNSS_G7020_UART_DMATxISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E GNSS_G7020_getState(void);

#endif /* BCDS_FEATURE_BSP_GNSS_G7020 */
#endif /* GNSS_G7020_H_ */
/**@}*//* @ingroup BSP_GNSS_MODULE */
