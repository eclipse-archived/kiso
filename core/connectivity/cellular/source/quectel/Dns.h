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
