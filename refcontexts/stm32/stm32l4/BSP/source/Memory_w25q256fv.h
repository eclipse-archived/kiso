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
 * @ingroup BSP_MEMORY_MODULE
 * @{
 * @file
 * @brief  Handling routines provided for SPI External memory W25Q256FV interrupts.
 */

#ifndef MEMORY_W25Q256FV_H_
#define MEMORY_W25Q256FV_H_

#include "BCDS_BSP_Memory_W25Q256FV.h"

#if BCDS_FEATURE_BSP_MEMORY_W25Q256FV

/**
 * @brief SPI interrupt service routine used to capture interrupts at the SPI
 * resource and to forward them to the MCU driver for processing.
 */
void Mem_W25Q256FV_SPIISR(void);

/**
 * @brief Interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void Mem_W25Q256FV_SPI_DMARxISR(void);

/**
 * @brief Interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void Mem_W25Q256FV_SPI_DMATxISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E Mem_W25Q256FV_getState(void);

#endif /* BCDS_FEATURE_BSP_MEMORY_W25Q256FV */
#endif /* MEMORY_W25Q256FV_H_ */
/**@}*//* @ingroup BSP_MEMORY_MODULE */
