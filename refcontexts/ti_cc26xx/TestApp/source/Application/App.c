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


/* system header files */
#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_APP

#include <string.h>

#include "BCDS_Tests.h"
#include "BCDS_TestSection.h"
#include "BCDS_CmdProcessor.h"

#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>

#include "App.h"

#define APP_STACK_SIZE UINT16_C(2048)
#define APP_QUEUE_SIZE  UINT8_C(10)

/**
 * @brief   processing of application initialization events
 *
 * @param[in]   parama1,param2
 *
 */

static void appInit(void *param1, uint32_t param2)
{
	Retcode_T retval = RETCODE_OK;
	BCDS_UNUSED(  param1 );
	BCDS_UNUSED(  param2 );
	BCDS_UNUSED(  retval );
	retval = TestSection_Initialize();
	assert(RETCODE_OK == retval);
}

/**
 * @brief  CommandProcessor Init happens here.
 *
 */
void TestApp_init(void)
{
    Retcode_T retval = RETCODE_OK;
	BCDS_UNUSED(retval);

    retval = CmdProcessor_Initialize(NULL, (char*)"CmdProcessor", 1, APP_STACK_SIZE, APP_QUEUE_SIZE);
    assert(RETCODE_OK == retval);

    retval = CmdProcessor_Enqueue(NULL, appInit, NULL, UINT32_C(0));
    assert(RETCODE_OK == retval);
}
