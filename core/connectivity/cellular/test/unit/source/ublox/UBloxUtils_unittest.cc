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
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_UBLOXUTILS

#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"

#undef KISO_MODULE_ID
#include "UBloxUtils.c"

#include <time.h>
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

class TS_CelToUbloxAddr : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        srand(time(nullptr));
    }
};

TEST_F(TS_CelToUbloxAddr, Ipv4Ok)
{
    Cellular_IpAddress_T fromAddr;
    fromAddr.Type = CELLULAR_IPADDRESSTYPE_IPV4;
    for (size_t i = 0; i < sizeof(fromAddr.Address.IPv4); ++i)
    {
        fromAddr.Address.IPv4[i] = (uint8_t)rand();
    }
    AT_UBlox_Address_T toAddr;
    memset(&toAddr, 0, sizeof(toAddr));
    toAddr.Type = AT_UBLOX_ADDRESSTYPE_INVALID;

    Retcode_T rc = CelToUbloxAddr(&fromAddr, &toAddr);

    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(AT_UBLOX_ADDRESSTYPE_IPV4, toAddr.Type);
    for (size_t i = 0; i < sizeof(fromAddr.Address.IPv4); ++i)
    {
        ASSERT_EQ(fromAddr.Address.IPv4[i], toAddr.Address.IPv4[i]);
    }
}

TEST_F(TS_CelToUbloxAddr, Ipv6Ok)
{
    Cellular_IpAddress_T fromAddr;
    fromAddr.Type = CELLULAR_IPADDRESSTYPE_IPV6;
    for (size_t i = 0; i < sizeof(fromAddr.Address.IPv6); ++i)
    {
        fromAddr.Address.IPv6[i] = (uint8_t)rand();
    }
    AT_UBlox_Address_T toAddr;
    memset(&toAddr, 0, sizeof(toAddr));
    toAddr.Type = AT_UBLOX_ADDRESSTYPE_INVALID;

    Retcode_T rc = CelToUbloxAddr(&fromAddr, &toAddr);

    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(AT_UBLOX_ADDRESSTYPE_IPV6, toAddr.Type);
    for (size_t i = 0; i < 8; ++i)
    {
        ASSERT_EQ(((uint16_t *)fromAddr.Address.IPv6)[i], toAddr.Address.IPv6[i]);
    }
}

TEST_F(TS_CelToUbloxAddr, InvalidIpTypeFail)
{
    Cellular_IpAddress_T fromAddr;
    fromAddr.Type = CELLULAR_IPADDRESSTYPE_MAX;
    AT_UBlox_Address_T toAddr;
    memset(&toAddr, 0, sizeof(toAddr));
    toAddr.Type = AT_UBLOX_ADDRESSTYPE_INVALID;

    Retcode_T rc = CelToUbloxAddr(&fromAddr, &toAddr);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR), rc);
    ASSERT_EQ(AT_UBLOX_ADDRESSTYPE_INVALID, toAddr.Type);
}

class TS_UbloxToCelAddr : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        FFF_RESET_HISTORY();

        srand(time(nullptr));
    }
};

TEST_F(TS_UbloxToCelAddr, Ipv4Ok)
{
    AT_UBlox_Address_T fromAddr;
    fromAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV4;
    for (size_t i = 0; i < sizeof(fromAddr.Address.IPv4); ++i)
    {
        fromAddr.Address.IPv4[i] = (uint8_t)rand();
    }
    Cellular_IpAddress_T toAddr;

    Retcode_T rc = UbloxToCelAddr(&fromAddr, &toAddr);

    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(CELLULAR_IPADDRESSTYPE_IPV4, toAddr.Type);
    for (size_t i = 0; i < sizeof(fromAddr.Address.IPv4); ++i)
    {
        ASSERT_EQ(fromAddr.Address.IPv4[i], toAddr.Address.IPv4[i]);
    }
}

TEST_F(TS_UbloxToCelAddr, Ipv6Ok)
{
    AT_UBlox_Address_T fromAddr;
    fromAddr.Type = AT_UBLOX_ADDRESSTYPE_IPV6;
    for (size_t i = 0; i < 8; ++i)
    {
        fromAddr.Address.IPv6[i] = (uint16_t)rand();
    }
    Cellular_IpAddress_T toAddr;

    Retcode_T rc = UbloxToCelAddr(&fromAddr, &toAddr);

    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(CELLULAR_IPADDRESSTYPE_IPV6, toAddr.Type);
    for (size_t i = 0; i < sizeof(fromAddr.Address.IPv6); ++i)
    {
        ASSERT_EQ(((uint8_t *)fromAddr.Address.IPv6)[i], toAddr.Address.IPv6[i]);
    }
}

TEST_F(TS_UbloxToCelAddr, InvalidIpTypeFail)
{
    AT_UBlox_Address_T fromAddr;
    fromAddr.Type = AT_UBLOX_ADDRESSTYPE_INVALID;
    Cellular_IpAddress_T toAddr;

    Retcode_T rc = UbloxToCelAddr(&fromAddr, &toAddr);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR), rc);
    ASSERT_EQ(CELLULAR_IPADDRESSTYPE_MAX, toAddr.Type);
}
