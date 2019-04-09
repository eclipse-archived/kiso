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
