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
 * @ingroup KISO_CELLULAR_COMMON
 *
 * @defgroup HARDWARE Hardware
 * @{
 *
 * @brief Adapter layer to manage BSP variant management and UART communication.
 * @file
 */
#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Kiso_MCU_UART.h"

/**
 * @brief Initializes this module by calling the variant related BSP-connect
 * function and setting up internal values.
 *
 * @param[in] uartCallback
 * Callback to be invoked by the UART ISR.
 *
 * @param[in] uartRxByte
 * A pointer where to store received byte
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Hardware_Initialize(MCU_UART_Callback_T uartCallback, uint8_t *uartRxByte);

/**
 * @brief Powers on the cellular modem by invoking the respective BSP-enable
 * function.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Hardware_Startup(void);

/**
 * @brief Powers off the cellular modem by invoking the respective BSP-disable
 * function.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Hardware_Shutdown(void);

/**
 * @brief Resets the cellular modem by invoking the respective BSP-reset
 * function, if available.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Hardware_Reset(void);

/**
 * @brief Returns the handle for communicating with the modem. Before calling
 * #Hardware_Initialize() this method returns NULL. After successful
 * initialization this method returns the initialized UART handle.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Hardware_GetCommunicationChannel(UART_T *uartHandle);

/**
 * @brief Deinitialize the hardware layer.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Hardware_Deinitialize(void);

#endif /* HARDWARE_H_ */

/** @} */
