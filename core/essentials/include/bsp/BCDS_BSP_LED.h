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
 * @defgroup    BCDS_HAL_BSP_LED LED
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for LEDs
 *
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_LED_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_LED_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_LED_Disable() will revert back what the BSP_LED_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_LED_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_LED_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the LEDs
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_LED_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_LED_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_LED_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_LED_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_LED_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_LED_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_LED_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_LED_H
#define BCDS_BSP_LED_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_LED

enum BSP_LED_Retcode_E
{
    RETCODE_LED_DISABLED = RETCODE_FIRST_CUSTOM_CODE,
};

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_LED_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will enable the LED identified by id and allows it to be switched On/Off.
 *
 * @pre         BSP_LED_Connect() successfully executed.
 *
 * @param[in]   id: identifier of the LED to be enabled.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_Enable(uint32_t id);

/**
 * @brief       Disables the control over an LED.
 *
 * @param       id : identifier of the desired LED: This is defined in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_Disable(uint32_t id);

/**
 * @brief       Disconnects the LED.
 *
 * @details     This function will tristate the GPIOs of the LEDs, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_Disconnect(void);

/**
 * @brief       Enables the control over all LEDs.
 *
 * @details     This function once called will allow the user to control all on-board LEDs.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_EnableAll(void);

/**
 * @brief       Disables the control over all LEDs.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_DisableAll(void);

/**
 * @brief       Switches on/off a particular LED.
 *
 * @details     This function is called to change the LED State.
 *
 * @param[in]   id : identifier of the desired led to switch
 *
 * @param[in]   command : The command to execute (e.g. switch ON, switch OFF, Toggle, etc. )
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_Switch(uint32_t id, uint32_t command);

/**
 * @brief       Switches on/off all LEDs.
 *
 * @details     This function is called to change the State of all LEDs.
 *
 * @param       command : The command to execute (e.g. switch ON, switch OFF, etc. )
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_SwitchAll(uint32_t command);

#endif /* BCDS_FEATURE_BSP_LED */

#endif /* BSP_LED_H */

/**@}*/
