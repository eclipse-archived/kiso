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
 */
#include "Kiso_Testing.h"
#include "TestRegistry.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TESTING_TESTREGISTRY

static TstSte_T *lookupTestSuite(uint8_t sId);
static TstCse_T *lookupTestCase(TstSte_T *testSuite, uint8_t cId);

static TstEnt_T testEntry;

/* @see TestRegistry.h for function description */
void TestRegistry_Initialize(uint8_t eId, SetupFct_T setup, TearDownFct_T teardown)
{
    (void)memset(&testEntry, 0, sizeof(TstEnt_T));

    testEntry.id = eId;
    testEntry.setup = setup;
    testEntry.teardown = teardown;
}

/* @see TestRegistry.h for function description */
Retcode_T TestRegistry_RegisterTestSuite(uint8_t sId, SetupFct_T setup, TearDownFct_T teardown)
{
    if (TEST_MAX_NUMBER_OF_TEST_SUITES <= testEntry.numTestSuites)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_OUT_OF_RESOURCES);
    }

    TstSte_T *suite = lookupTestSuite(sId);

    if (NULL != suite)
    {
        /* The test suite with the given ID is already registered */
        return RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_TESTING_SUITE_ALREADY_REGISTERED);
    }

    suite = &testEntry.testSuites[testEntry.numTestSuites];
    testEntry.numTestSuites++;

    suite->id = sId;
    suite->setup = setup;
    suite->teardown = teardown;

    return RETCODE_OK;
}

/* @see TestRegistry.h for function description */
Retcode_T TestRegistry_RegisterTestCase(uint8_t sId, uint8_t cId, SetupFct_T setup, RunFct_T run, TearDownFct_T teardown)
{
    if (NULL == run)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_INVALID_PARAM);
    }

    TstSte_T *suite = lookupTestSuite(sId);

    if (NULL == suite)
    {
        /* The suite is not found */
        return RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_INVALID_PARAM);
    }

    if (TEST_MAX_NUMBER_OF_TEST_CASES_PER_TEST_SUITE <= suite->numTestCases)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_OUT_OF_RESOURCES);
    }

    TstCse_T *testCase = lookupTestCase(suite, cId);

    if (NULL != testCase)
    {
        /* A test case with the given ID is already registered */
        return RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_TESTING_CASE_ALREADY_REGISTERED);
    }

    testCase = &suite->testCases[suite->numTestCases];
    suite->numTestCases++;

    testCase->id = cId;
    testCase->setup = setup;
    testCase->run = run;
    testCase->teardown = teardown;

    return RETCODE_OK;
}

/* @see TestRegistry.h for function description */
TstEnt_T *TestRegistry_LookupTestEntry(uint8_t eId)
{
    TstEnt_T *theTestEntry = NULL;

    if (eId == testEntry.id)
    {
        theTestEntry = &testEntry;
    }

    return theTestEntry;
}

/* @see TestRegistry.h for function description */
TstSte_T *TestRegistry_LookupTestSuite(uint8_t eId, uint8_t sId)
{
    TstSte_T *testSuite = NULL;

    if (eId == testEntry.id)
    {
        testSuite = lookupTestSuite(sId);
    }

    return testSuite;
}

/* @see TestRegistry.h for function description */
TstCse_T *TestRegistry_LookupTestCase(uint8_t eId, uint8_t sId, uint8_t cId)
{
    TstSte_T *testSuite = TestRegistry_LookupTestSuite(eId, sId);
    TstCse_T *testCase = NULL;

    if (NULL != testSuite)
    {
        testCase = lookupTestCase(testSuite, cId);
    }

    return testCase;
}

/* @see TestRegistry.h for function description */
Retcode_T TestEntry_Setup(TstEnt_T *theTestEntry, CCMsg_T *ccmsg)
{

    Retcode_T retcode = RETCODE_OK;
    if ((NULL == theTestEntry) || (NULL == ccmsg))
    {
        /* The suite is not found */
        retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_NULL_POINTER);
        return (retcode);
    }

    /* If test section setup pointer are null, it means that there is nothing to be done and that we can just send an
    Acknowledgement with status OK.*/
    if (NULL != theTestEntry->setup)
    {
        retcode = theTestEntry->setup(ccmsg);
    }
    return retcode;
}

/* @see TestRegistry.h for function description */
Retcode_T TestEntry_Teardown(TstEnt_T *theTestEntry, CCMsg_T *ccmsg)
{

    Retcode_T retcode = RETCODE_OK;

    if ((NULL == theTestEntry) || (NULL == ccmsg))
    {
        /* The suite is not found */
        retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_NULL_POINTER);
        return (retcode);
    }

    /*
     If test section teardown pointer are null, it means that there is nothing to be done and that we can just send an
     Acknowledgement with status OK.*/
    if (NULL != theTestEntry->teardown)
    {
        retcode = theTestEntry->teardown(ccmsg);
    }
    return retcode;
}

/* @see TestRegistry.h for function description */
Retcode_T TestSuite_Setup(TstSte_T *testSuite, CCMsg_T *ccmsg)
{

    Retcode_T retcode = RETCODE_OK;

    if ((NULL == testSuite) || (NULL == ccmsg))
    {
        /* The suite is not found */
        retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_NULL_POINTER);
        return (retcode);
    }
    /* If test suite setup pointer are null, it means that there is nothing to be done and that we can just send an
    Acknowledgement with status OK.*/
    if (NULL != testSuite->setup)
    {
        retcode = testSuite->setup(ccmsg);
    }

    return retcode;
}

/* @see TestRegistry.h for function description */
Retcode_T TestSuite_Teardown(TstSte_T *testSuite, CCMsg_T *ccmsg)
{
    Retcode_T retcode = RETCODE_OK;
    if ((NULL == testSuite) || (NULL == ccmsg))
    {
        /* The suite is not found */
        retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_NULL_POINTER);
        return (retcode);
    }
    /* If test suite teardown pointer are null, it means that there is nothing to be done and that we can just send an
    Acknowledgement with status OK.*/
    if (NULL != testSuite->teardown)
    {
        retcode = testSuite->teardown(ccmsg);
    }

    return retcode;
}

/* @see TestRegistry.h for function description */
Retcode_T TestCase_Setup(TstCse_T *testCase, CCMsg_T *ccmsg)
{

    Retcode_T retcode = RETCODE_OK;

    if ((NULL == testCase) || (NULL == ccmsg))
    {
        /* The suite is not found */
        retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_NULL_POINTER);
        return (retcode);
    }
    if (NULL != testCase->setup)
    {
        retcode = testCase->setup(ccmsg);
    }

    return retcode;
}

/* @see TestRegistry.h for function description */
void TestCase_Run(TstCse_T *testCase, CCMsg_T *ccmsg)
{

    if ((NULL == testCase) || (NULL == ccmsg) || (NULL == testCase->run))
    {
        Retcode_RaiseError(RETCODE(RETCODE_SEVERITY_ERROR, (uint32_t)RETCODE_NULL_POINTER));
    }
    else
    {
        testCase->run(ccmsg);
    }
}

/* @see TestRegistry.h for function description */
Retcode_T TestCase_Teardown(TstCse_T *testCase, CCMsg_T *ccmsg)
{

    Retcode_T retcode = RETCODE_OK;

    if ((NULL == testCase) || (NULL == ccmsg))
    {
        /* The suite is not found */
        retcode = RETCODE(RETCODE_SEVERITY_WARNING, (uint32_t)RETCODE_NULL_POINTER);
    }
    else
    {
        if (NULL != testCase->teardown)
        {
            retcode = testCase->teardown(ccmsg);
        }
    }

    return retcode;
}

/**
 * @brief finds a test suite by suite Id in the test suites registry.
*/
static TstSte_T *lookupTestSuite(uint8_t sId)
{
    TstSte_T *testSuite = NULL;

    for (uint32_t i = 0; i < testEntry.numTestSuites; i++)
    {
        if (sId == testEntry.testSuites[i].id)
        {
            testSuite = &testEntry.testSuites[i];
            break;
        }
    }

    return testSuite;
}

/**
 * @brief finds a test case by case Id in the test cases registry of the referenced test suite .
*/
static TstCse_T *lookupTestCase(TstSte_T *testSuite, uint8_t cId)
{
    TstCse_T *testCase = NULL;

    for (uint32_t i = 0; i < testSuite->numTestCases; i++)
    {
        if (cId == testSuite->testCases[i].id)
        {
            testCase = &testSuite->testCases[i];
            break;
        }
    }

    return testCase;
}
