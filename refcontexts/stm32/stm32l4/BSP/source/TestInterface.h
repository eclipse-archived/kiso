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
