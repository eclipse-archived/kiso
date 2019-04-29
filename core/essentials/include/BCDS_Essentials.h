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
 *
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

/**
 * @brief       Defines the number of return codes that can be used by each HAL component.
 *
 * @details     Defines the number of return codes that can be defined for each HAL component.
 */
#define HAL_RETURN_CODE_RANGE   (UINT32_C(50))

/**
 * @brief       Declares the return codes used by the HAL components
 *
 * @details     For each component there is a defined range of return codes reserved. The maximum number of codes
 *              available for a component is defined by HAL_RETURN_CODE_RANGE.
 *
 * @todo        CDDKv2 move to API headers
 */
enum BCDS_HAL_HalRetcode_E
{
    /* HAL GENERAL RETURN CODES */
    RETCODE_HAL_GENERAL = RETCODE_FIRST_CUSTOM_CODE,
    /* ... */
    RETCODE_HAL_GENERAL_MAX = RETCODE_HAL_GENERAL + HAL_RETURN_CODE_RANGE - 1,

    /* HAL UART RETURN CODES */
    RETCODE_UART_GENERAL, /**< Start of UART return codes */
    RETCODE_UART_INIT_FAIL, /**<*/
    RETCODE_UART_HAL_ERROR,
    RETCODE_UART_HAL_BUSY,
    RETCODE_UART_HAL_TIMEOUT,
    RETCODE_UART_DMA_ERROR,
    RETCODE_UART_MAX = RETCODE_UART_GENERAL + HAL_RETURN_CODE_RANGE - 1, /**< End of UART return codes */

    /* HAL LEUART RETURN CODES */
    RETCODE_LEUART_GENERAL, /**< Start of LEUART return codes */
    RETCODE_LEUART_INIT_FAIL,
    RETCODE_LEUART_HAL_ERROR,
    RETCODE_LEUART_HAL_BUSY,
    RETCODE_LEUART_HAL_TIMEOUT,
    RETCODE_LEUART_DMA_ERROR,
    RETCODE_LEUART_MAX = RETCODE_LEUART_GENERAL + HAL_RETURN_CODE_RANGE - 1, /**< End of LEUART return codes */

    /* HAL I2C RETURN CODES */
    RETCODE_IC2_GENERAL, /**< General I2C return code */

    /* ... */
    RETCODE_IC2_MAX = RETCODE_IC2_GENERAL + HAL_RETURN_CODE_RANGE - 1, /**< End of I2C return codes  */

    /* HAL SLEEP RETURN CODES */
    RETCODE_SLEEP_GENERAL, /**< General Sleep return code */
    RETCODE_SLEEP_NOSLEEP, /**< No Sleep Possible */
    RETCODE_SLEEP_MAX = RETCODE_SLEEP_GENERAL + HAL_RETURN_CODE_RANGE - 1, /**< End of SLEEP return codes */

    /* HAL TIMER RETURN CODES */
    RETCODE_TIMER_GENERAL, /**< General Timer return code */
    RETCODE_TIMER_NOHWHANDLE, /**< No hardware handle was provided */
    RETCODE_TIMER_NOINIT, /**< Timer handle was not initialized */
    RETCODE_TIMER_ALREADY_INITIALIZED,
    RETCODE_TIMER_IS_ACTIVE,
    RETCODE_TIMER_IS_NOT_ACTIVE,
    RETCODE_TIMER_INIT_FAIL,
    RETCODE_TIMER_DEINIT_FAIL,
    RETCODE_TIMER_START_FAIL,
    RETCODE_TIMER_STOP_FAIL,
    RETCODE_TIMER_MAX = RETCODE_TIMER_GENERAL + HAL_RETURN_CODE_RANGE - 1, /**< End of TIMER return codes */

    /* HAL RNG RETURN CODES  */
    RETCODE_RNG_HAL_ERROR,
    RETCODE_RNG_HAL_BUSY,
    RETCODE_RNG_HAL_TIMEOUT,

    /* HAL MCU CRC RETURN CODES  */
    RETCODE_CRC_HAL_INIT_FAIL,
    RETCODE_CRC_HAL_DEINIT_FAIL,

/* Put new declarations of component return codes here */
};

#endif /* BCDS_ESSENTIALS_H_ */
/**@} */
/* End of documentation group BCDS_ESSENTIALS */

