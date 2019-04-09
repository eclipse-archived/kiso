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
#ifndef LED_H_
#define LED_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */

/* additional interface header files */
#include "BCDS_Retcode.h"

/**
 * @brief   Enumeration to represents the Led types.
 */
enum LED_Type_E
{
    LED_RELEASE,  /* Must be first element */
	LED_GREEN,
	LED_RED,
	LED_BLUE,
	LED_OFF
};

/**
 * @brief   Enumeration to represents the Led types.
 */
typedef enum LED_Type_E LED_Type_T;

/* public global variable declarations */

/* inline function definitions */

/* function prototypes ****************************************************** */

/**
 * @brief This function will initialize the different LED's and configure them
 *
 * @retval    Retcode_T - status of general return codes.
 */

/* public global variable declarations */

/* inline function definitions */

/* function prototypes ****************************************************** */

/**
 * @brief This function will initialize the different LED's and configure them
 *
 * @retval    Retcode_T - status of general return codes.
 */

Retcode_T Led_Initialize(void);

/**
 * @brief This function will indicate the LED behaviour in various condition while GSM data trabsfer
 *
 * * @param [in]  type     : Enum providing the type of LED used for indication.
 *
 * @retval    Retcode_T - status of general return codes.
 */
Retcode_T Led_Indication(LED_Type_T Type);

/* Deprecated functions will be removed as part of PFM-659. Deprecated since vXXX (@todo: replace xxx with next version) */
BCDS_DEPRECATED(Retcode_T Led_initialize(void));
BCDS_DEPRECATED(Retcode_T Led_indication(LED_Type_T Type));

#endif /* LED_H_ */

/** ************************************************************************* */
