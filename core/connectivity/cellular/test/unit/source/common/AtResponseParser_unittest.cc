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

#include <gtest.h>

FFF_DEFINITION_BLOCK_START

FAKE_VOID_FUNC(CallbackError)

extern "C"
{

#ifndef GTEST
#define GTEST
#endif /* GTEST */

#define portNUM_CONFIGURABLE_REGIONS 1
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_ATPARSER

#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "queue_th.hh"

#undef RETCODE
#define RETCODE(severity, code) ((Retcode_T)code)

#undef KISO_MODULE_ID
#include "AtResponseParser.c"

#ifndef countof
#define countof(a) (sizeof(a) / sizeof(*(a)))
#endif

    static void AtrpTestErrorHandler()
    {
        CallbackError();
        AtResponseParser_Reset();
    }
}

FAKE_VOID_FUNC(CallbackResponseCode, AtResponseCode_T)
FAKE_VOID_FUNC(CallbackCmdEcho, uint8_t *, uint32_t)
FAKE_VOID_FUNC(CallbackCmd, uint8_t *, uint32_t)
FAKE_VOID_FUNC(CallbackCmdArg, uint8_t *, uint32_t)
FAKE_VOID_FUNC(CallbackMisc, uint8_t *, uint32_t)

FFF_DEFINITION_BLOCK_END

class AtResponseParser : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        state.EventResponseCodeCallback = CallbackResponseCode;
        state.EventCmdEchoCallback = CallbackCmdEcho;
        state.EventCmdCallback = CallbackCmd;
        state.EventCmdArgCallback = CallbackCmdArg;
        state.EventMiscCallback = CallbackMisc;
        state.EventErrorCallback = AtrpTestErrorHandler;
        state.StateCallback = AtrpStateRoot;

        RESET_FAKE(CallbackResponseCode);
        RESET_FAKE(CallbackCmdEcho);
        RESET_FAKE(CallbackCmd);
        RESET_FAKE(CallbackCmdArg);
        RESET_FAKE(CallbackMisc);
        RESET_FAKE(CallbackError);

        AtResponseParser_Reset();
    }

    virtual void TearDown()
    {
        ;
    }
};

TEST_F(AtResponseParser, AtrpTrimWhitespaceAllContent)
{
    /** @testcase{ AtResponseParserInternals::AtrpTrimWhitespaceAllContent: }
     *
     * Tests the successful execution of the parser reset.
     */

    const char *buffer = "\r\r\r   \n\r\n \r\n\r \n\n\n  ";
    uint32_t NewLength = strlen(buffer);

    uint8_t *TrimmedBuffer = AtrpTrimWhitespace((uint8_t *)buffer, NewLength, &NewLength);

    KISO_UNUSED(TrimmedBuffer);

    EXPECT_EQ(0U, NewLength);
}

TEST_F(AtResponseParser, AtResponseParserResetSuccess)
{
    /** @testcase{ AtResponseParser::AtResponseParserResetSuccess: }
     *
     * Tests the successful execution of the parser reset.
     */

    state.BufferPosition = 123;
    state.StateCallback = (AtrpStateCallback_T)1;

    AtResponseParser_Reset();

    EXPECT_EQ(0U, state.BufferPosition);
    EXPECT_EQ((AtrpStateCallback_T)AtrpStateRoot, state.StateCallback);
}

TEST_F(AtResponseParser, InternationalMobileSubscriberIdentification)
{
    /** @testcase{ AtResponseParser::InternationalMobileSubscriberIdentification: }
     *
     * Tests the successful parsing of a simple response (AT+CIMI\r\r\n+310170230316694\r\n\r\nOK\r\n).
     */

    const char *AtResponse = "AT+CIMI\r\r\n310170230316694\r\n\r\nOK\r\n";
    uint32_t AtResponseLength = strlen(AtResponse);
    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((uint8_t *)AtResponse, AtResponseLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, CallbackCmdEcho_fake.call_count);
    EXPECT_EQ(strlen("AT+CIMI"), CallbackCmdEcho_fake.arg1_val);

    EXPECT_EQ(4U, CallbackMisc_fake.call_count);                                 /** \todo: FIXME: should we really handle \n as individual events? */
    EXPECT_EQ(strlen("310170230316694\r\n"), CallbackMisc_fake.arg1_history[1]); /** should be the second misc, event incl. whitespace ... for now */

    // Unfortunately the cmdEcho_fake.arg0_val is lost by the AtrpResetBuffer-function inside AtResponseParser_Parse.
    EXPECT_EQ(1U, CallbackResponseCode_fake.call_count);
    EXPECT_EQ(AT_RESPONSE_CODE_OK, CallbackResponseCode_fake.arg0_val);
}

TEST_F(AtResponseParser, ReportMobileTerminationError)
{
    /** @testcase{ AtResponseParser::ReportMobileTerminationError: }
     *
     * Tests the successful parsing of a simple response (AT+CMEE?\r\r\n+CMEE: 0\r\n\r\nOK\r\n).
     */

    const char *AtResponse = "AT+CMEE?\r\r\n+CMEE: 0\r\n\r\nOK\r\n";
    uint32_t AtResponseLength = strlen(AtResponse);
    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((uint8_t *)AtResponse, AtResponseLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, CallbackCmdEcho_fake.call_count);
    EXPECT_EQ(strlen("AT+CMEE?"), CallbackCmdEcho_fake.arg1_val);

    EXPECT_EQ(1U, CallbackCmd_fake.call_count);
    EXPECT_EQ(strlen("CMEE"), CallbackCmd_fake.arg1_val);

    EXPECT_EQ(1U, CallbackCmdArg_fake.call_count);
    EXPECT_EQ(strlen("0"), CallbackCmdArg_fake.arg1_val);

    // Unfortunately the cmdEcho_fake.arg0_val is lost by the AtrpResetBuffer-function inside
    //      AtResponseParser_Parse.
    EXPECT_EQ(1U, CallbackResponseCode_fake.call_count);
    EXPECT_EQ(AT_RESPONSE_CODE_OK, CallbackResponseCode_fake.arg0_val);
}

TEST_F(AtResponseParser, RequestCompleteCapabilitiesList)
{
    /** @testcase{ AtResponseParser::RequestCompleteCapabilitiesList: }
     *
     * Tests the successful parsing of a simple response (AT+GCAP\r\n\r\n+GCAP: +FCLASS, +CGSM\r\n\r\nOK\r\n).
     */

    const char *AtResponse = "AT+GCAP\r\n\r\n+GCAP: +FCLASS, +CGSM\r\n\r\nOK\r\n";
    uint32_t AtResponseLength = strlen(AtResponse);
    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((uint8_t *)AtResponse, AtResponseLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, CallbackCmdEcho_fake.call_count);
    EXPECT_EQ(strlen("AT+GCAP"), CallbackCmdEcho_fake.arg1_val);
    EXPECT_EQ(4U, CallbackMisc_fake.call_count); /* FIXME: should we really handle \n as individual events? */
    EXPECT_EQ(0U, CallbackError_fake.call_count);

    EXPECT_EQ(1U, CallbackCmd_fake.call_count);
    EXPECT_EQ(strlen("GCAP"), CallbackCmd_fake.arg1_val);

    EXPECT_EQ(2U, CallbackCmdArg_fake.call_count);
    EXPECT_EQ(strlen("+FCLASS"), CallbackCmdArg_fake.arg1_history[0]);
    EXPECT_EQ(strlen("+CGSM"), CallbackCmdArg_fake.arg1_history[1]);

    // Unfortunately the cmdEcho_fake.arg0_val is lost by the AtrpResetBuffer-function inside
    //      AtResponseParser_Parse.
    EXPECT_EQ(1U, CallbackResponseCode_fake.call_count);
    EXPECT_EQ(AT_RESPONSE_CODE_OK, CallbackResponseCode_fake.arg0_val);
}

TEST_F(AtResponseParser, UrcNetworkSelectionControl)
{
    /** @testcase{ AtResponseParser::UrcNetworkSelectionControl }
     *
     * Tests the successful parsing of a simple response (\r\nPACSP1\r\n).
     */

    const char *AtResponse = "\r\n+PACSP1\r\n";
    uint32_t AtResponseLength = strlen(AtResponse);
    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((uint8_t *)AtResponse, AtResponseLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0U, CallbackError_fake.call_count);

    EXPECT_EQ(1U, CallbackCmd_fake.call_count);
    EXPECT_EQ(strlen("PACSP1"), CallbackCmd_fake.arg1_val);
}

TEST_F(AtResponseParser, UrcNetworkSelectionControlFollowedByCmd)
{
    /** @testcase{ AtResponseParser::UrcNetworkSelectionControl }
     *
     * Tests the successful parsing of a simple response (\r\n+PACSP1\r\n\r\n+UMWI: 1\r\n).
     */

    const char *AtResponse = "\r\n+PACSP1\r\n\r\n+UMWI: 1\r\n";
    uint32_t AtResponseLength = strlen(AtResponse);
    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((uint8_t *)AtResponse, AtResponseLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0U, CallbackError_fake.call_count);

    EXPECT_EQ(2U, CallbackCmd_fake.call_count);
    EXPECT_EQ(strlen("PACSP1"), CallbackCmd_fake.arg1_history[0]);
    EXPECT_EQ(strlen("UMWI"), CallbackCmd_fake.arg1_history[1]);

    EXPECT_EQ(1U, CallbackCmdArg_fake.call_count);
}

TEST_F(AtResponseParser, AtCommandEchoTerminatedByS3)
{
    /** @testcase{ AtResponseParser::UrcNetworkSelectionControl }
     *
     * Tests the successful parsing of an AT command echo (AT+PACSP\r) which is terminated only by an S3 character.
     */

    const char *AtResponse = "AT+PACSP\r";
    uint32_t AtResponseLength = strlen(AtResponse);
    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((uint8_t *)AtResponse, AtResponseLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0U, CallbackMisc_fake.call_count);
    EXPECT_EQ(0U, CallbackError_fake.call_count);

    EXPECT_EQ(1U, CallbackCmdEcho_fake.call_count);
    EXPECT_EQ(strlen("AT+PACSP"), CallbackCmdEcho_fake.arg1_history[0]);
}

TEST_F(AtResponseParser, AtResponseParserBufferOutOfSpace)
{
    /** @testcase{ AtResponseParser::AtResponseParserBufferOutOfSpace: }
     *
     * Tests buffer out of space condition.
     */

    uint8_t resp[ATRP_INTERNAL_BUFFER_LEN + 1];
    memset(resp, (int)'A', sizeof(resp));
    resp[sizeof(resp) - 1] = '\n';
    state.BufferPosition = 0;
    state.StateCallback = AtrpStateCmdEcho;

    Retcode_T retcode = AtResponseParser_Parse(resp, sizeof(resp));

    EXPECT_EQ(AT_RESPONSE_PARSER_PARSE_ERROR, Retcode_GetCode(retcode));
}

TEST_F(AtResponseParser, AtResponseParser_Parse_Fail)
{
    Retcode_T retcode;
    uint8_t buffer[1];

    AtrpSwitchState(NULL);
    retcode = AtResponseParser_Parse(buffer, sizeof(buffer));
    EXPECT_EQ(AT_RESPONSE_PARSER_NOT_INITIALIZED, Retcode_GetCode(retcode));
    AtResponseParser_Reset();
    retcode = AtResponseParser_Parse(buffer, 0);
    EXPECT_EQ(AT_RESPONSE_PARSER_INPUT_TOO_SHORT, Retcode_GetCode(retcode));
}

static const char *AtResponseParserParsingIncompleteResonseFull =
    "AT+USOCR=6\r\r\n+USOCR: 0\r\n\r\nOK\r\n";
class AtResponseParserParsingIncomplete : public testing::TestWithParam<uint32_t>
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        state.EventResponseCodeCallback = CallbackResponseCode;
        state.EventCmdEchoCallback = CallbackCmdEcho;
        state.EventCmdCallback = CallbackCmd;
        state.EventCmdArgCallback = CallbackCmdArg;
        state.EventMiscCallback = CallbackMisc;
        state.EventErrorCallback = AtrpTestErrorHandler;
        state.StateCallback = AtrpStateRoot;

        RESET_FAKE(CallbackResponseCode);
        RESET_FAKE(CallbackCmdEcho);
        RESET_FAKE(CallbackCmd);
        RESET_FAKE(CallbackCmdArg);
        RESET_FAKE(CallbackMisc);
        RESET_FAKE(CallbackError);

        AtResponseParser_Reset();
    }

    virtual void TearDown()
    {
        ;
    }
};

TEST_P(AtResponseParserParsingIncomplete, ParseIncomplete00)
{
    uint32_t split = GetParam();

    Retcode_T retcode00 = RETCODE_OK;
    Retcode_T retcode01 = RETCODE_OK;

    retcode00 = AtResponseParser_Parse((uint8_t *)AtResponseParserParsingIncompleteResonseFull,
                                       split);
    retcode01 = AtResponseParser_Parse(
        (uint8_t *)AtResponseParserParsingIncompleteResonseFull + split,
        strlen(AtResponseParserParsingIncompleteResonseFull) - split);

    EXPECT_EQ(RETCODE_OK, retcode00);
    EXPECT_EQ(RETCODE_OK, retcode01);
    EXPECT_EQ(0U, CallbackError_fake.call_count);

    EXPECT_EQ(1U, CallbackCmdEcho_fake.call_count);
    EXPECT_EQ(strlen("AT+USOCR=6"), CallbackCmdEcho_fake.arg1_val);

    EXPECT_EQ(1U, CallbackCmd_fake.call_count);
    EXPECT_EQ(strlen("USOCR"), CallbackCmd_fake.arg1_val);

    EXPECT_EQ(1U, CallbackCmdArg_fake.call_count);
    EXPECT_EQ(strlen("0"), CallbackCmdArg_fake.arg1_val);

    EXPECT_EQ(1U, CallbackResponseCode_fake.call_count);
    EXPECT_EQ(AT_RESPONSE_CODE_OK, CallbackResponseCode_fake.arg0_val);
}

std::vector<uint32_t> GenerateSplitTestcaseValues()
{
    uint32_t noOfSplits = strlen(AtResponseParserParsingIncompleteResonseFull) - 1;
    std::vector<uint32_t> result(noOfSplits);

    for (uint32_t i = 0; i < noOfSplits; i++)
    {
        result[i] = i + 1;
    }

    return result;
}

INSTANTIATE_TEST_CASE_P(ATRPi, AtResponseParserParsingIncomplete,
                        testing::ValuesIn(GenerateSplitTestcaseValues()));

TEST_F(AtResponseParser, TestAtrpStateResponseCode)
{
    const struct
    {
        const char *name;
        AtResponseCode_T code;
    } m[] = {
        {AT_RESPONSE_CODE_NAME_OK, AT_RESPONSE_CODE_OK},
        {AT_RESPONSE_CODE_NAME_CONNECT, AT_RESPONSE_CODE_CONNECT},
        {AT_RESPONSE_CODE_NAME_RING, AT_RESPONSE_CODE_RING},
        {AT_RESPONSE_CODE_NAME_NO_CARRIER, AT_RESPONSE_CODE_NO_CARRIER},
        {AT_RESPONSE_CODE_NAME_ERROR, AT_RESPONSE_CODE_ERROR},
        {AT_RESPONSE_CODE_NAME_NO_DIALTONE, AT_RESPONSE_CODE_NO_DIALTONE},
        {AT_RESPONSE_CODE_NAME_BUSY, AT_RESPONSE_CODE_BUSY},
        {AT_RESPONSE_CODE_NAME_NO_ANSWER, AT_RESPONSE_CODE_NO_ANSWER},
        {AT_RESPONSE_CODE_NAME_ABORTED, AT_RESPONSE_CODE_ABORTED}};

    /* test failing scenarios */
    uint8_t buffer[ATRP_INTERNAL_BUFFER_LEN] = {'\0'};
    uint32_t BufferLength = sizeof(buffer);
    EXPECT_EQ(1, AtrpStateResponseCode(buffer, 1));
    EXPECT_EQ(ATRP_PARSE_FAILURE_RETVAL, AtrpStateResponseCode(buffer, BufferLength));

    /* test passing scenarios */
    AtrpResetBuffer();
    for (uint32_t i = 0; i < countof(m); i++)
    {
        int32_t count = snprintf((char *)buffer, BufferLength, "%s\n", m[i].name);
        EXPECT_EQ(true, count > 0 && (uint32_t)count < BufferLength);
        EXPECT_EQ(count, AtrpStateResponseCode(buffer, count));
        EXPECT_EQ(m[i].code, CallbackResponseCode_fake.arg0_val);
    }
}

TEST_F(AtResponseParser, TestRegisterCallback)
{
    AtResponseParser_RegisterResponseCodeCallback(NULL);
    EXPECT_EQ(ATRP_RESPONSE_CODE_EVENT, (void *)NULL);
    AtResponseParser_RegisterResponseCodeCallback(CallbackResponseCode);
    EXPECT_EQ(ATRP_RESPONSE_CODE_EVENT, (void *)CallbackResponseCode);

    AtResponseParser_RegisterErrorCallback(NULL);
    EXPECT_EQ(ATRP_ERROR_EVENT, (void *)NULL);
    AtResponseParser_RegisterErrorCallback(AtrpTestErrorHandler);
    EXPECT_EQ(ATRP_ERROR_EVENT, (void *)AtrpTestErrorHandler);

    AtResponseParser_RegisterCmdEchoCallback(NULL);
    EXPECT_EQ(ATRP_CMD_ECHO_EVENT, (void *)NULL);
    AtResponseParser_RegisterCmdEchoCallback(CallbackCmdEcho);
    EXPECT_EQ(ATRP_CMD_ECHO_EVENT, (void *)CallbackCmdEcho);

    AtResponseParser_RegisterCmdCallback(NULL);
    EXPECT_EQ(ATRP_CMD_EVENT, (void *)NULL);
    AtResponseParser_RegisterCmdCallback(CallbackCmd);
    EXPECT_EQ(ATRP_CMD_EVENT, (void *)CallbackCmd);

    AtResponseParser_RegisterCmdArgCallback(NULL);
    EXPECT_EQ(ATRP_CMDARG_EVENT, (void *)NULL);
    AtResponseParser_RegisterCmdArgCallback(CallbackCmdArg);
    EXPECT_EQ(ATRP_CMDARG_EVENT, (void *)CallbackCmdArg);

    AtResponseParser_RegisterMiscCallback(NULL);
    EXPECT_EQ(ATRP_MISC_EVENT, (void *)NULL);
    AtResponseParser_RegisterMiscCallback(CallbackMisc);
    EXPECT_EQ(ATRP_MISC_EVENT, (void *)CallbackMisc);

    AtrpSwitchState(NULL);
    EXPECT_EQ(state.StateCallback, (void *)NULL);
    AtResponseParser_Reset();
    EXPECT_EQ(state.StateCallback, (void *)AtrpStateRoot);
}

TEST_F(AtResponseParser, TestAtrpStateError)
{
    EXPECT_EQ(0U, CallbackError_fake.call_count);
    EXPECT_EQ(0, AtrpStateError(NULL, 0));
    EXPECT_EQ(1U, CallbackError_fake.call_count);
}

TEST_F(AtResponseParser, AtrpRollbackBufferTest)
{
    state.BufferPosition = 1;
    AtrpRollbackBuffer(2);
    EXPECT_EQ(1U, state.BufferPosition);
}

TEST_F(AtResponseParser, AtrpStateCmdargTest)
{
    int32_t result = 0U;
    uint8_t buffer[ATRP_INTERNAL_BUFFER_LEN] = {'\0'};
    state.BufferPosition = 1024;
    result = AtrpStateCmdarg(buffer, 2);
    EXPECT_EQ(ATRP_PARSE_FAILURE_RETVAL, result);
}

TEST_F(AtResponseParser, AtrpStateErrorTest)
{
    int32_t result = 1U;
    state.EventErrorCallback = NULL;
    result = AtrpStateError(NULL, 0);
    EXPECT_EQ(0, result);
}
