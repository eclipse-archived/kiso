/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

/* include gtest interface */
#include <gtest.h>

extern "C"
{
#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_TASKMONITOR

#if BCDS_FEATURE_TASKMONITOR

/* start of global scope symbol and fake definitions section */

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"

#include "task_th.hh"


/* include module under test */
#include "TaskMonitor.c"

} /* end of global scope symbol and fake definitions section */

TaskHandle_t Tasklist[BCDS_TASKMONITOR_MAX_TASKS];
uint32_t TaskTag[BCDS_TASKMONITOR_MAX_TASKS];

TaskHookFunction_t xTaskGetApplicationTaskTagCustom( TaskHandle_t xTask )
{
	TaskHookFunction_t ret;
	uint32_t loopcnt;
	ret = (TaskHookFunction_t ) TaskTag[0];

	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		if(xTask == Tasklist[loopcnt])
		{
			ret = (TaskHookFunction_t ) TaskTag[loopcnt];
			break;
		}
	}

	return ret;
}

class TaskMonitor: public testing::Test
{
protected:

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        ;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

/* specify test cases ******************************************************* */

TEST_F(TaskMonitor, TaskMonitor_InitializeTest)
{
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T retVal;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = TaskMonitor_Initialize();

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
}

TEST_F(TaskMonitor, TaskMonitor_UpdateTest)
{
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint32_t *taskPtr;
    uint32_t data;
    taskPtr = &data;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    TaskMonitor_Update(taskPtr, 0xFFFFUL);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(data, 0xFFFFUL);
}

TEST_F(TaskMonitor, TaskMonitor_RegisterTest)
{

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T retVal;
    TaskHandle_t task;
    uint32_t loopcnt;
    /* reset the task monitor */
    task = (TaskHandle_t)&retVal;
    retVal = TaskMonitor_Initialize();
	EXPECT_EQ(RETCODE_OK, retVal);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
    {
       retVal = TaskMonitor_Register(task, (loopcnt+1));
       /* VERIFY : Compare the expected with actual */
       EXPECT_EQ(RETCODE_OK, retVal);
    }
    /* failure test case */
    retVal = TaskMonitor_Register(task, (loopcnt+1));
    EXPECT_NE(RETCODE_OK, retVal);

    retVal = TaskMonitor_Register(NULL, (loopcnt+1));
    EXPECT_NE(RETCODE_OK, retVal);

    retVal = TaskMonitor_Register(task, 0UL);
    EXPECT_NE(RETCODE_OK, retVal);

    retVal = TaskMonitor_Register(NULL, 0UL);
    EXPECT_NE(RETCODE_OK, retVal);
    /* reset the task monitor */
    retVal = TaskMonitor_Initialize();
    EXPECT_EQ(RETCODE_OK, retVal);
}

TEST_F(TaskMonitor, TaskMonitor_CheckTest)
{

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T retVal;
    bool monitorCheck;
    uint32_t loopcnt;
    /* reset the task monitor */

    retVal = TaskMonitor_Initialize();
	EXPECT_EQ(RETCODE_OK, retVal);
	memset(TaskTag, 0x00, sizeof(TaskTag));

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
    {
    	Tasklist[loopcnt] = (TaskHandle_t)&TaskTag[loopcnt];
    	retVal = TaskMonitor_Register(Tasklist[loopcnt], 1UL);
       /* VERIFY : Compare the expected with actual */
       EXPECT_EQ(RETCODE_OK, retVal);

       TaskMonitor_Update(&TaskTag[loopcnt], 500UL);
    }


    xTaskGetTickCount_fake.return_val = 510;
    xTaskGetApplicationTaskTag_fake.custom_fake = &xTaskGetApplicationTaskTagCustom;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    monitorCheck = TaskMonitor_Check();
    if(monitorCheck == false)
    {
    	retVal = RETCODE_OK;
    }
    else
    {
    	retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);

    }
    EXPECT_EQ(RETCODE_OK, retVal);

    xTaskGetTickCount_fake.return_val = 490UL;

	/* EXECISE: call relevant production code Interface with appropriate test inputs  */
	monitorCheck = TaskMonitor_Check();
    if(monitorCheck == false)
    {
    	retVal = RETCODE_OK;
    }
    else
    {
    	retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
    }
    EXPECT_NE(RETCODE_OK, retVal);

    xTaskGetTickCount_fake.return_val = 500UL;

	/* EXECISE: call relevant production code Interface with appropriate test inputs  */
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_EQ(RETCODE_OK, retVal);

	/* task 6 is not executed with in 1 second */
	xTaskGetTickCount_fake.return_val = 5000UL;
	/* update all tasks execution time */
	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		TaskMonitor_Update(&TaskTag[loopcnt], 4500UL);
	}
	/* change task 6 execution time to test the failure condition  */
	TaskMonitor_Update(&TaskTag[BCDS_TASKMONITOR_MAX_TASKS -1], 2100UL);
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_NE(RETCODE_OK, retVal);

	/* test with current tick count souver flow condition */
	xTaskGetTickCount_fake.return_val = 10UL; /* current time 10mS counter overflow*/
	/* update all tasks execution time */
	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		TaskMonitor_Update(&TaskTag[loopcnt], UINT32_MAX-5);
	}

	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_EQ(RETCODE_OK, retVal);

	/* test : last task has failed to execute */
	xTaskGetTickCount_fake.return_val = 150UL; /* current time 10mS counter overflow*/
	/* update all tasks execution time */
	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		TaskMonitor_Update(&TaskTag[loopcnt], UINT32_MAX-5);
	}
	/* change last task execution time to test the failure condition  */
	TaskMonitor_Update(&TaskTag[BCDS_TASKMONITOR_MAX_TASKS-1], UINT32_MAX-1000);
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_NE(RETCODE_OK, retVal);

	/* test : last task has failed to execute */
	xTaskGetTickCount_fake.return_val = 149UL; /* current time 10mS counter overflow*/
	/* update all tasks execution time */
	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		TaskMonitor_Update(&TaskTag[loopcnt], 150UL);
	}
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_EQ(RETCODE_OK, retVal);

	/* test : last task has failed to execute */
	xTaskGetTickCount_fake.return_val = 148UL; /* current time 10mS counter overflow*/
	/* update all tasks execution time */
	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		TaskMonitor_Update(&TaskTag[loopcnt], 150UL);
	}
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_EQ(RETCODE_OK, retVal);

	/* test : last task has failed to execute */
	xTaskGetTickCount_fake.return_val = 147UL; /* current time 10mS counter overflow*/
	/* update all tasks execution time */
	for(loopcnt = 0U; loopcnt < BCDS_TASKMONITOR_MAX_TASKS; loopcnt++)
	{
		TaskMonitor_Update(&TaskTag[loopcnt], 150UL);
	}
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_NE(RETCODE_OK, retVal);


	/* test : check the behavior if no tasks are registered to monitor */
	retVal = TaskMonitor_Initialize();
	EXPECT_EQ(RETCODE_OK, retVal);
	monitorCheck = TaskMonitor_Check();
	if(monitorCheck == false)
	{
		retVal = RETCODE_OK;
	}
	else
	{
		retVal = RETCODE(RETCODE_SEVERITY_ERROR, (Retcode_T)RETCODE_FAILURE);
	}
	EXPECT_EQ(RETCODE_OK, retVal);

}
#else
}
#endif
