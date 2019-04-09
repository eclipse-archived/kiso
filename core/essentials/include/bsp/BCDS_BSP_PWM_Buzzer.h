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
 * @defgroup    BCDS_HAL_BSP_PWM_BUZZER PWM Buzzer
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for PWM controlled buzzers.
 *
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_PWM_Buzzer_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_PWM_Buzzer_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_PWM_Buzzer_Disable() will revert back what the BSP_PWM_Buzzer_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_PWM_Buzzer_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_PWM_Buzzer_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the PWM buzzer:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_PWM_Buzzer_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_PWM_Buzzer_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_PWM_Buzzer_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_PWM_Buzzer_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_PWM_Buzzer_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_PWM_Buzzer_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_PWM_Buzzer_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_PWM_Buzzer_H
#define BCDS_BSP_PWM_Buzzer_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_PWM_BUZZER

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_PWM_Buzzer_Connect(void);

/**
 * @brief       Enables the control over the buzzer
 *
 * @details     This function once called will power up and allow the buzzer to receive Volume and frequency commands
 *              otherwise no effect for those commands.
 *
 * @pre         BSP_PWM_Buzzer_Connect() successfully executed.
 *
 * @param       id: identifier of the buzzer to switch
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_Buzzer_Enable(uint32_t id);

/**
 * @brief       Disables the volume an frequency control over the buzzer.
 *
 * @param       id: identifier of the buzzer to switch
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_Buzzer_Disable(uint32_t id);

/**
 * @brief       Disconnects the buzzer.
 *
 * @details     This function will tristate the GPIOs of the buzzer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_Buzzer_Disconnect(void);

/**
 * @brief       changes the volume and sound effect of the buzzer.
 *
 * @details     This function is called to change the volume from 0% to 100% and frequency from 2kHz to 22kHz of the
 *              buzzer.
 *
 * @param       id: identifier of the buzzer to switch
 *
 * @param       volumePercent: volume control
 *
 * @param       frequencyHz: frequency control
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_Buzzer_Switch(uint32_t id, uint32_t volumePercent,uint32_t frequencyHz);

#endif /* BCDS_FEATURE_BSP_PWM_BUZZER*/

#endif /* BCDS_BSP_PWM_Buzzer_H */

/**  @} */
