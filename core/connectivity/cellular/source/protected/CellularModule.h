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
 * @brief Internal header that defines module id's.
 */

#ifndef CELLULAR_MODULE_H
#define CELLULAR_MODULE_H

/**
 * @brief BCDS_MODULE_ID for each Cellular C module
 */
enum BCDS_CELLULAR_ModuleID_E
{
    BCDS_CELLULAR_MODULE_ID_ATPARSER = 1,
    BCDS_CELLULAR_MODULE_ID_ATQUEUE,
    BCDS_CELLULAR_MODULE_ID_BSP,
    BCDS_CELLULAR_MODULE_ID_CONTROL,
    BCDS_CELLULAR_MODULE_ID_DNS,
    BCDS_CELLULAR_MODULE_ID_ENGINE,
    BCDS_CELLULAR_MODULE_ID_GSM,
    BCDS_CELLULAR_MODULE_ID_LOG,
    BCDS_CELLULAR_MODULE_ID_LOG_USART,
    BCDS_CELLULAR_MODULE_ID_NETWORK,
    BCDS_CELLULAR_MODULE_ID_SMS,
    BCDS_CELLULAR_MODULE_ID_SOCKET,
    BCDS_CELLULAR_MODULE_ID_URC,
    BCDS_CELLULAR_MODULE_ID_UTILS
};

#endif /* CELLULAR_MODULE_H */
