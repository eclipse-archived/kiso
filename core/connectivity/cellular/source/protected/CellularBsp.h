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
 * @file 
 *
 * @brief Controls board specific parts of the module.
 */


#ifndef CELLULAR_BSP_H
#define CELLULAR_BSP_H

#include "BCDS_MCU_UART.h"

/**
 * @brief Initializes this module by setting up GPIO pins and a serial interface.
 * After this method has been called successfully (returned RETCODE_OK),
 * CellularBsp_GetCommunicationChannel() returns a valid value.
 *
 * @param uartCallback         An application UART ISR callback
 * @param uartRxByte           A pointer where to store received byte
 *
 * @return RETCODE_OK          If the cellular hardware was initialized
 * @return RETCODE_FAILURE     If the serial initialization failed
 *
 */
Retcode_T CellularBsp_Init(MCU_UART_Callback_T uartCallback, uint8_t *uartRxByte);

/**
 * @brief Powers on the cellular modem by triggering the POWER_ON GPIO pin.
 * This method does not communicate with the modem, so it has to be
 * initialized afterwards. Make sure you register the UART RX IRQ before
 * calling this method, as it results in data being sent from the modem.
 *
 * @return  RETCODE_OK          If the HW pins were switched correctly.
 *                              This does not guarantee that the modem started.
 */
Retcode_T CellularBsp_Startup(void);

/**
 * @brief Powers off the cellular modem by triggering the POWER_ON GPIO pin.
 *
 * @return  RETCODE_OK          If the HW pins were switched correctly.
 *                              This does not guarantee that the modem was powered off.
 *                              Use the UART to check.
 */
Retcode_T CellularBsp_Shutdown(void);

/**
 * @brief Resets the cellular modem by triggering the RESET GPIO pin.
 * This method does not guarantee that the modem was correctly reset.
 * Verify the correct restart by listening on the UART.
 *
 * @return  RETCODE_OK          If the HW pins were switched correctly.
 */
Retcode_T CellularBsp_Reset(void);

/**
 * @brief Returns the handle for communicating with the modem. Before calling
 * CellularBsp_Init() this method returns NULL. After successful initialization
 * this method returns the initialized UART handle.
 *
 * @return  NULL or a serial device handle for communicating with the modem
 */
Retcode_T CellularBsp_GetCommunicationChannel(UART_T* uartHandle);

/** @brief   Deinitialize the module
 *
 * @return  RETCODE_OK if the module is successfully deinitialized, a retcode compose otherwise.
 *
 */
Retcode_T CellularBsp_Deinit(void);

#endif /* CELLULAR_BSP_H */
