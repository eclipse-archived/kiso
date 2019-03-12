/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

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
