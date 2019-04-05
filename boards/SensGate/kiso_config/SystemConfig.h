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
 *  This file provides default configurations for the System module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */

#define SYS_OFF_CONTROL_PIN 						(GPIO_PIN_NO_14)
#define SYS_OFF_CONTROL_PORT 						(GPIO_PORT_D)

#define PERIPHERAL_SUPPLY_PIN 						(GPIO_PIN_NO_7)
#define PERIPHERAL_SUPPLY_PORT 						(GPIO_PORT_E)

#define CHARGING_EN_PIN 							(GPIO_PIN_NO_10)
#define CHARGING_EN_PORT 							(GPIO_PORT_G)

#define CHARGING_IN_EN1_PIN 						(GPIO_PIN_NO_11)
#define CHARGING_IN_EN1_PORT 						(GPIO_PORT_G)

#define CHARGING_IN_EN2_PIN 						(GPIO_PIN_NO_6)
#define CHARGING_IN_EN2_PORT 						(GPIO_PORT_G)

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* SYSTEMCONFIG_H_ */

/** ************************************************************************* */
