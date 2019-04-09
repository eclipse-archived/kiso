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


/* header definition ******************************************************** */
#ifndef BLEAPP_H_
#define BLEAPP_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */

/* additional interface header files */
#include "BCDS_Retcode.h"

/**
 * @brief   Enumeration to represent the Bluetooth event.
 */
enum BleApp_Event_E
{
	BLE_APP_EVENT_CONFIGURED,
	BLE_APP_EVENT_CONNECTED,
	BLE_APP_EVENT_DISCONNECTED,
};

/**
 * @brief   Typedef to represent the Bluetooth event code.
 */
typedef enum BleApp_Event_E BleApp_Event_T;

/**
 * @brief Callback function signature to be called on BLE event
 */
typedef void (*BleApp_EventCallback)(BleApp_Event_T Event);

/* public function prototype declarations */

/**
 * @brief This function will initialize the Ble Application.
 *
 * @return  RETCODE_OK on success or an error code otherwise.
 */
Retcode_T BleApp_Initialize(void);

/**
 * @brief This function will unintialize the Ble Application .
 *
 * @return  RETCODE_OK on success or an error code otherwise.
 */
Retcode_T BleApp_Deinitialize(void);

/* public global variable declarations */

/* inline function definitions */

/* function prototypes ****************************************************** */

#endif /* BLEAPP_H_ */

/** ************************************************************************* */
