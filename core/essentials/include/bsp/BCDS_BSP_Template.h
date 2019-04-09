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
 * @defgroup    BCDS_HAL_BSP_Template Template
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for Template device from Template Manufacturer.
 * <A HREF="https://www.bosch-sensortec.com/bst/products/products_overview"><B>Datasheet</B></A>
 * @details
 * After reset, the micro	controller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Template_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Template_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The device is then **controllable** through the microcontroller.
 *
 * BSP_Template_Disable() will revert back what the BSP_Tempplate_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_Template_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_Template_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the accelerometer. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Template_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Template_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Template_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Template_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Template_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Template_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Template_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_Template_H_
#define BCDS_BSP_Template_H_

#include "BCDS_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if 1
//#if BCDS_FEATURE_BSP_TEMPLATE

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function maps the GPIO pins to their desired functions to drive the Template device.
 *              Basic input/output operations are allowed after the successful execution of this function.
 *
 * @param[in]   deviceId: Id of the targeted device. The device id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Template_Connect(int32_t deviceId);

/**
 * @brief       Enables the control over the device.
 *
 * @details     This function will execute the required sequence to enable the device and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_Template_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. The device id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Template_Enable(int32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the device and if possible it will power it off.
 *
 * @param[in]   deviceId: Id of the targeted device. The device id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Template_Disable(int32_t deviceId);

/**
 * @brief       Disconnects the peripheral.
 *
 * @details     This function will tristate the GPIOs of the template, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Template_Disconnect(int32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the peripheral.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation which is MCU specific.
 *
 * @return      HWHandle: Handle to the SPI interface
 */
HWHandle_T BSP_Template_GetHandle(int32_t deviceId);

#endif /*BCDS_FEATURE_BSP_TEMPLATE*/

#endif /* BCDS_BSP_Template_H_ */

/**@}*/
