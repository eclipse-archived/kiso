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
 * @ingroup  KISO_C_EXAMPLE_SENSORS
 * @brief    This File represents the Module IDs for the Application C modules
 * @file
 *
 */

#ifndef KISO_APPINFO_H_
#define KISO_APPINFO_H_

#define TASK_PRIO_MAIN_CMD_PROCESSOR (UINT32_C(1))
#define TASK_STACK_SIZE_MAIN_CMD_PROCESSOR (UINT16_C(700))
#define TASK_Q_LEN_MAIN_CMD_PROCESSOR (UINT32_C(10))

/**
 * @brief
 *      Enumerates application modules which are reporting error codes according to RETCODE specification.
 *
 * @info
 *      Usage:
 *
 * @code{.c}
 *      #undef KISO_APP_MODULE_ID
 *      #define KISO_APP_MODULE_ID KISO_APP_MODULE_ID_xxx
 * @endcode
 *
 */
enum KISO_App_ModuleID_E
{
    KISO_APP_MODULE_MAIN = 1,
    KISO_APP_MODULE_ENV_SENSOR,
    KISO_APP_MODULE_ACCEL_SENSOR,
    KISO_APP_MODULE_SENSORS_COMMON,
    /* Define next module ID here and assign a value to it! */
};

#endif /* KISO_APPINFO_H_ */
