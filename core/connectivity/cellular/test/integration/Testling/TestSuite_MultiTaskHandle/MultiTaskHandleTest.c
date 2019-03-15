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

#include "BCDS_TestlingInfo.h"
#define BCDS_MODULE_ID  BCDS_TESTLING_CELLULAR_MODULE_ID_MULTI_TASK_HANDLE_TEST_SUITE

#include "BCDS_TestConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "CellularSupport.h"
#include "MultiTaskHandleTest.h"

#define TEST_SUITE_ID   BCDS_TEST_SUITE_MULTITASK_ID

static TaskHandle_t xHandle1 = NULL;
static TaskHandle_t xHandle2 = NULL;
static SemaphoreHandle_t TaskExitHandle;
static bool TaskExitEvent = false;

extern bool ErrorEventOccured;

/* Put private function declarations here */

/**
 * @brief   The task calls the config api endlessly, when the return value is
 * other than RETCODE_OK this task will be deleted and intimates the return value
 * to the calling function. It has a delay of 2Ms and priority 1.
 *
 * @param[in] pvParameter
 * A pointer to hold the pvParameter
 *
 */
static void vTaskWithMoreDelay(void *pvParameter);

/**
 * @brief   The task calls the config api endlessly, when the return value is
 * other than RETCODE_OK this task will be deleted and intimates the return value
 * to the calling function. It has a delay of 1Ms and priority 2.
 *
 * @param[in] pvParameter
 * A pointer to hold the pvParameter
 *
 */
static void vTaskWithLessDelay(void *pvParameter);

/**
 * @brief   The function describes the run for multitask handling test case
 * This test validates multi task handling by using config command
 *
 * @param[in] MessagePtr
 * A pointer to hold the TLV element structure
 *
 */
static void RunMultiTaskTest(CCMsg_T* MessagePtr);

/** The API is described while declaring the function(i.e., above)  */
void vTaskWithMoreDelay(void *pvParameter)
{
    BCDS_UNUSED(pvParameter);
    Retcode_T Retcode = RETCODE_OK;
    const TickType_t xDelay = DELAY_TIME_TASK_2 / portTICK_RATE_MS;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    memcpy(ConfigSetting.ApnName, APN_NAME, strlen(APN_NAME));

    memset(ConfigSetting.ApnUserName, '\0', sizeof(ConfigSetting.ApnUserName));
    memset(ConfigSetting.ApnPassword, '\0', sizeof(ConfigSetting.ApnPassword));

    for (;;)
    {
        if (ErrorEventOccured == TRUE)
        {
            Tests_SendReport(Retcode, "All ready error event occured");
        }
        else
        {
            while (RETCODE_OK == Retcode)
            {
                vTaskDelay(xDelay);
                Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);
                if (RETCODE_OK != Retcode)
                {
                    Tests_SendReport(Retcode, "Task 1 : Other Error Occurred");
                }
                if (ErrorEventOccured == TRUE)
                {
                    Tests_SendReport(RETCODE_FAILURE, "Task 1:Error Event Occurred.");
                    break;
                }
                if (TaskExitEvent)
                {
                    break;
                }
            }
        }
        xSemaphoreGive(TaskExitHandle);
        vTaskSuspend(NULL);
    }
}

/** The API is described while declaring the function(i.e., above)  */
void vTaskWithLessDelay(void *pvParameter)
{
    BCDS_UNUSED(pvParameter);
    Retcode_T Retcode = RETCODE_OK;
    const TickType_t xDelay = DELAY_TIME_TASK_1 / portTICK_RATE_MS;
    Cellular_ConfigurationSettings_T ConfigSetting = { { 0 }, { 0 }, { 0 } };

    memcpy(ConfigSetting.ApnName, APN_NAME, strlen(APN_NAME));

    memset(ConfigSetting.ApnUserName, '\0', sizeof(ConfigSetting.ApnUserName));
    memset(ConfigSetting.ApnPassword, '\0', sizeof(ConfigSetting.ApnPassword));

    for (;;)
    {
        if (ErrorEventOccured == TRUE)
        {
            Tests_SendReport(Retcode, "All ready error event occured");
        }
        else
        {
            while (RETCODE_OK == Retcode)
            {
                vTaskDelay(xDelay);
                Retcode = Cellular_Control(CELLULAR_CONFIG, (void*) &ConfigSetting);
                if (RETCODE_OK != Retcode)
                {
                    Tests_SendReport(Retcode, "Task 2 : Other Error Occurred");
                }
                if ((ErrorEventOccured == TRUE))
                {
                    Tests_SendReport(RETCODE_FAILURE, "Task 2:Error Event Occurred.");
                    break;
                }
                if (TaskExitEvent)
                {
                    break;
                }
            }
        }
        xSemaphoreGive(TaskExitHandle);
        vTaskSuspend(NULL);
    }
}

/** The API is described while declaring the function(i.e., above)  */
static void RunMultiTaskTest(CCMsg_T* MessagePtr)
{
    BCDS_UNUSED(MessagePtr);
    Retcode_T Retcode = RETCODE_FAILURE;
    int8_t RetValSemaphore, RetValTaskOne, RetValTaskTwo;
    const TickType_t vDelay = MAX_TIMEOUT_FOR_TEST_CASE_WAIT / portTICK_RATE_MS;

    if (NULL == MessagePtr)
    {
        Retcode = RETCODE_NULL_POINTER;
    }
    else
    {
        TaskExitHandle = xSemaphoreCreateBinary();
        if (NULL != TaskExitHandle)
        {

            TaskExitEvent = false;

            RetValTaskOne = xTaskCreate(vTaskWithMoreDelay, "Task1", STACK_SIZE_TASK_1, NULL, PRIORITY_TASK_1, &xHandle1);

            if ((pdPASS != RetValTaskOne) || (xHandle1 == NULL))
            {
                Tests_SendReport(Retcode, "Task 1 is not created");
                return;
            }

            RetValTaskTwo = xTaskCreate(vTaskWithLessDelay, "Task2", STACK_SIZE_TASK_2, NULL, PRIORITY_TASK_2, &xHandle2);

            if ((pdPASS != RetValTaskTwo) || (xHandle2 == NULL))
            {
                Tests_SendReport(Retcode, "Task 2 is not created");
                return;
            }

            RetValSemaphore = xSemaphoreTake(TaskExitHandle, vDelay);

            if (pdTRUE != RetValSemaphore)
            {
                Tests_SendReport(RETCODE_OK, "PASS");
            }

            TaskExitEvent = true;

            while (eTaskGetState(xHandle1) != eSuspended || eTaskGetState(xHandle2) != eSuspended)
            {
                vTaskDelay(DELAY_TIME_TASK_1 / portTICK_RATE_MS);
            }

            vTaskDelete(xHandle1);
            vTaskDelete(xHandle2);
            vSemaphoreDelete(TaskExitHandle);
        }
        else
        {
            /* No memory to create a new semaphore binary */
            Tests_SendReport(RETCODE_OUT_OF_RESOURCES, "No memory to create semaphore");
            return;
        }
    }

}

Retcode_T MultiTaskHandleTestSuite_Init(void)
{
    Retcode_T Retcode = RETCODE_OK;

    Retcode = Tests_RegisterTestSuite(TEST_SUITE_ID, NULL, NULL);

    if (RETCODE_OK == Retcode)
    {
        /* TC Testing MultiTast Handling */
        Retcode = TEST_REGISTER_TC(RunMultiTaskTest);
    }

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunMultiTaskTurnOnId, NULL, RunTurnOn, NULL);
    }

    if (RETCODE_OK == Retcode)
    {
        Retcode = Tests_RegisterTestCase(TEST_SUITE_ID, RunMultiTaskTurnOffId, NULL, RunTurnOff, NULL);
    }

    return Retcode;
}
