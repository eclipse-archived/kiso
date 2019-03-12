/******************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *
 *  Interface header for the Ble Application module.
 *
 * ****************************************************************************/

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
