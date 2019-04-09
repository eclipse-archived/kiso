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
 * @ingroup BSP_GSM_MODULE
 * @{
 *  @file
 *  @brief Handling routines provided for GSM related interrupts.
 */

#ifndef GSM_SIM800H_H_
#define GSM_SIM800H_H_

#include "BCDS_BSP_GSM_SIM800H.h"

#if BCDS_FEATURE_BSP_GSM_SIM800H

/**
 * @brief UART interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void GSM_SIM800H_UARTISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * receive channel and to forward them to the MCU driver for processing.
 */
void GSM_SIM800H_DMARxISR(void);

/**
 * @brief UART interrupt service routine used to capture interrupts at the DMA
 * transmit channel and to forward them to the MCU driver for processing.
 */
void GSM_SIM800H_DMATxISR(void);

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E GSM_SIM800H_getState(void);

#endif /* BCDS_FEATURE_BSP_GSM_SIM800H */

#endif /* GSM_SIM800H_H_ */
/**@}*//* @ingroup BSP_GSM_MODULE */
