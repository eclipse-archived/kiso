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
 * @file
 * @brief Small utility header that contains the module IDs used by Cellular.
 */
#ifndef KISO_CELLULARMODULES_H_
#define KISO_CELLULARMODULES_H_

/**
 * @brief Cellular Module-Ids
 */
enum KISO_CELLULAR_ModuleID_E
{
    KISO_CELLULAR_MODULE_ID_ATPARSER = 1,
    KISO_CELLULAR_MODULE_ID_ATQUEUE,
    KISO_CELLULAR_MODULE_ID_HARDWARE,
    KISO_CELLULAR_MODULE_ID_INIT,
    KISO_CELLULAR_MODULE_ID_POWER,
    KISO_CELLULAR_MODULE_ID_SIM,
    KISO_CELLULAR_MODULE_ID_URC,
    KISO_CELLULAR_MODULE_ID_NETWORK,
    KISO_CELLULAR_MODULE_ID_ENGINE,
    KISO_CELLULAR_MODULE_ID_UTILS,
    KISO_CELLULAR_MODULE_ID_UBLOXUTILS,
    KISO_CELLULAR_MODULE_ID_AT_3GPP_27007,
    KISO_CELLULAR_MODULE_ID_AT_UBLOX,
    KISO_CELLULAR_MODULE_ID_QUEUE,
    KISO_CELLULAR_MODULE_ID_DNS_SERVICE,
    KISO_CELLULAR_MODULE_ID_SMS_SERVICE,
    KISO_CELLULAR_MODULE_ID_SOCKET_SERVICE,
    KISO_CELLULAR_MODULE_ID_HTTP_SERVICE
};

#endif /* KISO_CELLULARMODULES_H_ */
