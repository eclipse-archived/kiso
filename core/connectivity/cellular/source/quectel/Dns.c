/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

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
