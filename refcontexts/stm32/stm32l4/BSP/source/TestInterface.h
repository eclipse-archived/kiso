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
 * @ingroup BSP_BSE_TEST_IF* @{
 *  @file
 *
 *  @brief Handling routines provided for test interface related interrupts.
 */

#ifndef TESTINTERFACE_H_
#define TESTINTERFACE_H_

#include "BCDS_BSP_TestInterface.h"

#if BCDS_FEATURE_BSP_TEST_INTERFACE

/**
 * @brief UART interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void TestInterface_UARTISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void TestInterface_UART_DMARxISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void TestInterface_UART_DMATxISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E TestInterface_getState(void);

#endif /* BCDS_FEATURE_BSP_TEST_INTERFACE */

#endif /* TESTINTERFACE_H_ */
/**@}*//* @ingroup BSP_BSE_TEST_IF */
