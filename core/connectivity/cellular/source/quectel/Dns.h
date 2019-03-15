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
 *
 * @file
 *
 * @brief Resolves a domain name to an IP address via the network provided DNS server.
 *
 * @details Here is a detailed description about the module
 **/
 
#ifndef QCTEL_DNS_H
#define QCTEL_DNS_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/**
 * @brief Resolves a domain name to an IP address via the network provided DNS server.
 *
 * @param[in]   DomainName          The fully qualified domain name to resolve (e.g., google.com)
 * @param[in]   DomainNameLength    The length of the domain name in bytes
 * @param[out]  IpAddress           The resolved IP address if the return value was RETCODE_OK
 *
 * @retval      RETCODE_OK          The domain name was resolved correctly
 * @retval      RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond to the request
 * @retval      RETCODE_INVALID_PARAM If one of the paramters was invalid (e.g., the domain name was NULL)
 * @retval      RETCODE_FAILURE     If the string returned by the modem could not be parsed to an IP address
 */
Retcode_T Cellular_ResolveDomainName(uint8_t *DomainName, uint32_t DomainNameLength, uint32_t *IpAddress);

#endif /* QCTEL_DNS_H */
