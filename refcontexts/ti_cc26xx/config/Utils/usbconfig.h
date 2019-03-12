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
 *  @file        usbconfig.h
 *
 *
 *  Interface header for the usbconfig module.
 *  This File will be called from the EM library .
 *  It requires the configurations for the USB driver.(example : configuring whether USB_HOST mode or USB_DEVICE mode to be enabled).
 *  At present , the configurations are defined to USB_ph.h for convenient of the user.
 *
 *
 * ****************************************************************************/
/**
 * @defgroup usbconfig USB configuration
 * @ingroup configuration
 *
 * @{
 * @brief Configuration header for the USB module
 *  \tableofcontents
 *  \section intro_sec USB
 *
 */
/* header definition ******************************************************** */
#ifndef XDK110_USBCONFIG_H_
#define XDK110_USBCONFIG_H_

/* public interface declaration ********************************************* */

/* The configurations required for the USB libraries are defined in USB_ph.h*/
#include "USB_ph.h"

/* public type and macro definitions */

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* XDK110_USBCONFIG_H_ */

/** ************************************************************************* */

/**@}*/