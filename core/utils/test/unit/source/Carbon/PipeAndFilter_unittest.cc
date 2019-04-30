/******************************************************************************/
/**
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

extern "C"
{/* start of global scope symbol and fake definitions section */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID CDDK_UTILS_MODULE_ID_PIPEANDFILTER

#if CDDK_FEATURE_PIPEANDFILTER

/* include faked interfaces */
#include "BCDS_Retcode_th.hh"

#include "task_th.hh"
#include "message_buffer_th.hh"
#include "stream_buffer_th.hh"

/* Ensure the filter-task will not run anymore */
#define RUN_FILTER_ALWAYS 0
/* include module under test */
#include "PipeAndFilter.c"

} /* end of global scope symbol and fake definitions section */

/* setup compile time configuration defines */
#define PIPE_SIZE 100
#define FILTER_STACK_SIZE 500
#define FILTER_PRIORITY 1

FAKE_VALUE_FUNC(Retcode_T, functionA, uint8_t *, uint32_t, uint8_t *, uint32_t *);

/** Used to fake the task handle */
TaskHandle_t taskHandleOfFakeFunction;
BaseType_t xTaskCreateCustom(TaskHookFunction_t func, const char * name, unsigned short size, void * param, UBaseType_t prio, TaskHandle_t* handle)
{
	(void)func;
	(void)name;
	(void)size;
	(void)param;
	(void)prio;
	(*handle) = taskHandleOfFakeFunction;
	return pdTRUE;
}

/** Used to fake the filter handle */
Retcode_T retcodeFakeValue;
uint32_t sizeBuffOutFakeValue;
Retcode_T filterCustom(uint8_t * bufferIn, uint32_t sizeBuffIn, uint8_t * bufferOut, uint32_t * sizeBuffOut)
{
	(void)bufferIn;
	(void)sizeBuffIn;
	(void)bufferOut;
	(*sizeBuffOut) = sizeBuffOutFakeValue;
	return retcodeFakeValue;
}

class PipeAndFilter: public testing::Test
{
protected:

    virtual void SetUp()
    {
        // Reset fakes
    	RESET_FAKE(xTaskCreate);
        RESET_FAKE(Retcode_RaiseError);
        RESET_FAKE(xMessageBufferCreate);
        RESET_FAKE(xTaskCreate);
        RESET_FAKE(xStreamBufferSend);
        RESET_FAKE(xStreamBufferSendFromISR);
        RESET_FAKE(xStreamBufferReceive);
        RESET_FAKE(functionA);

        // Initialization
        // TODO

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



TEST_F(PipeAndFilter, initializePipeSuccess)
{
    /** @testcase{ PipeAndFilter::initializePipeSuccess: }
     * Successful initialization
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	xStreamBufferGenericCreate_fake.return_val = (MessageBufferHandle_t)0xAAAAAAAA;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_CreatePipe(&pipeHandle);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(1, (int)xStreamBufferGenericCreate_fake.call_count);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(PipeAndFilter, initializePipeFailure)
{
    /** @testcase{ PipeAndFilter::initializePipeFailure: }
     * Not enough resources left in the heap
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	xStreamBufferGenericCreate_fake.return_val = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_CreatePipe(&pipeHandle);

    /* VERIFY : Compare the expected with actual */
    EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(PipeAndFilter, initializeFilterSuccess)
{
    /** @testcase{ PipeAndFilter::initializeFilterSuccess: }
     * Manage to successful initialize a filter
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;
	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;

	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, &filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(filter1.filterInternalHandle, taskHandleOfFakeFunction);
	EXPECT_EQ(pipeHandleIn.filterInternalHandle, taskHandleOfFakeFunction);
	EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(PipeAndFilter, initializeFilterSuccessWithNoInputPipe)
{
    /** @testcase{ PipeAndFilter::initializeFilterSuccess: }
     * Manage to successful initialize a filter
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;
	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;

	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, NULL, &pipeHandleOut, &filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(filter1.filterInternalHandle, taskHandleOfFakeFunction);
	EXPECT_EQ(RETCODE_OK, retcode);
}
TEST_F(PipeAndFilter, initializeFilterNullPointer)
{
    /** @testcase{ PipeAndFilter::initializeFilterNullPointer: }
     * Filter ressource is NULL
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T * filter1 = NULL;
	taskHandleOfFakeFunction = NULL;

	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_NE(RETCODE_OK, retcode);
}


TEST_F(PipeAndFilter, initializeFilterFailure)
{
    /** @testcase{ PipeAndFilter::initializeFilterFailure: }
     * Not enough resources left in the heap
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;
	taskHandleOfFakeFunction = NULL;

	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, &filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	ASSERT_TRUE(filter1.filterInternalHandle == NULL);
	ASSERT_TRUE(pipeHandleIn.filterInternalHandle == NULL);
	EXPECT_NE(RETCODE_OK, retcode);
}

TEST_F(PipeAndFilter, fillPipeSuccess)
{
    /** @testcase{ PipeAndFilter::fillPipeSuccess: }
     * Successful send
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	uint8_t buffer[5] ={1, 2, 3, 4, 5};
	xStreamBufferSend_fake.return_val = 5; // 5 bytes are sent


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_FillPipe(pipeHandle, buffer, sizeof(buffer));

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferSend_fake.call_count);
    EXPECT_EQ(retcode, RETCODE_OK);
}

TEST_F(PipeAndFilter, fillPipeNullPointers)
{
    /** @testcase{ PipeAndFilter::fillPipeNullPointers: }
     * Null pointer check
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	uint8_t buffer[5] ={1, 2, 3, 4, 5};
	xStreamBufferSend_fake.return_val = 5; // 5 bytes are sent


    /* EXECISE 1: call relevant production code Interface with appropriate test inputs  */
	pipeHandle.pipeInternalHandle = NULL;
	retcode = PipeAndFilter_FillPipe(pipeHandle, buffer, sizeof(buffer));

    /* VERIFY 1: Compare the expected with actual */
	EXPECT_EQ(0, (int)xStreamBufferSend_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);

    /* EXECISE 2: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_FillPipe(pipeHandle, (uint8_t *)NULL, sizeof(buffer));

    /* VERIFY 2: Compare the expected with actual */
	EXPECT_EQ(0, (int)xStreamBufferSend_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);

    /* EXECISE 3: call relevant production code Interface with appropriate test inputs  */
    pipeHandle.pipeInternalHandle = (PipeAndFilter_FilterInternalHandle_T)0x00000001;
    retcode = PipeAndFilter_FillPipe(pipeHandle, (uint8_t *)NULL, sizeof(buffer));

   /* VERIFY 3: Compare the expected with actual */
   	EXPECT_EQ(0, (int)xStreamBufferSend_fake.call_count);
   	EXPECT_NE(retcode, RETCODE_OK);
}

TEST_F(PipeAndFilter, fillPipeFailToFill)
{
    /** @testcase{ PipeAndFilter::fillPipeFailToFill: }
     * Buffer full
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	uint8_t buffer[5] ={1, 2, 3, 4, 5};
	xStreamBufferSend_fake.return_val = 0; // 0 bytes could be sent, full.


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_FillPipe(pipeHandle, buffer, sizeof(buffer));

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferSend_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);
}

TEST_F(PipeAndFilter, fillPipeFromISRSuccess)
{
    /** @testcase{ PipeAndFilter::fillPipeFromISRSuccess: }
     * Successful send from ISR
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	uint8_t buffer[5] ={1, 2, 3, 4, 5};
	xStreamBufferSendFromISR_fake.return_val = 5; // 5 bytes are sent


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_FillPipeFromISR(pipeHandle, buffer, sizeof(buffer));

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferSendFromISR_fake.call_count);
    EXPECT_EQ(retcode, RETCODE_OK);
}

TEST_F(PipeAndFilter, fillPipeFromISRNullPointers)
{
    /** @testcase{ PipeAndFilter::fillPipeFromISRNullPointers: }
     * Null pointer check
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	uint8_t buffer[5] ={1, 2, 3, 4, 5};
	xStreamBufferSendFromISR_fake.return_val = 5; // 5 bytes are sent


    /* EXECISE 1: call relevant production code Interface with appropriate test inputs  */
	pipeHandle.pipeInternalHandle = NULL;
	retcode = PipeAndFilter_FillPipeFromISR(pipeHandle, buffer, sizeof(buffer));

    /* VERIFY 1: Compare the expected with actual */
	EXPECT_EQ(0, (int)xStreamBufferSendFromISR_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);

    /* EXECISE 2: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_FillPipeFromISR(pipeHandle, (uint8_t *)NULL, sizeof(buffer));

    /* VERIFY 2: Compare the expected with actual */
	EXPECT_EQ(0, (int)xStreamBufferSendFromISR_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);

    /* EXECISE 3: call relevant production code Interface with appropriate test inputs  */
    pipeHandle.pipeInternalHandle = (PipeAndFilter_FilterInternalHandle_T)0x00000001;
    retcode = PipeAndFilter_FillPipeFromISR(pipeHandle, (uint8_t *)NULL, sizeof(buffer));

   /* VERIFY 3: Compare the expected with actual */
    EXPECT_EQ(0, (int)xStreamBufferSend_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);
}

TEST_F(PipeAndFilter, fillPipeFromISRFailToFill)
{
    /** @testcase{ PipeAndFilter::fillPipeFromISRFailToFill: }
     * Buffer full
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandle;
	uint8_t buffer[5] ={1, 2, 3, 4, 5};
	xStreamBufferSendFromISR_fake.return_val = 0; // 0 bytes could be sent, full.


    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	retcode = PipeAndFilter_FillPipeFromISR(pipeHandle, buffer, sizeof(buffer));

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferSendFromISR_fake.call_count);
    EXPECT_NE(retcode, RETCODE_OK);
}

TEST_F(PipeAndFilter, filterRunSuccessWithPipes)
{
    /** @testcase{ PipeAndFilter::filterRunSuccessWithPipes: }
     * Filter function was not given as parameter ot the filter-structure...
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;

	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;
	retcodeFakeValue = RETCODE_OK;
	sizeBuffOutFakeValue = 5;

	/* Set fake functions & values */
	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;
	functionA_fake.custom_fake = &filterCustom;
	xStreamBufferReceive_fake.return_val = 5; // 5 bytes are received
	xStreamBufferSend_fake.return_val = 5; // 5 bytes are send

	/* Initialize the filter */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, &filter1);
	/* Check initialization */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(&filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferReceive_fake.call_count);
	EXPECT_EQ(1, (int)xStreamBufferSend_fake.call_count);
	EXPECT_EQ(1, (int)functionA_fake.call_count);
}

TEST_F(PipeAndFilter, filterRunFailurePipeInCalledWithNoParam)
{
    /** @testcase{ PipeAndFilter::filterRunFailurePipeInCalledWithNoParam: }
     * Somehow the pipe send a message of 0 bytes...
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;

	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;
	retcodeFakeValue = RETCODE_OK;
	sizeBuffOutFakeValue = 5;

	/* Set fake functions & values */
	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;
	functionA_fake.custom_fake = &filterCustom;
	xStreamBufferReceive_fake.return_val = 0; // 0 bytes are received

	/* Initialize the filter */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, &filter1);
	/* Check initialization */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(&filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferReceive_fake.call_count);
	EXPECT_EQ(0, (int)xStreamBufferSend_fake.call_count);
	EXPECT_EQ(0, (int)functionA_fake.call_count);
	EXPECT_EQ(1, (int)Retcode_RaiseError_fake.call_count);
}

TEST_F(PipeAndFilter, filterRunFailurePipeOutParamMatching)
{
    /** @testcase{ PipeAndFilter::filterRunFailurePipeOutParamMatching: }
     * Somehow the pipe out is full
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;

	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;
	retcodeFakeValue = RETCODE_OK;
	sizeBuffOutFakeValue = 5;

	/* Set fake functions & values */
	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;
	functionA_fake.custom_fake = &filterCustom;
	xStreamBufferReceive_fake.return_val = 5; // 5 bytes are received
	xStreamBufferSend_fake.return_val = 0; // 0 bytes are send

	/* Initialize the filter */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, &filter1);
	/* Check initialization */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(&filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferReceive_fake.call_count);
	EXPECT_EQ(1, (int)xStreamBufferSend_fake.call_count);
	EXPECT_EQ(1, (int)functionA_fake.call_count);
	EXPECT_EQ(1, (int)Retcode_RaiseError_fake.call_count); // Because with RUN_FILTER_ALWAYS we force an exit, the raise error is called a second time
}

TEST_F(PipeAndFilter, filterRunFailurePipeOutParamOverflow)
{
    /** @testcase{ PipeAndFilter::filterRunFailurePipeOutParamOverflow: }
     * Somehow the buffer to send out is bigger than the pipe out
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Pipe_T pipeHandleIn;
	PipeAndFilter_Pipe_T pipeHandleOut;
	PipeAndFilter_Filter_T filter1;

	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;
	retcodeFakeValue = RETCODE_OK;
	sizeBuffOutFakeValue = 105;

	/* Set fake functions & values */
	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;
	functionA_fake.custom_fake = &filterCustom;
	xStreamBufferReceive_fake.return_val = 5; // 5 bytes are received
	xStreamBufferSend_fake.return_val = 0; // 105 bytes are send, exceeding pipe size

	/* Initialize the filter */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, &pipeHandleIn, &pipeHandleOut, &filter1);
	/* Check initialization */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(&filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)xStreamBufferReceive_fake.call_count);
	EXPECT_EQ(0, (int)xStreamBufferSend_fake.call_count);
	EXPECT_EQ(1, (int)functionA_fake.call_count);
	EXPECT_EQ(1, (int)Retcode_RaiseError_fake.call_count); // Because with RUN_FILTER_ALWAYS we force an exit, the raise error is called a second time
}

TEST_F(PipeAndFilter, filterRunSuccessWithNoPipes)
{
    /** @testcase{ PipeAndFilter::filterRunSuccessWithPipes: }
     * Filter function was not given as parameter ot the filter-structure...
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	Retcode_T retcode = RETCODE_OK;
	PipeAndFilter_Filter_T filter1;

	taskHandleOfFakeFunction = (TaskHandle_t)0x00000001;
	retcodeFakeValue = RETCODE_OK;

	/* Set fake functions & values */
	xTaskCreate_fake.custom_fake = &xTaskCreateCustom;
	functionA_fake.custom_fake = &filterCustom;

	/* Initialize the filter */
	retcode = PipeAndFilter_CreateFilter((PipeAndFilter_FilterFunction_T)functionA, NULL, NULL, &filter1);
	/* Check initialization */
	EXPECT_EQ(1, (int)xTaskCreate_fake.call_count);
	EXPECT_EQ(RETCODE_OK, retcode);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(&filter1);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)functionA_fake.call_count);
}

TEST_F(PipeAndFilter, filterRunFailure)
{
    /** @testcase{ PipeAndFilter::filterRunFailure: }
     * Filter function was not given as parameter to the filter-structure...
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	PipeAndFilter_Filter_T *filterContext = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(filterContext);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)Retcode_RaiseError_fake.call_count);
}

TEST_F(PipeAndFilter, filterRunFailureNoFunction)
{
    /** @testcase{ PipeAndFilter::filterRunFailureNoFunction: }
     * Random test case
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
	PipeAndFilter_Filter_T filterContext;
	filterContext.filterFunction = NULL;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
	RunFilter(&filterContext);

    /* VERIFY : Compare the expected with actual */
	EXPECT_EQ(1, (int)Retcode_RaiseError_fake.call_count);
}

#else
}

#endif /* if CDDK_FEATURE_PIPEANDFILTER */
