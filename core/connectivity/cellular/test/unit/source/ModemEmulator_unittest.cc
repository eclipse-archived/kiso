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
extern "C"
{
#define GTEST

#include "ModemEmulator.cc"
}
FFF_DEFINITION_BLOCK_END

class TS_SmokeTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        ConnectFakeModem();
    }

    virtual void TearDown()
    {
        DisconnectFakeModem();
    }
};

TEST_F(TS_SmokeTest, EchoReceivePass)
{
    const char *cmd = "UREG";
    const char *data = "+UREG: 1,0\r\n";
    uint8_t *buf = NULL;
    uint32_t bufLen = 0;

    Retcode_T retcode = RETCODE_OK;

    retcode = AtResponseParser_Parse((const uint8_t *)data, strlen(data));
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = AtResponseQueue_WaitForNamedCmd(0U, (const uint8_t *)cmd, strlen(cmd));
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(0U, &buf, &bufLen);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ("1", (char *)buf);
    AtResponseQueue_MarkBufferAsUnused();

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(0U, &buf, &bufLen);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ("0", (char *)buf);
    AtResponseQueue_MarkBufferAsUnused();
}

TEST_F(TS_SmokeTest, EchoSendPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "AT+CREG?\r\n";

    retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)cmd, strlen(cmd), 0);
    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_SmokeTest, EchoSendThenReceivePass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *rxcmd = "UREG";
    const char *txcmd = "AT+CREG?\r\n";
    uint8_t *buf = NULL;
    uint32_t bufLen = 0;
    const char *rxdata = "+UREG: 1,0\r\n";
    AddFakeAnswer(txcmd, rxdata);

    retcode = Engine_SendAtCommandWaitEcho((const uint8_t *)txcmd, strlen(txcmd), 0);
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = AtResponseQueue_WaitForNamedCmd(0U, (const uint8_t *)rxcmd, strlen(rxcmd));
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(0U, &buf, &bufLen);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ("1", (char *)buf);
    AtResponseQueue_MarkBufferAsUnused();

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(0U, &buf, &bufLen);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ("0", (char *)buf);
    AtResponseQueue_MarkBufferAsUnused();
}

TEST_F(TS_SmokeTest, NoEchoSendThenReceivePass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *rxcmd = "UREG";
    const char *txcmd = "AT+CREG?\r\n";
    uint8_t *buf = NULL;
    uint32_t bufLen = 0;
    const char *rxdata = "+UREG: 1,0\r\n";
    AddFakeAnswer(txcmd, rxdata);
    ModemEmulator_EnableEcho = false;

    retcode = Engine_SendAtCommand((const uint8_t *)txcmd, strlen(txcmd));
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = AtResponseQueue_WaitForNamedCmd(0U, (const uint8_t *)rxcmd, strlen(rxcmd));
    EXPECT_EQ(RETCODE_OK, retcode);

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(0U, &buf, &bufLen);
    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ("1", (char *)buf);
    AtResponseQueue_MarkBufferAsUnused();

    retcode = AtResponseQueue_WaitForArbitraryCmdArg(0U, &buf, &bufLen);
    EXPECT_EQ(RETCODE_OK, retcode);

    EXPECT_STREQ("0", (char *)buf);

    AtResponseQueue_MarkBufferAsUnused();
}
