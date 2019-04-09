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
 * @brief Resolves a domain name to an IP address via the network provided DNS server.
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_DNS

#include "CellularModule.h"
#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "Dns.h"

Retcode_T Cellular_ResolveDomainName(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress)
{
    BCDS_UNUSED(DomainName);
    BCDS_UNUSED(DomainNameLength);
    BCDS_UNUSED(IpAddress);

    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}
