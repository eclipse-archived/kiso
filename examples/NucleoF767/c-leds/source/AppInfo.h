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
 * @ingroup KISO_C_EXAMPLE_BLINKY
 * @brief This File represents the Module IDs for the Application C modules
 * @file
 *
 */

#ifndef APPINFO_H_
#define APPINFO_H_

#define TASK_PRIO_MAIN_CMD_PROCESSOR (UINT32_C(1))
#define TASK_STACK_SIZE_MAIN_CMD_PROCESSOR (UINT16_C(700))
#define TASK_Q_LEN_MAIN_CMD_PROCESSOR (UINT32_C(10))

/**
 * @brief KISO_APP_MODULE_ID for Application C module of XDK
 * @info  usage:
 *      #undef KISO_APP_MODULE_ID
 *      #define KISO_APP_MODULE_ID KISO_APP_MODULE_ID_xxx
 */
enum App_ModuleID_E
{
    APP_MODULE_ID_MAIN,
    APP_MODULE_ID_BLINKY_LED,
    /* Define next module ID here and assign a value to it! */
};

#endif /* APPINFO_H_ */
