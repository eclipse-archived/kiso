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

FFF_DEFINITION_BLOCK_START
extern "C"
{

/* setup compile time configuration defines */
#include "Kiso_Cellular.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_AT_UBLOX
#define GTEST
/* include faked interfaces */
#include "Kiso_MCU_UART_th.hh"
#include "SocketService_th.hh"
#include "HttpService_th.hh"
#include "Kiso_Logging_th.hh"

/* include module under test */
#include "ModemEmulator.cc"
#undef KISO_MODULE_ID
#include "AtUtils.c"
#undef KISO_MODULE_ID
#include "AT_UBlox.c"

#include <stdlib.h>
#include <time.h>
}
FFF_DEFINITION_BLOCK_END

#define TEST_AT_RESPONSE_OK ("OK\r\n")
#define TEST_AT_RESPONSE_ERROR ("ERROR\r\n")
#define TEST_GET_ATMNOUPROF_RESPONSE_FMT ("+%s:%" PRIu8 "\r\n%s")
#define TEST_SET_ATUSOCR_RESPONSE_FMT ("+%s:%" PRIu32 "\r\n%s")
#define TEST_SET_ATUSOWR_RESPONSE_FMT ("+%s:%" PRIu32 ",%" PRIu32 "\r\n%s")
#define TEST_SET_ATUSOST_RESPONSE_FMT ("+%s:%" PRIu32 ",%" PRIu32 "\r\n%s")
#define TEST_SET_ATUSORD_RESPONSE_FMTDATA ("+%s:%" PRIu32 ",%" PRIu32 ",\"%.*s\"\r\n%s")
#define TEST_SET_ATUSORD_RESPONSE_FMTNODATA ("+%s:%" PRIu32 ",%" PRIu32 "\r\n%s")
#define TEST_SET_ATUSORD_RESPONSE_FMTNOLEN ("+%s:%" PRIu32 ",\"\"\r\n%s")
#define TEST_SET_ATUSORF_RESPONSE_FMTIPV4 ("+%s:%" PRIu32 ",\"%d.%d.%d.%d\",%d,%" PRIu32 ",\"%.*s\"\r\n%s")
#define TEST_SET_ATUSORF_RESPONSE_FMTIPV6 ("+%s:%" PRIu32 ",\"%x:%x:%x:%x:%x:%x:%x:%x\",%d,%" PRIu32 ",\"%.*s\"\r\n%s")
#define TEST_SET_ATUSORF_RESPONSE_FMTNODATA ("+%s:%" PRIu32 ",%" PRIu32 "\r\n%s")
#define TEST_SET_ATUSORF_RESPONSE_FMTNOLEN ("+%s:%" PRIu32 ",\"\"\r\n%s")
#define TEST_GET_ATUDCONF_RESPONSE_FMTHEXMODE ("+%s:%" PRIu32 ",%" PRIu32 "\r\n%s")
#define TEST_GET_ATCCID_RESPONSE_FMT ("+%s:%s\r\n%s")
#define TEST_SET_ATUDNS_RESPONSE_FMT1 ("+%s:\"%d.%d.%d.%d\"\r\n%s")
#define TEST_SET_ATUDNS_RESPONSE_FMT2 ("+%s:\"%x:%x:%x:%x:%x:%x:%x:%x\"\r\n%s")
#define TEST_SET_ATUDNS_RESPONSE_FMT3 ("+%s:\"%s\"\r\n%s")
#define TEST_SET_ATUDWNFILE_RESPONSE_FMT (">%.*s")
#define TEST_SET_ATULSTFILE_RESPONSE_FMT1 ("+%s:\"%s\"\r\n%s")
#define TEST_SET_ATULSTFILE_RESPONSE_FMT2 ("+%s:%d\r\n%s")
#define TEST_SET_ATURDBLOCK_RESPONSE_FMT ("+%s:\"%s\",%d,\"%.*s\"\r\n%s")
#define TEST_URC_ATUSOLI_FMTIPV4IPV4 ("+%s:%" PRIu32 ",\"%d.%d.%d.%d\",%" PRIu32 ",%d,\"%d.%d.%d.%d\",%d\r\n")
#define TEST_URC_ATUSOLI_FMTIPV6IPV4 ("+%s:%" PRIu32 ",\"%x:%x:%x:%x:%x:%x:%x:%x\",%" PRIu32 ",%d,\"%d.%d.%d.%d\",%d\r\n")
#define TEST_URC_ATUSOLI_FMTIPV4IPV6 ("+%s:%" PRIu32 ",\"%d.%d.%d.%d\",%" PRIu32 ",%d,\"%x:%x:%x:%x:%x:%x:%x:%x\",%d\r\n")
#define TEST_URC_ATUSOLI_FMTIPV6IPV6 ("+%s:%" PRIu32 ",\"%x:%x:%x:%x:%x:%x:%x:%x\",%" PRIu32 ",%d,\"%x:%x:%x:%x:%x:%x:%x:%x\",%d\r\n")
#define TEST_URC_ATUSOCL_FMT ("+%s:%" PRIu32 "\r\n")
#define TEST_URC_ATUSORX_FMT ("+%s:%" PRIu32 ",%" PRIu32 "\r\n")
class TS_At_Set_URAT : public TS_ModemTest
{
protected:
    const char *FormatTrigger1(const AT_URAT_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATURAT1_FMT,
                                   (uint8_t)param->SelectedAcT);
    }

    const char *FormatTrigger2(const AT_URAT_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATURAT2_FMT,
                                   (uint8_t)param->SelectedAcT, param->PreferredAcT);
    }

    const char *FormatTrigger3(const AT_URAT_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATURAT3_FMT,
                                   (uint8_t)param->SelectedAcT, param->PreferredAcT, param->SecondPreferredAcT);
    }
};

TEST_F(TS_At_Set_URAT, GprsPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_GSM_GPRS_EGPRS;
    param.PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;

    AddFakeAnswer(FormatTrigger1(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_URAT, UmtsPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_UMTS;
    param.PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;

    AddFakeAnswer(FormatTrigger1(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_URAT, LtePass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_LTE;
    param.PreferredAcT = AT_URAT_PREFERREDACT_INVALID;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;

    AddFakeAnswer(FormatTrigger1(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_URAT, GprsUmtsPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_GSM_UMTS;
    param.PreferredAcT = AT_URAT_PREFERREDACT_UTRAN;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;

    AddFakeAnswer(FormatTrigger2(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_URAT, GprsLtePass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_GSM_LTE;
    param.PreferredAcT = AT_URAT_PREFERREDACT_LTE;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;

    AddFakeAnswer(FormatTrigger2(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_URAT, UmtsLtePass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_UMTS_LTE;
    param.PreferredAcT = AT_URAT_PREFERREDACT_LTE;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_INVALID;

    AddFakeAnswer(FormatTrigger2(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_URAT, Fail_invalidACT)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_INVALID;
    param.PreferredAcT = AT_URAT_PREFERREDACT_LTE;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_UTRAN;

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_At_Set_URAT, GprsUmtsLtePass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_URAT_Param_T param;
    param.SelectedAcT = AT_URAT_SELECTEDACT_GSM_UMTS_LTE;
    param.PreferredAcT = AT_URAT_PREFERREDACT_LTE;
    param.SecondPreferredAcT = AT_URAT_SECONDPREFERREDACT_UTRAN;

    AddFakeAnswer(FormatTrigger3(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_URAT(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_UMNOPROF : public TS_ModemTest
{
protected:
    const char *FormatTrigger(AT_UMNOPROF_Mno_T mno)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUMNOPROF_FMT,
                                   (int)mno);
    }
};

TEST_F(TS_At_Set_UMNOPROF, DefaultPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UMNOPROF_Mno_T mno = AT_UMNOPROF_MNO_DEFAULT;

    AddFakeAnswer(FormatTrigger(mno), TEST_AT_RESPONSE_OK);

    retcode = At_Set_UMNOPROF(mno);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_UMNOPROF, SimPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UMNOPROF_Mno_T mno = AT_UMNOPROF_MNO_SIM;

    AddFakeAnswer(FormatTrigger(mno), TEST_AT_RESPONSE_OK);

    retcode = At_Set_UMNOPROF(mno);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Get_UMNOPROF : public TS_ModemTest
{
protected:
    const char *FormatAnswer(AT_UMNOPROF_Mno_T mno)
    {
        return FormatIntoNewBuffer(&Answer, TEST_GET_ATMNOUPROF_RESPONSE_FMT,
                                   CMD_UBLOX_ATUMNOPROF, (uint8_t)mno, TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Get_UMNOPROF, DefaultMnoPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UMNOPROF_Mno_T mno = AT_UMNOPROF_MNO_INVALID;
    AT_UMNOPROF_Mno_T expectedMno = AT_UMNOPROF_MNO_DEFAULT;

    AddFakeAnswer(CMD_UBLOX_GET_ATUMNOPROF, FormatAnswer(expectedMno));

    retcode = At_Get_UMNOPROF(&mno);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expectedMno, mno);
}

TEST_F(TS_At_Get_UMNOPROF, SimMnoPass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UMNOPROF_Mno_T mno = AT_UMNOPROF_MNO_INVALID;
    AT_UMNOPROF_Mno_T expectedMno = AT_UMNOPROF_MNO_SIM;

    AddFakeAnswer(CMD_UBLOX_GET_ATUMNOPROF, FormatAnswer(expectedMno));

    retcode = At_Get_UMNOPROF(&mno);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expectedMno, mno);
}

TEST_F(TS_At_Get_UMNOPROF, InvalidMnoFail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UMNOPROF_Mno_T mno = AT_UMNOPROF_MNO_INVALID;
    AT_UMNOPROF_Mno_T expectedMno = AT_UMNOPROF_MNO_INVALID;

    AddFakeAnswer(CMD_UBLOX_GET_ATUMNOPROF, FormatAnswer(expectedMno));

    retcode = At_Get_UMNOPROF(&mno);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

// TEST_F(TS_At_Get_UMNOPROF, FailFunction)
// {
//     Retcode_T retcode;
//     AT_UMNOPROF_Mno_T mno = AT_UMNOPROF_MNO_INVALID;

//     retcode = At_Get_UMNOPROF(&mno);

//     EXPECT_NE(RETCODE_OK, retcode);
// }

class TS_At_Set_UBANDMASK : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_UBANDMASK_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUBANDMASK_FMT,
                                   param->Rat, param->Bitmask1, param->Bitmask2);
    }
};

TEST_F(TS_At_Set_UBANDMASK, NBIoT_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBANDMASK_Param_T param;
    param.Rat = AT_UBANDMASK_RAT_LTECATNB1;
    param.Bitmask1 = 0x80000; // activate band 20 (Vodafone)
    param.Bitmask2 = 0;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_UBANDMASK(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_USOCR : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_USOCR_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOCR_FMT,
                                   (int)param->Protocol, param->LocalPort);
    }

    const char *FormatAnswer(uint32_t socketId)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSOCR_RESPONSE_FMT,
                                   CMD_UBLOX_ATUSOCR, socketId, TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_USOCR, Tcp_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOCR_Param_T param;
    param.Protocol = AT_USOCR_PROTOCOL_TCP;
    param.LocalPort = 123;
    uint32_t expSocket = 4;
    uint32_t socket = 0;

    AddFakeAnswer(FormatTrigger(&param), FormatAnswer(expSocket));

    retcode = At_Set_USOCR(&param, &socket);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expSocket, socket);
}

TEST_F(TS_At_Set_USOCR, Udp_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOCR_Param_T param;
    param.Protocol = AT_USOCR_PROTOCOL_UDP;
    param.LocalPort = 123;
    uint32_t expSocket = 4;
    uint32_t socket = 0;

    AddFakeAnswer(FormatTrigger(&param), FormatAnswer(expSocket));

    retcode = At_Set_USOCR(&param, &socket);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expSocket, socket);
}

class TS_At_Set_USOCO : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_USOCO_Param_T *param)
    {
        switch (param->RemoteAddr.Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOCO_FMTIPV4,
                                       (int)param->Socket,
                                       (int)param->RemoteAddr.Address.IPv4[3],
                                       (int)param->RemoteAddr.Address.IPv4[2],
                                       (int)param->RemoteAddr.Address.IPv4[1],
                                       (int)param->RemoteAddr.Address.IPv4[0],
                                       (int)param->RemotePort);
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOCO_FMTIPV6,
                                       (int)param->Socket,
                                       (int)param->RemoteAddr.Address.IPv6[7],
                                       (int)param->RemoteAddr.Address.IPv6[6],
                                       (int)param->RemoteAddr.Address.IPv6[5],
                                       (int)param->RemoteAddr.Address.IPv6[4],
                                       (int)param->RemoteAddr.Address.IPv6[3],
                                       (int)param->RemoteAddr.Address.IPv6[2],
                                       (int)param->RemoteAddr.Address.IPv6[1],
                                       (int)param->RemoteAddr.Address.IPv6[0],
                                       (int)param->RemotePort);
        default:
            return NULL;
        }
    }
};

TEST_F(TS_At_Set_USOCO, IPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOCO_Param_T param;
    param.Socket = 4;
    param.RemoteAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    param.RemoteAddr.Address.IPv4[3] = 10;
    param.RemoteAddr.Address.IPv4[2] = 123;
    param.RemoteAddr.Address.IPv4[1] = 2;
    param.RemoteAddr.Address.IPv4[0] = 1;
    param.RemotePort = 1337;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_USOCO(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_USOCO, IPv6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOCO_Param_T param;
    param.Socket = 4;
    param.RemoteAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    param.RemoteAddr.Address.IPv6[7] = 0xFE80;
    param.RemoteAddr.Address.IPv6[6] = 0x1234;
    param.RemoteAddr.Address.IPv6[5] = 0x1010;
    param.RemoteAddr.Address.IPv6[4] = 0x1020;
    param.RemoteAddr.Address.IPv6[3] = 0x1030;
    param.RemoteAddr.Address.IPv6[2] = 0x1040;
    param.RemoteAddr.Address.IPv6[1] = 0x1050;
    param.RemoteAddr.Address.IPv6[0] = 0x1060;
    param.RemotePort = 1337;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_USOCO(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_USOCO, Fail_InvalidIPType)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOCO_Param_T param;
    param.RemoteAddr.Type = AT_UBLOX_ADDRESSTYPE_INVALID;

    retcode = At_Set_USOCO(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

class TS_HexToBin : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }
};

TEST_F(TS_HexToBin, LowerCase_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = '1';
    data[1] = 'a';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0x1A, bin[0]);
}

TEST_F(TS_HexToBin, UpperCase_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = '1';
    data[1] = 'A';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0x1A, bin[0]);
}

TEST_F(TS_HexToBin, InvalidChar1_Fail)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = '1';
    data[1] = '.';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_HexToBin, InvalidChar2_Fail)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = '1';
    data[1] = ':';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_HexToBin, InvalidChar3_Fail)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = '1';
    data[1] = 'G';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_HexToBin, InvalidChar4_Fail)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = '1';
    data[1] = 'g';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_HexToBin, InvalidChar5_Fail)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[2];
    data[0] = 'z';
    data[1] = '1';
    uint8_t bin[1];
    uint32_t binLength = sizeof(bin);

    retcode = HexToBin((const uint8_t *)data, bin, binLength);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

class TS_PrepareSendingWithBaseEncoding : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }
};

TEST_F(TS_PrepareSendingWithBaseEncoding, Pass)
{
    uint32_t socket = 1U;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    ss << "AT+USOWR=" << socket << "," << payload.length() << ","
       << "\"" << payload << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length() + 1];
    AT_USOWR_Param_T usowr;
    usowr.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usowr.Length = payload.length();
    usowr.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usowr.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendingWithBaseEncoding(sendBuffer, sizeof(sendBuffer), &usowr, &actLen);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(expSendBuffer.length(), actLen);
    EXPECT_STREQ(expSendBuffer.c_str(), sendBuffer);
}

TEST_F(TS_PrepareSendingWithBaseEncoding, InsufficientBufferLen_Fail)
{
    uint32_t socket = 1U;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    ss << "AT+USOWR=" << socket << "," << payload.length() << ","
       << "\"" << payload << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length()];
    AT_USOWR_Param_T usowr;
    usowr.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usowr.Length = payload.length();
    usowr.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usowr.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendingWithBaseEncoding(sendBuffer, sizeof(sendBuffer), &usowr, &actLen);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), rc);
    EXPECT_EQ(0U, actLen);
}

class TS_PrepareSendToWithBaseEncoding : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }

    void SetIPv4(AT_UBlox_Address_T &address, const uint8_t bytes[4])
    {
        address.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
        address.Address.IPv4[0] = bytes[3];
        address.Address.IPv4[1] = bytes[2];
        address.Address.IPv4[2] = bytes[1];
        address.Address.IPv4[3] = bytes[0];
    }

    std::string IPToString(const AT_UBlox_Address_T &address)
    {
        std::stringstream ss;
        ss << '"';
        switch (address.Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            for (ssize_t i = 3; i >= 0; --i)
            {
                ss << std::dec << (uint32_t)address.Address.IPv4[i];
                if (i > 0)
                    ss << '.';
            }
            break;
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            for (ssize_t i = 7; i >= 0; --i)
            {
                ss << std::hex << (uint32_t)address.Address.IPv6[i];
                if (i > 0)
                    ss << ':';
            }
            break;
        default:
            std::cerr << "Unexpected address type" << std::endl;
            exit(1);
        }
        ss << '"';
        return ss.str();
    }
};

TEST_F(TS_PrepareSendToWithBaseEncoding, Pass)
{
    uint32_t socket = 1U;
    uint8_t expIp[] = {10, 42, 0, 1};
    uint16_t expPort = 1337;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    AT_USOST_Param_T usost;
    usost.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usost.Length = payload.length();
    usost.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usost.RemotePort = expPort;
    this->SetIPv4(usost.RemoteIp, expIp);
    ss << "AT+USOST=" << socket << "," << IPToString(usost.RemoteIp) << "," << expPort << "," << payload.length() << ","
       << "\"" << payload << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length() + 1];
    usost.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendToWithBaseEncoding(sendBuffer, sizeof(sendBuffer), &usost, &actLen);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(expSendBuffer.length(), actLen);
    EXPECT_STREQ(expSendBuffer.c_str(), sendBuffer);
}

TEST_F(TS_PrepareSendToWithBaseEncoding, InsufficientBufferLen_Fail)
{
    uint32_t socket = 1U;
    uint8_t expIp[] = {10, 42, 0, 1};
    uint16_t expPort = 1337;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    AT_USOST_Param_T usost;
    usost.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usost.Length = payload.length();
    usost.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usost.RemotePort = expPort;
    this->SetIPv4(usost.RemoteIp, expIp);
    ss << "AT+USOST=" << socket << "," << IPToString(usost.RemoteIp) << "," << expPort << "," << payload.length() << ","
       << "\"" << payload << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length()];
    usost.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendToWithBaseEncoding(sendBuffer, sizeof(sendBuffer), &usost, &actLen);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), rc);
}

TEST_F(TS_PrepareSendToWithBaseEncoding, InvalidIpType_Fail)
{
    uint32_t socket = 1U;
    uint16_t expPort = 1337;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    AT_USOST_Param_T usost;
    usost.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usost.Length = payload.length();
    usost.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usost.RemotePort = expPort;
    usost.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    ss << "AT+USOST=" << socket << ",\"10.42.0.1\"," << expPort << "," << payload.length() << ","
       << "\"" << payload << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length() + 1];
    usost.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendToWithBaseEncoding(sendBuffer, sizeof(sendBuffer), &usost, &actLen);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

class TS_PrepareSendingWithHexEncoding : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }

    std::string ToHexStr(const std::string &s)
    {
        std::stringstream ss;
        for (const char &c : s)
            ss << std::hex << std::uppercase << (uint32_t)c;
        return ss.str();
    }
};

TEST_F(TS_PrepareSendingWithHexEncoding, Pass)
{
    uint32_t socket = 1U;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    ss << "AT+USOWR=" << socket << "," << payload.length() << ","
       << "\"" << ToHexStr(payload) << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length() + 1];
    AT_USOWR_Param_T usowr;
    usowr.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usowr.Length = payload.length();
    usowr.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usowr.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendingWithHexEncoding(sendBuffer, sizeof(sendBuffer), &usowr, &actLen);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(expSendBuffer.length(), actLen);
    sendBuffer[sizeof(sendBuffer) - 1] = '\0';
    EXPECT_STREQ(expSendBuffer.c_str(), sendBuffer);
}

TEST_F(TS_PrepareSendingWithHexEncoding, InsufficientBufferLen_Fail)
{
    uint32_t socket = 1U;
    std::string payload = "HELLO WORLD";
    std::stringstream ss;
    ss << "AT+USOWR=" << socket << "," << payload.length() << ","
       << "\"" << payload << "\""
       << "\r\n";
    std::string expSendBuffer = ss.str();
    char sendBuffer[expSendBuffer.length()];
    AT_USOWR_Param_T usowr;
    usowr.Data = reinterpret_cast<const uint8_t *>(payload.c_str());
    usowr.Length = payload.length();
    usowr.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    usowr.Socket = socket;
    uint32_t actLen = 0;

    Retcode_T rc = PrepareSendingWithHexEncoding(sendBuffer, sizeof(sendBuffer), &usowr, &actLen);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), rc);
    EXPECT_EQ(0U, actLen);
}

class TS_EncodePayloadAsHex : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }
};

TEST_F(TS_EncodePayloadAsHex, Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t binData[] = {0x01, 0x02, 0x10, 0x11};
    char hexData[sizeof(binData) * 2];

    retcode = EncodePayloadAsHex(binData, sizeof(binData), hexData, sizeof(hexData));

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp("01021011", hexData, sizeof(hexData)));
}

TEST_F(TS_EncodePayloadAsHex, HexTooSmall_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    uint8_t binData[] = {0x01, 0x02, 0x10, 0x11};
    char hexData[sizeof(binData) * 2 - 1];

    retcode = EncodePayloadAsHex(binData, sizeof(binData), hexData, sizeof(hexData));

    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(retcode));
}

class TS_At_Set_USOWR : public TS_ModemTest
{
protected:
    const char *FormatTriggerBaseEnc(const AT_USOWR_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOWR_FMTBASE,
                                   (int)param->Socket,
                                   (int)param->Length,
                                   (int)param->Length,
                                   param->Data);
    }

    const char *FormatTriggerHexEnc(const AT_USOWR_Param_T *param)
    {
        char hex[param->Length * 2];
        Retcode_T ret = EncodePayloadAsHex(param->Data, param->Length, hex, sizeof(hex));

        if (RETCODE_OK != ret)
            exit(1);

        int n = 0;
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOWR_FMTHEX,
                                   (int)param->Socket,
                                   (int)param->Length,
                                   &n,
                                   (int)sizeof(hex),
                                   hex);
    }

    const char *FormatAnswer(uint32_t socketId, uint32_t bytesSent)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSOWR_RESPONSE_FMT,
                                   CMD_UBLOX_ATUSOWR, socketId, bytesSent, TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_USOWR, BaseEncoding_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOWR_Param_T param;
    param.Socket = 5;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    param.Data = (const uint8_t *)"HELLO WORLD";
    param.Length = strlen((const char *)param.Data);

    AT_USOWR_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;

    AddFakeAnswer(FormatTriggerBaseEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOWR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_USOWR, BaseEncoding_InvalidPayload_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOWR_Param_T param;
    param.Socket = 5;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    param.Data = (const uint8_t *)"HELLO  \" WORLD";
    param.Length = strlen((const char *)param.Data);

    AT_USOWR_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;

    retcode = At_Set_USOWR(&param, &resp);

    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
}

TEST_F(TS_At_Set_USOWR, HexEncoding_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOWR_Param_T param;
    param.Socket = 5;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    param.Data = (const uint8_t *)"HELLO WORLD";
    param.Length = strlen((const char *)param.Data);

    AT_USOWR_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOWR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
}

TEST_F(TS_At_Set_USOWR, HexEncoding_BigPacket_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[CELLULAR_AT_SEND_BUFFER_SIZE / 3]; /* 1/3 the AT send buffer size */
    memset(data, 'A', sizeof(data));

    AT_USOWR_Param_T param;
    param.Socket = 5;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    param.Data = data;
    param.Length = sizeof(data);

    AT_USOWR_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOWR(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
}

TEST_F(TS_At_Set_USOWR, InvalidPayload)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[CELLULAR_AT_SEND_BUFFER_SIZE / 3]; /* 1/3 the AT send buffer size */
    memset(data, 'A', sizeof(data));

    AT_USOWR_Param_T param;
    param.Socket = 5;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_INVALID;
    param.Data = data;
    param.Length = sizeof(data);

    AT_USOWR_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;

    retcode = At_Set_USOWR(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_At_Set_USOWR, NotSupportedPayload)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t data[CELLULAR_AT_SEND_BUFFER_SIZE / 3]; /* 1/3 the AT send buffer size */
    memset(data, 'A', sizeof(data));

    AT_USOWR_Param_T param;
    param.Socket = 5;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BINARY;
    param.Data = data;
    param.Length = sizeof(data);

    AT_USOWR_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;

    retcode = At_Set_USOWR(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

class TS_At_Set_USORD : public TS_ModemTest
{
protected:
    const char *FormatTriggerHexEnc(const AT_USORD_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSORD_FMTHEX,
                                   param->Socket, param->Length);
    }

    const char *FormatAnswerWithData(uint32_t socket, uint32_t length, const uint8_t *data)
    {
        char hex[length * 2];

        Retcode_T ret = EncodePayloadAsHex(data, length, hex, sizeof(hex));

        if (RETCODE_OK != ret)
            exit(1);

        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORD_RESPONSE_FMTDATA,
                                   CMD_UBLOX_ATUSORD, socket, length, sizeof(hex), hex,
                                   TEST_AT_RESPONSE_OK);
    }

    const char *FormatAnswerWithoutData(uint32_t socket, uint32_t length)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORD_RESPONSE_FMTNODATA,
                                   CMD_UBLOX_ATUSORD, socket, length,
                                   TEST_AT_RESPONSE_OK);
    }

    const char *FormatAnswerWithoutLength(uint32_t socket)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORD_RESPONSE_FMTNOLEN,
                                   CMD_UBLOX_ATUSORD, socket,
                                   TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_USORD, Data_HexEncoding_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "HELLO";

    AT_USORD_Param_T param;
    param.Socket = 3;
    param.Length = strlen(expData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    uint8_t respDataBuffer[strlen(expData) * 2];
    AT_USORD_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;
    resp.Data = respDataBuffer;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswerWithData(param.Socket, param.Length, (const uint8_t *)expData));

    retcode = At_Set_USORD(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
    EXPECT_EQ(0, memcmp(expData, resp.Data, strlen(expData)));
}

TEST_F(TS_At_Set_USORD, NoData_NoLength_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "HELLO";
    uint32_t expLength = strlen(expData);

    AT_USORD_Param_T param;
    param.Socket = 3;
    param.Length = 0;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    AT_USORD_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;
    resp.Data = NULL;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswerWithoutData(param.Socket, expLength));

    retcode = At_Set_USORD(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(expLength, resp.Length);
}

TEST_F(TS_At_Set_USORD, NoData_SetLength_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "";
    uint32_t expLength = strlen(expData);
    uint8_t respData[10]; /* some non-zero length buffer */

    AT_USORD_Param_T param;
    param.Socket = 3;
    param.Length = sizeof(respData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    AT_USORD_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 100;
    resp.Data = respData;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswerWithoutLength(param.Socket));

    retcode = At_Set_USORD(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(expLength, resp.Length);
}

TEST_F(TS_At_Set_USORD, InvalidEncoding)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t respData[10]; /* some non-zero length buffer */

    AT_USORD_Param_T param;
    param.Socket = 3;
    param.Length = sizeof(respData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_INVALID;

    AT_USORD_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 100;
    resp.Data = respData;

    retcode = At_Set_USORD(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_At_Set_USORD, UnsupportedEncoding)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t respData[10]; /* some non-zero length buffer */

    AT_USORD_Param_T param;
    param.Socket = 3;
    param.Length = sizeof(respData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BINARY;

    AT_USORD_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 100;
    resp.Data = respData;

    retcode = At_Set_USORD(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

class TS_At_Set_USORF : public TS_ModemTest
{
protected:
    const char *FormatTriggerHexEnc(const AT_USORF_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSORF_FMTHEX,
                                   param->Socket, param->Length);
    }

    const char *FormatAnswer(uint32_t socket, const AT_UBlox_Address_T *address, uint16_t port, uint32_t length, const uint8_t *data)
    {
        char hex[length * 2];

        Retcode_T ret = EncodePayloadAsHex(data, length, hex, sizeof(hex));

        if (RETCODE_OK != ret)
            exit(1);

        switch (address->Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORF_RESPONSE_FMTIPV4,
                                       CMD_UBLOX_ATUSORF,
                                       socket,
                                       address->Address.IPv4[3],
                                       address->Address.IPv4[2],
                                       address->Address.IPv4[1],
                                       address->Address.IPv4[0],
                                       port,
                                       length, sizeof(hex), hex,
                                       TEST_AT_RESPONSE_OK);
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORF_RESPONSE_FMTIPV6,
                                       CMD_UBLOX_ATUSORF,
                                       socket,
                                       address->Address.IPv6[7],
                                       address->Address.IPv6[6],
                                       address->Address.IPv6[5],
                                       address->Address.IPv6[4],
                                       address->Address.IPv6[3],
                                       address->Address.IPv6[2],
                                       address->Address.IPv6[1],
                                       address->Address.IPv6[0],
                                       port,
                                       length, sizeof(hex), hex,
                                       TEST_AT_RESPONSE_OK);
        default:
            exit(1);
            return NULL;
        }
    }

    const char *FormatAnswer(uint32_t socket, uint32_t length)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORF_RESPONSE_FMTNODATA,
                                   CMD_UBLOX_ATUSORF, socket, length,
                                   TEST_AT_RESPONSE_OK);
    }

    const char *FormatAnswer(uint32_t socket)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSORF_RESPONSE_FMTNOLEN,
                                   CMD_UBLOX_ATUSORF, socket,
                                   TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_USORF, DataIPv4_HexEncoding_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "HELLO";
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    expAddr.Address.IPv4[3] = 10;
    expAddr.Address.IPv4[2] = 9;
    expAddr.Address.IPv4[1] = 8;
    expAddr.Address.IPv4[0] = 1;
    uint16_t expPort = 1337;

    AT_USORF_Param_T param;
    param.Socket = 3;
    param.Length = strlen(expData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    uint8_t respDataBuffer[strlen(expData) * 2];
    AT_USORF_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;
    resp.Data = respDataBuffer;
    resp.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    memset(resp.RemoteIp.Address.IPv6, 0, sizeof(resp.RemoteIp.Address.IPv6));
    resp.RemotePort = 0;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, &expAddr, expPort, param.Length, (const uint8_t *)expData));

    retcode = At_Set_USORF(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
    EXPECT_EQ(0, memcmp(expData, resp.Data, strlen(expData)));
    EXPECT_EQ(expAddr.Type, resp.RemoteIp.Type);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv4, resp.RemoteIp.Address.IPv4, sizeof(expAddr.Address.IPv4)));
    EXPECT_EQ(expPort, resp.RemotePort);
}

TEST_F(TS_At_Set_USORF, DataIPv6_HexEncoding_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "HELLO";
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    expAddr.Address.IPv6[7] = 0xfe80;
    expAddr.Address.IPv6[6] = 0x1234;
    expAddr.Address.IPv6[5] = 0x1234;
    expAddr.Address.IPv6[4] = 0x1234;
    expAddr.Address.IPv6[3] = 0x1234;
    expAddr.Address.IPv6[2] = 0x1234;
    expAddr.Address.IPv6[1] = 0x1234;
    expAddr.Address.IPv6[0] = 0x4321;
    uint16_t expPort = 1337;

    AT_USORF_Param_T param;
    param.Socket = 3;
    param.Length = strlen(expData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    uint8_t respDataBuffer[strlen(expData) * 2];
    AT_USORF_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;
    resp.Data = respDataBuffer;
    resp.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    memset(resp.RemoteIp.Address.IPv6, 0, sizeof(resp.RemoteIp.Address.IPv6));
    resp.RemotePort = 0;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, &expAddr, expPort, param.Length, (const uint8_t *)expData));

    retcode = At_Set_USORF(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
    EXPECT_EQ(0, memcmp(expData, resp.Data, strlen(expData)));
    EXPECT_EQ(expAddr.Type, resp.RemoteIp.Type);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, resp.RemoteIp.Address.IPv6, sizeof(expAddr.Address.IPv6)));
    EXPECT_EQ(expPort, resp.RemotePort);
}

TEST_F(TS_At_Set_USORF, NoData_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "HELLO";
    uint32_t expLength = strlen(expData);

    AT_USORF_Param_T param;
    param.Socket = 3;
    param.Length = 0;
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    AT_USORF_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 0;
    resp.Data = NULL;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, expLength));

    retcode = At_Set_USORF(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(expLength, resp.Length);
}

TEST_F(TS_At_Set_USORF, InvalidEncoding)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t respData[10]; /* some non-zero length buffer */

    AT_USORF_Param_T param;
    param.Socket = 3;
    param.Length = sizeof(respData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_INVALID;

    AT_USORF_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 100;
    resp.Data = respData;

    retcode = At_Set_USORF(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_At_Set_USORF, NotSupportedEncoding)
{
    Retcode_T retcode = RETCODE_OK;

    uint8_t respData[10]; /* some non-zero length buffer */

    AT_USORF_Param_T param;
    param.Socket = 3;
    param.Length = sizeof(respData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BINARY;

    AT_USORF_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 100;
    resp.Data = respData;

    retcode = At_Set_USORF(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

TEST_F(TS_At_Set_USORF, NoData_SetLength_Pass)
{
    Retcode_T retcode = RETCODE_OK;

    const char *expData = "";
    uint32_t expLength = strlen(expData);
    uint8_t respData[10]; /* some non-zero length buffer */

    AT_USORF_Param_T param;
    param.Socket = 3;
    param.Length = sizeof(respData);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;

    AT_USORF_Resp_T resp;
    resp.Socket = 0;
    resp.Length = 100;
    resp.Data = respData;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket));

    retcode = At_Set_USORF(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(expLength, resp.Length);
}

class TS_ParseIPv6LeftToRight : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        srand(time(NULL));
    }
};

TEST_F(TS_ParseIPv6LeftToRight, Full_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, OneSkippedMiddle_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[3] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 "::%" PRIx16 ":%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, TwoSkippedMiddle_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 "::%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, FourSkippedMiddle_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[5] = 0;
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 "::%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, SixSkippedMiddle_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[6] = 0;
    expAddr.Address.IPv6[5] = 0;
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 "::%" PRIx16,
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, OneSkippedEnd_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[0] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 "::",
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, FiveSkippedEnd_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    expAddr.Address.IPv6[0] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 ":%" PRIx16 "::",
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, SevenSkippedEnd_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[6] = 0;
    expAddr.Address.IPv6[5] = 0;
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    expAddr.Address.IPv6[0] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 "::",
                       expAddr.Address.IPv6[7]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, OneSkippedStart_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[7] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "::%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, ThreeSkippedStart_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[7] = 0;
    expAddr.Address.IPv6[6] = 0;
    expAddr.Address.IPv6[5] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "::%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, SevenSkippedStart_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[7] = 0;
    expAddr.Address.IPv6[6] = 0;
    expAddr.Address.IPv6[5] = 0;
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "::%" PRIx16,
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, AllZero_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)0;
    }
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16,
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, Full_Quotes_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 "\"",
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, OneSkippedMiddle_Quotes_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[3] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 "::%" PRIx16 ":%" PRIx16 ":%" PRIx16 "\"",
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, SevenSkippedEnd_Quotes_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[6] = 0;
    expAddr.Address.IPv6[5] = 0;
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    expAddr.Address.IPv6[0] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"%" PRIx16 "::\"",
                       expAddr.Address.IPv6[7]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, SevenSkippedStart_Quotes_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    expAddr.Address.IPv6[7] = 0;
    expAddr.Address.IPv6[6] = 0;
    expAddr.Address.IPv6[5] = 0;
    expAddr.Address.IPv6[4] = 0;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"::%" PRIx16 "\"",
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, AllZero_Quotes_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T expAddr;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv6[i] = (uint16_t)0;
    }
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 ":%" PRIx16 "\"",
                       expAddr.Address.IPv6[7],
                       expAddr.Address.IPv6[6],
                       expAddr.Address.IPv6[5],
                       expAddr.Address.IPv6[4],
                       expAddr.Address.IPv6[3],
                       expAddr.Address.IPv6[2],
                       expAddr.Address.IPv6[1],
                       expAddr.Address.IPv6[0]);
    printf("%.*s\n", len, ipToParse);
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)len, &addr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, addr.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_ParseIPv6LeftToRight, GroupTooBig1_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "0123:4567:89AB:CDEF:F0123:4567:89AB:CDEF";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, GroupTooBig2_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "0123F:4567:89AB:CDEF:0123:4567:89AB:CDEF";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, InvalidChar_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "0123:4567:89AB:CDEG:0123:4567:89AB:CDEF";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, GroupTooBig1_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB:CDEF:F0123:4567:89AB:CDEF\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, GroupTooBig2_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123F:4567:89AB:CDEF:0123:4567:89AB:CDEF\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, GroupTooBig3_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB:CDEF::0123F\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, InvalidChar_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB:CDEG:0123:4567:89AB:CDEF\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManySkips1_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB::0123::89AB:CDEF\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManySkips2_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB::::89AB:CDEF\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManySkips3_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB::0123:89AB:CDEF::\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManySkips4_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"::0123:4567:89AB:0123:89AB:CDEF::\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManyGroups1_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB:CDEF:0123:4567:89AB:CDEF:0123\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManyGroups2_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB:CDEF:0123::4567:89AB:CDEF:0123\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManyGroups3_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"::0123:4567:89AB:CDEF:0123:4567:89AB:CDEF:0123\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv6LeftToRight, TooManyGroups4_Quotes_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    const char *ipToParse = "\"0123:4567:89AB:CDEF:0123:4567:89AB:CDEF:0123::\"";
    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    memset(addr.Address.IPv6, 0, sizeof(addr.Address.IPv6));

    retcode = ParseIPv6LeftToRight((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &addr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

class TS_ParseIPv4 : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
        srand(time(NULL));
    }
};

TEST_F(TS_ParseIPv4, Normal_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T parsedAddr;
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv4[i] = (uint8_t)rand();
    }
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8,
                       expAddr.Address.IPv4[3],
                       expAddr.Address.IPv4[2],
                       expAddr.Address.IPv4[1],
                       expAddr.Address.IPv4[0]);
    printf("%.*s\n", len, ipToParse);

    retcode = ParseIPv4((const uint8_t *)ipToParse, (uint32_t)len, &parsedAddr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv4, parsedAddr.Address.IPv4, sizeof(expAddr.Address.IPv4)));
}

TEST_F(TS_ParseIPv4, Quotes_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T parsedAddr;
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv4[i] = (uint8_t)rand();
    }
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\"",
                       expAddr.Address.IPv4[3],
                       expAddr.Address.IPv4[2],
                       expAddr.Address.IPv4[1],
                       expAddr.Address.IPv4[0]);
    printf("%.*s\n", len, ipToParse);

    retcode = ParseIPv4((const uint8_t *)ipToParse, (uint32_t)len, &parsedAddr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv4, parsedAddr.Address.IPv4, sizeof(expAddr.Address.IPv4)));
}

TEST_F(TS_ParseIPv4, LeadingZero_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T parsedAddr;
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expAddr.Address.IPv4[i] = (uint8_t)rand();
    }
    expAddr.Address.IPv4[3] = 20;
    char ipToParse[AT_UBLOX_MAX_IP_STR_LENGTH + 1];
    int len = snprintf(ipToParse, sizeof(ipToParse), "\"%03" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\"",
                       expAddr.Address.IPv4[3],
                       expAddr.Address.IPv4[2],
                       expAddr.Address.IPv4[1],
                       expAddr.Address.IPv4[0]);
    printf("%.*s\n", len, ipToParse);

    retcode = ParseIPv4((const uint8_t *)ipToParse, (uint32_t)len, &parsedAddr);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv4, parsedAddr.Address.IPv4, sizeof(expAddr.Address.IPv4)));
}

TEST_F(TS_ParseIPv4, GroupTooBig1_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T parsedAddr;
    const char *ipToParse = "123.300.123.123";
    printf("%.*s\n", (int)strlen(ipToParse), ipToParse);

    retcode = ParseIPv4((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &parsedAddr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv4, GroupTooBig2_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T parsedAddr;
    const char *ipToParse = "123.2000.123.123";
    printf("%.*s\n", (int)strlen(ipToParse), ipToParse);

    retcode = ParseIPv4((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &parsedAddr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

TEST_F(TS_ParseIPv4, TooManyGroups_Fail)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UBlox_Address_T parsedAddr;
    const char *ipToParse = "123.123.123.123.123";
    printf("%.*s\n", (int)strlen(ipToParse), ipToParse);

    retcode = ParseIPv4((const uint8_t *)ipToParse, (uint32_t)strlen(ipToParse), &parsedAddr);

    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(retcode));
}

class TS_At_Set_USOST : public TS_ModemTest
{
protected:
    const char *FormatTriggerBaseEnc(const AT_USOST_Param_T *param)
    {
        switch (param->RemoteIp.Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOST_FMTIPV4BASE,
                                       (int)param->Socket,
                                       (int)param->RemoteIp.Address.IPv4[3],
                                       (int)param->RemoteIp.Address.IPv4[2],
                                       (int)param->RemoteIp.Address.IPv4[1],
                                       (int)param->RemoteIp.Address.IPv4[0],
                                       (int)param->RemotePort,
                                       (int)param->Length,
                                       (int)param->Length,
                                       param->Data);
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOST_FMTIPV6BASE,
                                       (int)param->Socket,
                                       (int)param->RemoteIp.Address.IPv6[7],
                                       (int)param->RemoteIp.Address.IPv6[6],
                                       (int)param->RemoteIp.Address.IPv6[5],
                                       (int)param->RemoteIp.Address.IPv6[4],
                                       (int)param->RemoteIp.Address.IPv6[3],
                                       (int)param->RemoteIp.Address.IPv6[2],
                                       (int)param->RemoteIp.Address.IPv6[1],
                                       (int)param->RemoteIp.Address.IPv6[0],
                                       (int)param->RemotePort,
                                       (int)param->Length,
                                       (int)param->Length,
                                       param->Data);
        default:
            exit(1);
            return NULL;
        }
    }

    const char *FormatTriggerHexEnc(const AT_USOST_Param_T *param)
    {
        char hex[param->Length * 2];
        Retcode_T ret = EncodePayloadAsHex(param->Data, param->Length, hex, sizeof(hex));

        if (RETCODE_OK != ret)
            exit(1);

        int n = 0;
        switch (param->RemoteIp.Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOST_FMTIPV4HEX,
                                       (int)param->Socket,
                                       (int)param->RemoteIp.Address.IPv4[3],
                                       (int)param->RemoteIp.Address.IPv4[2],
                                       (int)param->RemoteIp.Address.IPv4[1],
                                       (int)param->RemoteIp.Address.IPv4[0],
                                       (int)param->RemotePort,
                                       (int)param->Length,
                                       &n,
                                       (int)sizeof(hex),
                                       hex);
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOST_FMTIPV6HEX,
                                       (int)param->Socket,
                                       (int)param->RemoteIp.Address.IPv6[7],
                                       (int)param->RemoteIp.Address.IPv6[6],
                                       (int)param->RemoteIp.Address.IPv6[5],
                                       (int)param->RemoteIp.Address.IPv6[4],
                                       (int)param->RemoteIp.Address.IPv6[3],
                                       (int)param->RemoteIp.Address.IPv6[2],
                                       (int)param->RemoteIp.Address.IPv6[1],
                                       (int)param->RemoteIp.Address.IPv6[0],
                                       (int)param->RemotePort,
                                       (int)param->Length,
                                       &n,
                                       (int)sizeof(hex),
                                       hex);
        default:
            exit(1);
            return NULL;
        }
    }

    const char *FormatAnswer(uint32_t socketId, uint32_t bytesSent)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUSOST_RESPONSE_FMT,
                                   CMD_UBLOX_ATUSOST, socketId, bytesSent, TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_USOST, BaseIPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOST_Param_T param;
    param.Socket = 4;
    param.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    param.RemoteIp.Address.IPv4[3] = 10;
    param.RemoteIp.Address.IPv4[2] = 1;
    param.RemoteIp.Address.IPv4[1] = 2;
    param.RemoteIp.Address.IPv4[0] = 3;
    param.RemotePort = 1337;
    param.Data = (const uint8_t *)"HELLO";
    param.Length = strlen((const char *)param.Data);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    AT_USOST_Resp_T resp;

    AddFakeAnswer(FormatTriggerBaseEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOST(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
}

TEST_F(TS_At_Set_USOST, HexIPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOST_Param_T param;
    param.Socket = 4;
    param.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    param.RemoteIp.Address.IPv4[3] = 10;
    param.RemoteIp.Address.IPv4[2] = 1;
    param.RemoteIp.Address.IPv4[1] = 2;
    param.RemoteIp.Address.IPv4[0] = 3;
    param.RemotePort = 1337;
    param.Data = (const uint8_t *)"HELLO";
    param.Length = strlen((const char *)param.Data);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    AT_USOST_Resp_T resp;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOST(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
}

TEST_F(TS_At_Set_USOST, BaseIPv6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOST_Param_T param;
    param.Socket = 4;
    param.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    param.RemoteIp.Address.IPv6[7] = 0xfe80;
    param.RemoteIp.Address.IPv6[6] = 0x1234;
    param.RemoteIp.Address.IPv6[5] = 0x4321;
    param.RemoteIp.Address.IPv6[4] = 0x1234;
    param.RemoteIp.Address.IPv6[3] = 0x4321;
    param.RemoteIp.Address.IPv6[2] = 0x1234;
    param.RemoteIp.Address.IPv6[1] = 0x4321;
    param.RemoteIp.Address.IPv6[0] = 0xFEFE;
    param.RemotePort = 1337;
    param.Data = (const uint8_t *)"HELLO";
    param.Length = strlen((const char *)param.Data);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BASE;
    AT_USOST_Resp_T resp;

    AddFakeAnswer(FormatTriggerBaseEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOST(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
}

TEST_F(TS_At_Set_USOST, HexIPv6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOST_Param_T param;
    param.Socket = 4;
    param.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    param.RemoteIp.Address.IPv6[7] = 0xfe80;
    param.RemoteIp.Address.IPv6[6] = 0x1234;
    param.RemoteIp.Address.IPv6[5] = 0x4321;
    param.RemoteIp.Address.IPv6[4] = 0x1234;
    param.RemoteIp.Address.IPv6[3] = 0x4321;
    param.RemoteIp.Address.IPv6[2] = 0x1234;
    param.RemoteIp.Address.IPv6[1] = 0x4321;
    param.RemoteIp.Address.IPv6[0] = 0xFEFE;
    param.RemotePort = 1337;
    param.Data = (const uint8_t *)"HELLO";
    param.Length = strlen((const char *)param.Data);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_HEX;
    AT_USOST_Resp_T resp;

    AddFakeAnswer(FormatTriggerHexEnc(&param), FormatAnswer(param.Socket, param.Length));

    retcode = At_Set_USOST(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(param.Socket, resp.Socket);
    EXPECT_EQ(param.Length, resp.Length);
}

TEST_F(TS_At_Set_USOST, InvalidEncoding)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOST_Param_T param;
    param.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    param.RemotePort = 1337;
    param.Data = (const uint8_t *)"HELLO";
    param.Length = strlen((const char *)param.Data);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_INVALID;
    AT_USOST_Resp_T resp;

    retcode = At_Set_USOST(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(TS_At_Set_USOST, UnsupportedEncoding)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOST_Param_T param;
    param.RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    param.RemotePort = 1337;
    param.Data = (const uint8_t *)"HELLO";
    param.Length = strlen((const char *)param.Data);
    param.Encoding = AT_UBLOX_PAYLOADENCODING_BINARY;
    AT_USOST_Resp_T resp;

    retcode = At_Set_USOST(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), retcode);
}

class TS_At_Set_USOLI : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_USOLI_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOLI_FMT,
                                   param->Socket,
                                   param->Port);
    }
};

TEST_F(TS_At_Set_USOLI, Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOLI_Param_T param;
    param.Socket = 4;
    param.Port = 1337;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_USOLI(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_USOCL : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_USOCL_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUSOCL_FMT,
                                   (uint32_t)param->Socket);
    }
};

TEST_F(TS_At_Set_USOCL, Pass)
{
    Retcode_T retcode = RETCODE_OK;
    AT_USOCL_Param_T param;
    param.Socket = 3;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_USOCL(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

class TS_At_Set_UDCONF : public TS_ModemTest
{
protected:
    const char *FormatTriggerHexMode(const AT_UDCONF_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUDCONF_FMTHEXMODE,
                                   (int)param->Config,
                                   (int)*((bool *)param->Value));
    }
};

TEST_F(TS_At_Set_UDCONF, HexMode_Enable_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    bool enable = true;
    AT_UDCONF_Param_T param;
    param.Config = AT_UDCONF_CONFIG_HEXMODE;
    param.Value = &enable;

    AddFakeAnswer(FormatTriggerHexMode(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_UDCONF(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_UDCONF, HexMode_Disable_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    bool enable = false;
    AT_UDCONF_Param_T param;
    param.Config = AT_UDCONF_CONFIG_HEXMODE;
    param.Value = &enable;

    AddFakeAnswer(FormatTriggerHexMode(&param), TEST_AT_RESPONSE_OK);

    retcode = At_Set_UDCONF(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_At_Set_UDCONF, InvalidConfig)
{
    Retcode_T retcode = RETCODE_OK;
    bool enable = false;
    AT_UDCONF_Param_T param;
    param.Config = AT_UDCONF_CONFIG_INVALID;
    param.Value = &enable;

    retcode = At_Set_UDCONF(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

class TS_At_Get_UDCONF : public TS_ModemTest
{
protected:
    const char *FormatTriggerHexMode(const AT_UDCONF_Param_T *param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_GET_ATUDCONF_FMT,
                                   (int)param->Config);
    }

    const char *FormatAnswerHexMode(bool enabled)
    {
        return FormatIntoNewBuffer(&Answer, TEST_GET_ATUDCONF_RESPONSE_FMTHEXMODE,
                                   CMD_UBLOX_ATUDCONF,
                                   (uint32_t)AT_UDCONF_CONFIG_HEXMODE,
                                   (uint32_t)enabled,
                                   TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Get_UDCONF, HexMode_Enabled_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    bool expEnabled = true;
    bool enabled = false;
    AT_UDCONF_Param_T param;
    param.Config = AT_UDCONF_CONFIG_HEXMODE;
    param.Value = &enabled;

    AddFakeAnswer(FormatTriggerHexMode(&param), FormatAnswerHexMode(expEnabled));

    retcode = At_Get_UDCONF(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expEnabled, *((bool *)param.Value));
}

TEST_F(TS_At_Get_UDCONF, HexMode_Disabled_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    bool expEnabled = false;
    bool enabled = true;
    AT_UDCONF_Param_T param;
    param.Config = AT_UDCONF_CONFIG_HEXMODE;
    param.Value = &enabled;

    AddFakeAnswer(FormatTriggerHexMode(&param), FormatAnswerHexMode(expEnabled));

    retcode = At_Get_UDCONF(&param);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expEnabled, *((bool *)param.Value));
}

class TS_At_HandleUrc_UUSOCL : public TS_ModemTest
{
protected:
    virtual void SetUp()
    {
        TS_ModemTest::SetUp();

        srand(time(NULL));

        RESET_FAKE(SocketService_NotifySocketClosed);
    }
};

TEST_F(TS_At_HandleUrc_UUSOCL, Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSOCL_FMT,
                           CMD_UBLOX_ATUUSOCL,
                           expSocketId);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSOCL();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketClosed_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketClosed_fake.arg0_val);
}

class TS_At_HandleUrc_UUSORX : public TS_ModemTest
{
protected:
    virtual void SetUp()
    {
        TS_ModemTest::SetUp();

        srand(time(NULL));

        RESET_FAKE(SocketService_NotifySocketDataReceived);
    }
};

TEST_F(TS_At_HandleUrc_UUSORX, UUSORD_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    uint32_t expLength = (uint32_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSORX_FMT,
                           CMD_UBLOX_ATUUSORD,
                           expSocketId,
                           expLength);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSORD();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketDataReceived_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketDataReceived_fake.arg0_val);
    EXPECT_EQ(expLength, SocketService_NotifySocketDataReceived_fake.arg1_val);
}

TEST_F(TS_At_HandleUrc_UUSORX, UUSORF_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    uint32_t expLength = (uint32_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSORX_FMT,
                           CMD_UBLOX_ATUUSORF,
                           expSocketId,
                           expLength);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSORF();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketDataReceived_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketDataReceived_fake.arg0_val);
    EXPECT_EQ(expLength, SocketService_NotifySocketDataReceived_fake.arg1_val);
}

static AT_UBlox_Address_T UUSOLI_RemoteIp;
static AT_UBlox_Address_T UUSOLI_LocalIp;

static void Custom_SocketService_NotifySocketAccepted(uint32_t createdSocketId,
                                                      const AT_UBlox_Address_T *remoteIp,
                                                      uint16_t remotePort,
                                                      uint32_t listeningSocketId,
                                                      const AT_UBlox_Address_T *localIp,
                                                      uint16_t listeningPort)
{
    KISO_UNUSED(createdSocketId);
    KISO_UNUSED(remotePort);
    KISO_UNUSED(listeningSocketId);
    KISO_UNUSED(listeningPort);

    memcpy(&UUSOLI_RemoteIp, remoteIp, sizeof(UUSOLI_RemoteIp));
    memcpy(&UUSOLI_LocalIp, localIp, sizeof(UUSOLI_LocalIp));
}

class TS_At_HandleUrc_UUSOLI : public TS_ModemTest
{
protected:
    virtual void SetUp()
    {
        TS_ModemTest::SetUp();

        srand(time(NULL));

        RESET_FAKE(SocketService_NotifySocketAccepted);
        SocketService_NotifySocketAccepted_fake.custom_fake = Custom_SocketService_NotifySocketAccepted;

        memset(UUSOLI_RemoteIp.Address.IPv6, 0, sizeof(UUSOLI_RemoteIp.Address.IPv6));
        UUSOLI_RemoteIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
        memset(UUSOLI_LocalIp.Address.IPv6, 0, sizeof(UUSOLI_LocalIp.Address.IPv6));
        UUSOLI_LocalIp.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    }
};

TEST_F(TS_At_HandleUrc_UUSOLI, IPv4_IPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    AT_UBlox_Address_T expRemoteIp;
    expRemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expRemoteIp.Address.IPv4[i] = (uint8_t)rand();
    }
    uint16_t expRemotePort = (uint16_t)rand();
    uint32_t expListeningSocket = (uint32_t)rand();
    AT_UBlox_Address_T expLocalIp;
    expLocalIp.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expLocalIp.Address.IPv4[i] = (uint8_t)rand();
    }
    uint16_t expListeningPort = (uint16_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSOLI_FMTIPV4IPV4,
                           CMD_UBLOX_ATUUSOLI,
                           expSocketId,
                           expRemoteIp.Address.IPv4[3],
                           expRemoteIp.Address.IPv4[2],
                           expRemoteIp.Address.IPv4[1],
                           expRemoteIp.Address.IPv4[0],
                           expRemotePort,
                           expListeningSocket,
                           expLocalIp.Address.IPv4[3],
                           expLocalIp.Address.IPv4[2],
                           expLocalIp.Address.IPv4[1],
                           expLocalIp.Address.IPv4[0],
                           expListeningPort);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSOLI();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketAccepted_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketAccepted_fake.arg0_val);
    EXPECT_EQ(expRemoteIp.Type, UUSOLI_RemoteIp.Type);
    EXPECT_EQ(0, memcmp(expRemoteIp.Address.IPv4, UUSOLI_RemoteIp.Address.IPv4, sizeof(UUSOLI_RemoteIp.Address.IPv4)));
    EXPECT_EQ(expRemotePort, SocketService_NotifySocketAccepted_fake.arg2_val);
    EXPECT_EQ(expListeningSocket, SocketService_NotifySocketAccepted_fake.arg3_val);
    EXPECT_EQ(expLocalIp.Type, UUSOLI_LocalIp.Type);
    EXPECT_EQ(0, memcmp(expLocalIp.Address.IPv4, UUSOLI_LocalIp.Address.IPv4, sizeof(UUSOLI_LocalIp.Address.IPv4)));
    EXPECT_EQ(expListeningPort, SocketService_NotifySocketAccepted_fake.arg5_val);
}

TEST_F(TS_At_HandleUrc_UUSOLI, IPv4_IPv6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    AT_UBlox_Address_T expRemoteIp;
    expRemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expRemoteIp.Address.IPv4[i] = (uint8_t)rand();
    }
    uint16_t expRemotePort = (uint16_t)rand();
    uint32_t expListeningSocket = (uint32_t)rand();
    AT_UBlox_Address_T expLocalIp;
    expLocalIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expLocalIp.Address.IPv6[i] = (uint16_t)rand();
    }
    uint16_t expListeningPort = (uint16_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSOLI_FMTIPV4IPV6,
                           CMD_UBLOX_ATUUSOLI,
                           expSocketId,
                           expRemoteIp.Address.IPv4[3],
                           expRemoteIp.Address.IPv4[2],
                           expRemoteIp.Address.IPv4[1],
                           expRemoteIp.Address.IPv4[0],
                           expRemotePort,
                           expListeningSocket,
                           expLocalIp.Address.IPv6[7],
                           expLocalIp.Address.IPv6[6],
                           expLocalIp.Address.IPv6[5],
                           expLocalIp.Address.IPv6[4],
                           expLocalIp.Address.IPv6[3],
                           expLocalIp.Address.IPv6[2],
                           expLocalIp.Address.IPv6[1],
                           expLocalIp.Address.IPv6[0],
                           expListeningPort);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSOLI();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketAccepted_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketAccepted_fake.arg0_val);
    EXPECT_EQ(expRemoteIp.Type, UUSOLI_RemoteIp.Type);
    EXPECT_EQ(0, memcmp(expRemoteIp.Address.IPv4, UUSOLI_RemoteIp.Address.IPv4, sizeof(UUSOLI_RemoteIp.Address.IPv4)));
    EXPECT_EQ(expRemotePort, SocketService_NotifySocketAccepted_fake.arg2_val);
    EXPECT_EQ(expListeningSocket, SocketService_NotifySocketAccepted_fake.arg3_val);
    EXPECT_EQ(expLocalIp.Type, UUSOLI_LocalIp.Type);
    EXPECT_EQ(0, memcmp(expLocalIp.Address.IPv6, UUSOLI_LocalIp.Address.IPv6, sizeof(UUSOLI_LocalIp.Address.IPv6)));
    EXPECT_EQ(expListeningPort, SocketService_NotifySocketAccepted_fake.arg5_val);
}
TEST_F(TS_At_HandleUrc_UUSOLI, IPv6_IPv4_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    AT_UBlox_Address_T expRemoteIp;
    expRemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expRemoteIp.Address.IPv6[i] = (uint8_t)rand();
    }
    uint16_t expRemotePort = (uint16_t)rand();
    uint32_t expListeningSocket = (uint32_t)rand();
    AT_UBlox_Address_T expLocalIp;
    expLocalIp.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (uint32_t i = 0; i < AT_UBLOX_IPV4_GROUP_COUNT; i++)
    {
        expLocalIp.Address.IPv4[i] = (uint16_t)rand();
    }
    uint16_t expListeningPort = (uint16_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSOLI_FMTIPV6IPV4,
                           CMD_UBLOX_ATUUSOLI,
                           expSocketId,
                           expRemoteIp.Address.IPv6[7],
                           expRemoteIp.Address.IPv6[6],
                           expRemoteIp.Address.IPv6[5],
                           expRemoteIp.Address.IPv6[4],
                           expRemoteIp.Address.IPv6[3],
                           expRemoteIp.Address.IPv6[2],
                           expRemoteIp.Address.IPv6[1],
                           expRemoteIp.Address.IPv6[0],
                           expRemotePort,
                           expListeningSocket,
                           expLocalIp.Address.IPv4[3],
                           expLocalIp.Address.IPv4[2],
                           expLocalIp.Address.IPv4[1],
                           expLocalIp.Address.IPv4[0],
                           expListeningPort);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSOLI();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketAccepted_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketAccepted_fake.arg0_val);
    EXPECT_EQ(expRemoteIp.Type, UUSOLI_RemoteIp.Type);
    EXPECT_EQ(0, memcmp(expRemoteIp.Address.IPv6, UUSOLI_RemoteIp.Address.IPv6, sizeof(UUSOLI_RemoteIp.Address.IPv6)));
    EXPECT_EQ(expRemotePort, SocketService_NotifySocketAccepted_fake.arg2_val);
    EXPECT_EQ(expListeningSocket, SocketService_NotifySocketAccepted_fake.arg3_val);
    EXPECT_EQ(expLocalIp.Type, UUSOLI_LocalIp.Type);
    EXPECT_EQ(0, memcmp(expLocalIp.Address.IPv4, UUSOLI_LocalIp.Address.IPv4, sizeof(UUSOLI_LocalIp.Address.IPv4)));
    EXPECT_EQ(expListeningPort, SocketService_NotifySocketAccepted_fake.arg5_val);
}

TEST_F(TS_At_HandleUrc_UUSOLI, IPv6_IPv6_Pass)
{
    Retcode_T retcode = RETCODE_OK;
    uint32_t expSocketId = (uint32_t)rand();
    AT_UBlox_Address_T expRemoteIp;
    expRemoteIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expRemoteIp.Address.IPv6[i] = (uint8_t)rand();
    }
    uint16_t expRemotePort = (uint16_t)rand();
    uint32_t expListeningSocket = (uint32_t)rand();
    AT_UBlox_Address_T expLocalIp;
    expLocalIp.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    for (uint32_t i = 0; i < AT_UBLOX_IPV6_GROUP_COUNT; i++)
    {
        expLocalIp.Address.IPv6[i] = (uint16_t)rand();
    }
    uint16_t expListeningPort = (uint16_t)rand();
    char cmd[1024];
    int32_t len = snprintf(cmd, sizeof(cmd), TEST_URC_ATUSOLI_FMTIPV6IPV6,
                           CMD_UBLOX_ATUUSOLI,
                           expSocketId,
                           expRemoteIp.Address.IPv6[7],
                           expRemoteIp.Address.IPv6[6],
                           expRemoteIp.Address.IPv6[5],
                           expRemoteIp.Address.IPv6[4],
                           expRemoteIp.Address.IPv6[3],
                           expRemoteIp.Address.IPv6[2],
                           expRemoteIp.Address.IPv6[1],
                           expRemoteIp.Address.IPv6[0],
                           expRemotePort,
                           expListeningSocket,
                           expLocalIp.Address.IPv6[7],
                           expLocalIp.Address.IPv6[6],
                           expLocalIp.Address.IPv6[5],
                           expLocalIp.Address.IPv6[4],
                           expLocalIp.Address.IPv6[3],
                           expLocalIp.Address.IPv6[2],
                           expLocalIp.Address.IPv6[1],
                           expLocalIp.Address.IPv6[0],
                           expListeningPort);

    (void)AtResponseParser_Parse((uint8_t *)cmd, (uint32_t)len);

    retcode = At_HandleUrc_UUSOLI();

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(1U, SocketService_NotifySocketAccepted_fake.call_count);
    EXPECT_EQ(expSocketId, SocketService_NotifySocketAccepted_fake.arg0_val);
    EXPECT_EQ(expRemoteIp.Type, UUSOLI_RemoteIp.Type);
    EXPECT_EQ(0, memcmp(expRemoteIp.Address.IPv6, UUSOLI_RemoteIp.Address.IPv6, sizeof(UUSOLI_RemoteIp.Address.IPv6)));
    EXPECT_EQ(expRemotePort, SocketService_NotifySocketAccepted_fake.arg2_val);
    EXPECT_EQ(expListeningSocket, SocketService_NotifySocketAccepted_fake.arg3_val);
    EXPECT_EQ(expLocalIp.Type, UUSOLI_LocalIp.Type);
    EXPECT_EQ(0, memcmp(expLocalIp.Address.IPv6, UUSOLI_LocalIp.Address.IPv6, sizeof(UUSOLI_LocalIp.Address.IPv6)));
    EXPECT_EQ(expListeningPort, SocketService_NotifySocketAccepted_fake.arg5_val);
}

class TS_At_Get_CCID : public TS_ModemTest
{
protected:
    const char *FormatAnswer(const char *iccid)
    {
        return FormatIntoNewBuffer(&Answer, TEST_GET_ATCCID_RESPONSE_FMT,
                                   CMD_UBLOX_ATCCID, iccid, TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Get_CCID, 19Char_Pass)
{
    const char *expIccid = "1234567890123456789";

    Retcode_T retcode = RETCODE_OK;
    AT_CCID_Resp_T resp;

    AddFakeAnswer(CMD_UBLOX_GET_ATCCID, FormatAnswer(expIccid));

    retcode = At_Get_CCID(&resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CCID_TYPE_19CHAR, resp.Type);
    EXPECT_STREQ(expIccid, resp.Iccid);
}

TEST_F(TS_At_Get_CCID, 20Char_Pass)
{
    const char *expIccid = "12345678901234567890";

    Retcode_T retcode = RETCODE_OK;
    AT_CCID_Resp_T resp;

    AddFakeAnswer(CMD_UBLOX_GET_ATCCID, FormatAnswer(expIccid));

    retcode = At_Get_CCID(&resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(AT_CCID_TYPE_20CHAR, resp.Type);
    EXPECT_STREQ(expIccid, resp.Iccid);
}

class TS_At_Set_UDNSRN : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const char *domain)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUDNSRN_FMT1,
                                   AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP,
                                   domain);
    }

    const char *FormatTrigger(const AT_UBlox_Address_T *ip)
    {
        switch (ip->Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUDNSRN_FMT2,
                                       AT_UDNSRN_RESOLUTIONTYPE_IPTODOMAIN,
                                       ip->Address.IPv4[3],
                                       ip->Address.IPv4[2],
                                       ip->Address.IPv4[1],
                                       ip->Address.IPv4[0]);
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUDNSRN_FMT3,
                                       AT_UDNSRN_RESOLUTIONTYPE_IPTODOMAIN,
                                       ip->Address.IPv6[7],
                                       ip->Address.IPv6[6],
                                       ip->Address.IPv6[5],
                                       ip->Address.IPv6[4],
                                       ip->Address.IPv6[3],
                                       ip->Address.IPv6[2],
                                       ip->Address.IPv6[1],
                                       ip->Address.IPv6[0]);
        default:
            exit(1);
            return NULL;
        }
    }

    const char *FormatAnswer(const AT_UBlox_Address_T *ip)
    {
        switch (ip->Type)
        {
        case AT_UBLOX_ADDRESSTYPE_IPV4:
            return FormatIntoNewBuffer(&Answer, TEST_SET_ATUDNS_RESPONSE_FMT1,
                                       CMD_UBLOX_ATUDNSRN,
                                       ip->Address.IPv4[3],
                                       ip->Address.IPv4[2],
                                       ip->Address.IPv4[1],
                                       ip->Address.IPv4[0],
                                       TEST_AT_RESPONSE_OK);
        case AT_UBLOX_ADDRESSTYPE_IPV6:
            return FormatIntoNewBuffer(&Answer, TEST_SET_ATUDNS_RESPONSE_FMT2,
                                       CMD_UBLOX_ATUDNSRN,
                                       ip->Address.IPv6[7],
                                       ip->Address.IPv6[6],
                                       ip->Address.IPv6[5],
                                       ip->Address.IPv6[4],
                                       ip->Address.IPv6[3],
                                       ip->Address.IPv6[2],
                                       ip->Address.IPv6[1],
                                       ip->Address.IPv6[0],
                                       TEST_AT_RESPONSE_OK);
        default:
            exit(1);
            return NULL;
        }
    }

    const char *FormatAnswer(const char *domain)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUDNS_RESPONSE_FMT3,
                                   CMD_UBLOX_ATUDNSRN,
                                   domain,
                                   TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_UDNSRN, DomainToIPv4_Pass)
{
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    expAddr.Address.IPv4[3] = 139;
    expAddr.Address.IPv4[2] = 15;
    expAddr.Address.IPv4[1] = 248;
    expAddr.Address.IPv4[0] = 104;

    Retcode_T retcode = RETCODE_OK;
    AT_UDNSRN_Param_T param;
    param.ResolutionType = AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP;
    param.DomainIpString.Domain = "bosch.com";
    AT_UDNSRN_Resp_T resp;

    AddFakeAnswer(FormatTrigger(param.DomainIpString.Domain), FormatAnswer(&expAddr));

    retcode = At_Set_UDNSRN(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expAddr.Type, resp.DomainIpString.Ip.Type);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv4, resp.DomainIpString.Ip.Address.IPv4, sizeof(expAddr.Address.IPv4)));
}

TEST_F(TS_At_Set_UDNSRN, DomainToIPv6_Pass)
{
    AT_UBlox_Address_T expAddr;
    expAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    expAddr.Address.IPv6[7] = 0x2607;
    expAddr.Address.IPv6[6] = 0xf8b0;
    expAddr.Address.IPv6[5] = 0x4005;
    expAddr.Address.IPv6[4] = 0x805;
    expAddr.Address.IPv6[3] = 0;
    expAddr.Address.IPv6[2] = 0;
    expAddr.Address.IPv6[1] = 0;
    expAddr.Address.IPv6[0] = 0x200e;

    Retcode_T retcode = RETCODE_OK;
    AT_UDNSRN_Param_T param;
    param.ResolutionType = AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP;
    param.DomainIpString.Domain = "bosch.com";
    AT_UDNSRN_Resp_T resp;

    AddFakeAnswer(FormatTrigger(param.DomainIpString.Domain), FormatAnswer(&expAddr));

    retcode = At_Set_UDNSRN(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ(expAddr.Type, resp.DomainIpString.Ip.Type);
    EXPECT_EQ(0, memcmp(expAddr.Address.IPv6, resp.DomainIpString.Ip.Address.IPv6, sizeof(expAddr.Address.IPv6)));
}

TEST_F(TS_At_Set_UDNSRN, IPv4ToDomain_Pass)
{
    const char *expDomain = "bosch.com";

    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    addr.Address.IPv4[3] = 139;
    addr.Address.IPv4[2] = 15;
    addr.Address.IPv4[1] = 248;
    addr.Address.IPv4[0] = 104;

    Retcode_T retcode = RETCODE_OK;
    AT_UDNSRN_Param_T param;
    param.ResolutionType = AT_UDNSRN_RESOLUTIONTYPE_IPTODOMAIN;
    param.DomainIpString.Ip = &addr;
    AT_UDNSRN_Resp_T resp;

    AddFakeAnswer(FormatTrigger(param.DomainIpString.Ip), FormatAnswer(expDomain));

    retcode = At_Set_UDNSRN(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ(expDomain, resp.DomainIpString.Domain);
}

TEST_F(TS_At_Set_UDNSRN, IPv6ToDomain_Pass)
{
    const char *expDomain = "google.com";

    AT_UBlox_Address_T addr;
    addr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    addr.Address.IPv6[7] = 0x2607;
    addr.Address.IPv6[6] = 0xf8b0;
    addr.Address.IPv6[5] = 0x4005;
    addr.Address.IPv6[4] = 0x805;
    addr.Address.IPv6[3] = 0;
    addr.Address.IPv6[2] = 0;
    addr.Address.IPv6[1] = 0;
    addr.Address.IPv6[0] = 0x200e;

    Retcode_T retcode = RETCODE_OK;
    AT_UDNSRN_Param_T param;
    param.ResolutionType = AT_UDNSRN_RESOLUTIONTYPE_IPTODOMAIN;
    param.DomainIpString.Ip = &addr;
    AT_UDNSRN_Resp_T resp;

    AddFakeAnswer(FormatTrigger(param.DomainIpString.Ip), FormatAnswer(expDomain));

    retcode = At_Set_UDNSRN(&param, &resp);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_STREQ(expDomain, resp.DomainIpString.Domain);
}

TEST_F(TS_At_Set_UDNSRN, FailInvalidResolution)
{
    Retcode_T retcode = RETCODE_OK;
    AT_UDNSRN_Param_T param;
    param.ResolutionType = AT_UDNSRN_RESOLUTIONTYPE_INVALID;
    AT_UDNSRN_Resp_T resp;

    retcode = At_Set_UDNSRN(&param, &resp);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

class TS_At_Set_UHTTP : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_UHTTP_Param_T *param)
    {
        switch (param->Opcode)
        {
        case AT_UHTTP_OPCODE_SERVER_IP:
        case AT_UHTTP_OPCODE_SERVER_NAME:
        case AT_UHTTP_OPCODE_USERNAME:
        case AT_UHTTP_OPCODE_PASSWORD:
        case AT_UHTTP_OPCODE_CUSTOM_HEADER:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUHTTP2_FMT,
                                       param->ProfileId, param->Opcode, param->Value.String);
        case AT_UHTTP_OPCODE_AUTH_TYPE:
        case AT_UHTTP_OPCODE_SERVER_PORT:
        case AT_UHTTP_OPCODE_SECURE_OPTION:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUHTTP3_FMT,
                                       param->ProfileId, param->Opcode, param->Value.Numeric);
        case AT_UHTTP_OPCODE_RESERVED0:
        case AT_UHTTP_OPCODE_RESERVED1:
        case AT_UHTTP_OPCODE_INVALID:
        default:
            exit(1);
            return NULL;
        }
    }
};

TEST_F(TS_At_Set_UHTTP, ServerIp_Pass)
{
    AT_UHTTP_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_1;
    param.Opcode = AT_UHTTP_OPCODE_SERVER_IP;
    param.Value.String = "127.0.0.1";

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UHTTP(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_At_Set_UHTTP, ServerName_Pass)
{
    AT_UHTTP_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_1;
    param.Opcode = AT_UHTTP_OPCODE_SERVER_NAME;
    param.Value.String = "bosch.com";

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UHTTP(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_At_Set_UHTTP, ServerPort_Pass)
{
    AT_UHTTP_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_1;
    param.Opcode = AT_UHTTP_OPCODE_SERVER_PORT;
    param.Value.Numeric = 1337;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UHTTP(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_At_Set_UHTTP, fail_invalid_opcode)
{
    AT_UHTTP_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_1;
    param.Opcode = AT_UHTTP_OPCODE_INVALID;
    param.Value.Numeric = 1337;

    Retcode_T rc = At_Set_UHTTP(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

class TS_At_Set_UHTTPC : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_UHTTPC_Param_T *param)
    {
        switch (param->Command)
        {
        case AT_UHTTPC_COMMAND_HEAD:
        case AT_UHTTPC_COMMAND_GET:
        case AT_UHTTPC_COMMAND_DELETE:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUHTTPC1_FMT,
                                       param->ProfileId, param->Command,
                                       param->PathOnServer,
                                       param->ResponseFilename);
        case AT_UHTTPC_COMMAND_PUT:
        case AT_UHTTPC_COMMAND_POST_FILE:
        case AT_UHTTPC_COMMAND_POST_DATA:
        case AT_UHTTPC_COMMAND_GET_FOTA:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUHTTPC2_FMT,
                                       param->ProfileId, param->Command,
                                       param->PathOnServer,
                                       param->ResponseFilename,
                                       param->Payload,
                                       param->ContentType);
        case AT_UHTTPC_COMMAND_INVALID:
        default:
            exit(1);
            return NULL;
        }
    }
};

TEST_F(TS_At_Set_UHTTPC, GET_Pass)
{
    AT_UHTTPC_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_3;
    param.Command = AT_UHTTPC_COMMAND_GET;
    param.PathOnServer = "/some/resource";
    param.ResponseFilename = "response.dat";
    param.Payload = NULL;
    param.ContentType = AT_UHTTPC_CONTENT_INVALID;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UHTTPC(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_At_Set_UHTTPC, POSTFile_Pass)
{
    AT_UHTTPC_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_3;
    param.Command = AT_UHTTPC_COMMAND_POST_DATA;
    param.PathOnServer = "/some/resource";
    param.ResponseFilename = "response.dat";
    param.Payload = "HELLO WORLD!";
    param.ContentType = AT_UHTTPC_CONTENT_TEXT_PLAIN;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UHTTPC(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_At_Set_UHTTPC, PUT_Pass)
{
    AT_UHTTPC_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_3;
    param.Command = AT_UHTTPC_COMMAND_PUT;
    param.PathOnServer = "/some/resource";
    param.ResponseFilename = "response.dat";
    param.Payload = "uploadme.txt";
    param.ContentType = AT_UHTTPC_CONTENT_TEXT_PLAIN;

    AddFakeAnswer(FormatTrigger(&param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UHTTPC(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_At_Set_UHTTPC, Fail_Invalid_Command)
{
    AT_UHTTPC_Param_T param;
    param.ProfileId = AT_UHTTP_PROFILE_ID_3;
    param.Command = AT_UHTTPC_COMMAND_INVALID;
    param.PathOnServer = "/some/resource";
    param.ResponseFilename = "response.dat";
    param.Payload = "uploadme.txt";
    param.ContentType = AT_UHTTPC_CONTENT_TEXT_PLAIN;

    Retcode_T rc = At_Set_UHTTPC(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

class TS_At_HandleUrc_UUHTTPCR : public TS_ModemTest
{
protected:
    virtual void SetUp() override
    {
        TS_ModemTest::SetUp();

        RESET_FAKE(HttpService_NotifyResult);
    }
};

TEST_F(TS_At_HandleUrc_UUHTTPCR, Pass)
{
    AT_UHTTP_ProfileId_T profileId = static_cast<AT_UHTTP_ProfileId_T>(rand() % (AT_UHTTP_PROFILE_ID_3 + 1));
    AT_UHTTPC_Command_T command = static_cast<AT_UHTTPC_Command_T>(rand() % (AT_UHTTPC_COMMAND_POST_DATA + 1));
    bool result = true;
    std::stringstream ss;
    ss << '+' << CMD_UBLOX_ATUUHTTPCR << ':' << (int)profileId << ',' << (int)command << ',' << (int)result << "\r\n";
    std::string atResponse = ss.str();
    (void)AtResponseParser_Parse(reinterpret_cast<const uint8_t *>(atResponse.c_str()), atResponse.length());

    Retcode_T rc = At_HandleUrc_UUHTTPCR();

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(1U, HttpService_NotifyResult_fake.call_count);
    EXPECT_EQ(profileId, HttpService_NotifyResult_fake.arg0_val);
    EXPECT_EQ(command, HttpService_NotifyResult_fake.arg1_val);
    EXPECT_EQ(result, HttpService_NotifyResult_fake.arg2_val);
}

class TS_At_Set_UTEST : public TS_ModemTest
{
protected:
    const char *FormatTrigger(uint32_t param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUTEST_FMT, (int)param);
    }
};

TEST_F(TS_At_Set_UTEST, Pass)
{
    uint32_t mode = rand() % 2;

    AddFakeAnswer(FormatTrigger(mode), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UTEST(mode);

    EXPECT_EQ(RETCODE_OK, rc);
}

class TS_At_Set_UDWNFILE : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_UDWNFILE_Param_T &param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUDWNFILE_FMT,
                                   param.Filename, param.DataSize);
    }
    const char *ForamtAnswer(const AT_UDWNFILE_Param_T &param)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATUDWNFILE_RESPONSE_FMT,
                                   (int)param.DataSize, (const char *)param.Data);
    }
};

TEST_F(TS_At_Set_UDWNFILE, SingleLine_Pass)
{
    std::string data = "HELLO WORLD";
    AT_UDWNFILE_Param_T param;
    param.Filename = "test.txt";
    param.Data = (const uint8_t *)data.c_str();
    param.DataSize = data.length();

    AddFakeAnswer(FormatTrigger(param), ForamtAnswer(param));

    Retcode_T rc = At_Set_UDWNFILE(&param);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ((uint32_t)(AT_EVENT_TYPE_ALL - AT_EVENT_TYPE_MISC), AtResponseQueue_GetEventMask());
}

TEST_F(TS_At_Set_UDWNFILE, MultiLineLf_Pass)
{
    std::string data = "HELLO WORLD\nTHIS IS THE SECOND LINE\n\nTHE FORUTH\nTHREE IS EMPTY!";
    AT_UDWNFILE_Param_T param;
    param.Filename = "test.txt";
    param.Data = (const uint8_t *)data.c_str();
    param.DataSize = data.length();

    AddFakeAnswer(FormatTrigger(param), ForamtAnswer(param));

    Retcode_T rc = At_Set_UDWNFILE(&param);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ((uint32_t)(AT_EVENT_TYPE_ALL - AT_EVENT_TYPE_MISC), AtResponseQueue_GetEventMask());
}

TEST_F(TS_At_Set_UDWNFILE, MultiLineCrLf_Pass)
{
    std::string data = "HELLO WORLD\r\nTHIS IS THE SECOND LINE\r\n\r\nTHE FORUTH\r\nTHREE IS EMPTY!";
    AT_UDWNFILE_Param_T param;
    param.Filename = "test.txt";
    param.Data = (const uint8_t *)data.c_str();
    param.DataSize = data.length();

    AddFakeAnswer(FormatTrigger(param), ForamtAnswer(param));

    Retcode_T rc = At_Set_UDWNFILE(&param);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ((uint32_t)(AT_EVENT_TYPE_ALL - AT_EVENT_TYPE_MISC), AtResponseQueue_GetEventMask());
}

class TS_At_Set_UDELFILE : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_UDELFILE_Param_T &param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATUDELFILE_FMT,
                                   param.Filename);
    }
};

TEST_F(TS_At_Set_UDELFILE, Pass)
{
    AT_UDELFILE_Param_T param;
    param.Filename = "test.txt";

    AddFakeAnswer(FormatTrigger(param), TEST_AT_RESPONSE_OK);

    Retcode_T rc = At_Set_UDELFILE(&param);

    EXPECT_EQ(RETCODE_OK, rc);
}

class TS_At_Set_ULSTFILE : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_ULSTFILE_Param_T &param)
    {
        switch (param.Opcode)
        {
        case AT_ULSTFILE_OPCODE_LIST:
        case AT_ULSTFILE_OPCODE_FREE:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATULSTFILE_FMT1,
                                       param.Opcode);
        case AT_ULSTFILE_OPCODE_SIZE:
            return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATULSTFILE_FMT2,
                                       param.Opcode, param.Filename);
        default:
            std::cerr << "not supported!" << std::endl;
            exit(1);
            return NULL;
        }
    }

    const char *FormatAnswer(const AT_ULSTFILE_Param_T &param, uint32_t value)
    {
        switch (param.Opcode)
        {
        case AT_ULSTFILE_OPCODE_FREE:
        case AT_ULSTFILE_OPCODE_SIZE:
            return FormatIntoNewBuffer(&Answer, TEST_SET_ATULSTFILE_RESPONSE_FMT2,
                                       CMD_UBLOX_ATULSTFILE, (int)value, TEST_AT_RESPONSE_OK);
        case AT_ULSTFILE_OPCODE_LIST:
        default:
            std::cerr << "not supported!" << std::endl;
            exit(1);
            return NULL;
        }
    }
};

TEST_F(TS_At_Set_ULSTFILE, Size_Pass)
{
    AT_ULSTFILE_Param_T param;
    param.Opcode = AT_ULSTFILE_OPCODE_SIZE;
    param.Filename = "test.txt";
    param.Filesize = 0;
    uint32_t expFilesize = (uint32_t)rand();

    AddFakeAnswer(FormatTrigger(param), FormatAnswer(param, expFilesize));

    Retcode_T rc = At_Set_ULSTFILE(&param);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(expFilesize, param.Filesize);
}

TEST_F(TS_At_Set_ULSTFILE, Free_NotSupported)
{
    AT_ULSTFILE_Param_T param;
    param.Opcode = AT_ULSTFILE_OPCODE_FREE;
    param.Filename = NULL;
    param.Filesize = 0;

    Retcode_T rc = At_Set_ULSTFILE(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

TEST_F(TS_At_Set_ULSTFILE, List_NotSupported)
{
    AT_ULSTFILE_Param_T param;
    param.Opcode = AT_ULSTFILE_OPCODE_LIST;
    param.Filename = NULL;
    param.Filesize = 0;

    Retcode_T rc = At_Set_ULSTFILE(&param);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

class TS_At_Set_URDBLOCK : public TS_ModemTest
{
protected:
    const char *FormatTrigger(const AT_URDBLOCK_Param_T &param)
    {
        return FormatIntoNewBuffer(&Trigger, CMD_UBLOX_SET_ATURDBLOCK_FMT,
                                   param.Filename, param.Offset, param.Size);
    }

    const char *FormatAnswer(const AT_URDBLOCK_Param_T &param, const void *data)
    {
        return FormatIntoNewBuffer(&Answer, TEST_SET_ATURDBLOCK_RESPONSE_FMT,
                                   CMD_UBLOX_ATURDBLOCK, param.Filename,
                                   (int)param.Size,
                                   (int)param.Size, data,
                                   TEST_AT_RESPONSE_OK);
    }
};

TEST_F(TS_At_Set_URDBLOCK, SingleLineFullRead_Pass)
{
    std::string expPayload = "HELLO WOLRD";
    uint8_t payloadBuffer[expPayload.length() + 1];
    memset(payloadBuffer, '\0', sizeof(payloadBuffer));

    AT_URDBLOCK_Param_T param;
    param.Filename = "test.txt";
    param.Offset = 0; //(uint32_t)rand()) % expPayload.length();
    param.Size = expPayload.length() - param.Offset;
    AT_URDBLOCK_Resp_T resp;
    memset(resp.Filename, '\0', sizeof(resp.Filename));
    resp.Size = 0;
    resp.Data = payloadBuffer;

    AddFakeAnswer(FormatTrigger(param), FormatAnswer(param, expPayload.c_str() + param.Offset));

    Retcode_T rc = At_Set_URDBLOCK(&param, &resp);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_GE(param.Size, resp.Size);
    payloadBuffer[sizeof(payloadBuffer) - 1] = '\0';
    EXPECT_STREQ(expPayload.c_str() + param.Offset, (const char *)payloadBuffer);
}

TEST_F(TS_At_Set_URDBLOCK, SingleLinePartialRead_Pass)
{
    std::string expPayload = "HELLO WOLRD";
    uint8_t payloadBuffer[expPayload.length() + 1];
    memset(payloadBuffer, '\0', sizeof(payloadBuffer));

    AT_URDBLOCK_Param_T param;
    param.Filename = "test.txt";
    param.Offset = 0;
    param.Size = (expPayload.length() - param.Offset) / 2;
    AT_URDBLOCK_Resp_T resp;
    memset(resp.Filename, '\0', sizeof(resp.Filename));
    resp.Size = 0;
    resp.Data = payloadBuffer;

    AddFakeAnswer(FormatTrigger(param), FormatAnswer(param, expPayload.c_str() + param.Offset));

    Retcode_T rc = At_Set_URDBLOCK(&param, &resp);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_GE(param.Size, resp.Size);
    payloadBuffer[sizeof(payloadBuffer) - 1] = '\0';
    EXPECT_STREQ(expPayload.substr(param.Offset, resp.Size).c_str(), (const char *)payloadBuffer);
}

TEST_F(TS_At_Set_URDBLOCK, MultiLineFullRead_Pass)
{
    std::string expPayload = "HELLO WOLRD\r\nthis is the second line\r\nnow the third\r\nlet's drop in an empty one as well\r\n\r\ndone";
    uint8_t payloadBuffer[expPayload.length() + 1];
    memset(payloadBuffer, '\0', sizeof(payloadBuffer));

    AT_URDBLOCK_Param_T param;
    param.Filename = "test.txt";
    param.Offset = 0;
    param.Size = expPayload.length() - param.Offset;
    AT_URDBLOCK_Resp_T resp;
    memset(resp.Filename, '\0', sizeof(resp.Filename));
    resp.Size = 0;
    resp.Data = payloadBuffer;

    AddFakeAnswer(FormatTrigger(param), FormatAnswer(param, expPayload.c_str() + param.Offset));

    Retcode_T rc = At_Set_URDBLOCK(&param, &resp);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_GE(param.Size, resp.Size);
    payloadBuffer[sizeof(payloadBuffer) - 1] = '\0';
    EXPECT_STREQ(expPayload.substr(param.Offset, resp.Size).c_str(), (const char *)payloadBuffer);
}

TEST_F(TS_At_Set_URDBLOCK, MultiLinePartialRead_Pass)
{
    std::string expPayload = "HELLO WOLRD\r\nthis is the second line\r\nnow the third\r\nlet's drop in an empty one as well\r\n\r\ndone";
    uint8_t payloadBuffer[expPayload.length() + 1];
    memset(payloadBuffer, '\0', sizeof(payloadBuffer));

    AT_URDBLOCK_Param_T param;
    param.Filename = "test.txt";
    param.Offset = 0;
    param.Size = (expPayload.length() - param.Offset) / 2;
    AT_URDBLOCK_Resp_T resp;
    memset(resp.Filename, '\0', sizeof(resp.Filename));
    resp.Size = 0;
    resp.Data = payloadBuffer;

    AddFakeAnswer(FormatTrigger(param), FormatAnswer(param, expPayload.c_str() + param.Offset));

    Retcode_T rc = At_Set_URDBLOCK(&param, &resp);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_GE(param.Size, resp.Size);
    payloadBuffer[sizeof(payloadBuffer) - 1] = '\0';
    EXPECT_STREQ(expPayload.substr(param.Offset, resp.Size).c_str(), (const char *)payloadBuffer);
}
