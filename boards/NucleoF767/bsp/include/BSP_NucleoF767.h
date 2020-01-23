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
 * @defgroup    BSP_NUCLEOF767 BSP
 * @{
 * @brief       Nucleo F767 Board Support Package
 *
 * @details     Reflect the circuit-board and implements the needed interfaces present in \ref KISO_HAL_BSP_IF
 *
 * @warning     **maxm8** and **w25q64** were not tested. No Drivers were also implemented.
 *
 * @file
 */

#ifndef BSP_NUCLEOF767_H_
#define BSP_NUCLEOF767_H_

#include "Kiso_HAL.h"

/**
 * Module IDs provided by the BSP for retcode composition
 */
enum BSP_NUCLEOF767y_Modules_E
{
    /* source units */
    MODULE_BSP_API_BOARD,
    MODULE_BSP_API_LED,
    MODULE_BSP_API_TEST_IF,
    MODULE_BSP_TIME,
    MODULE_BSP_BUTTON,
};

/**
 * Return Codes provided by the BSP for retcode composition
 */
enum BSP_NucleoF767_Retcodes_E
{
    /* BSP Return codes */
    RETCODE_BSP_GENERIC_FAILURE = RETCODE_FIRST_CUSTOM_CODE, /**< Failure code not described below */
    RETCODE_BSP_I2C_INIT_FAILED,
    RETCODE_BSP_I2C_DEINIT_FAILED,
    RETCODE_BSP_I2C_RELEASE_FAILED,
    RETCODE_BSP_I2C_ACQUIRE_FAILED,
    RETCODE_BSP_UART_INIT_FAILED,
    RETCODE_BSP_UART_DEINIT_FAILED,
    RETCODE_BSP_SPI_INIT_FAILED,
    RETCODE_BSP_SPI_DEINIT_FAILED,
    RETCODE_BSP_BOARD_PWR_VOLTAGE_SCALING_FAILED,
    RETCODE_BSP_BOARD_OSCILLATORS_INIT_FAILED,
    RETCODE_BSP_BOARD_AMBA_CLOCK_INIT_FAILED,
    RETCODE_BSP_BOARD_PERIPHERALS_CLOCK_INIT_FAILED,
    RETCODE_BSP_TIME_INIT_FAILED,
    RETCODE_BSP_TIME_START_FAILED,
    RETCODE_BSP_USBD_INIT_FAILED,
    RETCODE_BSP_USBD_START_FAILED,
    RETCODE_BSP_USBD_CLASS_REGISTER_FAILED,
    RETCODE_BSP_USBD_INTERFACE_REGISTER_FAILED,
};

/**
 * @enum        IDs of available LEDs
 */
enum NucleoF767_LED_Ids_E
{
    NUCLEOF767_LED_ALL = 0,  /**< Id to address All LEDs */
    NUCLEOF767_LED_RED_ID,   /**< Id to address Red LED */
    NUCLEOF767_LED_GREEN_ID, /**< Id to address Green LED */
    NUCLEOF767_LED_BLUE_ID,  /**< Id to address Blue LED */
};

/**
 * @enum        IDs of available Buttons
 */
enum NucleoF767_Button_Ids_E
{
    NUCLEOF767_BUTTON_ALL = 0, /**< Id to address All Buttons */
    NUCLEOF767_BUTTON_USER     /**< Id to address User Button */

};

/**
 * @enum        Commands applicable for LED module
 */
enum NucleoF767_LED_Commands_E
{
    NUCLEOF767_LED_COMMAND_OFF,    //!< NUCLEOF767_LED_COMMAND_OFF
    NUCLEOF767_LED_COMMAND_ON,     //!< NUCLEOF767_LED_COMMAND_ON
    NUCLEOF767_LED_COMMAND_TOGGLE, //!< NUCLEOF767_LED_COMMAND_TOGGLE
};

/**
 * @enum        Enumeration of BSP states
 */
enum BSP_State_Transition_E
{
    BSP_STATE_TO_RESET = 0x01 << 0,        /**< The device can transit to reset state */
    BSP_STATE_TO_INIT = 0x01 << 1,         /**< The device can transit to initialized state */
    BSP_STATE_TO_CONNECTED = 0x01 << 2,    /**< The device can transit to connected state */
    BSP_STATE_TO_ENABLED = 0x01 << 3,      /**< The device can transit to enabled state */
    BSP_STATE_TO_DISABLED = 0x01 << 4,     /**< The device can transit to disabled state */
    BSP_STATE_TO_DISCONNECTED = 0x01 << 5, /**< The device can transit to disconnected state */
    BSP_STATE_TO_ERROR = 0x01 << 6,        /**< The device can transit to error state */
};

enum BSP_State_E
{
    BSP_STATE_RESET = 0x00,
    BSP_STATE_INIT = BSP_STATE_TO_CONNECTED,
    BSP_STATE_CONNECTED = BSP_STATE_TO_ENABLED | BSP_STATE_TO_DISCONNECTED | BSP_STATE_TO_ERROR, /**< The device is connected*/
    BSP_STATE_ENABLED = BSP_STATE_TO_DISABLED | BSP_STATE_TO_ERROR,                              /**< The device is enabled */
    BSP_STATE_DISABLED = BSP_STATE_TO_ENABLED | BSP_STATE_TO_DISCONNECTED | BSP_STATE_TO_ERROR,  /**< The device is at disabled state */
    BSP_STATE_DISCONNECTED = BSP_STATE_TO_CONNECTED | BSP_STATE_TO_ERROR,                        /**< The device is at disconnected state */
    BSP_STATE_ERROR = 0xFF,
};

/**
 * @def BSP_BOARD_DEFAULT_PARAM1
 * Default param1 for BSP_Board_Initialize()
 */
#define BSP_BOARD_DEFAULT_PARAM1 UINT32_C(0)
/**
 * @def BSP_BOARD_DEFAULT_PARAM2
 * Default param2 for BSP_Board_Initialize()
 */
#define BSP_BOARD_DEFAULT_PARAM2 NULL

#endif /* BSP_NUCLEOF767_H_ */

/**@}*/
