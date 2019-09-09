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
 *  @file
 *
 *  Module test specification for the KisoCRC_unittest module.
 *
 * The unit test file template follows the Four-Phase test pattern. 
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
extern "C"
{
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_CRC

#if KISO_FEATURE_CRC

/* include faked interfaces */
#include "Kiso_Retcode_th.hh"
/* include module under test */
#include "CRC.c"
/* end of global scope symbol and fake definitions section */
}

/* create test fixture initializing all variables automatically */
class CRCRoutines: public testing::Test
{
protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
       ;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do */
    }
};

/**
 * Module test cases to test  CRC calculation
 */

/* specify test cases ******************************************************* */
TEST_F(CRCRoutines,testNullpointerAsMsg)
{
    /** @testcase{CRCRoutines::NullpointerAsMsg: }
     * CRC_8 API is called to check the Null condition
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t * msg = NULL;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated, msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retVal);
}

TEST_F(CRCRoutines, testNullpointerInitValue)
{
   /** @testcase{CRCRoutines::NullpointerInitValue: }
      * CRC_8 API is called to check the Null condition
      */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xFF};
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), NULL, msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retVal);
}

TEST_F(CRCRoutines, testLengthAsZero)
{
   /** @testcase{CRCRoutines::testLengthAsZero }
      * CRC_8 API is called to check the Null condition for buffer length
      */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00};
    uint8_t crc8_calculated = UINT8_C(0X0) ;
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated, msg,UINT16_C(0));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0X0), crc8_calculated);
}

TEST_F(CRCRoutines, testMsgLen1)
{
    /** @testcase{CRCRoutines::MsgLen_1 }
       * CRC_8 API is called to check the crc When one byte is passed to data buffer
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated, msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0xee), crc8_calculated);
}

TEST_F(CRCRoutines, testMsgLen2)
{
    /** @testcase{CRCRoutines::MsgLen_2 }
       *CRC_8 API is called to check the crc When two bytes are passed to data buffer
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55, 0xaa};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated, msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0X66),crc8_calculated);
}

TEST_F(CRCRoutines, testMsgLen5)
{
    /** @testcase{CRCRoutines::MsgLen_5: }
       * CRC_8 API is called to check the crc When 5 bytes are passed to data buffer
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xff, 0xb4, 0x1d, 0x55, 0xaa};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0x56), crc8_calculated);
}

TEST_F(CRCRoutines,testChangedPoly)
{
    /** @testcase{CRCRoutines::ChangedPoly: }
       * CRC_8 API is called to check the crc When 5 bytes are passed to data buffer with a different poly value
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xff, 0xb4, 0x1d, 0x55, 0xaa};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xff), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0x5A), crc8_calculated);
}

TEST_F(CRCRoutines,testChangedInitValue)
{
    /** @testcase{CRCRoutines::testChangedInitValue: }
     * CRC_8 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x1,0x2};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0x33);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0x74), crc8_calculated);
}

TEST_F(CRCRoutines,testSpecialCharacter)
{
    /** @testcase{CRCRoutines::testSpecialCharacter: }
     * CRC_8 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
     uint8_t msg[] = {'*','&','!','~','#'};
     Retcode_T retVal;
     uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
      retVal = CRC_8(UINT8_C(0xba), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
      EXPECT_EQ(RETCODE_OK, retVal);
      EXPECT_EQ(UINT8_C(0x16), crc8_calculated);
}

TEST_F(CRCRoutines,testAlphabets)
{
    /** @testcase{CRCRoutines::testAlphabets: }
     * CRC_8 API is called to check the crc of the bytes present in a buffer according to length
     */
    uint8_t msg[] = {'A','B','C'};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0x62), crc8_calculated);
}

TEST_F(CRCRoutines,testSmallAlphabets)
{
    /** @testcase{CRCRoutines::testSmallAlphabets: }
     * CRC_8 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {'a','b','c'};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT8_C(0x8E), crc8_calculated);
}

TEST_F(CRCRoutines,testWrongBytes)
{
    /** @testcase{CRCRoutines::testWrongBytes: }
     * CRC_8 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {8,9,5,1,6};
    Retcode_T retVal;
    uint8_t crc8_calculated = UINT8_C(0xaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_8(UINT8_C(0xba), &crc8_calculated , msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_NE(UINT8_C(0xFF), crc8_calculated);
}

TEST_F(CRCRoutines, testCRC16NullAsInitvalue)
{
    /** @testcase{CRCRoutines::testCRC16NullAsInitvalue: }
       * CRC_16 API is called to check the Null condition for the init value
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x08};
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), NULL, msg,(uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retVal);
}

TEST_F(CRCRoutines, testCRC16NullAsMsg)
{
    /** @testcase{CRCRoutines::testCRC16NullAsMsg: }
       * CRC_16 API is called to check the Null condition for the message buffer
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t *msg = NULL;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retVal);
}

TEST_F(CRCRoutines, testCRC16NullAsLength)
{
    /** @testcase{CRCRoutines::testCRC16NullAsLength: }
       * CRC_16 API is called to check the Null condition for the buffer length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55};
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,UINT16_C(0));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xaaaa), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16MsgLen_1)
{
    /** @testcase{CRCRoutines::testCRC16MsgLen_1: }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x32e7), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16MsgLenAs1)
{
    /** @testcase{CRCRoutines::testCRC16MsgLenAs1: }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x01};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x884a), crc16_calculated);
}

TEST_F(CRCRoutines, tesCRC16tMsgLen1)
{
    /** @testcase{CRCRoutines::tesCRC16tMsgLen1: }
       *  CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x08};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t)sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x7861), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLength_1)
{
    /** @testcase{CRCRoutines::testCRC16MsgLength_1 }
       *CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x1d46), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLengthAs1)
{
    /** @testcase{CRCRoutines::testCRC16MsgLengthAs1: }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    /* test bit set in high nibble, too */
    uint8_t msg[] = {0xaa};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xaa00), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLen_2)
{
    /** @testcase{CRCRoutines::testCRC16MsgLen_2: }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11, 0x88};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg, (uint16_t)sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xc1c1), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLen2)
{
    /** @testcase{CRCRoutines::testCRC16MsgLen2 }
       *CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00, 0x00};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg, (uint16_t)sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x2d4e), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLength_2)
{
    /** @testcase{CRCRoutines::testCRC16MsgLength_2: }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xff, 0xff};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x16a7), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLengthAs2)
{
    /** @testcase{CRCRoutines::testCRC16MsgLengthAs2: }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xaa, 0x55};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x09c2), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC16MsgLengthAs_2)
{
    /** @testcase{CRCRoutines::testCRC16MsgLengthAs_2 }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55, 0xaa};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t) sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x322b), crc16_calculated);
}

TEST_F(CRCRoutines, tesCRC16tMsgLen_20)
{
    /** @testcase{CRCRoutines::tesCRC16tMsgLen_20 }
       * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11, 0x88, 0x01, 0xf4,
                     0x3f, 0x00, 0x00, 0x55,
                     0xcc, 0xde, 0x22, 0xee,
                     0x1f, 0xd2, 0xcc, 0xd0,
                     0x77, 0x66, 0x55, 0xbb};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, msg,(uint16_t)sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x84fd), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16CharValue)
{
    /** @testcase{CRCRoutines::testCRC16CharValue: }
     *CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'h','e','l','l','o','c','o','m','o'};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xDDEA), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16LengthGTFifty)
{
    /** @testcase{CRCRoutines::testCRC16LengthGTFifty: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0X1,0X2,0X3,0X4,0X5,0X6,0X7,0X8,0X9,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30,0X31,0X32,0X33,
            0X34,0X35,0X36,0X37,0X38,0X39,0X40,0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,0X50,0X51,0X52,0X53,0X54,0X55};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x22BF), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16LT100)
{
    /** @testcase{CRCRoutines::testCRC16LT100: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0X1,0X2,0X3,0X4,0X5,0X6,0X7,0X8,0X9,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30,0X31,0X32,0X33,
            0X34,0X35,0X36,0X37,0X38,0X39,0X40,0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,0X50,0X51,0X52,0X53,0X54,0X55,0X1,0X2,0X3,0X4,0X5,0X6,0X7,0X8,0X9,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x84B8), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16Length200)
{
    /** @testcase{CRCRoutines::testCRC16Length200: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    uint8_t dataBuffer[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
            34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,
            70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
            34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,
            70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x69CD), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16ChangePoly)
{
    /** @testcase{CRCRoutines::testCRC16ChangePoly: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0x55};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xaaaa), &crc16_calculated, dataBuffer, (uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xFF1C), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16ChangeInitVal)
{
    /** @testcase{CRCRoutines::testCRC16ChangeInitVal: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0x55};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaa55);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x5CC2), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16ChangeInitValPoly)
{
    /** @testcase{CRCRoutines::testCRC16ChangeInitValPoly: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0xaa};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0x5555);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0x5a5a), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xDDB2), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16SpecialCharacter)
{
    /** @testcase{CRCRoutines::testCRC16SpecialCharacter: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'@','$','^'};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x3101), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16Alphabets)
{
    /** @testcase{CRCRoutines::testCRC16Alphabets: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'A','B','C'};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0x7D0F), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16SmallAlphabets)
{
    /** @testcase{CRCRoutines::testCRC16SmallAlphabets: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'a','b','c'};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));;
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT16_C(0xA76B), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC16WrongBytes)
{
    /** @testcase{CRCRoutines::testCRC16WrongBytes: }
     * CRC_16 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {8,9,5,1,6};
    Retcode_T retVal;
    uint16_t crc16_calculated = UINT16_C(0xaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_16(UINT16_C(0xbaad), &crc16_calculated, dataBuffer,(uint16_t)sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_NE(UINT16_C(0x84fd), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32NullpointerInitvalue)
{
    /** @testcase{CRCRoutines::testCRC32NullpointerInitvalue: }
       * CRC_32 API is called to check the Null condition
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55};
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), NULL, msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retVal);
}

TEST_F(CRCRoutines,testCRC32NullpointerMsg)
{
    /** @testcase{CRCRoutines::testCRC32NullpointerMsg: }
       * CRC_32 API is called to check the Null condition
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55};
    Retcode_T retVal;
    uint32_t crc32_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc32_calculated, 0, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), retVal);
}

TEST_F(CRCRoutines, testCRC32NullAsLength)
{
    /** @testcase{CRCRoutines::testCRC32NullAsLength: }
       * CRC_32 API is called to check the Null condition for the buffer length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55};
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg,UINT16_C(0));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xaaaaaaaa), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLength1)
{
    /** @testcase{CRCRoutines::testCRC32MsgLength1: }
       *CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55};
    Retcode_T retVal;
    uint32_t crc32_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc32_calculated, msg, (uint16_t)(sizeof(msg)/sizeof(msg[0])));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xA32DA3C2), crc32_calculated);
}

TEST_F(CRCRoutines,testCRC32MsgLen_1)
{
    /** @testcase{CRCRoutines::testCRC32MsgLen_1: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x323432e7), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32MsgLenAs1)
{
    /** @testcase{CRCRoutines::testCRC32MsgLenAs1: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x01};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x8899884a), crc16_calculated);
}

TEST_F(CRCRoutines, tesCRC32tMsgLen1)
{
    /** @testcase{CRCRoutines::tesCRC32tMsgLen1: }
       *  CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x08};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x78b47861), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLength_1)
{
    /** @testcase{CRCRoutines::testCRC32MsgLength_1 }
       *CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x1d991d46), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLengthAs1)
{
    /** @testcase{CRCRoutines::testCRC32MsgLengthAs1: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    /* test bit set in high nibble, too */
    uint8_t msg[] = {0xaa};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xaaaaaa00), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLen_2)
{
    /** @testcase{CRCRoutines::testCRC32MsgLen_2: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11, 0x88};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x1EB4C1C1), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLen2)
{
    /** @testcase{CRCRoutines::testCRC32MsgLen2 }
       *CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00, 0x00};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xFE682D4E), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLength_2)
{
    /** @testcase{CRCRoutines::testCRC32MsgLength_2 }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xff, 0xff};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x80CB16A7), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLengthAs2)
{
    /** @testcase{CRCRoutines::testCRC32MsgLengthAs2: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xaa, 0x55};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xA32D09C2), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLengthAs_2)
{
    /** @testcase{CRCRoutines::testCRC32MsgLengthAs_2 }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55, 0xaa};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xDD8E322B), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLen_4)
{
    /** @testcase{CRCRoutines::testCRC32MsgLen_4: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11, 0x88,0x11,0x88};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x20C1E501), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLen4)
{
    /** @testcase{CRCRoutines::testCRC32MsgLen4 }
       *CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x00, 0x00,0x00,0x00};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xBB96D334), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLength_4)
{
    /** @testcase{CRCRoutines::testCRC32MsgLength_4: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xff, 0xff,0xff,0xff};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x5DCB699A), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLengthAs4)
{
    /** @testcase{CRCRoutines::testCRC32MsgLengthAs4: }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0xaa, 0x55,0xaa,0x55};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xE3F6ECA5), crc16_calculated);
}

TEST_F(CRCRoutines, testCRC32MsgLengthAs_4)
{
    /** @testcase{CRCRoutines::testCRC32MsgLengthAs_4 }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x55, 0xaa,0x55,0xaa};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x05AB560B), crc16_calculated);
}

TEST_F(CRCRoutines, tesCRC32tMsgLen_20)
{
    /** @testcase{CRCRoutines::tesCRC32tMsgLen_20 }
       * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
       */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t msg[] = {0x11, 0x88, 0x01, 0xf4,
                     0x3f, 0x00, 0x00, 0x55,
                     0xcc, 0xde, 0x22, 0xee,
                     0x1f, 0xd2, 0xcc, 0xd0,
                     0x77, 0x66, 0x55, 0xbb};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, msg, sizeof(msg)/sizeof(msg[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xf8749e30), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32CharValue)
{
    /** @testcase{CRCRoutines::testCRC32CharValue: }
     *CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'h','e','l','l','o','c','o','m','o'};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x07CBC613), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32LengthGTFifty)
{
    /** @testcase{CRCRoutines::testCRC32LengthGTFifty: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0X1,0X2,0X3,0X4,0X5,0X6,0X7,0X8,0X9,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30,0X31,0X32,0X33,
            0X34,0X35,0X36,0X37,0X38,0X39,0X40,0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,0X50,0X51,0X52,0X53,0X54,0X55};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xE643B297), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32LT100)
{
    /** @testcase{CRCRoutines::testCRC32LT100: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0X1,0X2,0X3,0X4,0X5,0X6,0X7,0X8,0X9,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30,0X31,0X32,0X33,
            0X34,0X35,0X36,0X37,0X38,0X39,0X40,0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,0X50,0X51,0X52,0X53,0X54,0X55,0X1,0X2,0X3,0X4,0X5,0X6,0X7,0X8,0X9,0X10,0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xF628BA09), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32Length200)
{
    /** @testcase{CRCRoutines::testCRC32Length200: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */

    uint8_t dataBuffer[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
            34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,
            70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
            34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,
            70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xF039B77E), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32ChangePoly)
{
    /** @testcase{CRCRoutines::testCRC32ChangePoly: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0x55};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xaaaaaaaa), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xFFFFFF1C), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32ChangeInitVal)
{
    /** @testcase{CRCRoutines::testCRC32ChangeInitVal: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0x55};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaa55aa55);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x5C2D5CC2), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32ChangeInitValPoly)
{
    /** @testcase{CRCRoutines::testCRC32ChangeInitValPoly: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {0xaa};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0x55555555);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0x5a5a5a5a), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xDDDDDDB2), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32SpecialCharacter)
{
    /** @testcase{CRCRoutines::testCRC32SpecialCharacter: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'@','$','^'};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x7C7EB750), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32Alphabets)
{
    /** @testcase{CRCRoutines::testCRC32Alphabets: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'A','B','C'};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0xD70BFB5E),crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32SmallAlphabets)
{
    /** @testcase{CRCRoutines::testCRC32SmallAlphabets: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {'a','b','c'};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));;
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0x8A76441D), crc16_calculated);
}

TEST_F(CRCRoutines,testCRC32WrongBytes)
{
    /** @testcase{CRCRoutines::testCRC32WrongBytes: }
     * CRC_32 API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {8,9,5,1,6};
    Retcode_T retVal;
    uint32_t crc16_calculated = UINT32_C(0xaaaaaaaa);
    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32(UINT32_C(0xbaadbaad), &crc16_calculated, dataBuffer, sizeof(dataBuffer)/sizeof(dataBuffer[0]));
    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_NE(UINT32_C(0x12345678), crc16_calculated);
}

TEST_F(CRCRoutines,CRC32EathStandardInvalidTest)
{
    /** @testcase{CRCRoutines::TEST CRC32EathStandardInvalidTest: }
     * CRC_32BitEthernetStandard API is called to check invalid conditions like NULL parameters
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {8,119,20,25,85,51,125,220,52,69,97,200,166,188,98,20,32,16,199};
    Retcode_T retVal;
    uint32_t crc32_Utils;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32_Reverse(0xEDB88320, NULL, dataBuffer, sizeof(dataBuffer) / sizeof(dataBuffer[0]));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32_Reverse(0xEDB88320, &crc32_Utils, NULL, sizeof(dataBuffer) / sizeof(dataBuffer[0]));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    retVal = CRC_32_Reverse(0xEDB88320, NULL, NULL, sizeof(dataBuffer) / sizeof(dataBuffer[0]));

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
}

TEST_F(CRCRoutines,TestCRC32EathStandard)
{
    /** @testcase{CRCRoutines::TEST TestCRC32EathStandard: }
     * CRC_32BitEthernetStandard API is called to check the crc of the bytes present in a buffer according to length
     */
    /* SETUP: Declare and initialize local variables required only by this test case */
    uint8_t dataBuffer[] = {8,119,20,25,85,51,125,220,52,69,97,200,166,188,98,20,32,16,199};
    Retcode_T retVal;
    uint32_t crc32_Utils;

    /* EXECISE: call relevant production code Interface with appropriate test inputs  */
    CRC32_EATH_STD_INIT(crc32_Utils);
    retVal = CRC_32_Reverse(0xEDB88320, &crc32_Utils, dataBuffer, sizeof(dataBuffer) / sizeof(dataBuffer[0]));
    CRC32_INVERSE(crc32_Utils);

    /* VERIFY : Compare the expected with actual */
    EXPECT_EQ(0x6A237477u, crc32_Utils);
    EXPECT_EQ(RETCODE_OK, retVal);
}

/*****************************************************************************************/
#else
}
#endif /* if KISO_FEATURE_CRC */
