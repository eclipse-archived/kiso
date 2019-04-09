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

#ifndef TESTSUITE_SLEEP_H_
#define TESTSUITE_SLEEP_H_


enum TestSuite_5_RetCode_E
{
    TESTSUITE_SLEEP_SETUP_TRIGGERED_SEVERAL_TIMES = RETCODE_FIRST_CUSTOM_CODE,
    TESTSUITE_SLEEP_RUN_TRIGGERED_SEVERAL_TIMES,
    TESTSUITE_SLEEP_TEARDOWN_TRIGGERED_SEVERAL_TIMES,
};

Retcode_T TestSuite_Sleep_initialize(uint8_t testSuiteId);


#endif /* TESTSUITE_SLEEP_H_ */
