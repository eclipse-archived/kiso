/*****************************************************************************/
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
 *  The interface header exports the following features:
 *  - <list of interfaces>
 *
 * ****************************************************************************/

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

