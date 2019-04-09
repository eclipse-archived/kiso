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
 * @defgroup    BCDS_HAL_BSP_IRSensor_Node Infra Red Sensor
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for Infra Red sensor
 * @todo deprecate this API
 *
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_IRSensorNode_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_IRSensorNode_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_IRSensorNode_Disable() will revert back what the BSP_IRSensorNode_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_IRSensorNode_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_IRSensorNode_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the sensor device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_IRSensorNode_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_IRSensorNode_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_IRSensorNode_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_IRSensorNode_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_IRSensorNode_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_IRSensorNode_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_IRSensorNode_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_IRSensorNode_H
#define BCDS_BSP_IRSensorNode_H

#include "BCDS_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_IRSENSOR_NODE

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_IRSensorNode_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the sensor and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_IRSensorNode_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_IRSensorNode_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the sensor and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_IRSensorNode_Disable(void);

/**
 * @brief       Disconnects the sensor.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_IRSensorNode_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the sensor.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_IRSensorNode_GetI2CHandle(void);

#endif /* BCDS_FEATURE_BSP_IRSENSOR_NODE */

#endif /* BCDS_BSP_IRSensorNode_H */

/**  @} */
