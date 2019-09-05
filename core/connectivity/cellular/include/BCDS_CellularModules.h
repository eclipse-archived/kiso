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
 * @brief Small utility header that contains the module IDs used by Cellular.
 */
#ifndef BCDS_CELLULARMODULES_H_
#define BCDS_CELLULARMODULES_H_

/**
 * @brief Cellular Module-Ids
 */
enum BCDS_CELLULAR_ModuleID_E
{
    BCDS_CELLULAR_MODULE_ID_ATPARSER = 1,
    BCDS_CELLULAR_MODULE_ID_ATQUEUE,
    BCDS_CELLULAR_MODULE_ID_HARDWARE,
    BCDS_CELLULAR_MODULE_ID_INIT,
    BCDS_CELLULAR_MODULE_ID_POWER,
    BCDS_CELLULAR_MODULE_ID_SIM,
    BCDS_CELLULAR_MODULE_ID_URC,
    BCDS_CELLULAR_MODULE_ID_NETWORK,
    BCDS_CELLULAR_MODULE_ID_DNS,
    BCDS_CELLULAR_MODULE_ID_ENGINE,
    BCDS_CELLULAR_MODULE_ID_UTILS,
    BCDS_CELLULAR_MODULE_ID_UBLOXUTILS,
    BCDS_CELLULAR_MODULE_ID_AT_3GPP_27007,
    BCDS_CELLULAR_MODULE_ID_AT_UBLOX,
    BCDS_CELLULAR_MODULE_ID_SMS_SERVICE,
    BCDS_CELLULAR_MODULE_ID_SOCKET_SERVICE,
    BCDS_CELLULAR_MODULE_ID_HTTP_SERVICE
};

#endif /* BCDS_CELLULARMODULES_H_ */

