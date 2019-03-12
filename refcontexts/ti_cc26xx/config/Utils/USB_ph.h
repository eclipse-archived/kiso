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
 *  @file        USB_ph.h
 *
 *
 *  Configuration header for the USB_ph module.
 *
 * ****************************************************************************/
/**
 * @defgroup usbconfig USB configuration
 * @ingroup config
 *
 * @{
 * @brief Configuration header for the USB module.
 *  \tableofcontents
 *  \section intro_sec USB
 *
 */
/* header definition ******************************************************** */
#ifndef XDK110_USB_PH_H_
#define XDK110_USB_PH_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */

#define USB_SERIAL_NUMBER_LENGTH       UINT8_C(16)
#define USB_DEVICE                                       /**< USB DEVICE mode or HOST mode must be defined */

#define USB_PRODUCT_ID 0x017B                            /**< The product Id is defined here*/

#define BCDS_PRODUCT_NAME 'X','D','K',' ','A','p','p','l','i','c','a','t','i','o','n',             /**< The product name is defined here*/
#define BCDS_SERIAL_NO '1','2','3','4','5','6','7','8','9','0','1','2','3','4','5','6'             /**< The Serial number is defined here*/

/**< retargetio.c must be included if you are enabling USB_USE_PRINTF */

#define USB_USE_PRINTF                                   /**< When using printf , the string is retargetted to USB . The transfer is blocking at present. */

#define NUM_EP_USED                     UINT8_C(3)       /**< Specify number of endpoints used (in addition to EP0).*/
#define USB_BULK_SIZE                   UINT8_C(64)    	 /**< This is the max. packet size.        */
#define NUM_APP_TIMERS                  UINT8_C(0)       /**< Specify number of application timers you need. Receving or Transmitting Data is not dependent of this macro. */
#define USB_SET_BAUDRATE                UINT32_C(0) 	 /**< Baudrate. Receving or Transmitting Data is not dependent of this macro. 									   */
#define USB_SET_STOPBITS_MODE           UINT8_C(0)       /**< Stop bits, 0=1 1=1.5 2=2. Receving or Transmitting Data is not dependent of this macro.           	       */
#define USB_SET_PARITY                  UINT8_C(0)       /**< 0=None 1=Odd 2=Even 3=Mark 4=Space. Receving or Transmitting Data is not dependent of this macro. 		   */
#define USB_SET_DATA_BITS               UINT8_C(0)       /**< 0, 5, 6, 7, 8 or 16. Receving or Transmitting Data is not dependent of this macro.               	           */


/* local function prototype declarations */

/* local module global variable declarations */

/* local inline function definitions */

#endif /* XDK110_USB_PH_H_ */
/**@}*/

/** ************************************************************************* */
