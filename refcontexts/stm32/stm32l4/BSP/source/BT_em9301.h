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
