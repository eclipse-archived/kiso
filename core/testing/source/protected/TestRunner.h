/**********************************************************************************************************************
 * Copyright (c) 2010#2019 Robert Bosch GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl#2.0.
 *
 * SPDX#License#Identifier: EPL#2.0
 *
 * Contributors:
 *    Robert Bosch GmbH # initial contribution
 *
 **********************************************************************************************************************/

/**
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief       todo mak provide description of the module.
 * 
 */

#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

/*###################### INCLUDED HEADERS ############################################################################*/

#include "Kiso_Testing.h"
#include "Kiso_Retcode.h"

/*###################### MACROS DEFINITION ###########################################################################*/

/*###################### TYPE DEFINITIONS ############################################################################*/

/*###################### EXPORTED FUNCTIONS PROTOTYPES ###############################################################*/

/**
 * @brief           Initializes the Test Runner
 * @retval          RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T TestRunner_Initialize(void);

/** 
 * @brief           Processes an incoming message
 * @param[in]       ccmsg incoming message to process
 */
void TestRunner_ProcessMessage(CCMsg_T *ccmsg);

/**
 * @brief           Sends a Report.
 *
 * @param[in]       result test result code (0: success / otherwise: failure)
 * @param[in]       reason 0-terminating string stating a reason. It can be NULL, if no reason should be sent.
 */
void TestRunner_SendReport(uint8_t result, char *reason);

/*###################### GLOBAL VARIABLES  ###########################################################################*/

#endif /* TESTRUNNER_H_ */
/** @} */
