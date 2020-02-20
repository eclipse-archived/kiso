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
 * @ingroup KISO_CELLULAR
 * @defgroup KISO_CELLULAR_DNS DNS
 * @{
 * @brief Cellular Domain Name Lookup Service API.
 *
 * @details If the Cellular modem offers an internal DNS-lookup mechanism, this
 * data service will map a unified interface to this stack. In case the selected
 * Cellular variant does not provide an DNS-lookup capabilities, this API may
 * return a #Retcode_T indicating #RETCODE_NOT_SUPPORTED.
 *
 * @file
 */
#ifndef KISO_CELLULARDNSSERVICE_H_
#define KISO_CELLULARDNSSERVICE_H_

#include "Kiso_Cellular.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/**
 * @brief Resolve a Fully-Qualified Domain Name (FQDN) to a IP address.
 *
 * @param[in] dataContext
 * Data-context that selects which primary (or secondary) DNS server is
 * contacted during lookup.
 *
 * @param[in] fqdn
 * C-string of the Fully-Qualified Domain Name.
 *
 * @param[out] result
 * Will be set to the resolve IP address associated with the given FQDN.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T CellularDns_ResolveDomain(
    const Cellular_DataContext_T *dataContext,
    const char *fqdn,
    Cellular_IpAddress_T *result);

#endif /* KISO_CELLULARDNSSERVICE_H_ */

/** @} */
