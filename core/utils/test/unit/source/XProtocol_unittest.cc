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
 * Module test specification for the X-Protocol module.
 */

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
extern "C"
{
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_XPROTOCOL

#if KISO_FEATURE_XPROTOCOL

/* include faked interfaces */
#include "Kiso_CRC_th.hh"
/* include module under test */
#include "XProtocol.c"
/* end of global scope symbol and fake definitions section */
}

#define TEST_DATA_SIZE            6U    /**< size of buffer */
#define MAX_FRAME_SIZE            1000U /**< MAX size of buffer */
#define MIN_FRAME_SIZE            1U    /**< MIN size of buffer */
#define MAX_DATA_LENGTH           1000U /**< MIN size of buffer */

static uint16_t CrcChecksum;

/* CRC_16_custom_fake function locally defined */
static Retcode_T CRC_16_custom_fake(uint16_t poly, uint16_t *shifter,
        uint8_t *data_p, uint16_t len)
{
    KISO_UNUSED(poly);
    KISO_UNUSED(data_p);
    KISO_UNUSED(len);
    *shifter = CrcChecksum;

    return RETCODE_OK;
}

/* Retcode function locally defined */
Retcode_T Retcode_compose(uint32_t package,
        Retcode_Severity_T severity, uint32_t code)
{
    uint32_t p = (package & 0x000000FF) << 24;
    uint32_t s = ((uint32_t) severity & 0x000000FF) << 16;
    uint32_t c = (code & 0x0000FFFF);
    Retcode_T retcode = (code == UINT32_C(0)) ?
            (Retcode_T) UINT32_C(0) : (Retcode_T) (p | s | c);

    return (retcode);
}

/* create test fixture initializing all variables automatically */
class XProtocolRoutines: public testing::Test
{
protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        CRC_16_fake.custom_fake = CRC_16_custom_fake;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do */
    }
};

/**
 * Module test cases to test X-Protocol
 */

/* specify test cases ******************************************************* */

TEST_F(XProtocolRoutines, testXprotocolInit)
{
    /** @testcase{XProtocolRoutines::testXprotocolInit: }
     * XProtocol_Init API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    Retcode_T retVal = XProtocol_Init();

    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
}

TEST_F(XProtocolRoutines, testEncodeFrameFail)
{
    /** @testcase{XProtocolRoutines::testEncodeFrameFail: }
     * XProtocol_EncodeFrame API is called to check the Null condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t nodata[3] = { 1 };
    uint8_t frame[13] = { 1 };
    uint32_t framelength;
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(NULL, sizeof(nodata), MAX_FRAME_SIZE, frame,
            &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(nodata, sizeof(nodata), MAX_FRAME_SIZE, NULL, NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(nodata, sizeof(nodata), MAX_FRAME_SIZE, NULL,
            &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(nodata, sizeof(nodata), MAX_FRAME_SIZE, frame, NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(nodata, sizeof(nodata), MIN_FRAME_SIZE, frame,
            &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t data[TEST_DATA_SIZE] = { 0xC0, 0xC1, 0xC2, 0xC9, 0xDB, 0xDB };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 0, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 1, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 2, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 4, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 13, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* Random custom crc checksum just to test what happens when maxframelength is too
     * small for encoded frame */
    CrcChecksum = 0xC0B8;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 2, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* Random custom crc checksum just to test what happens when maxframelength is too
     * small for encoded frame */
    CrcChecksum = 0xC1DB;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 3, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* Random custom crc checksum just to test what happens when maxframelength is too
     * small for encoded frame */
    CrcChecksum = 0xC1C9;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 3, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* Random custom crc checksum just to test what happens when maxframelength is too
     * small for encoded frame */
    CrcChecksum = 0xA1A2;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 5, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* Random custom crc checksum just to test what happens when maxframelength is too
     * small for encoded frame */
    CrcChecksum = 0xA1A2;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 13, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* Random custom crc checksum just to test what happens when maxframelength is too
     * small for encoded frame */
    CrcChecksum = 0xC1DB;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), 2, frame, &framelength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);
}

TEST_F(XProtocolRoutines, testEncodeFrame)
{
    /** @testcase{XProtocolRoutines::testEncodeFrame: }
     * XProtocol_EncodeFrame API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: the custom-valid checksum we provide(fake CRC_16) and comes from the
     * data[TEST_DATA_SIZE] */
    CrcChecksum = 0x72B8;
    Retcode_T retVal = RETCODE_OK;
    const uint8_t data[TEST_DATA_SIZE] = { 0xC0, 0xC1, 0xC2, 0xC9, 0xDB, 0xDB };
    uint8_t frame[MAX_FRAME_SIZE] = { 1 };
    uint8_t testframe[] = { 0xC0, 0x72, 0xB8, 0xDB, 0xDC, 0xC1, 0xC2, 0xDB, 0xDE, 0xDB,
            0xDD, 0xDB, 0xDD, 0xC9 };
    uint32_t framelen;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data, sizeof(data), sizeof(frame), frame, &framelen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(14), framelen);
    for (uint32_t x = UINT32_C(0); x < framelen; x++)
    {
        EXPECT_EQ(testframe[x], frame[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: the custom-valid checksum we provide(fake CRC_16) and comes from
     * data1[TEST_DATA_SIZE] */
    CrcChecksum = 0x61F0;
    const uint8_t data1[TEST_DATA_SIZE] = { 0xC1, 0xC1, 0xC2, 0xC3, 0xDB, 0xD2 };
    uint8_t testframe1[] = { 0xC0, 0x61, 0xF0, 0xC1, 0xC1, 0xC2, 0xC3, 0xDB, 0xDD, 0xD2,
            0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data1, sizeof(data1), sizeof(frame), frame,
            &framelen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(11), framelen);
    for (uint32_t x = UINT32_C(0); x < framelen; x++)
    {
        EXPECT_EQ(testframe1[x], frame[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: the custom-valid checksum we provide(fake CRC_16) and comes from
     * data2[2] */
    CrcChecksum = 0x8844;
    const uint8_t data2[2] = { 0xDD, 0xD2 };
    uint8_t testframe2[] = { 0xC0, 0x88, 0x44, 0xDD, 0xD2, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data2, sizeof(data2), sizeof(frame), frame,
            &framelen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(6), framelen);
    for (uint32_t x = UINT32_C(0); x < framelen; x++)
    {
        EXPECT_EQ(testframe2[x], frame[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: the custom-non-valid checksum we provide(fake CRC_16) in order to
     * check the case of special characters(C0) existence in checksum */
    CrcChecksum = 0xC0C0;
    uint8_t testframe3[] = { 0xC0, 0xDB, 0xDC, 0xDB, 0xDC, 0xDD, 0xD2, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data2, sizeof(data2), sizeof(frame), frame,
            &framelen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(sizeof(testframe3), framelen);
    for (uint32_t x = UINT32_C(0); x < framelen; x++)
    {
        EXPECT_EQ(testframe3[x], frame[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: the custom-non-valid checksum we provide(fake CRC_16) in order to
     * check the case of special characters(C9) existence in checksum */
    CrcChecksum = 0xC9C9;
    uint8_t testframe4[] = { 0xC0, 0xDB, 0xDE, 0xDB, 0xDE, 0xDD, 0xD2, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data2, sizeof(data2), sizeof(frame), frame,
            &framelen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(sizeof(testframe4), framelen);
    for (uint32_t x = UINT32_C(0); x < framelen; x++)
    {
        EXPECT_EQ(testframe4[x], frame[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: the custom-non-valid checksum we provide(fake CRC_16) in order to
     * check the case of special characters(DB) existence in checksum */
    CrcChecksum = 0xDBDB;
    uint8_t testframe5[] = { 0xC0, 0xDB, 0xDD, 0xDB, 0xDD, 0xDD, 0xD2, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_EncodeFrame(data2, sizeof(data2), sizeof(frame), frame,
            &framelen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(sizeof(testframe5), framelen);
    for (uint32_t x = UINT32_C(0); x < framelen; x++)
    {
        EXPECT_EQ(testframe5[x], frame[x]);
    }
}

TEST_F(XProtocolRoutines, testGetPayloadLengthFail)
{
    /** @testcase{XProtocolRoutines::testGetPayloadLengthFail: }
     * XProtocol_GetPayloadLength API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame1[TEST_DATA_SIZE] = { 0xC1, 0xC1, 0xC2, 0xC3, 0xDB, 0xD2 };
    Retcode_T retVal = RETCODE_OK;

    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_GetPayloadLength(frame1, sizeof(frame1), NULL);

    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    uint32_t payloadlen;

    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_GetPayloadLength(NULL, sizeof(frame1), &payloadlen);

    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame2[TEST_DATA_SIZE] = { 0xC0, 0xDB, 0xDE, 0xDB, 0xDC, 0xC9 };

    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_GetPayloadLength(frame2, sizeof(frame2), &payloadlen);

    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED),
            retVal);
}

TEST_F(XProtocolRoutines, testGetPayloadLength)
{
    /** @testcase{XProtocolRoutines::testGetPayloadLength: }
     * XProtocol_GetPayloadLength API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame1[TEST_DATA_SIZE] = { 0xC0, 0xC1, 0xC2, 0xDB, 0xDC, 0xC9 };
    Retcode_T retVal = RETCODE_OK;
    uint32_t payloadlen;

    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_GetPayloadLength(frame1, sizeof(frame1), &payloadlen);

    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), payloadlen);

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame3[TEST_DATA_SIZE] = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC9 };

    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_GetPayloadLength(frame3, sizeof(frame3), &payloadlen);

    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(2), payloadlen);
}

TEST_F(XProtocolRoutines, testDecodeFrameFail)
{
    /** @testcase{XProtocolRoutines::testDecodeFrameFail: }
     * XProtocol_DecodeFrame API is called to check the Null condition
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame[3] = { 1 };
    uint8_t data[MAX_DATA_LENGTH] = { 1 };
    uint32_t datalength;
    Retcode_T retVal;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(NULL, sizeof(frame), sizeof(data), data,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame, sizeof(frame), sizeof(data), NULL,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame, sizeof(frame), sizeof(data), data, NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame1[2] = { 0xDE, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame1, sizeof(frame1), sizeof(data), data,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_START_DELIMITER_MISSING),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t frame2[2] = { 0xC0, 0xC1 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame2, sizeof(frame2), sizeof(data), data,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_END_DELIMITER_MISSING),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* we set a custom crcChecksum same with frame's checksum(not including esc
     * letters) */
    CrcChecksum = 0x61F0;
    const uint8_t frame3[11] = { 0xC0, 0x61, 0xF0, 0xC1, 0xC1, 0xC2, 0xC3, 0xDB, 0xDD,
            0xD2, 0xC9 };
    uint8_t data1[MAX_DATA_LENGTH] = { 1 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame3, sizeof(frame3), 2, data1, &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_DATA_BUFFER_TOO_SMALL),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* we set the VALID custom crcChecksum for frame4 payload */
    CrcChecksum = 0xFD4A;
    const uint8_t frame4[6] = { 0xC0, 0xFD, 0x41, 0xD1, 0xD1, 0xC9 };
    uint8_t data3[MAX_FRAME_SIZE] = { 1 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame4, sizeof(frame4), sizeof(data3), data3,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* we provide the actual VALID custom crcChecksum for frame5 payload */
    CrcChecksum = 0xDB5C;
    const uint8_t frame5[6] = { 0xC0, 0xDB, 0xDC, 0xD1, 0xD1, 0xC9 };
    uint8_t data4[MAX_FRAME_SIZE] = { 1 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame5, sizeof(frame5), sizeof(data4), data4,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* we provide the actual VALID custom crcChecksum for frame6 payload */
    CrcChecksum = 0xDB5C;
    const uint8_t frame6[6] = { 0xC0, 0xDB, 0xDE, 0xD1, 0xD1, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame6, sizeof(frame6), sizeof(data4), data4,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* we provide the actual VALID custom crcChecksum for frame7 payload */
    CrcChecksum = 0xDB5C;
    const uint8_t frame7[6] = { 0xC0, 0xDB, 0xDD, 0xD1, 0xD1, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame7, sizeof(frame7), sizeof(data4), data4,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_INTEGRITY_FAILED),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* we provide the actual VALID custom crcChecksum for frame9 payload */
    CrcChecksum = 0x1281;
    const uint8_t frame9[7] = { 0xC0, 0xDB, 0xDC, 0xD1, 0xDB, 0xD1, 0xC9 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame9, 0, sizeof(data4), data4,
            &datalength);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_XPROTOCOL_FRAME_BUFFER_TOO_SMALL),
            retVal);
}

TEST_F(XProtocolRoutines, testDecodeFrame)
{
    /** @testcase{XProtocolRoutines::testDecodeFrame: }
     * XProtocol_DecodeFrame API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: Custom checksum provided by fake CRC_16 function. Must be the valid
     * checksum of expected testdata1 payload */
    CrcChecksum = 0x61F0;
    const uint8_t frame[11] = { 0xC0, 0x61, 0xF0, 0xC1, 0xC1, 0xC2, 0xC3, 0xDB, 0xDD,
            0xD2, 0xC9 };
    Retcode_T retVal = RETCODE_OK;
    uint8_t data1[MAX_FRAME_SIZE] = { 1 };
    uint8_t testdata1[] = { 0xC1, 0xC1, 0xC2, 0xC3, 0xDB, 0xD2 };
    uint32_t datalen;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame, sizeof(frame), sizeof(data1), data1,
            &datalen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(6), datalen);
    for (uint32_t x = UINT32_C(0); x < datalen; x++)
    {
        EXPECT_EQ(testdata1[x], data1[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: Custom checksum provided by fake CRC_16 function. Must be the valid
     * checksum of expected testdata2 payload */
    CrcChecksum = 0x72B8;
    const uint8_t frame1[14] = { 0xC0, 0x72, 0xB8, 0xDB, 0xDC, 0xC1, 0xC2, 0xDB, 0xDE,
            0xDB, 0xDD, 0xDB, 0xDD, 0xC9 };
    uint8_t data2[MAX_FRAME_SIZE] = { 1 };
    uint8_t testdata2[] = { 0xC0, 0xC1, 0xC2, 0xC9, 0xDB, 0xDB };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame1, sizeof(frame1), sizeof(data2), data2,
            &datalen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(6), datalen);
    for (uint32_t x = UINT32_C(0); x < datalen; x++)
    {
        EXPECT_EQ(testdata2[x], data2[x]);
    }

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* CrcChecksum: Custom checksum provided by fake CRC_16 function. Must be the valid
     * checksum of expected testdata3 payload */
    CrcChecksum = 0xFD4A;
    const uint8_t frame2[6] = { 0xC0, 0xFD, 0x4A, 0xD1, 0xD1, 0xC9 };
    uint8_t data3[MAX_FRAME_SIZE] = { 1 };
    uint8_t testdata3[] = { 0xD1, 0xD1 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_DecodeFrame(frame2, sizeof(frame2), sizeof(data3), data3,
            &datalen);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(2), datalen);
    for (uint32_t x = UINT32_C(0); x < datalen; x++)
    {
        EXPECT_EQ(testdata3[x], data3[x]);
    }
}

TEST_F(XProtocolRoutines, testIsCompleteFrameFail)
{
    /** @testcase{XProtocolRoutines::testIsCompleteFrameFail: }
     * XProtocol_IsCompleteFrame API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T retVal = RETCODE_OK;
    const uint8_t frame[3] = { 0x32, 0x34, 0x40 };
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(NULL, sizeof(frame), NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    const uint8_t *lastCheckPosition = frame;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(frame, sizeof(frame), &lastCheckPosition);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_XPROTOCOL_FRAME_NOT_COMPLETE_YET),
            retVal);
    EXPECT_EQ(&frame[2], lastCheckPosition);

    /* SETUP: Declare and initialize local variables required only by this test case */
    lastCheckPosition = frame;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(frame, 0, &lastCheckPosition);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_XPROTOCOL_START_DELIMITER_MISSING),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(frame, 1, NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_XPROTOCOL_FRAME_NOT_COMPLETE_YET),
            retVal);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(frame, 0, NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_XPROTOCOL_START_DELIMITER_MISSING),
            retVal);
}

TEST_F(XProtocolRoutines, testIsCompleteFrame)
{
    /** @testcase{XProtocolRoutines::testIsCompleteFrame: }
     * XProtocol_IsCompleteFrame API is called
     */

    /* SETUP: Declare and initialize local variables required only by this test case */
    Retcode_T retVal = RETCODE_OK;
    const uint8_t frame[6] = { 0xC0, 0x34, 0x09, 0x33, 0x34, 0xC9 };
    const uint8_t *lastCheckPosition = frame;
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(frame, sizeof(frame), &lastCheckPosition);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    /* we expect this "lastCheckPosition" because the last byte of frame is an ED */
    EXPECT_EQ(&frame[5], lastCheckPosition);

    /* SETUP: Declare and initialize local variables required only by this test case */
    /* EXECISE: call relevant production code Interface with appropriate test inputs */
    retVal = XProtocol_IsCompleteFrame(frame, sizeof(frame), NULL);
    /* VERIFY: Compare the expected with actual */
    EXPECT_EQ(RETCODE_OK, retVal);
    /* we expect this "lastCheckPosition" because we set the third value of the
     * function as NULL so, we don't expect the lastCheckPosition to change. */
    EXPECT_EQ(&frame[5], lastCheckPosition);
}
#else
}
#endif /* if KISO_FEATURE_XPROTOCOL */
