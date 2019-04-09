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

/**
 * @file 
 * @brief Lora Drivers config header.
 */

#ifndef BCDS_LORADRIVERSCONFIG_H_
#define BCDS_LORADRIVERSCONFIG_H_

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */
/*
 * Below are the Features that are supported by this Package.
 * Please enable/disable them, based on the Project demand.
 */
/** 1 = Enabled, 0 = Disabled */
#ifndef BCDS_RN2483_LORA_DRIVER
#define BCDS_RN2483_LORA_DRIVER        (1)         /**< Enable BCDS_RN2483_LORA_DRIVER */
#endif
/* Put the function declarations here */


#endif /* BCDS_LORADRIVERSCONFIG_H_ */

