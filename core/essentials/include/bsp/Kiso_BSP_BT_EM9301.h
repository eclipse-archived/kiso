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
 * @defgroup    KISO_HAL_BSP_BT_EM9301 Bluetooth EM9301
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for EM9301 Bluetooth controller from EM Microelectronic.
 * <A HREF="https://www.emmicroelectronic.com/sites/default/files/products/datasheets/9301-DS_1.PDF"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the bluetooth controller is requested a call to
 * BSP_BT_EM9301_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_BT_EM9301_Enable() will execute the required sequence to enable the bluetooth controller and its dedicated
 * MCU resources (e.g. SPI). The device is then **controllable** through the microcontroller.
 *
 * BSP_BT_EM9301_Disable() will revert back what the BSP_BT_EM9301_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_BT_EM9301_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_BT_EM9301_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the bluetooth controller. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the bluetooth controller :
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_BT_EM9301_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_BT_EM9301_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_BT_EM9301_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_BT_EM9301_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_BT_EM9301_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_BT_EM9301_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_BT_EM9301_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_BT_EM9301_H
#define KISO_BSP_BT_EM9301_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_BT_EM9301

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @param       control parameter of the connect function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the bluetooth controller and allows it to
 *              receive, execute commands, and transmit data.
 *
 * @pre         BSP_BT_EM9301_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_Enable(void);

/**
 * @brief       Disables the control over the bluetooth controller.
 *
 * @details     This function once called will disable control over the accelerometer and if possible power it off
 *              as described in the datasheet.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_Disable(void);

/**
 * @brief       Disconnects the bluetooth controller.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interfaces used by the bluetooth controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_BT_EM9301_GetUARTHandle(void);

/**
 * @brief       Resets the peripheral.
 *
 * @details     This function once called will reset the EM9301 Bluetooth controller as described in the vendors
 *              datasheet
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_Reset(void);

/**
 * @brief       Sets the WU (Wake-Up) pin to high voltage level
 *
 * @details     This function sets the WU line to high voltage level as seen by the device in order to
 *              wakeup the controller in UART mode.
 *
 * @pre         BSP_BT_EM9301_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_SetWUHigh(void);

/**
 * @brief       Sets the WU (Wake-Up) pin to low voltage level
 *
 * @details     This function sets the WU line to low voltage level as seen by the device.
 *
 * @pre         BSP_BT_EM9301_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BT_EM9301_SetWULow(void);

#endif /* KISO_FEATURE_BSP_BT_EM9301 */

#endif /* KISO_BSP_BT_EM9301_H */

/**  @} */

