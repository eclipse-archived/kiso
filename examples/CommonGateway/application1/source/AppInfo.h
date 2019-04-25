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
 * @brief This File represents the Module IDs for the Application C modules
 *
 */

#ifndef XDK_APPINFO_H_
#define XDK_APPINFO_H_


#define TASK_PRIO_MAIN_CMD_PROCESSOR                (UINT32_C(1))
#define TASK_STACK_SIZE_MAIN_CMD_PROCESSOR          (UINT16_C(700))
#define TASK_Q_LEN_MAIN_CMD_PROCESSOR                (UINT32_C(10))

/**
 * @brief BCDS_APP_MODULE_ID for Application C module of XDK
 * @info  usage:
 *      #undef BCDS_APP_MODULE_ID
 *      #define BCDS_APP_MODULE_ID BCDS_APP_MODULE_ID_xxx
 */
enum XDK_App_ModuleID_E
{
    XDK_APP_MODULE_ID_MAIN,
    XDK_APP_MODULE_ID_LED_BUTTON,
/* Define next module ID here and assign a value to it! */
};

#endif /* XDK_APPINFO_H_ */
