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
