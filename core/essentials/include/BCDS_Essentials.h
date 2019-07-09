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
 *
 * @file
 * @defgroup    BCDS_ESSENTIALS Essentials
 * @{
 *
 * @brief       Provides supported enumerations for Essentials component
 * 				\image html Essentials-overview.png
 * @details This package contains essential components and modules of the BCDS software platform like the
 * hardware abstraction layer (HAL) including a microcontroller unit abstraction (MCU),
 * the interface for the board support package (BSP) as well as basic types, an assertion and a
 * return code support module.
 *
 */

#ifndef BCDS_ESSENTIALS_H_
#define BCDS_ESSENTIALS_H_

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

/**
 * @enum        BCDS_Essentials_ModuleID_E
 *
 * @brief       BCDS_MODULE_ID for each module in the ESSENTIALS package, used for the macros in BCDS_Retcode.h
 *
 * @details     usage: For a new module in this package, add a new ID value to this enum, then use it in the module's C
 *              file:
 *
 * @code
 *      #undef BCDS_MODULE_ID
 *      #define BCDS_MODULE_ID BCDS_ESSENTIALS_MODULE_ID_xxx
 * @endcode
 */
enum BCDS_Essentials_ModuleID_E
{
    BCDS_ESSENTIALS_MODULE_ID_ASSERT = 1,
    BCDS_ESSENTIALS_MODULE_ID_RETCODE,

    BCDS_ESSENTIALS_MODULE_ID_UART,
    BCDS_ESSENTIALS_MODULE_ID_SPI,
    BCDS_ESSENTIALS_MODULE_ID_I2C,
    BCDS_ESSENTIALS_MODULE_ID_FLASH,
    BCDS_ESSENTIALS_MODULE_ID_PWM,
    BCDS_ESSENTIALS_MODULE_ID_WATCHDOG,
    BCDS_ESSENTIALS_MODULE_ID_LOCK,
    BCDS_ESSENTIALS_MODULE_ID_CRITICALSECTION,
    BCDS_ESSENTIALS_MODULE_ID_FAULTHANDLER,
    BCDS_ESSENTIALS_MODULE_ID_LEUART,
    BCDS_ESSENTIALS_MODULE_ID_SLEEP,
    BCDS_ESSENTIALS_MODULE_ID_TIMER,
    BCDS_ESSENTIALS_MODULE_ID_SIGNAL,
    BCDS_ESSENTIALS_MODULE_ID_DELAY,
    BCDS_ESSENTIALS_MODULE_ID_TIMESTAMP,
    BCDS_ESSENTIALS_MODULE_ID_DMA,
    BCDS_ESSENTIALS_MODULE_ID_GPIO,
    BCDS_ESSENTIALS_MODULE_ID_RNG,
    BCDS_ESSENTIALS_MODULE_ID_CRC,
    BCDS_ESSENTIALS_MODULE_ID_FLASH_INTERN,
};


#endif /* BCDS_ESSENTIALS_H_ */
/**@} */