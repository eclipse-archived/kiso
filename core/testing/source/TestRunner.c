/*******************************************************************************
 * Copyright (c) 2010-2020 Robert Bosch GmbH
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
 ******************************************************************************/

/**
 * @file
 *
 * @brief
 * The TestRunner dispatches the commands to the test sections and test suites  and processes the acknowledgments
 * received from the test controller.
 */
#include "Kiso_Testing.h"
#include "TestRunner.h"
#include "Kiso_CmdProcessor.h"
#include "CChannel.h"
#include "TestRegistry.h"
#include "FreeRTOS.h"
#include "timers.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TESTING_TESTRUNNER

#define TEST_ENTRY_SETUP 1
#define TEST_SUITE_SETUP 2
#define TEST_CASE_SETUP 3
#define TEST_CASE_RUN 13
#define TEST_ENTRY_TEARDOWN 21
#define TEST_SUITE_TEARDOWN 22
#define TEST_CASE_TEARDOWN 23

#define RUNNER_TIME_TO_WAIT_FOR_ACK_MS (UINT8_C(3000))

/** Structure containing the handler for the Test Runner */
struct TestRunner_S
{
    uint8_t id; /**< Message token corresponding to the message type */
    uint8_t numberOfRetries;
    CmdProcessor_T cmdProcessor;
    TimerHandle_t timer;
    uint8_t lastReceivedMessageToken;
    bool waitingForAck;
};

static void dispatcher(CCMsg_T *ccmsg, uint32_t unusedParameter);
static void processAck(CCMsg_T *ccmsg);
static void processCommand(CCMsg_T *ccmsg);
static void ackTimerCallbackFunction(TimerHandle_t timer);
static bool isMsgAnAck(CCMsg_T *ccmsg);
static bool isMsgACommand(CCMsg_T *ccmsg);

static struct TestRunner_S testRunner;

/* @see TestRunner.h for function description */
Retcode_T TestRunner_Initialize(void)
{
    Retcode_T retcode = RETCODE_OK;
    testRunner.id = 0U;

    retcode = CmdProcessor_Initialize(&testRunner.cmdProcessor,
                                      "Test Runner",
                                      TEST_RUNNER_TASK_PRIO,
                                      TEST_RUNNER_TASK_STACK_DEPTH,
                                      TEST_RUNNER_QUEUE_SIZE);

    if (RETCODE_OK == retcode)
    {
        testRunner.timer = xTimerCreate((const char *const) "Test Runner Timer",
                                        RUNNER_TIME_TO_WAIT_FOR_ACK_MS,
                                        pdFALSE,
                                        NULL,
                                        ackTimerCallbackFunction);
        if (NULL == testRunner.timer)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
        }
    }
    if (RETCODE_OK == retcode)
    {
        retcode = CChannel_Initialize();
    }
    return retcode;
}

/* @see TestRunner.h for function description */
void TestRunner_ProcessMessage(CCMsg_T *ccmsg)
{
    Retcode_T retcode = CmdProcessor_Enqueue(&testRunner.cmdProcessor,
                                             (CmdProcessor_Func_T)dispatcher,
                                             ccmsg,
                                             0);

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
}

/* @see TestRunner.h for function description */
void TestRunner_SendReport(uint8_t result, char *reason)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = CChannel_SendReport(result, reason);

    if (RETCODE_OK == retcode)
    {
        if (pdFAIL == xTimerStart(testRunner.timer, RUNNER_TIME_TO_WAIT_FOR_ACK_MS))
        {
            Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES));
        }
        testRunner.numberOfRetries = 3;
        testRunner.waitingForAck = true;
    }
    else
    {
        Retcode_RaiseError(retcode);
    }
}

static void dispatcher(CCMsg_T *ccmsg, uint32_t unusedParameter)
{
    KISO_UNUSED(unusedParameter);

    if (isMsgAnAck(ccmsg))
    {
        processAck(ccmsg);
    }
    else if (isMsgACommand(ccmsg))
    {
        processCommand(ccmsg);
    }
    else
    {
        //@todo handle the fact that the type is not supported
        //      CChannel_PrepareAck(ccmsg);
        //      CChannel_SendAck(RETCODE_OK);
    }

    CChannel_FreeCCMsg(ccmsg);
}

static void processAck(CCMsg_T *ccmsg)
{
    if (CChannel_DoesAckMatchReport(ccmsg))
    {

        if (pdFAIL == xTimerStop(testRunner.timer, 0))
        {
            //@todo raise some error?
        }

        testRunner.waitingForAck = false;
        testRunner.numberOfRetries = 0;
    }
    else
    {
        //@todo do we just ignore it? or raise an error?
    }
}

static void processCommand(CCMsg_T *ccmsg)
{
    Retcode_T retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    if (testRunner.waitingForAck)
    {
        /**
         * this means the Test Coordinator did receive the Report but the ack was lost
         * so we can just stop the timer and continue
         */
        if (true != xTimerStop(testRunner.timer, 0))
        {
            //@todo raise some error?
        }
        testRunner.waitingForAck = false;
        testRunner.numberOfRetries = 0;
    }
    if (testRunner.lastReceivedMessageToken == ccmsg->header.messageToken)
    {
        /**
         * if we receive again a command, we shouldn't do the associated action
         * we should however resend the ack.
         */
        retcode = CChannel_ResendAck();

        if (RETCODE_OK != retcode)
        {
            Retcode_RaiseError(retcode);
        }
        return;
    }

    uint8_t result = RETCODE_OK;

    TstEnt_T *testEntry = TestRegistry_LookupTestEntry(ccmsg->header.testEntry);
    TstSte_T *testSuite = TestRegistry_LookupTestSuite(ccmsg->header.testEntry, ccmsg->header.testSuite);
    TstCse_T *testCase = TestRegistry_LookupTestCase(ccmsg->header.testEntry,
                                                     ccmsg->header.testSuite, ccmsg->header.testCase);

    testRunner.lastReceivedMessageToken = ccmsg->header.messageToken;

    CChannel_PrepareAck(ccmsg);

    switch (ccmsg->header.messageType)
    {
    case TEST_ENTRY_SETUP:
        if (NULL != testEntry)
        {
            retcode = TestEntry_Setup(testEntry, ccmsg);
        }
        else
        {
            /* @todo: The error code in Retcode_T occupies 16 bits. But, the argument type passed to CChannel_SendAck() is uint8_t.
             * The test coordination protocol has an error code of 8 bits. This is a problem, as we would end up in overflow
             * and due to the unsigned nature of the variable, we would wrap around. This needs to be fixed. */
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    case TEST_SUITE_SETUP:
        if (NULL != testSuite)
        {
            retcode = TestSuite_Setup(testSuite, ccmsg);
        }
        else
        {
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    case TEST_CASE_SETUP:
        if (NULL != testCase)
        {
            retcode = TestCase_Setup(testCase, ccmsg);
        }
        else
        {
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    case TEST_CASE_RUN:
        if (NULL != testCase)
        {
            retcode = CChannel_SendAck(RETCODE_OK);

            if (RETCODE_OK != retcode)
            {
                Retcode_RaiseError(retcode);
            }
            else
            {
                CChannel_PrepareReport(ccmsg);

                TestCase_Run(testCase, ccmsg);
            }
            return;
        }
        else
        {
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    case TEST_ENTRY_TEARDOWN:
        if (NULL != testEntry)
        {
            retcode = TestEntry_Teardown(testEntry, ccmsg);
        }
        else
        {
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    case TEST_SUITE_TEARDOWN:
        if (NULL != testSuite)
        {
            retcode = TestSuite_Teardown(testSuite, ccmsg);
        }
        else
        {
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    case TEST_CASE_TEARDOWN:
        if (NULL != testCase)
        {
            retcode = TestCase_Teardown(testCase, ccmsg);
        }
        else
        {
            result = (uint8_t)RETCODE_INVALID_PARAM;
        }
        break;
    default:
        result = (uint8_t)RETCODE_INVALID_PARAM;
        break;
    }

    if ((uint8_t)RETCODE_INVALID_PARAM != result)
    {
        result = Retcode_GetCode(retcode);
    }

    retcode = CChannel_SendAck(result);

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
}

static void ackTimerCallbackFunction(TimerHandle_t timer)
{
    KISO_UNUSED(timer);
    Retcode_T retcode = RETCODE_OK;

    if (testRunner.numberOfRetries > 0)
    {
        testRunner.numberOfRetries--;

        retcode = CChannel_ResendReport();

        if (RETCODE_OK == retcode)
        {
            if (true != xTimerStart(testRunner.timer, RUNNER_TIME_TO_WAIT_FOR_ACK_MS))
            {
                Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_OUT_OF_RESOURCES));
            }
        }
        else
        {
            Retcode_RaiseError(retcode);
        }
    }
    else
    {
        testRunner.waitingForAck = false;
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_TESTING_REPORT_TIMEOUT));
    }
}

/**
 * @brief checks the type of the message and returns true if it is an acknowledgement message.
 */
static bool isMsgAnAck(CCMsg_T *ccmsg)
{
    return CCMSG_GET_TYPE(ccmsg) == CCHANNEL_MSG_TYPE_ACK;
}

/**
 * @brief checks the type of the message and returns true if it is a command message.
 */
static bool isMsgACommand(CCMsg_T *ccmsg)
{
    return CCMSG_GET_TYPE(ccmsg) == CCHANNEL_MSG_TYPE_COMMAND;
}
