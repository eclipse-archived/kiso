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

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_UBLOXUTILS

#include "UBloxUtils.h"
#include "Kiso_Cellular.h"
#include "AT_UBlox.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

Retcode_T CelToUbloxAddr(const Cellular_IpAddress_T *from, AT_UBlox_Address_T *to)
{
    switch (from->Type)
    {
    case CELLULAR_IPADDRESSTYPE_IPV4:
        to->Type = AT_UBLOX_ADDRESSTYPE_IPV4;
        memcpy(to->Address.IPv4, from->Address.IPv4, sizeof(to->Address.IPv4));
        return RETCODE_OK;
    case CELLULAR_IPADDRESSTYPE_IPV6:
        to->Type = AT_UBLOX_ADDRESSTYPE_IPV6;
        memcpy(to->Address.IPv6, from->Address.IPv6, sizeof(to->Address.IPv6));
        return RETCODE_OK;
    default:
        to->Type = AT_UBLOX_ADDRESSTYPE_INVALID;
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }
}

Retcode_T UbloxToCelAddr(const AT_UBlox_Address_T *from, Cellular_IpAddress_T *to)
{
    switch (from->Type)
    {
    case AT_UBLOX_ADDRESSTYPE_IPV4:
        to->Type = CELLULAR_IPADDRESSTYPE_IPV4;
        memcpy(to->Address.IPv4, from->Address.IPv4, sizeof(to->Address.IPv4));
        return RETCODE_OK;
    case AT_UBLOX_ADDRESSTYPE_IPV6:
        to->Type = CELLULAR_IPADDRESSTYPE_IPV6;
        memcpy(to->Address.IPv6, from->Address.IPv6, sizeof(to->Address.IPv6));
        return RETCODE_OK;
    default:
        to->Type = CELLULAR_IPADDRESSTYPE_MAX;
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }
}
