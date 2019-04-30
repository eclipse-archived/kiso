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
/* include faked interfaces */

#define portNUM_CONFIGURABLE_REGIONS    1
#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_DNS

#include "CellularModule.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"
#include "AtResponseParser_th.hh"
#include "AtResponseQueue_th.hh"
#include "Engine_th.hh"

#undef RETCODE
#define RETCODE(severity,code) ((Retcode_T) code)

#undef BCDS_MODULE_ID
#include "Utils.c"

#undef BCDS_MODULE_ID
#include "Dns.c"

static Retcode_T RequestDispatchCustom(uint32_t timeout,
        CellularRequest_CallableFunction_T function, void* parameter, uint32_t ParameterLength)
{
    BCDS_UNUSED(timeout);
    return function(parameter, ParameterLength);
}

static Retcode_T WaitArbCmdArg = RETCODE_OK;
const char ip1[] = "\"192.168.1.2\"";
const char ip2[] = "1";
static const char *WaitArbCmdIp = ip1;

static Retcode_T WaitArbitraryCmdArgCustom(uint32_t timeout, uint8_t **BufferPtr,
        uint32_t *BufferLen)
{
    BCDS_UNUSED(timeout);

    *BufferPtr = (uint8_t *) WaitArbCmdIp;
    *BufferLen = strlen(WaitArbCmdIp);
    return WaitArbCmdArg;
}

static Retcode_T WaitArbitaryResponseCustom(uint32_t timeout, AtResponseCode_T *response)
{
    BCDS_UNUSED(timeout);

    *response = AT_RESPONSE_CODE_OK;
    return RETCODE_OK;
}

}
//-- extern "C"

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class CellularDns: public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(AtResponseQueue_WaitForNamedCmdEcho);
        RESET_FAKE(AtResponseQueue_WaitForNamedCmd);
        RESET_FAKE(AtResponseQueue_WaitForArbitraryCmdArg);
        RESET_FAKE(CellularRequest_Dispatch);

        AtResponseQueue_WaitForArbitraryCmdArg_fake.custom_fake = WaitArbitraryCmdArgCustom;
        AtResponseQueue_WaitForArbitraryResponseCode_fake.custom_fake =
                WaitArbitaryResponseCustom;
        CellularRequest_Dispatch_fake.custom_fake = RequestDispatchCustom;
    }

    virtual void TearDown()
    {
        ;
    }
};

TEST_F(CellularDns, TestCellularResolveDomainName)
{
    uint32_t ip;
    const char name[] = "abc";
    uint32_t lenght = strlen(name);

    /* test failing scenarios */

    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_ResolveDomainName(NULL, lenght, &ip));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_ResolveDomainName((uint8_t *)name,
            CELLULAR_DNS_MAX_DOMAIN_NAME_LENGTH + 1, &ip));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_ResolveDomainName((uint8_t *)name, -1, &ip));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_ResolveDomainName((uint8_t *)name, 0, &ip));
    EXPECT_EQ(RETCODE_INVALID_PARAM, Cellular_ResolveDomainName((uint8_t *)name, lenght, NULL));

    EXPECT_EQ(RETCODE_OK, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));

    /* test passing scenario */
    EXPECT_EQ(RETCODE_OK, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));
    EXPECT_EQ(2U, CellularEngine_SendAtCommand_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmdEcho_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForNamedCmd_fake.call_count);
    EXPECT_EQ(2U, AtResponseQueue_WaitForArbitraryCmdArg_fake.call_count);
    EXPECT_EQ((uint32_t)((192 << 24) | (168 << 16) | (1 << 8) | 2), ip);

    WaitArbCmdIp = ip2;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));

    WaitArbCmdArg = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));
    AtResponseQueue_WaitForNamedCmdEcho_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));
    CellularEngine_SendAtCommand_fake.return_val = RETCODE_FAILURE;
    EXPECT_EQ(RETCODE_FAILURE, Cellular_ResolveDomainName((uint8_t *)name, lenght, &ip));
}
