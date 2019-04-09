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
 * @ingroup BSP_XDK110_LoRa_MODULE
 * @{
 */
/**
 *
 *  @file
 *
 *  @brief Handling routines provided for LoRa module related interrupts
 *
 */

#ifndef LoRa_SX1272_H
#define LoRa_SX1272_H

#include "BCDS_BSP_LoRa_SX1272.h"

#if BCDS_FEATURE_BSP_LORA_SX1272

/**
 * @brief USART transmit interrupt service routine used to capture interrupts at the USART
 * resource and to forward them to the MCU driver for processing.
 */
void LoRa_SX1272_SPITxISR(void);

/**
 * @brief USART receive interrupt service routine used to capture interrupts at the USART
 * resource and to forward them to the MCU driver for processing.
 */
void LoRa_SX1272_SPIRxISR(void);

HWHandle_T BSP_LoRa_SX1272_GetSPIHandle(void);

#endif /* BCDS_FEATURE_BSP_LoRa_SX1272 */

#endif /* LoRa_SX1272_H */

/**@}*/

