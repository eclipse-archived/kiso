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
 * @defgroup    KISO_ESSENTIALS Essentials
 * @ingroup     KISO_CORE 
 * @{
 *
 * @brief       This package contains essential components and modules of the KISO software platform like the
 * hardware abstraction layer (HAL) including a microcontroller unit abstraction (MCU),
 * the interface for the board support package (BSP) as well as basic types, an assertion and a
 * return code support module.
 *              \image html Essentials-overview.png
 *
 */

#ifndef KISO_ESSENTIALS_H_
#define KISO_ESSENTIALS_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/**
 * @enum        KISO_Essentials_ModuleID_E
 *
 * @brief       KISO_MODULE_ID for each module in the ESSENTIALS package, used for the macros in Kiso_Retcode.h
 *
 * @details     usage: For a new module in this package, add a new ID value to this enum, then use it in the module's C
 *              file:
 *
 * @code
 *      #undef KISO_MODULE_ID
 *      #define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_xxx
 * @endcode
 */
enum KISO_Essentials_ModuleID_E
{
    KISO_ESSENTIALS_MODULE_ID_ASSERT = 1,
    KISO_ESSENTIALS_MODULE_ID_RETCODE,

    KISO_ESSENTIALS_MODULE_ID_UART,
    KISO_ESSENTIALS_MODULE_ID_SPI,
    KISO_ESSENTIALS_MODULE_ID_I2C,
    KISO_ESSENTIALS_MODULE_ID_FLASH,
    KISO_ESSENTIALS_MODULE_ID_PWM,
    KISO_ESSENTIALS_MODULE_ID_WATCHDOG,
    KISO_ESSENTIALS_MODULE_ID_LOCK,
    KISO_ESSENTIALS_MODULE_ID_CRITICALSECTION,
    KISO_ESSENTIALS_MODULE_ID_FAULTHANDLER,
    KISO_ESSENTIALS_MODULE_ID_LEUART,
    KISO_ESSENTIALS_MODULE_ID_SLEEP,
    KISO_ESSENTIALS_MODULE_ID_TIMER,
    KISO_ESSENTIALS_MODULE_ID_SIGNAL,
    KISO_ESSENTIALS_MODULE_ID_DELAY,
    KISO_ESSENTIALS_MODULE_ID_TIMESTAMP,
    KISO_ESSENTIALS_MODULE_ID_DMA,
    KISO_ESSENTIALS_MODULE_ID_GPIO,
    KISO_ESSENTIALS_MODULE_ID_RNG,
    KISO_ESSENTIALS_MODULE_ID_CRC,
    KISO_ESSENTIALS_MODULE_ID_FLASH_INTERN,
};

#endif /* KISO_ESSENTIALS_H_ */
       /**@} */