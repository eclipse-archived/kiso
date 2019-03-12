/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

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
