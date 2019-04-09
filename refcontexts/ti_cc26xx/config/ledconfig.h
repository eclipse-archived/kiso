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


#ifndef BCDS_LEDCONFIG_H_
#define BCDS_LEDCONFIG_H_

/* public interface declaration */

/* public type and macro definitions */

#define MAXIMUM_NUMBER_INSTANCE UINT8_C(5)

extern LED_object_t LED_red;
extern LED_object_t LED_green;

#define PWM_RED_LED         (&LED_red)
#define PWM_GREEN_LED       (&LED_green)

/* public function prototype declarations */


#endif /* BCDS_LEDCONFIG_H_ */

