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
