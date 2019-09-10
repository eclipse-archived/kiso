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
/**
 * @file
 *
 *
 *  Module test specification for the Lean B2CAP module.
 *
 * ****************************************************************************/

/* setup testing framework ************************************************** */

/* include gtest */
#include <gtest.h>

extern "C"
{
/* setup compile time configuration defines */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LEANB2CAP

#if KISO_FEATURE_LEANB2CAP

/* include faked interfaces */
#include "Kiso_Retcode_th.hh"

#include "LeanB2CAPConfig.h"
#include "Kiso_LeanB2CAP.h"
#include "LeanB2CAP.h"

#undef LEAN_B2CAP_INSTANCE_COUNT
#define LEAN_B2CAP_INSTANCE_COUNT				UINT8_C(4)

#include "CRC.c"
/* include module under test */
#include "LeanB2CAP.c"
}

/* define a fake call-back function for the frame processor related tests */
void LeanB2CAPCallback_Ttest (LeanB2CAP_Event_T Event, void * Data)
{
    (void) Event;
    (void) Data;
}

static Retcode_T RcFile[100U];
static uint8_t RcIndex = 0U;
static uint32_t CallBackCallCountIndex = 0UL;
static uint32_t CallBackErrorCountIndex = 0UL;
static uint32_t CallBackPassCountIndex = 0UL;

void LeanB2CAPCallbackDummy(LeanB2CAP_Event_T Event, void * Info)
{
    CallBackCallCountIndex++;

    switch (Event)
    {
    case LEAN_B2CAP_EVENT_FRAME_RX_COMPLETE:
    	CallBackPassCountIndex++;
    	break;
    case LEAN_B2CAP_EVENT_FRAME_RX_ERROR:
    	CallBackErrorCountIndex++;
    	if(RcIndex >= 99U)
    	{
    		RcIndex = 0U;
    	}
    	RcFile[RcIndex++] = *(Retcode_T *)Info;
    	break;
    default:
    	break;
    }
}

/* create test fixture initializing all variables automatically */

class LeanB2CAP: public testing::Test
{
protected:

    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        CallBackCallCountIndex = 0UL;
        CallBackErrorCountIndex = 0UL;
        CallBackPassCountIndex = 0UL;
        RcIndex = 0U;
        FFF_RESET_HISTORY()
        ;

    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
    }

    void InitializeHandle()
    {

    }

    LeanB2CAP_HandlePtr_T m_Handle;   /**< Test LeanB2CAP handle */
};

/**
 *  Function to test Lean B2CAP initialization
 *
 */
TEST_F(LeanB2CAP, LeanB2CAPInitPassTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPInitPassTest: }
     *
     * API used to test the Lean B2CAP initialization with valid parameters
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);

    /* Validate implementation internal details*/
    EXPECT_EQ(&LeanB2CAPCallback_Ttest, LeanB2CAPHandle[0].Callback);
    EXPECT_EQ(true, LeanB2CAPHandle[0].HandleOccupance);
    EXPECT_EQ(RETCODE_OK, Return_status);

    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPInitFailTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPInitFailTest: }
     *
     * API used to test the Lean B2CAP initialization with assigning the all
     * LeanB2CAP handles with data.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo, NULL);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPInitToFailTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPInitFailTest: }
     *
     * API used to test the Lean B2CAP initialization with assigning the all
     * LeanB2CAP handles with data.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    Return_status = LeanB2CAP_Initialize(NULL, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

    Return_status = LeanB2CAP_Initialize(&m_Handle, NULL, &LeanB2CAPCallback_Ttest);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

    ValidOutputDataStorageBufferInfo.FrameDataPointer = NULL;
    Return_status = LeanB2CAP_Initialize(NULL, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPInitFail1Test)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPInitFail1Test: }
     *
     * API used to test the Lean B2CAP initialization with assigning the all
     * LeanB2CAP handles with data.
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    LeanB2CAP_HandlePtr_T Handle1;
    LeanB2CAP_HandlePtr_T Handle2;
    LeanB2CAP_HandlePtr_T Handle3;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);
    LeanB2CAP_Initialize(&Handle1, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);
    LeanB2CAP_Initialize(&Handle2, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);
    LeanB2CAP_Initialize(&Handle3, &ValidOutputDataStorageBufferInfo, &LeanB2CAPCallback_Ttest);
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallback_Ttest);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&Handle1);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&Handle2);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&Handle3);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPDeinitHandleNullTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPDeinitHandleNullTest: }
     *
     * API used to test the Lean B2CAP initialization with callback as NULL
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Return_status = LeanB2CAP_Deinitialize(NULL);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPDeinitHandleInvalidTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPDeinitHandleInvalidTest: }
     *
     * API used to test the Lean B2CAP initialization with callback as garbage address
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    LeanB2CAP_HandlePtr_T Handle = (LeanB2CAP_HandlePtr_T )0x12345678;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    Return_status = LeanB2CAP_Deinitialize(&Handle);

    /* Validate implementation internal details*/
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);
}


TEST_F(LeanB2CAP, LeanB2CAP_ResetRxDataProcessorPassTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_ResetRxDataProcessorPassTest: }
     *
     * API used to test the data received with LeanB2CAP_ResetRxDataProcessor()
     * with valid parameters and calling LeanB2CAP_Initialize() API
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* Validating implementation with API initialization */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallback_Ttest);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_ResetRxDataProcessor(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);

    EXPECT_EQ(0UL, LeanB2CAPHandle[0].CRC2OfRxFrameInProgress);
    EXPECT_NE((LeanB2CAP_HandlePtr_T *)NULL, &m_Handle);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_ResetRxDataProcHandleNullTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_ResetRxDataProcHandleNullTest: }
     *
     * API used to test the data received with LeanB2CAP_ResetRxDataProcessor()
     * with NULL as Handle parameter
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* Validating implementation with API initialization */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallback_Ttest);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_ResetRxDataProcessor(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_ResetRxDataProcGarbageHandleTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_ResetRxDataProcGarbageHandleTest: }
     *
     * API used to test the data received with LeanB2CAP_ResetRxDataProcessor()
     * with garbage value of Handle parameter
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    LeanB2CAP_HandlePtr_T DummyHandle = (LeanB2CAP_HandlePtr_T )0xFFFFFFFF;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* Validating implementation with API initialization */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallback_Ttest);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_ResetRxDataProcessor(&DummyHandle);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPBuilderPassTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPBuilderPassTest: }
     *
     * API used to test the data received with LeanB2CAP_Builder()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[400] = {0x01,0x02,0x03,0x04,0x05},TestFrameBuf[400];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_FrameData_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.FrameDataPointer = TestFrameBuf;
    TestLeanB2CAPBuildOuput.FrameDataSize = sizeof(TestFrameBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 5;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(0x55, TestLeanB2CAPBuildOuput.FrameDataPointer[0]);
    EXPECT_EQ(8, TestLeanB2CAPBuildOuput.FrameDataPointer[1]);
    EXPECT_EQ(0, TestLeanB2CAPBuildOuput.FrameDataPointer[2]);
    EXPECT_EQ(LEAN_B2CAP_CMD_WRITE, TestLeanB2CAPBuildOuput.FrameDataPointer[3]);
    EXPECT_EQ(0xB4, TestLeanB2CAPBuildOuput.FrameDataPointer[4]);
    EXPECT_EQ(0x01, TestLeanB2CAPBuildOuput.FrameDataPointer[5]);
    EXPECT_EQ(0x02, TestLeanB2CAPBuildOuput.FrameDataPointer[6]);
    EXPECT_EQ(0x03, TestLeanB2CAPBuildOuput.FrameDataPointer[7]);
    EXPECT_EQ(0x04, TestLeanB2CAPBuildOuput.FrameDataPointer[8]);
    EXPECT_EQ(0x05, TestLeanB2CAPBuildOuput.FrameDataPointer[9]);
    EXPECT_EQ(0x0B, TestLeanB2CAPBuildOuput.FrameDataSize);
    EXPECT_EQ(0x58, TestLeanB2CAPBuildOuput.FrameDataPointer[10]);
    EXPECT_EQ(RETCODE_OK, Return_status);

    TestRawDataInput.DataSize = 335;
    TestLeanB2CAPBuildOuput.FrameDataSize = sizeof(TestFrameBuf);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(0x55, TestLeanB2CAPBuildOuput.FrameDataPointer[0]);
    EXPECT_EQ(0x52, TestLeanB2CAPBuildOuput.FrameDataPointer[1]);
    EXPECT_EQ(0x01, TestLeanB2CAPBuildOuput.FrameDataPointer[2]);
    EXPECT_EQ(341, TestLeanB2CAPBuildOuput.FrameDataSize);
    EXPECT_EQ(RETCODE_OK, Return_status);

    TestLeanB2CAPBuildOuput.FrameDataSize = 4UL;
    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_OP_BUFFER_INSUFFICIENT), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPBuilderInputDataBuffNullTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPBuilderInputDataBuffNullTest: }
     *
     * API used to test the data received with LeanB2CAP_Builder()
     * with input rawDataBuffer as NULL
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    LeanB2CAP_FrameData_T TestLeanB2CAPBuildOuput;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

    TestLeanB2CAPBuildOuput.FrameDataPointer = NULL;
    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

}

TEST_F(LeanB2CAP, LeanB2CAPBuilderOutputDataNullTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPBuilderOutputDataNullTest: }
     *
     * API used to test the data received with LeanB2CAP_Builder()
     * with OutputDataBuffer as NULL
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_FrameData_T TestLeanB2CAPBuildOuput;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(NULL,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

    TestLeanB2CAPBuildOuput.FrameDataPointer = NULL;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAPBuilderFailTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAPBuilderFailTest: }
     *
     * API used to test the data received with LeanB2CAP_Builder()
     * with invalid command type and invalid datasize
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[400] = {0x01,0x02,0x03,0x04,0x05},TestFrameBuf[400];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_FrameData_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.FrameDataPointer = TestFrameBuf;
    TestLeanB2CAPBuildOuput.FrameDataSize = sizeof(TestFrameBuf);
    TestRawDataInput.CommandType = (LeanB2CAP_CommandType_T)0x00;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 5;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_CMD_TYPE_ERROR), Return_status);

    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataSize = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Builder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_LENGTH_ERROR), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_RxDataProcPassTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_RxDataProcPassTest: }
     *
     * API used to test the data received with LeanB2CAP_RxDataProcessor()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[100] = {0x55,8,0,LEAN_B2CAP_CMD_WRITE,0xB4,0x01,0x02,0x03,0x04,0x05,0x58};
    LeanB2CAP_FrameData_T PayLoad;
    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 11;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallback_Ttest);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_RxDataProcZeroLengthFail)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_RxDataProcZeroLengthFail: }
     *
     * API used to test the data validation ofLeanB2CAP_RxDataProcZeroLengthFail()
     * with zero as data length
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[100] = {0x55,8,0,LEAN_B2CAP_CMD_WRITE,0xB4,0x01,0x02,0x03,0x04,0x05,0x58};
    LeanB2CAP_FrameData_T PayLoad;
    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 0;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallback_Ttest);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_RxDataProcMultiFrameTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_RxDataProcMultiFrameTest: }
     *
     * API used to test the data received with LeanB2CAP_RxDataProcessor()
     * with 2 valid frames
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[100] = {0x55,8,0,LEAN_B2CAP_CMD_WRITE,0xB4,0x01,0x02,0x03,0x04,0x05,0xB0, \
            0x55,6,0,LEAN_B2CAP_CMD_WRITE,0xC4,0x01,0x02,0x03,0xAC};
    LeanB2CAP_FrameData_T PayLoad;
    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 20;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallbackDummy);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);
    EXPECT_EQ(2UL, CallBackCallCountIndex);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_RxDataProcPatchMultiFrameTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_RxDataProcPatchMultiFrameTest: }
     *
     * API used to test the data received with LeanB2CAP_RxDataProcessor()
     * with 2 valid frames send multiple times
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x55,8,0};
    LeanB2CAP_FrameData_T PayLoad;
    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 3;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallbackDummy);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    uint8_t TestBuff1[10] = {LEAN_B2CAP_CMD_WRITE,0xB4,0x01,0x02};
    PayLoad.FrameDataPointer = TestBuff1;
    PayLoad.FrameDataSize = 4;
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    uint8_t TestBuff2[10] = {0x03,0x04,0x05,0xAE,0x55,6,0};
    PayLoad.FrameDataPointer = TestBuff2;
    PayLoad.FrameDataSize = 7;
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);
    EXPECT_EQ(1UL, CallBackCallCountIndex);

    /* call relevant production code Interface and verify implementation */
    uint8_t TestBuff3[10] = {LEAN_B2CAP_CMD_WRITE,0xC4,0x01,0x02,0x03,0xAC};
    PayLoad.FrameDataPointer = TestBuff3;
    PayLoad.FrameDataSize = 6;
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);
    EXPECT_EQ(2UL, CallBackCallCountIndex);

    /* call relevant production code Interface and verify implementation */
    uint8_t TestBuff4[10] = {0x55,4,0,LEAN_B2CAP_CMD_WRITE,0xD4,0x01,0x68};
    PayLoad.FrameDataPointer = TestBuff4;
    PayLoad.FrameDataSize = 6;
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_RxDataProcContSendTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_RxDataProcContSendTest: }
     *
     * API used to test the data received by LeanB2CAP_RxDataProcessor()
     * with 2 valid frames for 1000 times and check for callback function count
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[100] = {0x55,8,0,LEAN_B2CAP_CMD_WRITE,0xB4,0x01,0x02,0x03,0x04,0x05,0xC1, \
            0x55,6,0,LEAN_B2CAP_CMD_WRITE,0xC4,0x01,0x02,0x03,0x5e};
    LeanB2CAP_FrameData_T PayLoad;
    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 20;
    uint16_t SendCount;
    uint8_t OutputDataBuffer[2000];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 2000;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallbackDummy);
    EXPECT_EQ(RETCODE_OK, Return_status);

    for (SendCount = 0UL;SendCount < 10000UL;SendCount++)
    {
        /* call relevant production code Interface and verify implementation */
        Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
        EXPECT_EQ(RETCODE_OK, Return_status);
    }

    EXPECT_EQ(20000UL, CallBackCallCountIndex);
    EXPECT_EQ(10000UL, CallBackErrorCountIndex);
    EXPECT_EQ(10000UL, CallBackPassCountIndex);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_RxDataProcFailTest)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_RxDataProcContSendTest: }
     *
     * API used to test the data received by LeanB2CAP_RxDataProcFailTest()
     * with different invalid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x55,0x01,0x00,0x00,0x05,0x03,0x05};
    uint8_t TestBuff1[10] = {0x22,0x05,0x00,0x57,0x05,0x03,0x05};
    LeanB2CAP_FrameData_T PayLoad;
    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 8;
    LeanB2CAP_HandlePtr_T Handle1 = (LeanB2CAP_HandlePtr_T)0x12345678;
    uint8_t OutputDataBuffer[200];
    LeanB2CAP_FrameData_T ValidOutputDataStorageBufferInfo;

    ValidOutputDataStorageBufferInfo.FrameDataPointer = OutputDataBuffer;
    ValidOutputDataStorageBufferInfo.FrameDataSize = 200;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallbackDummy);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(NULL, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Deinitialize(&m_Handle);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_Initialize(&m_Handle, &ValidOutputDataStorageBufferInfo,&LeanB2CAPCallbackDummy);
    EXPECT_EQ(RETCODE_OK, Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&Handle1, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);

    PayLoad.FrameDataPointer = TestBuff1;
    PayLoad.FrameDataSize = 1;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_LEANB2CAP_SD_ERROR, Retcode_GetCode(RcFile[0]));
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(RcFile[0]));

    PayLoad.FrameDataPointer = TestBuff;
    PayLoad.FrameDataSize = 7;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_LEANB2CAP_LENGTH_ERROR, Retcode_GetCode(RcFile[0]));
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(RcFile[0]));

    uint8_t TestBuff2[10] = {0x55,0x04,0x00,0x00,0x05,0x03,0x05};
    PayLoad.FrameDataPointer = TestBuff2;
    PayLoad.FrameDataSize = 7;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_CMD_TYPE_ERROR), RcFile[0]);

    uint8_t TestBuff6[10] = {0x55,0x04,0x00,0xFF,0x05,0x03,0x05};
    PayLoad.FrameDataPointer = TestBuff6;
    PayLoad.FrameDataSize = 7;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_CMD_TYPE_ERROR), RcFile[0]);

    PayLoad.FrameDataPointer = TestBuff2;
    PayLoad.FrameDataSize = 7;
    m_Handle->FrameProcState = LEAN_B2CAP_FRMPRC_ERROR;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_CMD_TYPE_ERROR), RcFile[0]);

    PayLoad.FrameDataPointer = TestBuff2;
    PayLoad.FrameDataSize = 7;
    m_Handle->FrameProcState = (LeanB2CAP_FrameProcStates_T )UINT8_C(13);
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_FRAME_ERROR), RcFile[0]);

    uint8_t TestBuff3[10] = {0x55,6,0,LEAN_B2CAP_CMD_WRITE,0xC0,0x01,0x02,0x03,0xAC};
    PayLoad.FrameDataPointer = TestBuff3;
    PayLoad.FrameDataSize = 9;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_LEANB2CAP_CRC_ERROR, Retcode_GetCode(RcFile[0]));
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(RcFile[0]));

    uint8_t TestBuff4[10] = {0x55,6,0,LEAN_B2CAP_CMD_WRITE,0xC4,0x01,0x02,0x03,0xA1};
    PayLoad.FrameDataPointer = TestBuff4;
    PayLoad.FrameDataSize = 9;
    RcIndex = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE_LEANB2CAP_CRC_ERROR, Retcode_GetCode(RcFile[0]));
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(RcFile[0]));

    m_Handle->Callback = NULL;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_LEANB2CAP_INVALID_CALLBACK), Return_status);

    uint8_t TestBuff5[10] = {0x55,6,0,LEAN_B2CAP_CMD_WRITE,0xC4,0x01,0x02,0x03,0xAC};
    PayLoad.FrameDataPointer = TestBuff5;
    PayLoad.FrameDataSize = 9;
    LeanB2CAPHandle[1].Callback = NULL;
    m_Handle->FrameProcState = LEAN_B2CAP_FRMPRC_SD;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, &PayLoad);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_LEANB2CAP_INVALID_CALLBACK), Return_status);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_RxDataProcessor(&m_Handle, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilder)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilder: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilder()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(0x55, TestLeanB2CAPBuildOuput.HeaderPointer[0]);
    EXPECT_EQ(0x05, TestLeanB2CAPBuildOuput.HeaderPointer[1]);
    EXPECT_EQ(0x00, TestLeanB2CAPBuildOuput.HeaderPointer[2]);
    EXPECT_EQ(LEAN_B2CAP_CMD_WRITE, TestLeanB2CAPBuildOuput.HeaderPointer[3]);
    EXPECT_EQ(0xDC, TestLeanB2CAPBuildOuput.HeaderPointer[4]);
    EXPECT_EQ(0x05, TestLeanB2CAPBuildOuput.HeaderLength);
    EXPECT_EQ(0xFA, TestLeanB2CAPBuildOuput.FooterPointer[0]);
    EXPECT_EQ(0x01, TestLeanB2CAPBuildOuput.FooterLength);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderCrcFailure)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderCrcFailure: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderCrcFailure()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x02},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(0x55, TestLeanB2CAPBuildOuput.HeaderPointer[0]);
    EXPECT_EQ(0x05, TestLeanB2CAPBuildOuput.HeaderPointer[1]);
    EXPECT_EQ(0x00, TestLeanB2CAPBuildOuput.HeaderPointer[2]);
    EXPECT_EQ(LEAN_B2CAP_CMD_WRITE, TestLeanB2CAPBuildOuput.HeaderPointer[3]);
    EXPECT_EQ(0xDC, TestLeanB2CAPBuildOuput.HeaderPointer[4]);
    EXPECT_EQ(0x05, TestLeanB2CAPBuildOuput.HeaderLength);
    EXPECT_EQ(0xFC, TestLeanB2CAPBuildOuput.FooterPointer[0]);
    EXPECT_EQ(0x01, TestLeanB2CAPBuildOuput.FooterLength);
    EXPECT_EQ(RETCODE_OK, Return_status);
}


TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderPasingUdpData)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderPasingUdpData: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderPasingUdpData()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t
    TestBuff[20] ={ 0x02,0x03,0x00,0x32,0x2B,0xA8,0xC0,0xFB,0xBD,0x04,0x00,0x43,0x4F,0x53,0x50},TestHeaderBuf[50],TestFooterBuf[50];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 15;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(0x55, TestLeanB2CAPBuildOuput.HeaderPointer[0]);
    EXPECT_EQ(0x12, TestLeanB2CAPBuildOuput.HeaderPointer[1]);
    EXPECT_EQ(0x00, TestLeanB2CAPBuildOuput.HeaderPointer[2]);
    EXPECT_EQ(LEAN_B2CAP_CMD_WRITE, TestLeanB2CAPBuildOuput.HeaderPointer[3]);
    EXPECT_EQ(0x64, TestLeanB2CAPBuildOuput.HeaderPointer[4]);
    EXPECT_EQ(0x05, TestLeanB2CAPBuildOuput.HeaderLength);
    EXPECT_EQ(0xC0, TestLeanB2CAPBuildOuput.FooterPointer[0]);
    EXPECT_EQ(0x01, TestLeanB2CAPBuildOuput.FooterLength);
    EXPECT_EQ(RETCODE_OK, Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderHeaderNull)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderHeaderNull: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderHeaderNull()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = NULL;
    TestLeanB2CAPBuildOuput.FooterLength = UINT16_C(0);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderFooterNull)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderFooterNull: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderFooterNull()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = NULL;
    TestLeanB2CAPBuildOuput.HeaderLength =UINT16_C(0);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderRawdataNull)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderRawdataNull: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderRawdataNull()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = NULL;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderNull)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderNull: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderNull()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01,0x06};
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = NULL;
    TestLeanB2CAPBuildOuput.HeaderLength =UINT16_C(0);
    TestLeanB2CAPBuildOuput.FooterPointer = NULL;
    TestLeanB2CAPBuildOuput.FooterLength = UINT16_C(0);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 3;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderInputpointerNull)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderInputpointerNull: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderInputpointerNull()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = NULL;
    TestLeanB2CAPBuildOuput.HeaderLength =UINT16_C(0);
    TestLeanB2CAPBuildOuput.FooterPointer = NULL;
    TestLeanB2CAPBuildOuput.FooterLength = UINT16_C(0);

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderOutputpointerNull)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderOutputpointerNull: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderOutputpointerNull()
     * with valid parameters
     *
     */
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilder: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilder()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01};
    LeanB2CAP_RawData_T TestRawDataInput;

    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(NULL,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Return_status);
}


TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderInvalidHeaderLength)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderInvalidHeaderLength: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderInvalidHeaderLength()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =UINT16_C(2);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderInvalidFooterLength)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderInvalidFooterLength: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderInvalidFooterLength()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength =UINT16_C(0);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderInvalidRawdataLength)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderInvalidRawdataLength: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderInvalidRawdataLength()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength =sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderCommandTypeWrong)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderCommandTypeWrong: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderCommandTypeWrong()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =sizeof(TestHeaderBuf);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = sizeof(TestFooterBuf);
    TestRawDataInput.CommandType = (LeanB2CAP_CommandType_T)125;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 2;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_LEANB2CAP_CMD_TYPE_ERROR), Return_status);
}

TEST_F(LeanB2CAP, LeanB2CAP_OverheadBuilderInvalidParam)
{
    /** @testcase{ LeanB2CAP::LeanB2CAP_OverheadBuilderInvalidParam: }
     *
     * API used to test the data received with LeanB2CAP_OverheadBuilderInvalidParam()
     * with valid parameters
     *
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T Return_status;
    uint8_t TestBuff[10] = {0x05,0x01},TestHeaderBuf[10],TestFooterBuf[10];
    LeanB2CAP_RawData_T TestRawDataInput;
    LeanB2CAP_Overhead_T TestLeanB2CAPBuildOuput;
    TestLeanB2CAPBuildOuput.HeaderPointer = TestHeaderBuf;
    TestLeanB2CAPBuildOuput.HeaderLength =UINT16_C(2);
    TestLeanB2CAPBuildOuput.FooterPointer = TestFooterBuf;
    TestLeanB2CAPBuildOuput.FooterLength = UINT16_C(0);
    TestRawDataInput.CommandType = LEAN_B2CAP_CMD_WRITE;
    TestRawDataInput.DataPointer = TestBuff;
    TestRawDataInput.DataSize = 0;

    /* call relevant production code Interface and verify implementation */
    Return_status = LeanB2CAP_OverheadBuilder(&TestLeanB2CAPBuildOuput,&TestRawDataInput);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), Return_status);
}
#else
}
#endif /* #if KISO_FEATURE_LEANB2CAP */
