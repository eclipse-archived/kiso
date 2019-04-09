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
 * @ingroup BSP_XDK110_UMTS_QUECTEL_UC20_BOARD
 * @{
 * @file
 * @brief UMTS Quectel UC20 cellular module board settings header file
 * @details This file holds static configurations which are meant to be
 * assigned to a peripheral at the XDK110_RD extension board at a certain point
 * in time for the Initialize, Enable, Disable, functions.
 * It is preferred to have only macro definitions in this file so that it
 * contains compile time necessary information only.
 */

#ifndef UMTS_QUECTEL_UC20_H
#define UMTS_QUECTEL_UC20_H

#if BCDS_FEATURE_BSP_UMTS_QUECTELUC20

#include "BCDS_BSP_UMTS_QuectelUC20.h"

#define UMTS_GPIO_CONNECT_DELAY_MS           UINT32_C(100)/**< Macro to represent the required delay after enabling power and reset pin */
#define UMTS_RESET_PROCESSING_DELAY_MS       UINT32_C(150)/**< Macro to represent the required delay during reset */
#define UMTS_POWER_ON_PROCESSING_DELAY_MS    UINT32_C(100)/**< Macro to represent the required delay  during Power ON*/
#define UMTS_POWER_OFF_PROCESSING_DELAY_MS   UINT32_C(1100)/**< Macro to represent the required delay  during Power OFF*/
#define UMTS_POWER_ON_PIN                    BSP_EXTENSIONPORT_GPIO_PA1/**< Macro to represent the Power ON pin*/
#define UMTS_RESET_PIN                       BSP_EXTENSIONPORT_GPIO_PE2/**< Macro to represent the Reset pin*/
#define UMTS_UART_BAUDRATE                   115200/**< Macro to represent UART baudrate */

#endif /* BCDS_FEATURE_BSP_UMTS_QUECTELUC20 */
#endif /* UMTS_QUECTEL_UC20_H */
