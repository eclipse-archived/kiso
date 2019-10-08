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
 * @defgroup    KISO_HAL_BSP_Sensor_TMP116 Temperature Sensor TMP116
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for TMP116 temperature sensor from Texas Instruments.
 * <A HREF="http://www.ti.com/lit/ds/symlink/tmp116.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_TMP116_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_TMP116_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_TMP116_Disable() will revert back what the BSP_TMP116_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_TMP116_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration
 * e.g. high impedance.
 *
 * BSP_TMP116_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the TMP112 temperature sensor:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_TMP116_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_TMP116_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_TMP116_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_TMP116_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_TMP116_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_TMP116_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_TMP116_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_TMP116_H_
#define KISO_BSP_TMP116_H_

#include "Kiso_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_TMP116

/**
 * @brief       Type definition for the sensor interrupt callback
 *
 * @details     The application should pass a function pointer of the same signature (**void function(int32_t,uint32_t)**)
 *              to BSP_TMP116_AlertEnable() in order to handle interrupts coming from ALERT lines.
 */
typedef void (*BSP_TMP116_InterruptCallback_T)(int32_t deviceId, uint32_t interruptEdge);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP116_Connect(int32_t deviceId);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the sensor and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_TMP116_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. This id is given by the BSP in
 *              your-bsp-repoinclude/BSP_<yourBoardName>.h
 * @note        deviceId=0 is reserved to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP116_Enable(int32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the sensor and if possible power it off.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP116_Disable(int32_t deviceId);

/**
 * @brief       Disconnects the temperature sensor.
 *
 * @details     This function will tristate the GPIOs of the sensor, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in your-bsp-repoinclude/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_TMP116_Disconnect(int32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the temperature sensor.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @param       deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in your-bsp-repoinclude/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_TMP116_GetHandle(int32_t deviceId);

/**
 * @brief       Enables ALERT interrupt handling for the peripheral.
 *
 * @details     This function allows interrupting the MCU on level change at ALERT pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_TMP116_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @param[in]   callback Function passed by the application to process interrupts coming from interrupt line.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP116_AlertEnable(int32_t deviceId, BSP_TMP116_InterruptCallback_T callback);

/**
 * @brief       Disables ALERT interrupt Handling.
 *
 * @details     This function once called will disable the ALERT interrupt handling at BSP level e.g. in the NVIC and
 *              will stop forwarding events to the application through the interrupt callback.
 *
 * @param[in]   deviceId    Device Id given by the BSP in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP116_AlertDisable(int32_t deviceId);

#endif /*KISO_FEATURE_BSP_TMP116*/
#endif /* KISO_BSP_TMP116_H_ */

/**@}*/ /* KISO_HAL_BSP_TMP116 */
