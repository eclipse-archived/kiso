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
 *  Interface header for the Bike Sensor eBike application module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_APP_H_
#define BCDS_APP_H_

/* additional interface header files */
#include "BCDS_Retcode.h"

/* public interface declaration ********************************************* */

/* public type and macro definitions */

/* public function prototype declarations */

/**
 * @brief Enumeration to represent the return codes of BikeSensor Application.
 */
enum App_Retcode_E
{
	/* main file error codes */
	RETCODE_MAIN_STARTUP_FAILED = RETCODE_FIRST_CUSTOM_CODE,

	/* can manager error codes */
	RETCODE_CAN_INIT_FAILED,
	RETCODE_CAN_TASK_CREATE_FAILED,
	RETCODE_CAN_INIT_NOT_DONE,
    RETCODE_CAN_DEVICE_SHUTDOWN_FAILED,

	/* system state machine file error codes */
	RETCODE_SSM_TIMER_START_FAILED,
	RETCODE_SSM_TIMER_STOP_FAILED,

	/* user interface error codes */
	RETCODE_UI_TIMER_CREATE_FAILED,
	RETCODE_UI_TIMER_START_FAILED,
	RETCODE_UI_TIMER_STOP_FAILED,
	RETCODE_UI_BUTTON_INIT_FAILED,
	RETCODE_UI_BUTTON_CONFIG_FAILED,
	RETCODE_UI_BUTTON_EVENT_UNKNOWN,
	RETCODE_UI_SHUTDOWN_CB_FAILED,

	/* eeprom manager error codes */
	RETCODE_EEP_EEPMA_INIT_FAILED,
    RETCODE_FSH_LOCK_FAILED,
    RETCODE_FSH_UNLOCK_FAILED,
    RETCODE_FSH_ERASE_FAILED,

    /* User Agent error codes */
    RETCODE_UA_INVALID_PARAM,

    /* can transfer error codes */
    RETCODE_CAN_TRANSFER_NULL_POINTER,
    RETCODE_CAN_TRANSFER_NULL_UNINITIALIZED,
    RETCODE_CAN_TRANSFER_SIZE_INVALID,
    RETCODE_CAN_TRANSFER_ADDRESS_INVALID,
    RETCODE_CAN_TRANSFER_DATA_INVALID,
    RETCODE_CAN_TRANSFER_NO_TRANSFER_IN_PROGRESS,
    RETCODE_CAN_TRANSFER_TRANSFER_IN_PROGRESS,
    RETCODE_CAN_TRANSFER_BUSY,
    RETCODE_CAN_TRANSFER_UNFINISHED_TRANSFER,

    /* can transfer file error codes */
    RETCODE_CAN_TRANSFER_CRC_INIT_FAILED,
    RETCODE_CAN_TRANSFER_CRC_COMPUTE_FAILED,
    RETCODE_CAN_TRANSFER_CRC_FINALIZE_FAILED,
    RETCODE_CAN_TRANSFER_INIT_FATFS_FAILED,
    RETCODE_CAN_TRANSFER_OPEN_FILE_FAILED,
    RETCODE_CAN_TRANSFER_PREALLOCATE_FILE_FAILED,
    RETCODE_CAN_TRANSFER_WRITE_FILE_FAILED,
    RETCODE_CAN_TRANSFER_WRITE_FILE_HEADER_FAILED,
    RETCODE_CAN_TRANSFER_CLOSE_FILE_FAILED,

    /* can transfer task error codes */
    RETCODE_CAN_TRANSFER_CREATE_TASK_FAILED,
    RETCODE_CAN_TRANSFER_CREATE_SEM_FAILED,
    RETCODE_CAN_TRANSFER_TAKE_SEM_FAILED,
    RETCODE_CAN_TRANSFER_GIVE_SEM_FAILED,
    RETCODE_CAN_TRANSFER_SEND_NOTIFICATION_FAILED,
    RETCODE_CAN_TRANSFER_WAIT_NOTIFICATION_FAILED,
    RETCODE_CAN_TRANSFER_TASK_TERMINATED,
};

#endif /* BCDS_APP_H_ */

/** ************************************************************************* */
