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
