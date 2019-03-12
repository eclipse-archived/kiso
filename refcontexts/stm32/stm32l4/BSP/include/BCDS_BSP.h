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

#ifndef BCDS_BSP_H_
#define BCDS_BSP_H_

/**
 * @defgroup BSPInfo Board Support Package - Package Information
 * @{
 * @file
 * @brief Package information header
 * @details The header defines the package and version IDs and provides a list
 * of private module IDs for the Retcode module APIs. The BSP package receives
 * its public interfaces from the HAL package. Related C module IDs are defined
 * by BCDS_HALInfo.h.
 */

/* Include all headers which are needed by this file. */

#include "BCDS_Retcode.h"
#include "BCDS_Essentials.h"

/* Put the type and macro definitions here */

/**
 * @brief BCDS_MODULE_ID for each private BSP C module
 * @info  Usage in C source files:
 *      #undef BCDS_MODULE_ID
 *      #define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_xxx
 */
enum BCDS_BSP_ModuleID_E
{
    BCDS_BSP_MODULE_ID_ANALOGSENSOR = 50,
    BCDS_BSP_MODULE_ID_LDO_TLV70233,
    BCDS_BSP_MODULE_ID_SYSTEMCLOCK,
    BCDS_BSP_MODULE_ID_WATCHDOG,

    /* Define next private Module ID here */
};

/* Put the function declarations here */

/**@} *//* @defgroup BSPInfo */
#endif /* BCDS_BSP_H_ */
