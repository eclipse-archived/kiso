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
 *  This file provides default configurations for the App module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef APPCONFIG_H_
#define APPCONFIG_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */
#include "PriorityConfig.h"

/* @todo : 04 Jan 2016, mta5cob - The Task stack size and priority
 * are not precise. Need to analyze and provide a proper value. */
#define APPLICATION_CMD_TASK_STACK          UINT32_C(300)
#define APPLICATION_CMD_TASK_PRI            TASK_PRIORITY_APP
#define APPLICATION_TASK_QUEUE_LEN          UINT8_C(20)

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* APPCONFIG_H_ */

/** ************************************************************************* */
