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
 *  Configuration header for the reset module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_RESETCONFIG_H_
#define BCDS_RESETCONFIG_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */

/* public function prototype declarations */

/* public global variable declarations */
const uint8_t Boot_Reset_Cmd[] = "#reBoot$"; /* Reset Magic word */
const uint8_t Appl_Reset_Cmd[] = "#reSet$";  /* command for application Reset */

/* inline function definitions */

#endif /* BCDS_RESETCONFIG_H_ */

/** ************************************************************************* */
