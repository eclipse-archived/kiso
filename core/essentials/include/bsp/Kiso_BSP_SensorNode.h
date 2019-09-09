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
 * @defgroup    KISO_HAL_BSP_Sensor_Node Sensor Node
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       Generic interface to the sensor node BSP API.
 *
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_SensorNode_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_SensorNode_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_SensorNode_Disable() will revert back what the BSP_SensorNode_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_SensorNode_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_SensorNode_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the sensor node
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_SensorNode_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_SensorNode_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_SensorNode_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_SensorNode_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_SensorNode_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_SensorNode_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_SensorNode_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_SensorNode_H
#define KISO_BSP_SensorNode_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_SENSOR_NODE

/**
 * @brief       Type definition for the sensor interrupt callback
 *
 * @details     The application should pass a function pointer of the same signature (**void function(uint32_t,uint32_t)**)
 *              to BSP_SensorNode_Enable() in order to handle interrupts coming from dedicated sensor's interrupt
 *              lines.
 */
typedef void (*BSP_Sensor_InterruptCallback_T)(uint32_t, uint32_t);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_SensorNode_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the sensor and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_SensorNode_Connect() successfully executed.
 *
 * @param[in]   id identifier of the sensor targeted by the enable request.
 *
 * @param[in]   callback: If the sensor has dedicated interrupt lines a callback is required to process the generated
 *              events.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SensorNode_Enable(uint32_t id, BSP_Sensor_InterruptCallback_T callback);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over requested sensor and if possible power it off.
 *
 * @param[in]   id: identifier of  the sensor targeted by the disable request. This is defined in the Board Interface
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SensorNode_Disable(uint32_t id);

/**
 * @brief       Disconnects the sensor node.
 *
 * @details     This function will tristate the GPIOs of the sensor node, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SensorNode_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the sensor node.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_SensorNode_GetI2CHandle(void);

/**
 * @brief       Returns the boolean (TRUE/FALSE) based on the sensor node Enable/Disable status used by the peripheral
 *              in case I2C is used for the sensor node.
 *
 * @details     Only after successful Disable of all the sensor nodes this API will return TRUE. Even if one sensor node
 *              is enabled then we get FALSE from this API
 *
 * @return      Bool: Boolean (TRUE/FALSE).
 */
bool BSP_SensorNode_IsAllNodesDisabled(void);

#endif /* KISO_FEATURE_BSP_SENSOR_NODE */

#endif /* KISO_BSP_SensorNode_H */

/**  @} */
