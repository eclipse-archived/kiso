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
#ifndef APP_H_
#define APP_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */

/* public function prototype declarations */

/* additional interface header files */
#include "BCDS_Retcode.h"

/**
 * @brief   This starts the Bike Sensor eBike application.
 *
 * @return  An error code failure. On success the function would not return.
 */
Retcode_T App_Start(void);

#endif /* APP_H_ */

/** ************************************************************************* */
