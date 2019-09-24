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
 * @defgroup    KISO_HAL_BSP_BMA280 Accelerometer BMA280
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for BMA280 acceleration sensor from Bosch Sensortec.
 * <A HREF="https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BMA280-DS000.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the accelerometer is requested a call to
 * BSP_BMA280_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_BMA280_Enable() will execute the required sequence to enable the accelerometer and its dedicated
 * MCU resources (e.g. SPI). The accelerometer is then **controllable** through the microcontroller.
 *
 * BSP_BMA280_Disable() will revert back what the BSP_BMA280_Enable() has done and set the
 * accelerometer and its related MCU resources into disabled state.
 *
 * BSP_BMA280_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_BMA280_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the accelerometer. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the H3LIS accelerometer
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_BMA280_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_BMA280_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_BMA280_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_BMA280_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_BMA280_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_BMA280_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_BMA280_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_BMA280_H_
#define KISO_BSP_BMA280_H_

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_BMA280

/**
 * @brief       Type definition for the sensor interrupt callback
 *
 * @details     The application should pass a function pointer of the same signature (**void function(int32_t, uint32_t)**)
 *              to BSP_ADXL372_Int1Enable() and BSP_ADXL372_Int2Enable() in order to handle interrupts coming from
 *              INT1 or INT2 lines.
 */
typedef void (*BSP_BMA280_InterruptCallback_T)(int32_t deviceId, uint32_t interruptEdge);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the accelerometer.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @param[in]   deviceId: Id of the targeted device. The device id is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Connect(int32_t deviceId);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the accelerometer and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_BMA280_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. This id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is reserved to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Enable(int32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the accelerometer and if possible power it off.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Disable(int32_t deviceId);

/**
 * @brief       Disconnects the accelerometer.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Disconnect(int32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the accelerometer.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @param       deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_BMA280_GetHandle(int32_t deviceId);

/**
 * @brief       Executes a control command for the BMA280 sensor.
 *
 * @details     This function executes a special control command implemented by the BSP for the application.
 *
 * @param[in]   command: Command to execute defined by the BSP implementation in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @param[in]   arg: Optional argument of the command to execute, defined by the BSP implementation
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BMA280_Control(uint32_t command, void *arg);

/**
 * @brief       Enables INT1 interrupt handling for the peripheral.
 *
 * @details     This function allows interrupting the MCU on level change at INT1 pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_BMA280_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @param[in]   callback Function passed by the application to process interrupts coming from interrupt line.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Int1Enable(int32_t deviceId, BSP_BMA280_InterruptCallback_T callback);

/**
 * @brief       Disables INT1 interrupt Handling.
 *
 * @details     This function once called will disable the INT1 interrupt handling at BSP level e.g. in the NVIC and
 *              will stop forwarding events to the application through the interrupt callback.
 *
 * @pre         BSP_BMA280_Connect() successfully executed.
 *
 * @param[in]   deviceId    Device Id given by the BSP in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Int1Disable(int32_t deviceId);

/**
 * @brief       Enables INT2 interrupt handling for the peripheral.
 *
 * @details     This function allows interrupting the MCU on level change at INT2 pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_BMA280_Connect() successfully executed.
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
Retcode_T BSP_BMA280_Int2Enable(int32_t deviceId, BSP_BMA280_InterruptCallback_T callback);

/**
 * @brief       Disables INT2 interrupt Handling.
 *
 * @details     This function once called will disable the INT2 interrupt handling at BSP level e.g. in the NVIC and
 *              will stop forwarding events to the application through the interrupt callback.
 *
 * @param[in]   deviceId    Device Id given by the BSP in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMA280_Int2Disable(int32_t deviceId);

/**
 * @brief       Sets Chip-Select pin to high level.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_BMA280_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BMA280_SetCSHigh(int32_t deviceId);

/**
 * @brief       Sets Chip-Select pin to low level.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_BMA280_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BMA280_SetCSLow(int32_t deviceId);

/**@}*/ /* KISO_HAL_BSP_BMA280 */

#endif /*KISO_FEATURE_BSP_BMA280*/
#endif /* KISO_BSP_BMA280_H_ */
