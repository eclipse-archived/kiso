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
 * @defgroup KISO_HAL_BSP_GenericI2c GenericI2c
 * @ingroup KISO_HAL_BSP_IF
 * @{
 * @brief BSP API for a generic i2c communication device.
 *
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_GenericI2c_Connect() function is required to map its GPIOs to their desired function (input, output, I2C, etc).
 *
 * A call to BSP_GenericI2c_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. I2C). The Device is then **controllable** through the microcontroller.
 *
 * BSP_GenericI2c_Disable() will revert back what the BSP_GenericI2c_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_GenericI2c_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_GenericI2c_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the test interface:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_GenericI2c_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_GenericI2c_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_GenericI2c_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_GenericI2c_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_GenericI2c_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_GenericI2c_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_GenericI2c_Disconnect()"]
 *  }
 * @enddot
 * 
 */

#ifndef KISO_BSP_GENERICI2C_H
#define KISO_BSP_GENERICI2C_H

#include "Kiso_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_GENERIC_I2C

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 * 
 * @param       id The identifier of the targeted I2C device. The device should be known to the application.
 * 
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_GenericI2c_Connect(uint32_t id);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the device and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_GenericI2c_Connect() successfully executed.
 * 
 * @param       id The identifier of the targeted I2C device. The device should be known to the application.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GenericI2c_Enable(uint32_t id);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the device.
 *
 * @param       id The identifier of the targeted I2C device. The device should be known to the application.
 * 
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GenericI2c_Disable(uint32_t id);

/**
 * @brief       Disconnects the peripheral.
 *
 * @details     This function will tristate the GPIOs of the test interface, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param       id The identifier of the targeted I2C device. The device should be known to the application.
 * 
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GenericI2c_Disconnect(uint32_t id);

/**
 * @brief       Returns the handle of the communication interface used by the device.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @param       id The identifier of the targeted I2C device. The device should be known to the application.
 * 
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_GenericI2c_GetHandle(uint32_t id);

/**
 * @brief       Executes a user defined control.
 *
 * @details     This function is optional, it shall implement user defined controls which will be exposed to the 
 *              application layer.
 * @warning     It is recommended that only the application uses user controls because those are defined in 
 *              BSP_<Board>.h which is not accessible by the Middleware drivers.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GenericI2c_UserControl(uint32_t control, void *param);

#endif /* KISO_FEATURE_BSP_GENERIC_I2C */

#endif /* KISO_BSP_GENERICI2C_H */

/**  @} */
