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

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START
extern "C"
{

/* setup compile time configuration defines */
#include "Kiso_Cellular.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_AT_3GPP_27007
#define GTEST

/* include faked interfaces */
#include "Kiso_Assert_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Logging_th.hh"
#include "FreeRTOS_th.hh"
#include "task_th.hh"

#include <time.h>

/* include module under test */
#include "ModemEmulator.cc"
#undef KISO_MODULE_ID
#include "AtUtils.c"
#undef KISO_MODULE_ID
#include "At3Gpp27007.c"
}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

#define TEST_AT_GET_CREG_URC ("+CREG: %d,%d\r\nOK\r\n")
#define TEST_AT_GET_CREG_URC_LOC ("+CREG: %d,%d,\"%04X\",\"%08X\"\r\nOK\r\n")

#define TEST_AT_GET_CGREG_URC ("+CGREG: %d,%d\r\nOK\r\n")
#define TEST_AT_GET_CGREG_URC_LOC ("+CGREG: %d,%d,\"%04X\",\"%08X\",%d,\"%02X\"\r\nOK\r\n")

#define TEST_AT_GET_CEREG_URC ("+CEREG: %d,%d\r\nOK\r\n")
#define TEST_AT_GET_CEREG_URC_LOC ("+CEREG: %d,%d,\"%04X\",\"%08X\",%d\r\nOK\r\n")

#define TEST_IPV4 "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8
#define TEST_IPV6 "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8
#define TEST_AT_RESP_CGPADDR_IPV4 ("+CGPADDR: %" PRIu8 ",\"" TEST_IPV4 "\"\r\nOK\r\n")
#define TEST_AT_RESP_CGPADDR_IPV4_IPV4 ("+CGPADDR: %" PRIu8 ",\"" TEST_IPV4 "\",\"" TEST_IPV4 "\"\r\nOK\r\n")
#define TEST_AT_RESP_CGPADDR_IPV4_IPV6 ("+CGPADDR: %" PRIu8 ",\"" TEST_IPV4 "\",\"" TEST_IPV6 "\"\r\nOK\r\n")
#define TEST_AT_RESP_CGPADDR_IPV6 ("+CGPADDR: %" PRIu8 ",\"" TEST_IPV6 "\"\r\nOK\r\n")
#define TEST_AT_RESP_CGPADDR_IPV6_IPV4 ("+CGPADDR: %" PRIu8 ",\"" TEST_IPV6 "\",\"" TEST_IPV4 "\"\r\nOK\r\n")
#define TEST_AT_RESP_CGPADDR_IPV6_IPV6 ("+CGPADDR: %" PRIu8 ",\"" TEST_IPV6 "\",\"" TEST_IPV6 "\"\r\nOK\r\n")
#define TEST_AT_RESP_CGPADDR_INVALID ("+CGPADDR: %" PRIu8 "\r\nOK\r\n")

#define TEST_AT_RESP_CFUN ("+CFUN: %" PRIu8 "\r\nOK\r\n")

#define TEST_AT_RESPONSE_OK ("OK\r\n")
#define TEST_AT_RESPONSE_ERROR ("ERROR\r\n")

class TS_At_HandleUrc_CREG : public TS_ModemTest
{
protected:
    virtual void SetUp()
    {
        TS_ModemTest::SetUp();

        RESET_FAKE(Engine_NotifyNewState);
    }
};

TEST_F(TS_At_HandleUrc_CREG, StatPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CREG: 1\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

TEST_F(TS_At_HandleUrc_CREG, StatLacCiPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CREG: 1,\"4E54\",\"44A5\"\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

TEST_F(TS_At_HandleUrc_CREG, StatLacCiAcTPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CREG: 1,\"4E54\",\"44A5\",2\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

class TS_At_HandleUrc_CGREG : public TS_ModemTest
{
protected:
    virtual void SetUp()
    {
        TS_ModemTest::SetUp();

        RESET_FAKE(Engine_NotifyNewState);
    }
};

TEST_F(TS_At_HandleUrc_CGREG, StatPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CGREG: 1\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CGREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

TEST_F(TS_At_HandleUrc_CGREG, StatLacCiPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CGREG: 1,\"4E54\",\"44A5\"\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CGREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

TEST_F(TS_At_HandleUrc_CGREG, StatLacCiAcTRacPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CGREG: 1,\"4E54\",\"44A5\",2,\"FB\"\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CGREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

class TS_At_HandleUrc_CEREG : public TS_ModemTest
{
protected:
    virtual void SetUp()
    {
        TS_ModemTest::SetUp();

        RESET_FAKE(Engine_NotifyNewState);
    }
};

TEST_F(TS_At_HandleUrc_CEREG, StatPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CEREG: 1\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CEREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

TEST_F(TS_At_HandleUrc_CEREG, StatTacCiAcTPass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *cmd = "+CEREG: 1,\"3a9b\",\"0000c33d\",7\r\n";

    (void)AtResponseParser_Parse((const uint8_t *)cmd, strlen(cmd));

    retcode = At_HandleUrc_CEREG();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
}

class TS_At_Set_CREG : public TS_ModemTest
{
protected:
    const char *FormatTrigger(AT_CXREG_N_T n)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCREG_FMT,
                                   n);
    }
};
TEST_F(TS_At_Set_CREG, DisablePass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_DISABLED), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CREG(AT_CXREG_N_DISABLED);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CREG, UrcPass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CREG(AT_CXREG_N_URC);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CREG, UrcLocPass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CREG(AT_CXREG_N_URC_LOC);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CREG, UrcLocCauseFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_CAUSE), TEST_AT_RESPONSE_ERROR);

    retcode = At_Set_CREG(AT_CXREG_N_URC_LOC_CAUSE);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Retcode_GetCode(retcode));
}

TEST_F(TS_At_Set_CREG, UrcLocPsmFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_PSM), TEST_AT_RESPONSE_ERROR);

    retcode = At_Set_CREG(AT_CXREG_N_URC_LOC_PSM);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Retcode_GetCode(retcode));
}

TEST_F(TS_At_Set_CREG, UrcLocPsmCauseFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_PSM_CAUSE), TEST_AT_RESPONSE_ERROR);

    retcode = At_Set_CREG(AT_CXREG_N_URC_LOC_PSM_CAUSE);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Retcode_GetCode(retcode));
}

class TS_At_Set_CGREG : public TS_ModemTest
{
protected:
    const char *FormatTrigger(AT_CXREG_N_T n)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCGREG_FMT,
                                   n);
    }
};
TEST_F(TS_At_Set_CGREG, DisablePass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_DISABLED), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGREG(AT_CXREG_N_DISABLED);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGREG, UrcPass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGREG(AT_CXREG_N_URC);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGREG, UrcLocPass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGREG(AT_CXREG_N_URC_LOC);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGREG, UrcLocCauseFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_CAUSE), TEST_AT_RESPONSE_ERROR);

    retcode = At_Set_CGREG(AT_CXREG_N_URC_LOC_CAUSE);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Retcode_GetCode(retcode));
}

TEST_F(TS_At_Set_CGREG, UrcLocPsmFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_PSM), TEST_AT_RESPONSE_ERROR);

    retcode = At_Set_CGREG(AT_CXREG_N_URC_LOC_PSM);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Retcode_GetCode(retcode));
}

TEST_F(TS_At_Set_CGREG, UrcLocPsmCauseFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_PSM_CAUSE), TEST_AT_RESPONSE_ERROR);

    retcode = At_Set_CGREG(AT_CXREG_N_URC_LOC_PSM_CAUSE);

    EXPECT_EQ(RETCODE_CELLULAR_RESPONDED_ERROR, Retcode_GetCode(retcode));
}

class TS_At_Set_CEREG : public TS_ModemTest
{
protected:
    const char *FormatTrigger(AT_CXREG_N_T n)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCEREG_FMT,
                                   n);
    }
};
TEST_F(TS_At_Set_CEREG, DisablePass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_DISABLED), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CEREG(AT_CXREG_N_DISABLED);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CEREG, UrcPass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CEREG(AT_CXREG_N_URC);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CEREG, UrcLocPass)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CEREG(AT_CXREG_N_URC_LOC);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CEREG, UrcLocCauseFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_CAUSE), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CEREG(AT_CXREG_N_URC_LOC_CAUSE);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CEREG, UrcLocPsmFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_PSM), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CEREG(AT_CXREG_N_URC_LOC_PSM);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CEREG, UrcLocPsmCauseFail)
{
    Retcode_T retcode = RETCODE_OK;

    AddFakeAnswer(FormatTrigger(AT_CXREG_N_URC_LOC_PSM_CAUSE), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CEREG(AT_CXREG_N_URC_LOC_PSM_CAUSE);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Get_CREG : public TS_ModemTest
{
protected:
    AT_CXREG_Stat_T AnswerStat;
    uint16_t AnswerLac;
    uint32_t AnswerCi;

    virtual void SetUp()
    {
        TS_ModemTest::SetUp();
        srand(time(NULL));
    }

    const char *FormatAnswer(AT_CXREG_N_T n)
    {
        const char *fmt = NULL;
        switch (n)
        {
        case AT_CXREG_N_DISABLED:
        case AT_CXREG_N_URC:
            fmt = TEST_AT_GET_CREG_URC;
            AnswerStat = (AT_CXREG_Stat_T)(rand() % (AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING + 1));
            break;
        case AT_CXREG_N_URC_LOC:
            fmt = TEST_AT_GET_CREG_URC_LOC;
            AnswerStat = (AT_CXREG_Stat_T)(rand() % (AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING + 1));
            AnswerLac = (uint16_t)(rand() % (UINT16_MAX - 1));
            AnswerCi = (uint32_t)(rand() % (UINT32_MAX - 1));
            break;
        default:
            exit(1);
            return NULL;
        }
        /* Value 8 is not present in the enum. If rand() happens to return 8 we
         * need to overwrite that value. */
        if (8 == AnswerStat)
        {
            AnswerStat = AT_CXREG_STAT_NOT;
        }
        return FormatIntoNewBuffer(&Answer, fmt,
                                   n, AnswerStat, AnswerLac, AnswerCi);
    }
};

TEST_F(TS_At_Get_CREG, DisabledPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCREG, FormatAnswer(AT_CXREG_N_DISABLED));

    retcode = At_Get_CREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_DISABLED, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
}

TEST_F(TS_At_Get_CREG, UrcPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCREG, FormatAnswer(AT_CXREG_N_URC));

    retcode = At_Get_CREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_URC, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
}

TEST_F(TS_At_Get_CREG, UrcLocPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCREG, FormatAnswer(AT_CXREG_N_URC_LOC));

    retcode = At_Get_CREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_URC_LOC, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
    EXPECT_EQ(AnswerLac, param.Lac);
    EXPECT_EQ(AnswerCi, param.Ci);
    EXPECT_EQ(AT_CXREG_ACT_INVALID, param.AcT);
}

class TS_At_Get_CGREG : public TS_ModemTest
{
protected:
    AT_CXREG_Stat_T AnswerStat;
    uint16_t AnswerLac;
    uint32_t AnswerCi;
    AT_CXREG_AcT_T AnswerAcT;
    uint8_t AnswerRac;

    virtual void SetUp()
    {
        TS_ModemTest::SetUp();
        srand(time(NULL));
    }

    const char *FormatAnswer(AT_CXREG_N_T n)
    {
        const char *fmt = NULL;
        switch (n)
        {
        case AT_CXREG_N_DISABLED:
        case AT_CXREG_N_URC:
            fmt = TEST_AT_GET_CGREG_URC;
            AnswerStat = (AT_CXREG_Stat_T)(rand() % (AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING + 1));
            break;
        case AT_CXREG_N_URC_LOC:
            fmt = TEST_AT_GET_CGREG_URC_LOC;
            AnswerStat = (AT_CXREG_Stat_T)(rand() % (AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING + 1));
            AnswerLac = (uint16_t)(rand() % (UINT16_MAX - 1));
            AnswerCi = (uint32_t)(rand() % (UINT32_MAX - 1));
            AnswerAcT = (AT_CXREG_AcT_T)(rand() % (AT_CXREG_ACT_EUTRA_NR + 1));
            AnswerRac = (uint8_t)(rand() % (UINT8_MAX - 1));
            break;
        default:
            exit(1);
            return NULL;
        }
        /* Value 8 is not present in the enum. If rand() happens to return 8 we
         * need to overwrite that value. */
        if (8 == AnswerStat)
        {
            AnswerStat = AT_CXREG_STAT_NOT;
        }

        return FormatIntoNewBuffer(&Answer, fmt,
                                   n, AnswerStat, AnswerLac, AnswerCi, AnswerAcT, AnswerRac);
    }
};

TEST_F(TS_At_Get_CGREG, DisabledPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCGREG, FormatAnswer(AT_CXREG_N_DISABLED));

    retcode = At_Get_CGREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_DISABLED, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
}

TEST_F(TS_At_Get_CGREG, UrcPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCGREG, FormatAnswer(AT_CXREG_N_URC));

    retcode = At_Get_CGREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_URC, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
}

TEST_F(TS_At_Get_CGREG, UrcLocPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCGREG, FormatAnswer(AT_CXREG_N_URC_LOC));

    retcode = At_Get_CGREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_URC_LOC, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
    EXPECT_EQ(AnswerLac, param.Lac);
    EXPECT_EQ(AnswerCi, param.Ci);
    EXPECT_EQ(AnswerAcT, param.AcT);
    EXPECT_EQ(AnswerRac, param.Rac);
}

class TS_At_Get_CEREG : public TS_ModemTest
{
protected:
    AT_CXREG_Stat_T AnswerStat;
    uint16_t AnswerTac;
    uint32_t AnswerCi;
    AT_CXREG_AcT_T AnswerAcT;

    virtual void SetUp()
    {
        TS_ModemTest::SetUp();
        srand(time(NULL));
    }

    const char *FormatAnswer(AT_CXREG_N_T n)
    {
        const char *fmt = NULL;
        switch (n)
        {
        case AT_CXREG_N_DISABLED:
        case AT_CXREG_N_URC:
            fmt = TEST_AT_GET_CEREG_URC;
            AnswerStat = (AT_CXREG_Stat_T)(rand() % (AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING + 1));
            break;
        case AT_CXREG_N_URC_LOC:
            fmt = TEST_AT_GET_CEREG_URC_LOC;
            AnswerStat = (AT_CXREG_Stat_T)(rand() % (AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING + 1));
            AnswerTac = (uint16_t)(rand() % (UINT16_MAX - 1));
            AnswerCi = (uint32_t)(rand() % (UINT32_MAX - 1));
            AnswerAcT = (AT_CXREG_AcT_T)(rand() % (AT_CXREG_ACT_EUTRA_NR + 1));
            break;
        default:
            exit(1);
            return NULL;
        }
        /* Value 8 is not present in the enum. If rand() happens to return 8 we
         * need to overwrite that value. */
        if (8 == AnswerStat)
        {
            AnswerStat = AT_CXREG_STAT_NOT;
        }

        return FormatIntoNewBuffer(&Answer, fmt,
                                   n, AnswerStat, AnswerTac, AnswerCi, AnswerAcT);
    }
};

TEST_F(TS_At_Get_CEREG, DisabledPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CEREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCEREG, FormatAnswer(AT_CXREG_N_DISABLED));

    retcode = At_Get_CEREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_DISABLED, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
}

TEST_F(TS_At_Get_CEREG, UrcPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CEREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCEREG, FormatAnswer(AT_CXREG_N_URC));

    retcode = At_Get_CEREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_URC, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
}

TEST_F(TS_At_Get_CEREG, UrcLocPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CEREG_Param_T param;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCEREG, FormatAnswer(AT_CXREG_N_URC_LOC));

    retcode = At_Get_CEREG(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CXREG_N_URC_LOC, param.N);
    EXPECT_EQ(AnswerStat, param.Stat);
    EXPECT_EQ(AnswerTac, param.Tac);
    EXPECT_EQ(AnswerCi, param.Ci);
    EXPECT_EQ(AnswerAcT, param.AcT);
}

class TS_At_Set_COPS : public TS_ModemTest
{
protected:
    const char *FormatTrigger(AT_COPS_Mode_T mode)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCOPS_FMT,
                                   mode);
    }
};

TEST_F(TS_At_Set_COPS, ManualFail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T params;
    params.Mode = AT_COPS_MODE_MANUAL;
    params.Format = AT_COPS_FORMAT_INVALID;
    params.Oper.Numeric = 0;
    params.AcT = AT_COPS_ACT_INVALID;

    retcode = At_Set_COPS(&params);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

TEST_F(TS_At_Set_COPS, FormatOnlyFail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T params;
    params.Mode = AT_COPS_MODE_SET_FORMAT_ONLY;
    params.Format = AT_COPS_FORMAT_INVALID;
    params.Oper.Numeric = 0;
    params.AcT = AT_COPS_ACT_INVALID;

    retcode = At_Set_COPS(&params);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

TEST_F(TS_At_Set_COPS, ManualThenAutoFail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T params;
    params.Mode = AT_COPS_MODE_MANUAL_THEN_AUTOMATIC;
    params.Format = AT_COPS_FORMAT_INVALID;
    params.Oper.Numeric = 0;
    params.AcT = AT_COPS_ACT_INVALID;

    retcode = At_Set_COPS(&params);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

TEST_F(TS_At_Set_COPS, InvalidModeFail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T params;
    params.Mode = AT_COPS_MODE_INVALID;
    params.Format = AT_COPS_FORMAT_INVALID;
    params.Oper.Numeric = 0;
    params.AcT = AT_COPS_ACT_INVALID;

    retcode = At_Set_COPS(&params);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_At_Set_COPS, AutomaticPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T params;
    params.Mode = AT_COPS_MODE_AUTOMATIC;
    params.Format = AT_COPS_FORMAT_INVALID;
    params.Oper.Numeric = 0;
    params.AcT = AT_COPS_ACT_INVALID;

    AddFakeAnswer(FormatTrigger(params.Mode), TEST_AT_RESPONSE_OK);

    retcode = At_Set_COPS(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_COPS, DeregisterPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_COPS_Param_T params;
    params.Mode = AT_COPS_MODE_DEREGISTER;
    params.Format = AT_COPS_FORMAT_INVALID;
    params.Oper.Numeric = 0;
    params.AcT = AT_COPS_ACT_INVALID;

    AddFakeAnswer(FormatTrigger(params.Mode), TEST_AT_RESPONSE_OK);

    retcode = At_Set_COPS(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_CGDCONT : public TS_ModemTest
{
protected:
    const char *FormatTrigger1(const AT_CGDCONT_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCGDCONT_FMT1,
                                   param->Cid);
    }

    const char *FormatTrigger2(const AT_CGDCONT_Param_T *param)
    {
        const char *pdpType = "INVALID";

        switch (param->PdpType)
        {
        case AT_CGDCONT_PDPTYPE_IP:
            pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IP;
            break;
        case AT_CGDCONT_PDPTYPE_IPV6:
            pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV6;
            break;
        case AT_CGDCONT_PDPTYPE_IPV4V6:
            pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV4V6;
            break;
        case AT_CGDCONT_PDPTYPE_INVALID:
            break;
        default:
            exit(1);
            break;
        }

        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCGDCONT_FMT2,
                                   param->Cid, pdpType);
    }

    const char *FormatTrigger3(const AT_CGDCONT_Param_T *param)
    {
        const char *pdpType = "INVALID";

        switch (param->PdpType)
        {
        case AT_CGDCONT_PDPTYPE_IP:
            pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IP;
            break;
        case AT_CGDCONT_PDPTYPE_IPV6:
            pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV6;
            break;
        case AT_CGDCONT_PDPTYPE_IPV4V6:
            pdpType = ARG_3GPP_27007_ATCGDCONT_PDPTYPE_IPV4V6;
            break;
        case AT_CGDCONT_PDPTYPE_INVALID:
            exit(1);
            break;
        default:
            exit(1);
            break;
        }

        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCGDCONT_FMT3,
                                   param->Cid, pdpType, param->Apn);
    }
};

TEST_F(TS_At_Set_CGDCONT, Clear_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGDCONT_Param_T params;
    params.Cid = 1;
    params.PdpType = AT_CGDCONT_PDPTYPE_INVALID;
    params.Apn = NULL;

    AddFakeAnswer(FormatTrigger1(&params), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGDCONT(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGDCONT, IPV6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGDCONT_Param_T params;
    params.Cid = 1;
    params.PdpType = AT_CGDCONT_PDPTYPE_IPV6;
    params.Apn = NULL;

    AddFakeAnswer(FormatTrigger2(&params), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGDCONT(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGDCONT, IPV4V6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGDCONT_Param_T params;
    params.Cid = 1;
    params.PdpType = AT_CGDCONT_PDPTYPE_IPV4V6;
    params.Apn = NULL;

    AddFakeAnswer(FormatTrigger2(&params), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGDCONT(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGDCONT, IPV6_ClearAPN_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGDCONT_Param_T params;
    params.Cid = 1;
    params.PdpType = AT_CGDCONT_PDPTYPE_IPV4V6;
    params.Apn = "";

    AddFakeAnswer(FormatTrigger3(&params), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGDCONT(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGDCONT, IPV6_DummyAPN_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGDCONT_Param_T params;
    params.Cid = 1;
    params.PdpType = AT_CGDCONT_PDPTYPE_IPV4V6;
    params.Apn = "some.apn.net";

    AddFakeAnswer(FormatTrigger3(&params), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGDCONT(&params);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_CGACT : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_CGACT_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCGACT_FMT,
                                   param->State, param->Cid);
    }
};

TEST_F(TS_At_Set_CGACT, Activate1Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGACT_Param_T param;
    param.Cid = 1;
    param.State = AT_CGACT_STATE_ACTIVATED;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGACT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CGACT, Deactivate2Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGACT_Param_T param;
    param.Cid = 2;
    param.State = AT_CGACT_STATE_DEACTIVATED;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CGACT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_CGPADDR : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_CGPADDR_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_EXE_ATCGPADDR,
                                   param->Cid);
    }

    const char *FormatAnswer(uint8_t cid, const AT_CGPADDR_Address_T *addr1, const AT_CGPADDR_Address_T *addr2)
    {
        switch (addr1->Type)
        {
        case AT_CGPADDR_ADDRESSTYPE_IPV4:
            switch (addr2->Type)
            {
            case AT_CGPADDR_ADDRESSTYPE_IPV4:
                return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_IPV4_IPV4,
                                           cid,
                                           addr1->Address.IPv4[3],
                                           addr1->Address.IPv4[2],
                                           addr1->Address.IPv4[1],
                                           addr1->Address.IPv4[0],

                                           addr2->Address.IPv4[3],
                                           addr2->Address.IPv4[2],
                                           addr2->Address.IPv4[1],
                                           addr2->Address.IPv4[0]);
            case AT_CGPADDR_ADDRESSTYPE_IPV6:
                return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_IPV4_IPV6,
                                           cid,
                                           addr1->Address.IPv4[3],
                                           addr1->Address.IPv4[2],
                                           addr1->Address.IPv4[1],
                                           addr1->Address.IPv4[0],

                                           addr2->Address.IPv6[15],
                                           addr2->Address.IPv6[14],
                                           addr2->Address.IPv6[13],
                                           addr2->Address.IPv6[12],
                                           addr2->Address.IPv6[11],
                                           addr2->Address.IPv6[10],
                                           addr2->Address.IPv6[9],
                                           addr2->Address.IPv6[8],
                                           addr2->Address.IPv6[7],
                                           addr2->Address.IPv6[6],
                                           addr2->Address.IPv6[5],
                                           addr2->Address.IPv6[4],
                                           addr2->Address.IPv6[3],
                                           addr2->Address.IPv6[2],
                                           addr2->Address.IPv6[1],
                                           addr2->Address.IPv6[0]);
            case AT_CGPADDR_ADDRESSTYPE_INVALID:
                return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_IPV4,
                                           cid,
                                           addr1->Address.IPv4[3],
                                           addr1->Address.IPv4[2],
                                           addr1->Address.IPv4[1],
                                           addr1->Address.IPv4[0]);
            default:
                return NULL;
            }
            break;
        case AT_CGPADDR_ADDRESSTYPE_IPV6:
            switch (addr2->Type)
            {
            case AT_CGPADDR_ADDRESSTYPE_IPV4:
                return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_IPV6_IPV4,
                                           cid,
                                           addr1->Address.IPv6[15],
                                           addr1->Address.IPv6[14],
                                           addr1->Address.IPv6[13],
                                           addr1->Address.IPv6[12],
                                           addr1->Address.IPv6[11],
                                           addr1->Address.IPv6[10],
                                           addr1->Address.IPv6[9],
                                           addr1->Address.IPv6[8],
                                           addr1->Address.IPv6[7],
                                           addr1->Address.IPv6[6],
                                           addr1->Address.IPv6[5],
                                           addr1->Address.IPv6[4],
                                           addr1->Address.IPv6[3],
                                           addr1->Address.IPv6[2],
                                           addr1->Address.IPv6[1],
                                           addr1->Address.IPv6[0],

                                           addr2->Address.IPv4[3],
                                           addr2->Address.IPv4[2],
                                           addr2->Address.IPv4[1],
                                           addr2->Address.IPv4[0]);
            case AT_CGPADDR_ADDRESSTYPE_IPV6:
                return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_IPV6_IPV6,
                                           cid,
                                           addr1->Address.IPv6[15],
                                           addr1->Address.IPv6[14],
                                           addr1->Address.IPv6[13],
                                           addr1->Address.IPv6[12],
                                           addr1->Address.IPv6[11],
                                           addr1->Address.IPv6[10],
                                           addr1->Address.IPv6[9],
                                           addr1->Address.IPv6[8],
                                           addr1->Address.IPv6[7],
                                           addr1->Address.IPv6[6],
                                           addr1->Address.IPv6[5],
                                           addr1->Address.IPv6[4],
                                           addr1->Address.IPv6[3],
                                           addr1->Address.IPv6[2],
                                           addr1->Address.IPv6[1],
                                           addr1->Address.IPv6[0],

                                           addr2->Address.IPv6[15],
                                           addr2->Address.IPv6[14],
                                           addr2->Address.IPv6[13],
                                           addr2->Address.IPv6[12],
                                           addr2->Address.IPv6[11],
                                           addr2->Address.IPv6[10],
                                           addr2->Address.IPv6[9],
                                           addr2->Address.IPv6[8],
                                           addr2->Address.IPv6[7],
                                           addr2->Address.IPv6[6],
                                           addr2->Address.IPv6[5],
                                           addr2->Address.IPv6[4],
                                           addr2->Address.IPv6[3],
                                           addr2->Address.IPv6[2],
                                           addr2->Address.IPv6[1],
                                           addr2->Address.IPv6[0]);
            case AT_CGPADDR_ADDRESSTYPE_INVALID:
                return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_IPV6,
                                           cid,
                                           addr1->Address.IPv6[15],
                                           addr1->Address.IPv6[14],
                                           addr1->Address.IPv6[13],
                                           addr1->Address.IPv6[12],
                                           addr1->Address.IPv6[11],
                                           addr1->Address.IPv6[10],
                                           addr1->Address.IPv6[9],
                                           addr1->Address.IPv6[8],
                                           addr1->Address.IPv6[7],
                                           addr1->Address.IPv6[6],
                                           addr1->Address.IPv6[5],
                                           addr1->Address.IPv6[4],
                                           addr1->Address.IPv6[3],
                                           addr1->Address.IPv6[2],
                                           addr1->Address.IPv6[1],
                                           addr1->Address.IPv6[0]);
            default:
                return NULL;
            }
            break;
        case AT_CGPADDR_ADDRESSTYPE_INVALID:
            return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CGPADDR_INVALID,
                                       cid);
        default:
            return NULL;
        }
    }
};

TEST_F(TS_At_Set_CGPADDR, Addr1IPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGPADDR_Param_T param;
    param.Cid = 1;
    AT_CGPADDR_Resp_T resp;
    resp.Address1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address1.Address.IPv6, 0, sizeof(resp.Address1.Address.IPv6));
    resp.Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address2.Address.IPv6, 0, sizeof(resp.Address2.Address.IPv6));

    AT_CGPADDR_Address_T expAddr1;
    expAddr1.Type = AT_CGPADDR_ADDRESSTYPE_IPV4;
    expAddr1.Address.IPv4[3] = 127;
    expAddr1.Address.IPv4[2] = 0;
    expAddr1.Address.IPv4[1] = 0;
    expAddr1.Address.IPv4[0] = 1;
    AT_CGPADDR_Address_T expAddr2;
    expAddr2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;

    AddFakeAnswer(FormatTrigger(&param), FormatAnswer(param.Cid, &expAddr1, &expAddr2));

    retcode = At_Set_CGPADDR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expAddr1.Type, resp.Address1.Type);
    for (uint32_t i = 0; i < sizeof(resp.Address1.Address.IPv4); ++i)
    {
        EXPECT_EQ(expAddr1.Address.IPv4[i], resp.Address1.Address.IPv4[i]);
    }
    EXPECT_EQ(expAddr2.Type, resp.Address2.Type);
}

TEST_F(TS_At_Set_CGPADDR, Addr1IPv6_Addr2IPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGPADDR_Param_T param;
    param.Cid = 1;
    AT_CGPADDR_Resp_T resp;
    resp.Address1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address1.Address.IPv6, 0, sizeof(resp.Address1.Address.IPv6));
    resp.Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address2.Address.IPv6, 0, sizeof(resp.Address2.Address.IPv6));

    AT_CGPADDR_Address_T expAddr1;
    expAddr1.Type = AT_CGPADDR_ADDRESSTYPE_IPV6;
    expAddr1.Address.IPv6[15] = 0xFE;
    expAddr1.Address.IPv6[14] = 0x80;
    expAddr1.Address.IPv6[13] = 0x01;
    expAddr1.Address.IPv6[12] = 0x02;
    expAddr1.Address.IPv6[11] = 0x03;
    expAddr1.Address.IPv6[10] = 0x04;
    expAddr1.Address.IPv6[9] = 0x05;
    expAddr1.Address.IPv6[8] = 0x06;
    expAddr1.Address.IPv6[7] = 0x07;
    expAddr1.Address.IPv6[6] = 0x08;
    expAddr1.Address.IPv6[5] = 0x09;
    expAddr1.Address.IPv6[4] = 0x0A;
    expAddr1.Address.IPv6[2] = 0x0B;
    expAddr1.Address.IPv6[1] = 0x0C;
    expAddr1.Address.IPv6[0] = 0x0D;

    AT_CGPADDR_Address_T expAddr2;
    expAddr2.Type = AT_CGPADDR_ADDRESSTYPE_IPV4;
    expAddr2.Address.IPv4[3] = 127;
    expAddr2.Address.IPv4[2] = 0;
    expAddr2.Address.IPv4[1] = 0;
    expAddr2.Address.IPv4[0] = 1;

    AddFakeAnswer(FormatTrigger(&param), FormatAnswer(param.Cid, &expAddr1, &expAddr2));

    retcode = At_Set_CGPADDR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expAddr1.Type, resp.Address1.Type);
    for (uint32_t i = 0; i < sizeof(resp.Address1.Address.IPv6); ++i)
    {
        EXPECT_EQ(expAddr1.Address.IPv6[i], resp.Address1.Address.IPv6[i]);
    }
    EXPECT_EQ(expAddr2.Type, resp.Address2.Type);
    for (uint32_t i = 0; i < sizeof(resp.Address1.Address.IPv4); ++i)
    {
        EXPECT_EQ(expAddr1.Address.IPv4[i], resp.Address1.Address.IPv4[i]);
    }
}

TEST_F(TS_At_Set_CGPADDR, Addr1IPv6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGPADDR_Param_T param;
    param.Cid = 1;
    AT_CGPADDR_Resp_T resp;
    resp.Address1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address1.Address.IPv6, 0, sizeof(resp.Address1.Address.IPv6));
    resp.Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address2.Address.IPv6, 0, sizeof(resp.Address2.Address.IPv6));

    AT_CGPADDR_Address_T expAddr1;
    expAddr1.Type = AT_CGPADDR_ADDRESSTYPE_IPV6;
    expAddr1.Address.IPv6[15] = 0xFE;
    expAddr1.Address.IPv6[14] = 0x84;
    expAddr1.Address.IPv6[13] = 0x12;
    expAddr1.Address.IPv6[12] = 0x34;
    expAddr1.Address.IPv6[11] = 0x56;
    expAddr1.Address.IPv6[10] = 0x78;
    expAddr1.Address.IPv6[9] = 0x13;
    expAddr1.Address.IPv6[8] = 0x37;
    expAddr1.Address.IPv6[7] = 0xFE;
    expAddr1.Address.IPv6[6] = 0x80;
    expAddr1.Address.IPv6[5] = 0;
    expAddr1.Address.IPv6[4] = 0;
    expAddr1.Address.IPv6[3] = 0;
    expAddr1.Address.IPv6[2] = 0;
    expAddr1.Address.IPv6[1] = 0x13;
    expAddr1.Address.IPv6[0] = 0x37;
    AT_CGPADDR_Address_T expAddr2;
    expAddr2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;

    AddFakeAnswer(FormatTrigger(&param), FormatAnswer(param.Cid, &expAddr1, &expAddr2));

    retcode = At_Set_CGPADDR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expAddr1.Type, resp.Address1.Type);
    for (uint32_t i = 0; i < sizeof(resp.Address1.Address.IPv6); ++i)
    {
        EXPECT_EQ(expAddr1.Address.IPv6[i], resp.Address1.Address.IPv6[i]);
    }
    EXPECT_EQ(expAddr2.Type, resp.Address2.Type);
}

TEST_F(TS_At_Set_CGPADDR, NoAddr_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CGPADDR_Param_T param;
    param.Cid = 7;
    AT_CGPADDR_Resp_T resp;
    resp.Address1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address1.Address.IPv6, 0, sizeof(resp.Address1.Address.IPv6));
    resp.Address2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    memset(resp.Address2.Address.IPv6, 0, sizeof(resp.Address2.Address.IPv6));

    AT_CGPADDR_Address_T expAddr1;
    expAddr1.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;
    expAddr1.Address.IPv4[3] = 0;
    expAddr1.Address.IPv4[2] = 0;
    expAddr1.Address.IPv4[1] = 0;
    expAddr1.Address.IPv4[0] = 0;
    AT_CGPADDR_Address_T expAddr2;
    expAddr2.Type = AT_CGPADDR_ADDRESSTYPE_INVALID;

    AddFakeAnswer(FormatTrigger(&param), FormatAnswer(param.Cid, &expAddr1, &expAddr2));

    retcode = At_Set_CGPADDR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expAddr1.Type, resp.Address1.Type);
    for (uint32_t i = 0; i < sizeof(resp.Address1.Address.IPv4); ++i)
    {
        EXPECT_EQ(expAddr1.Address.IPv4[i], resp.Address1.Address.IPv4[i]);
    }
    EXPECT_EQ(expAddr2.Type, resp.Address2.Type);
}

class TS_ExtractCgpaddrAddress : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TS_ExtractCgpaddrAddress, IPv4_Normal_Pass)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "192.168.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(192, addr.Address.IPv4[3]);
    EXPECT_EQ(168, addr.Address.IPv4[2]);
    EXPECT_EQ(2, addr.Address.IPv4[1]);
    EXPECT_EQ(100, addr.Address.IPv4[0]);
    EXPECT_EQ(AT_CGPADDR_ADDRESSTYPE_IPV4, addr.Type);
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_LeadingZeros_Pass)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "192.168.002.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(192, addr.Address.IPv4[3]);
    EXPECT_EQ(168, addr.Address.IPv4[2]);
    EXPECT_EQ(2, addr.Address.IPv4[1]);
    EXPECT_EQ(100, addr.Address.IPv4[0]);
    EXPECT_EQ(AT_CGPADDR_ADDRESSTYPE_IPV4, addr.Type);
}

TEST_F(TS_ExtractCgpaddrAddress, IPv6_Normal_Pass)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "254.128.1.2.3.4.5.6.7.8.9.10.11.12.19.55";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(254, addr.Address.IPv6[15]);
    EXPECT_EQ(128, addr.Address.IPv6[14]);
    EXPECT_EQ(1, addr.Address.IPv6[13]);
    EXPECT_EQ(2, addr.Address.IPv6[12]);
    EXPECT_EQ(3, addr.Address.IPv6[11]);
    EXPECT_EQ(4, addr.Address.IPv6[10]);
    EXPECT_EQ(5, addr.Address.IPv6[9]);
    EXPECT_EQ(6, addr.Address.IPv6[8]);
    EXPECT_EQ(7, addr.Address.IPv6[7]);
    EXPECT_EQ(8, addr.Address.IPv6[6]);
    EXPECT_EQ(9, addr.Address.IPv6[5]);
    EXPECT_EQ(10, addr.Address.IPv6[4]);
    EXPECT_EQ(11, addr.Address.IPv6[3]);
    EXPECT_EQ(12, addr.Address.IPv6[2]);
    EXPECT_EQ(19, addr.Address.IPv6[1]);
    EXPECT_EQ(55, addr.Address.IPv6[0]);
    EXPECT_EQ(AT_CGPADDR_ADDRESSTYPE_IPV6, addr.Type);
}

TEST_F(TS_ExtractCgpaddrAddress, IPv6_LeadingZeros_Pass)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "254.128.1.02.003.4.005.6.7.8.9.10.11.12.19.55";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(254, addr.Address.IPv6[15]);
    EXPECT_EQ(128, addr.Address.IPv6[14]);
    EXPECT_EQ(1, addr.Address.IPv6[13]);
    EXPECT_EQ(2, addr.Address.IPv6[12]);
    EXPECT_EQ(3, addr.Address.IPv6[11]);
    EXPECT_EQ(4, addr.Address.IPv6[10]);
    EXPECT_EQ(5, addr.Address.IPv6[9]);
    EXPECT_EQ(6, addr.Address.IPv6[8]);
    EXPECT_EQ(7, addr.Address.IPv6[7]);
    EXPECT_EQ(8, addr.Address.IPv6[6]);
    EXPECT_EQ(9, addr.Address.IPv6[5]);
    EXPECT_EQ(10, addr.Address.IPv6[4]);
    EXPECT_EQ(11, addr.Address.IPv6[3]);
    EXPECT_EQ(12, addr.Address.IPv6[2]);
    EXPECT_EQ(19, addr.Address.IPv6[1]);
    EXPECT_EQ(55, addr.Address.IPv6[0]);
    EXPECT_EQ(AT_CGPADDR_ADDRESSTYPE_IPV6, addr.Type);
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_Quotes_Pass)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "\"192.168.2.100\"";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(192, addr.Address.IPv4[3]);
    EXPECT_EQ(168, addr.Address.IPv4[2]);
    EXPECT_EQ(2, addr.Address.IPv4[1]);
    EXPECT_EQ(100, addr.Address.IPv4[0]);
    EXPECT_EQ(AT_CGPADDR_ADDRESSTYPE_IPV4, addr.Type);
}

TEST_F(TS_ExtractCgpaddrAddress, IPv6_Quotes_Pass)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "\"254.128.1.2.3.4.5.6.7.8.9.10.11.12.19.55\"";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(254, addr.Address.IPv6[15]);
    EXPECT_EQ(128, addr.Address.IPv6[14]);
    EXPECT_EQ(1, addr.Address.IPv6[13]);
    EXPECT_EQ(2, addr.Address.IPv6[12]);
    EXPECT_EQ(3, addr.Address.IPv6[11]);
    EXPECT_EQ(4, addr.Address.IPv6[10]);
    EXPECT_EQ(5, addr.Address.IPv6[9]);
    EXPECT_EQ(6, addr.Address.IPv6[8]);
    EXPECT_EQ(7, addr.Address.IPv6[7]);
    EXPECT_EQ(8, addr.Address.IPv6[6]);
    EXPECT_EQ(9, addr.Address.IPv6[5]);
    EXPECT_EQ(10, addr.Address.IPv6[4]);
    EXPECT_EQ(11, addr.Address.IPv6[3]);
    EXPECT_EQ(12, addr.Address.IPv6[2]);
    EXPECT_EQ(19, addr.Address.IPv6[1]);
    EXPECT_EQ(55, addr.Address.IPv6[0]);
    EXPECT_EQ(AT_CGPADDR_ADDRESSTYPE_IPV6, addr.Type);
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_InvalidOctet1_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "300.168.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv6_InvalidOctet1_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "300.128.1.2.3.4.5.6.7.8.9.10.11.12.19.55";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_InvalidOctet2_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "200.1680.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv6_InvalidOctet2_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "254.128.1.2.3.4.5.6.7.8.9.10.11.12.19.1055";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_MissingOctet_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "100.168.2";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, NullBuff_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;

    retcode = ExtractCgpaddrAddress(NULL, 0, &addr);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, ZeroLen_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "200.1680.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, 0, &addr);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_LeadingZero_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "20.0016.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, BuffTooLong_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "192.168.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, AT_3GPP_27007_MAX_IP_STR_LENGTH + 1, &addr);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_InvalidChar1_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "192.+.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ExtractCgpaddrAddress, IPv4_InvalidChar2_Fail)
{
    Retcode_T retcode;
    AT_CGPADDR_Address_T addr;
    const char *addrStr = "192.a.2.100";

    retcode = ExtractCgpaddrAddress((const uint8_t *)addrStr, strlen(addrStr), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

class TS_At_Set_CPIN : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const char *pin)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCPIN_FMT,
                                   pin);
    }
};

TEST_F(TS_At_Set_CPIN, Pass)
{
    Retcode_T retcode = RETCODE_OK;
    const char *pin = "1234";

    AddFakeAnswer(FormatTrigger(pin), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CPIN(pin);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_CFUN : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_CFUN_Param_T *param)
    {
        if (AT_CFUN_RST_INVALID == param->Rst)
        {
            return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCFUN_FMT1,
                                       param->Fun);
        }
        else
        {
            return FormatIntoNewBuffer(&Trigger, CMD_3GPP_27007_SET_ATCFUN_FMT2,
                                       param->Fun, param->Rst);
        }
    }
};

TEST_F(TS_At_Set_CFUN, Minimum_InvalidReset_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Param_T param;
    param.Fun = AT_CFUN_FUN_MINIMUM;
    param.Rst = AT_CFUN_RST_INVALID;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CFUN(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CFUN, Minimum_NoReset_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Param_T param;
    param.Fun = AT_CFUN_FUN_MINIMUM;
    param.Rst = AT_CFUN_RST_NORESET;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CFUN(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CFUN, Full_NoReset_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Param_T param;
    param.Fun = AT_CFUN_FUN_FULL;
    param.Rst = AT_CFUN_RST_NORESET;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CFUN(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CFUN, Full_Reset_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Param_T param;
    param.Fun = AT_CFUN_FUN_FULL;
    param.Rst = AT_CFUN_RST_RESET;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CFUN(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_CFUN, Reserved15_Reset_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Param_T param;
    param.Fun = (AT_CFUN_Fun_T)15;
    param.Rst = AT_CFUN_RST_RESET;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_CFUN(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Get_CFUN : public TS_ModemTest
{
protected:
    const char *FormatAnswer(AT_CFUN_Fun_T fun)
    {
        return FormatIntoNewBuffer(&Answer, TEST_AT_RESP_CFUN,
                                   fun);
    }
};

TEST_F(TS_At_Get_CFUN, Minimum_Pass)
{
    AT_CFUN_Fun_T expFun = AT_CFUN_FUN_MINIMUM;
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Resp_T resp;
    resp.Fun = AT_CFUN_FUN_INVALID;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCFUN, FormatAnswer(expFun));

    retcode = At_Get_CFUN(&resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expFun, resp.Fun);
}

TEST_F(TS_At_Get_CFUN, Full_Pass)
{
    AT_CFUN_Fun_T expFun = AT_CFUN_FUN_FULL;
    Retcode_T retcode = RETCODE_OK;
    AT_CFUN_Resp_T resp;
    resp.Fun = AT_CFUN_FUN_INVALID;

    AddFakeAnswer(CMD_3GPP_27007_GET_ATCFUN, FormatAnswer(expFun));

    retcode = At_Get_CFUN(&resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expFun, resp.Fun);
}
