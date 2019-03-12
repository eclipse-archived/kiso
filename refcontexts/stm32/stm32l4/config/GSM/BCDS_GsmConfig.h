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

/* header definition ******************************************************** */
#ifndef BCDS_GSMCONFIG_H_
#define BCDS_GSMCONFIG_H_

/* local interface declaration ********************************************** */

#ifndef GSM_MAX_NUM_UDP_SOCKETS
#define GSM_MAX_NUM_UDP_SOCKETS                     (UINT32_C(3))
#endif
/** Define LB2C command response time out */
#define GSM_DEFAULT_TIMEOUT_SEND_MESSAGE_MS         UINT32_C(1000)

/**< Max UDP data length which must not exceed 1472
 * UDP data received/sent more than this size is discarded */
#define GSMUDPSOCKET_SEND_MAX_LENGTH                (1472UL)

/**< Max UDP data receive length is 1472 + LeanB2CAP frame overhead + misc */
#define GSM_RX_BUFFER_LEN                           (1536UL)

/**< Max UDP data transfer length is 1472 + LeanB2CAP frame overhead + misc */
#define GSM_TX_BUFFER_LEN                           (1536UL)

/** Max. message length limit */
#define SMS_TEXT_MESSAGE_MAX_SIZE        			(160U)

/** Min. message length limit */
#define SMS_TEXT_MESSAGE_MIN_SIZE		    		(1U)

/** Min length of mobile number */
#define SMS_MOBILE_NUMBER_MIN_SIZE          		(1U)

/** Max length of mobile number */
#define SMS_MOBILE_NUMBER_MAX_SIZE              	(20U)

#define GSM_PORTINTERFACE_BUFFER_SIZE		        (UINT32_C(512))

#define GSM_TASK_PRIORITY							(UINT32_C(5)) 		/**< Gsm task priority */
#define GSM_TASK_STACK_SIZE							(UINT32_C(600))		/**< Gsm task stack size */

#define PORTINTERFACE_RESET_STATUS_WAIT_TIME        (UINT32_C(6000))    /**< GSM power status wait time */
#define PORTINTERFACE_RESET_HOLD_TIME               (UINT32_C(200))     /**< GSM reset line hold time */

/* local function prototype declarations */

/* local module global variable declarations */

/* local inline function definitions */

#endif /* BCDS_GSMCONFIG_H_ */
