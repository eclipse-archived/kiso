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

#include "BCDS_TestlingInfo.h"
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_NETWORK_TEST_SUITE

#include "BCDS_Basics.h"
#include <FreeRTOS.h>
#include <task.h>
#include "BCDS_Tests.h"
#include "CellularSupport.h"
#include "NetworkTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_NETWORK_ID

static void RunNetworkInitTestCase(CCMsg_T* MessagePtr)
{
	Retcode_T retcode = RETCODE_OK;

    if (NULL == MessagePtr)
    {
    	retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    	Tests_SendReport(retcode, "MessagePtr is null");
        return;
    }

    retcode = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != retcode)
    {
    	Tests_SendReport(retcode, "Network init: fail");
        return;
    }
    Tests_SendReport(retcode, "Network Init: Pass");
	return ;
}

static void RunNetworkInitTwice(CCMsg_T* MessagePtr)
{
    Retcode_T RetStatus = RETCODE_OK;
    uint32_t CountValue;

    if (NULL == MessagePtr)
    {
        RetStatus = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RetStatus, "MessagePtr is null");
        return;
    }

    for (CountValue = NWINIT_TWICE_TESTCASE_COUNT; CountValue > UINT8_C(0); CountValue--)
    {
        RetStatus = CellularSupport_NetworkInit(MessagePtr);
        if (RETCODE_OK != RetStatus)
        {
            Tests_SendReport(RetStatus, "Network Init Fail");
            return;
        }
    }
    Tests_SendReport(RetStatus, NULL);
    return;
}

static void RunNetworkGPRSConnect(CCMsg_T* MessagePtr)
{

    Retcode_T RetStatus = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        RetStatus = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RetStatus, "MessagePtr is null");
        return;
    }

    RetStatus = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Init Fail");
        return;
    }

    RetStatus = CellularSupport_NetworkConfig(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Config Fail");
        return;
    }

    RetStatus = CellularSupport_Connect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Connect Fail");
        return;
    }

    RetStatus = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network disconnect Fail");
        return;
    }

    Tests_SendReport(RetStatus, NULL);
    return;
}

static void RunNewtworkInit120SecDly(CCMsg_T* MessagePtr)
{

    Retcode_T RetStatus = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        RetStatus = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RetStatus, "MessagePtr is null");
        return;
    }

    RetStatus = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Init Fail");
        return;
    }

    vTaskDelay(DELAY_120_SECS / portTICK_RATE_MS);

    RetStatus = CellularSupport_NetworkConfig(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Config Fail");
        return;
    }

    RetStatus = CellularSupport_Connect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Connect Fail");
        return;
    }

    RetStatus = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network disconnect Fail");
        return;
    }

    Tests_SendReport(RetStatus, NULL);
    return;
}

static void RunNewtworkConfig120SecDly(CCMsg_T* MessagePtr)
{

    Retcode_T RetStatus = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        RetStatus = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RetStatus, "MessagePtr is null");
        return;
    }

    RetStatus = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Init Fail");
        return;
    }

    RetStatus = CellularSupport_NetworkConfig(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Config Fail");
        return;
    }

    vTaskDelay(DELAY_120_SECS / portTICK_RATE_MS);

    RetStatus = CellularSupport_Connect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Connect Fail");
        return;
    }

    RetStatus = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network disconnect Fail");
        return;
    }

    Tests_SendReport(RetStatus, NULL);
    return;
}

static void RunNewtworkConnect120SecDly(CCMsg_T* MessagePtr)
{

    Retcode_T RetStatus = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        RetStatus = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RetStatus, "MessagePtr is null");
        return;
    }

    RetStatus = CellularSupport_NetworkInit(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Init Fail");
        return;
    }

    RetStatus = CellularSupport_NetworkConfig(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Config Fail");
        return;
    }

    RetStatus = CellularSupport_Connect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network Connect Fail");
        return;
    }

    vTaskDelay(DELAY_120_SECS / portTICK_RATE_MS);

    RetStatus = CellularSupport_DisConnect(MessagePtr);
    if (RETCODE_OK != RetStatus)
    {
        Tests_SendReport(RetStatus, "Network disconnect Fail");
        return;
    }

    Tests_SendReport(RetStatus, NULL);
    return;
}

static void RunNetworkSequence5times(CCMsg_T* MessagePtr)
{
	uint32_t count;
    Retcode_T RetStatus = RETCODE_OK;

    if (NULL == MessagePtr)
    {
        RetStatus = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
        Tests_SendReport(RetStatus, "MessagePtr is null");
        return;
    }

    for (count = 0; count < NETWORK_SEQUENCE_COUNT; count++)
    {

		RetStatus = CellularSupport_NetworkInit(MessagePtr);
		if (RETCODE_OK != RetStatus)
		{
			Tests_SendReport(RetStatus, "Network Init Fail");
			return;
		}

		RetStatus = CellularSupport_NetworkConfig(MessagePtr);
		if (RETCODE_OK != RetStatus)
		{
			Tests_SendReport(RetStatus, "Network Config Fail");
			return;
		}

		RetStatus = CellularSupport_Connect(MessagePtr);
		if (RETCODE_OK != RetStatus)
		{
			Tests_SendReport(RetStatus, "Network Connect Fail");
			return;
		}

		RetStatus = CellularSupport_DisConnect(MessagePtr);
		if (RETCODE_OK != RetStatus)
		{
			Tests_SendReport(RetStatus, "Network disconnect Fail");
			return;
		}

    }

	Tests_SendReport(RetStatus, NULL);
	return;
}

Retcode_T NetworkTestSuite_Init(void)
{
	Retcode_T retcode = RETCODE_OK;

	retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == retcode)
    {
    	/*TC 1.2.1 Initialization of Cellular network */
        retcode = TEST_REGISTER_TC(RunNetworkInitTestCase);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.2.2 Initialization of Cellular network twice */
        retcode = TEST_REGISTER_TC(RunNetworkInitTwice);
    }

    if (RETCODE_OK == retcode)
    {
        /* TC 1.2.3 Init, Configure, Connect, Disconnect the Cellular network */
        retcode = TEST_REGISTER_TC(RunNetworkGPRSConnect);
    }

	if (RETCODE_OK == retcode)
	{
		 /* TC 1.2.4 Init,Time delay(120 secs),Configure,Connect,Disconnect the Cellular network */
		retcode = TEST_REGISTER_TC(RunNewtworkInit120SecDly);
	}

	if (RETCODE_OK == retcode)
	{
		/* TC 1.2.5 Init,Configure,Time delay(120 secs),Connect,Disconnect the Cellular network */
		retcode = TEST_REGISTER_TC(RunNewtworkConfig120SecDly);
	}

	if (RETCODE_OK == retcode)
	{
		/* TC 1.2.6 Init,Configure,Connect,Time delay(120 secs),Disconnect the Cellular network */
		retcode = TEST_REGISTER_TC(RunNewtworkConnect120SecDly);
	}

	if (RETCODE_OK == retcode)
	{
		/* TC 1.2.7 Init, Configure, Connect, Disconnect(5 times) the Cellular network */
		retcode = TEST_REGISTER_TC(RunNetworkSequence5times);
	}

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNetworkTurnOnId, NULL, RunTurnOn, NULL);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunNetworkTurnOffId, NULL, RunTurnOff, NULL);
    }

	return retcode;
}
