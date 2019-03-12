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
 *  Configuration header for the BCDS Bluetooth components.
 *
 * ****************************************************************************/

#ifndef BCDS_BLUETOOTHCONFIG_H_
#define BCDS_BLUETOOTHCONFIG_H_

/* additional interface header files */

/* public interface declaration */

/* public type and macro definitions */

#define BLE_TASK_STACK_SIZE								(UINT32_C(440))
#define BLE_TASK_PRIORITY								(UINT32_C(3))

/**< Default value is set to 4 because of stack's internal send buffer size. Minimum size is 1
Note: If this is called based on callback synchronization, it shall equate to atleast the number of threads calling it */
#define BLE_SEND_QUEUE_SIZE								(UINT32_C(4))

/**< Number of paired devices the BLE stack can keep track. Minimum count is 1*/
#define BLE_PAIRED_DEVICE_COUNT 						(UINT8_C(1))

/**< Minimum size is 1. Maximum size is 256*/
#define BLE_DEFERRED_RX_BUFFER_SIZE 					(UINT32_C(256))


/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* BCDS_BLUETOOTHCONFIG_H_    */

/** ************************************************************************* */
