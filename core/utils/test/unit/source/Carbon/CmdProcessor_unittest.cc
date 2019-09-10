/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
 *
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

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */

FFF_DEFINITION_BLOCK_START

/* setup compile time configuration defines */
//-- instead of Kiso_BSP_BoardConfig.h mock
/* include faked interfaces */
extern "C"
{
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_CMDPROCESSOR

#if KISO_FEATURE_CMDPROCESSOR

/* include faked interfaces */
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#include "task_th.hh"
#include "queue_th.hh"

/* include module under test */
#include "CmdProcessor.c"

}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

/* setup compile time configuration defines */
#define TASK_PRIORITY     UINT32_C(1)     /**< Task Priority should be less then timer task priority */
#define STACK_SIZE        UINT32_C(256)   /**< stack size of the task */
#define QUEUE_SIZE        UINT32_C(3)     /**< size of the queue.Holds command processor command structure variables */
#define INIT_VAL          UINT32_C(2)     /**< Used as an initial value to variables */
#define QUEUE_CREATED     UINT32_C(1)     /**< Used as a return value for successful queue creation */

/* Fake function created to pass as a argument to the  cmdProcessor_Enqueue function */
void fake_fn(void *, uint32_t)
{
    ;
}

static CmdProcessor_Cmd_T cmd;

signed long myXQueueReceive(QueueHandle_t xQueue, void * pvBuffer, TickType_t xTicksToWait)
{
    KISO_UNUSED(xQueue);

    EXPECT_EQ(portMAX_DELAY, xTicksToWait);

    *((CmdProcessor_Cmd_T*)pvBuffer) = cmd;

    return pdPASS;
}

BaseType_t xTaskCreate_fake_success(TaskFunction_t, const char * const,
                                    const configSTACK_DEPTH_TYPE,
                                    void * const, UBaseType_t,
                                    TaskHandle_t * const pxCreatedTask)
{
    *pxCreatedTask = (TaskHandle_t) 0x123;
    return pdTRUE;
}

class CmdProcessor: public testing::Test
{
protected:

    virtual void SetUp()
    {
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xQueueCreate);
        RESET_FAKE(xQueueSend);
        RESET_FAKE(xQueueSendFromISR);
        RESET_FAKE(vTaskSuspend);
        RESET_FAKE(vTaskResume);
        RESET_FAKE(xQueueReceive);
        RESET_FAKE(Retcode_RaiseError);

        memset(&cmd, 0, sizeof(CmdProcessor_Cmd_T));

        xTaskCreate_fake.custom_fake = xTaskCreate_fake_success;
        xQueueCreate_fake.return_val = (QueueHandle_t) QUEUE_CREATED;

        Retcode_T retVal = CmdProcessor_Initialize(&cmdProcessor, "abc", TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);
        EXPECT_EQ(RETCODE_OK, retVal);

        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xQueueCreate);

        FFF_RESET_HISTORY();
    }

    CmdProcessor_T cmdProcessor = {0};
};

/* specify test cases ******************************************************* */

/**
 *  Module is used to execute the function from the queue and it is provided by the apis.
 *  It contains the overall test cases to test the queue creation and task creation by the defined apis in C module
 */

TEST_F(CmdProcessor, cmdProcessorIntializationQueueFail)
{
    CmdProcessor_T cmdProcessorInstance;
    Retcode_T retVal = RETCODE_OK;
    xQueueCreate_fake.return_val = NULL;

    retVal = CmdProcessor_Initialize(&cmdProcessorInstance, "abc", TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
}

TEST_F(CmdProcessor, cmdProcessorIntializationTaskFail)
{
    CmdProcessor_T cmdProcessorInstance;
    Retcode_T retVal = RETCODE_OK;
    xQueueCreate_fake.return_val = (QueueHandle_t) QUEUE_CREATED;
    xTaskCreate_fake.return_val = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;

    retVal = CmdProcessor_Initialize(&cmdProcessorInstance, "abc", TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
}

TEST_F(CmdProcessor, cmdProcessorIntializationTaskSuccess)
{
    CmdProcessor_T cmdProcessorInstance;
    Retcode_T retVal = RETCODE_FAILURE;
    xTaskCreate_fake.return_val = pdTRUE;
    xQueueCreate_fake.return_val = (QueueHandle_t) QUEUE_CREATED;

    retVal = CmdProcessor_Initialize(&cmdProcessorInstance, "abc", TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);

    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
}

TEST_F(CmdProcessor, cmdProcessorIntializationNullParam)
{
    Retcode_T retVal = RETCODE_FAILURE;

    retVal = CmdProcessor_Initialize(NULL, "abc", TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(0), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xQueueCreate_fake.call_count);
}

TEST_F(CmdProcessor, cmdProcessorTaskNameExceedsLength)
{
    CmdProcessor_T cmdProcessorInstance;
    char taskName[] = "abcdefghijklmnopqrstuvwxya123456789";
    char truncatedTaskName[] = "abcdefghijklmnopqrstuvwxya12345";
    Retcode_T retVal = RETCODE_FAILURE;
    xQueueCreate_fake.return_val = (QueueHandle_t) QUEUE_CREATED;
    xTaskCreate_fake.return_val = pdTRUE;

    retVal = CmdProcessor_Initialize(&cmdProcessorInstance, taskName, TASK_PRIORITY, STACK_SIZE, QUEUE_SIZE);

    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
    ASSERT_EQ(INT32_C(0), strcmp(xTaskCreate_fake.arg1_val, truncatedTaskName));
    EXPECT_EQ(UINT32_C(1), xTaskCreate_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xQueueCreate_fake.call_count);
}

TEST_F(CmdProcessor, CmdProcessorEnqueueCmdProcessorNull)
{
    Retcode_T retVal = RETCODE_OK;
    retVal = CmdProcessor_Enqueue(NULL, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorEnqueueCmdProcessorQueueNull)
{
    Retcode_T retVal = RETCODE_OK;
    cmdProcessor.queue = NULL;
    retVal = CmdProcessor_Enqueue(&cmdProcessor, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorEnqueueQueueFull)
{
    Retcode_T retVal = RETCODE_OK;
    xQueueSend_fake.return_val = errQUEUE_FULL;
    retVal = CmdProcessor_Enqueue(&cmdProcessor, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(UINT32_C(1), xQueueSend_fake.call_count);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorEnqueueSuccess)
{
    Retcode_T retVal = RETCODE_OK;
    xQueueSend_fake.return_val = pdPASS;

    retVal = CmdProcessor_Enqueue(&cmdProcessor, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(UINT32_C(1), xQueueSend_fake.call_count);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorEnqueueFromISRSuccess)
{
    Retcode_T retVal = RETCODE_FAILURE;
    xQueueSendFromISR_fake.return_val = pdPASS;

    retVal = CmdProcessor_EnqueueFromIsr(&cmdProcessor, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xQueueSendFromISR_fake.call_count);
}

TEST_F(CmdProcessor, CmdProcessorEnqueueFromISRQueueFull)
{
    Retcode_T retVal = RETCODE_FAILURE;
    xQueueSendFromISR_fake.return_val = errQUEUE_FULL;

    retVal = CmdProcessor_EnqueueFromIsr(&cmdProcessor, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retVal));
    EXPECT_EQ(UINT32_C(1), xQueueSendFromISR_fake.call_count);
}

TEST_F(CmdProcessor, CmdProcessorEnqueueFromISRCmdProcessorNull)
{
    Retcode_T retVal = RETCODE_OK;

    retVal = CmdProcessor_EnqueueFromIsr(NULL, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorEnqueueFromISRQueueNull)
{
    Retcode_T retVal = RETCODE_OK;
    cmdProcessor.queue = NULL;

    retVal = CmdProcessor_EnqueueFromIsr(&cmdProcessor, fake_fn, NULL, INIT_VAL);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retVal));
}

TEST_F(CmdProcessor, CmdProcessorSuspendCmdProcessorNull)
{
    CmdProcessor_Suspend(NULL);

    EXPECT_EQ(UINT32_C(0), vTaskSuspend_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}

TEST_F(CmdProcessor, CmdProcessorSuspendSuccess)
{
    CmdProcessor_Suspend(&cmdProcessor);

    EXPECT_EQ(UINT32_C(1), vTaskSuspend_fake.call_count);
    EXPECT_EQ(UINT32_C(0), Retcode_RaiseError_fake.call_count);
}

TEST_F(CmdProcessor, CmdProcessorSuspendTaskNull)
{
    cmdProcessor.task = NULL;

    CmdProcessor_Suspend(&cmdProcessor);

    EXPECT_EQ(UINT32_C(0), vTaskSuspend_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}

TEST_F(CmdProcessor, CmdProcessorResumeNullParam)
{
    CmdProcessor_Resume(NULL);


    EXPECT_EQ(UINT32_C(0), vTaskResume_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}

TEST_F(CmdProcessor, CmdProcessorResumeSuccess)
{
    CmdProcessor_Resume(&cmdProcessor);


    EXPECT_EQ(UINT32_C(1), vTaskResume_fake.call_count);
    EXPECT_EQ(UINT32_C(0), Retcode_RaiseError_fake.call_count);
}

TEST_F(CmdProcessor, CmdProcessorResumeTaskNull)
{
    cmdProcessor.task = NULL;

    CmdProcessor_Resume(&cmdProcessor);

    EXPECT_EQ(UINT32_C(0), vTaskResume_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}

TEST_F(CmdProcessor, CmdProcessorDequeueSuccess)
{
    xQueueReceive_fake.custom_fake = &myXQueueReceive;
    cmd.func = &fake_fn;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    Dequeue(&cmdProcessor);

    EXPECT_EQ(UINT32_C(1), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(0), Retcode_RaiseError_fake.call_count);
}

TEST_F(CmdProcessor, CmdProcessorDequeueFail)
{
    xQueueReceive_fake.custom_fake = &myXQueueReceive;
    cmd.func = NULL;
    cmd.param1 = NULL;
    cmd.param2 = UINT32_C(0);

    Dequeue(&cmdProcessor);

    EXPECT_EQ(UINT32_C(1), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));

}

TEST_F(CmdProcessor, CmdProcessorDequeueReceiveFail)
{
    xQueueReceive_fake.return_val = pdFAIL;

    Dequeue(&cmdProcessor);

    EXPECT_EQ(UINT32_C(1), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_CMDPROCESSOR_QUEUE_ERROR, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}

TEST_F(CmdProcessor, CmdProcessorDequeueCmdPrcsrFail)
{
    Dequeue(NULL);

    EXPECT_EQ(UINT32_C(0), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}


TEST_F(CmdProcessor, CmdProcessorDequeueCmdPrcsrQueueFail)
{
    cmdProcessor.queue = NULL;

    Dequeue(&cmdProcessor);

    EXPECT_EQ(UINT32_C(0), xQueueReceive_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseError_fake.call_count);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(Retcode_RaiseError_fake.arg0_val));
}
#else
}

#endif