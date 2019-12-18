/*******************************************************************************
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
 ******************************************************************************/

#include <gtest.h>

extern "C"
{
#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_DNS_SERVICE

#include "Engine_th.hh"
#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#undef KISO_MODULE_ID
#include "UBloxUtils.c"
#undef KISO_MODULE_ID
#include "DnsService.c"
}

class TS_Smoke : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }
};

TEST_F(TS_Smoke, Compile_Success)
{
}

namespace F_Engine_Dispatch_SaveArgs
{
static void *Data = nullptr;
static uint32_t Length = 0;

static Retcode_T Func(CellularRequest_CallableFunction_T, uint32_t, void *data, uint32_t len)
{
    Data = malloc(len);
    memcpy(Data, data, len);
    Length = len;
    return RETCODE_OK;
}
} // namespace F_Engine_Dispatch_SaveArgs

class TS_CellularDns_ResolveDomain : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(Engine_Dispatch);

        Engine_Dispatch_fake.custom_fake = F_Engine_Dispatch_SaveArgs::Func;
    }

    virtual void TearDown() override
    {
        if (F_Engine_Dispatch_SaveArgs::Data)
        {
            free(F_Engine_Dispatch_SaveArgs::Data);
            F_Engine_Dispatch_SaveArgs::Data = nullptr;
        }
        F_Engine_Dispatch_SaveArgs::Length = 0;
    }
};

TEST_F(TS_CellularDns_ResolveDomain, Ok)
{
    Cellular_DataContext_T dataContext;
    dataContext.Id = 1;
    const char *fqdn = "eclipse.org";
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    memset(&result.Address.IPv6, 0U, sizeof(result.Address.IPv6));

    Retcode_T rc = CellularDns_ResolveDomain(&dataContext, fqdn, &result);

    const struct CellularDns_ResolveDomainParam_S *param = reinterpret_cast<const struct CellularDns_ResolveDomainParam_S *>(F_Engine_Dispatch_SaveArgs::Data);
    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(1U, Engine_Dispatch_fake.call_count);
    ASSERT_EQ(sizeof(struct CellularDns_ResolveDomainParam_S), F_Engine_Dispatch_SaveArgs::Length);
    ASSERT_EQ(&dataContext, param->DataContext);
    ASSERT_STREQ(fqdn, param->Fqdn);
}

TEST_F(TS_CellularDns_ResolveDomain, InvalidInputDataContextFail)
{
    const char *fqdn = "eclipse.org";
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    memset(&result.Address.IPv6, 0U, sizeof(result.Address.IPv6));

    Retcode_T rc = CellularDns_ResolveDomain(NULL, fqdn, &result);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    ASSERT_EQ(0U, Engine_Dispatch_fake.call_count);
}

TEST_F(TS_CellularDns_ResolveDomain, InvalidInputFqdnFail)
{
    Cellular_DataContext_T dataContext;
    dataContext.Id = 1;
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    memset(&result.Address.IPv6, 0U, sizeof(result.Address.IPv6));

    Retcode_T rc = CellularDns_ResolveDomain(&dataContext, NULL, &result);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    ASSERT_EQ(0U, Engine_Dispatch_fake.call_count);
}

TEST_F(TS_CellularDns_ResolveDomain, InvalidInputResultFail)
{
    Cellular_DataContext_T dataContext;
    dataContext.Id = 1;
    const char *fqdn = "eclipse.org";

    Retcode_T rc = CellularDns_ResolveDomain(&dataContext, fqdn, NULL);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    ASSERT_EQ(0U, Engine_Dispatch_fake.call_count);
}

TEST_F(TS_CellularDns_ResolveDomain, EngineDispatchFail)
{
    Cellular_DataContext_T dataContext;
    dataContext.Id = 1;
    const char *fqdn = "eclipse.org";
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    memset(&result.Address.IPv6, 0U, sizeof(result.Address.IPv6));
    RESET_FAKE(Engine_Dispatch);
    Engine_Dispatch_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    Retcode_T rc = CellularDns_ResolveDomain(&dataContext, fqdn, &result);

    ASSERT_EQ(Engine_Dispatch_fake.return_val, rc);
    ASSERT_EQ(1U, Engine_Dispatch_fake.call_count);
}

namespace F_At_Set_UDNSRN_SaveArgsAndSetResult
{
AT_UDNSRN_Param_T Param;
AT_UDNSRN_Resp_T Result;

Retcode_T Func(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp)
{
    Param = *param;
    *resp = Result;
    return RETCODE_OK;
}
} // namespace F_At_Set_UDNSRN_SaveArgsAndSetResult

class TS_ResolveDomain : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(At_Set_UDNSRN);

        At_Set_UDNSRN_fake.custom_fake = F_At_Set_UDNSRN_SaveArgsAndSetResult::Func;
        memset(&F_At_Set_UDNSRN_SaveArgsAndSetResult::Result, 0, sizeof(F_At_Set_UDNSRN_SaveArgsAndSetResult::Result));
    }
};

TEST_F(TS_ResolveDomain, Ok)
{
    Cellular_DataContext_T dataContext;
    const char *fqdn = "eclipse.org";
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    struct CellularDns_ResolveDomainParam_S param;
    param.DataContext = &dataContext;
    param.Fqdn = fqdn;
    param.Result = &result;

    AT_UBlox_Address_T expUbloxAddr;
    expUbloxAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    expUbloxAddr.Address.IPv4[3] = 127;
    expUbloxAddr.Address.IPv4[2] = 0;
    expUbloxAddr.Address.IPv4[1] = 0;
    expUbloxAddr.Address.IPv4[0] = 1;
    F_At_Set_UDNSRN_SaveArgsAndSetResult::Result.DomainIpString.Ip = expUbloxAddr;

    Retcode_T rc = ResolveDomain(&param, sizeof(param));

    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(1U, At_Set_UDNSRN_fake.call_count);
    ASSERT_EQ(AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP, F_At_Set_UDNSRN_SaveArgsAndSetResult::Param.ResolutionType);
    ASSERT_STREQ(param.Fqdn, F_At_Set_UDNSRN_SaveArgsAndSetResult::Param.DomainIpString.Domain);
    ASSERT_EQ(CELLULAR_IPADDRESSTYPE_IPV4, param.Result->Type);
    for (size_t i = 0; i < sizeof(param.Result->Address.IPv4); ++i)
    {
        ASSERT_EQ(expUbloxAddr.Address.IPv4[i], param.Result->Address.IPv4[i]);
    }
}

TEST_F(TS_ResolveDomain, UdnsrnFail)
{
    Cellular_DataContext_T dataContext;
    const char *fqdn = "eclipse.org";
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    struct CellularDns_ResolveDomainParam_S param;
    param.DataContext = &dataContext;
    param.Fqdn = fqdn;
    param.Result = &result;

    RESET_FAKE(At_Set_UDNSRN);
    At_Set_UDNSRN_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    Retcode_T rc = ResolveDomain(&param, sizeof(param));

    ASSERT_EQ(At_Set_UDNSRN_fake.return_val, rc);
    ASSERT_EQ(1U, At_Set_UDNSRN_fake.call_count);
}

TEST_F(TS_ResolveDomain, InvalidIpResultFail)
{
    Cellular_DataContext_T dataContext;
    const char *fqdn = "eclipse.org";
    Cellular_IpAddress_T result;
    result.Type = CELLULAR_IPADDRESSTYPE_MAX;
    struct CellularDns_ResolveDomainParam_S param;
    param.DataContext = &dataContext;
    param.Fqdn = fqdn;
    param.Result = &result;

    AT_UBlox_Address_T expUbloxAddr;
    expUbloxAddr.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    F_At_Set_UDNSRN_SaveArgsAndSetResult::Result.DomainIpString.Ip = expUbloxAddr;

    Retcode_T rc = ResolveDomain(&param, sizeof(param));

    ASSERT_EQ(RETCODE_COMPOSE(KISO_PACKAGE_ID, KISO_CELLULAR_MODULE_ID_UBLOXUTILS, RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR), rc);
    ASSERT_EQ(1U, At_Set_UDNSRN_fake.call_count);
}
