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
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_QUERY_TEST_SUITE

#include <FreeRTOS.h>
#include "CellularSupport.h"
#include "QueryTestSuite.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_QUERY_ID

static void RunQueryNetworkInformationFromModem(CCMsg_T* MessagePtr)
{
	Retcode_T retcode = RETCODE_OK;
	Cellular_NetworkInfo_T NetworkInfo;
	uint8_t NetworkInfoBuff[QUERY_TEST_CASE_NETWORK_INFO_LENGTH];

	if (NULL == MessagePtr)
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(retcode, " input parameter null ");
		return;
	}
	retcode = CellularSupport_NetworkInit(MessagePtr);
	if (RETCODE_OK != retcode)
	{
		Tests_SendReport(retcode, "Fail: NW init fail");
		return;
	}

	retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_NETWORKINFO,&NetworkInfo);
	if (RETCODE_OK != retcode)
	{
		Tests_SendReport(retcode, "Fail: NW info fail");
		return;
	}
	memcpy(NetworkInfoBuff, &NetworkInfo, sizeof(Cellular_NetworkInfo_T));
	NetworkInfoBuff[QUERY_TEST_NW_CODE_LEN_POSITION] += QUERY_TEST_ASCII_CON_VAL;
	NetworkInfoBuff[QUERY_TEST_STRING_END_POSITION] = '\0';
	Tests_SendReport(retcode,(char *)NetworkInfoBuff);
}

static void RunQueryErrorInformationFromModem(CCMsg_T* MessagePtr)
{
	Retcode_T retcode = RETCODE_OK;
	uint8_t ErrorInfo;

	if (NULL == MessagePtr)
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(retcode, " input parameter null ");
		return;
	}
	retcode = CellularSupport_NetworkInit(MessagePtr);
	if (RETCODE_OK != retcode)
	{
		Tests_SendReport(retcode, "Fail: NW init fail");
		return;
	}

	retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_ERROR,&ErrorInfo);
	if ((RETCODE_OK != retcode) || (QUERY_TEST_NO_ERROR != ErrorInfo))
	{
		Tests_SendReport(retcode, "Fail: Error info fail");
		return;
	}
	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunQueryCellularStatusInitialized(CCMsg_T* MessagePtr)
{
	Retcode_T retcode = RETCODE_OK;
	Cellular_DeviceStatus_T Value;

	if (NULL == MessagePtr)
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(retcode, " input parameter null ");
		return;
	}
	retcode = CellularSupport_NetworkInit(MessagePtr);
	if (RETCODE_OK != retcode)
	{
		Tests_SendReport(retcode, "Fail: NW init fail");
		return;
	}

	retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_STATUS,&Value);
	if (RETCODE_OK != retcode)
	{
		Tests_SendReport(retcode, "Fail: Cellular init fail");
		return;
	}
	if(CELLULAR_DEVICESTATUS_INIT != Value)
	{
		Tests_SendReport(retcode, "Fail: Cellular init fail");
		return;
	}
	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunQueryCellularStatusConnected(CCMsg_T* MessagePtr)
{
	Retcode_T retcode = RETCODE_OK;
	Cellular_DeviceStatus_T Value;

	if (NULL == MessagePtr)
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(retcode, " input parameter null ");
		return;
	}

	retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_STATUS,&Value);
	if (RETCODE_OK != retcode)
	{
		Tests_SendReport(retcode, "Fail: Cellular connect fail");
		return;
	}
	if(CELLULAR_DEVICESTATUS_CONNECTED != Value)
	{
		Tests_SendReport(retcode, "Fail: Cellular connect fail");
		return;
	}
	Tests_SendReport(RETCODE_OK, "PASS");
}

static void RunQueryCellularStatusDisconnected(CCMsg_T* MessagePtr)
{
	Retcode_T retcode = RETCODE_OK;
	Cellular_DeviceStatus_T Value;

	if (NULL == MessagePtr)
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
		Tests_SendReport(retcode, " input parameter null ");
		return;
	}

	retcode = Cellular_QueryModem(CELLULAR_QUERYINFO_STATUS,&Value);
	if ((RETCODE_OK != retcode) || (CELLULAR_DEVICESTATUS_DISCONNECTED != Value))
	{
		Tests_SendReport(retcode, "Fail: Cellular disconnect fail");
		return;
	}
	Tests_SendReport(RETCODE_OK, "PASS");
}

Retcode_T QueryTestSuite_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* cellular on/connect test section register */
    retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);
    if (RETCODE_OK == retcode)
    {
        /* TC 1.5.1 Network information from modem */
        retcode = TEST_REGISTER_TC(RunQueryNetworkInformationFromModem);
    }
    if (RETCODE_OK == retcode)
    {
        /* TC 1.5.2 Network information from modem */
        retcode = TEST_REGISTER_TC(RunQueryErrorInformationFromModem);
    }
    if (RETCODE_OK == retcode)
    {
        /* TC 1.5.3 Cellular status - Initialized */
        retcode = TEST_REGISTER_TC(RunQueryCellularStatusInitialized);
    }
    if (RETCODE_OK == retcode)
    {
        /* TC 1.5.4 Cellular status - connected */
        retcode = TEST_REGISTER_TC(RunQueryCellularStatusConnected);
    }
    if (RETCODE_OK == retcode)
    {
        /* TC 1.5.5 Cellular status - Disconnected */
        retcode = TEST_REGISTER_TC(RunQueryCellularStatusDisconnected);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunQueryTurnOnId, NULL, RunTurnOn, NULL);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunQueryNetworkInitId, NULL, RunNetworkInit, NULL);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunQueryNetworkConfigConnectId, NULL, RunNetworkConfigConnect, NULL);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunQueryDisconnectId, NULL, RunDisconnect, NULL);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunQueryTurnOffId, NULL, RunTurnOff, NULL);
    }

    return (retcode);
}
