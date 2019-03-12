/*****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  This module initializes all the test application.
 *
 * ****************************************************************************/

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
