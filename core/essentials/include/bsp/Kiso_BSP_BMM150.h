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
 * @defgroup    KISO_HAL_BSP_Sensor_BMM150 Magnetometer BMM150
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for BMM150 Magnetometer sensor from Bosch Sensortec.
 * <A HREF="https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BMM150-DS001.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the magnetometer is requested a call to
 * BSP_BMM150_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_BMM150_Enable() will execute the required sequence to enable the magnetometer and its dedicated
 * MCU resources (e.g. SPI). The magnetometer is then **controllable** through the microcontroller.
 *
 * BSP_BMM150_Disable() will revert back what the BSP_BMM150_Enable() has done and set the
 * magnetometer and its related MCU resources into disabled state.
 *
 * BSP_BMM150_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration
 * e.g. high impedance.
 *
 * BSP_BMM150_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the magnetometer. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the BMM150 magnetometer :
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_BMM150_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_BMM150_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_BMM150_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_BMM150_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_BMM150_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_BMM150_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_BMM150_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_BMM150_H_
#define KISO_BSP_BMM150_H_

#include "Kiso_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_BMM150

/**
 * @brief       Type definition for the sensor interrupt callback
 *
 * @details     The application should pass a function pointer of the same signature (**void function(int32_t,uint32_t)**)
 *              to BSP_BMM150_IntEnable() and BSP_BMM150_DrdyEnable() in order to handle interrupts
 *              coming from INT or DRDY lines.
 */
typedef void (*BSP_BMM150_InterruptCallback_T)(int32_t deviceId, uint32_t interruptEdge);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the magnetometer.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @param[in]   deviceId: Id of the targeted magnetometer. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_Connect(int32_t deviceId);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the magnetometer and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. This id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is reserved to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_Enable(int32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the magnetometer and if possible power it off.
 *
 * @param[in]   deviceId: Id of the targeted magnetometer. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_Disable(int32_t deviceId);

/**
 * @brief       Disconnects the magnetometer.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted magnetometer. The device id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_Disconnect(int32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the peripheral.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HAL_DeviceHandle_T: Handle for operating all devices of this type in the board.
 */
HWHandle_T BSP_BMM150_GetHandle(int32_t deviceId);

/***
 * @brief       Returns a reference to the initialization parameters
 *
 */
void * BSP_BMM150_GetInitParams(int32_t deviceId);

/**
 * @brief       Enables INT interrupt handling for the peripheral.
 *
 * @details     This function allows interrupting the MCU on level change at INT pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted magnetometer. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @param[in]   callback Function passed by the application to process interrupts coming from interrupt line.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_IntEnable(int32_t deviceId, BSP_BMM150_InterruptCallback_T callback);

/**
 * @brief       Disables INT interrupt Handling.
 *
 * @details     This function once called will disable the INT interrupt handling at BSP level e.g. in the NVIC and
 *              will stop forwarding events to the application through the interrupt callback.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @param[in]   deviceId    Device Id given by the BSP in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_IntDisable(int32_t deviceId);

/**
 * @brief       Enables DRDY (data ready) interrupt handling  for the peripheral.
 *
 *
 * @details     This function allows interrupting the MCU on level change at DRDY pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @param[in]   callback Function passed by the application to process DRDY interrupts.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_DrdyEnable(int32_t deviceId, BSP_BMM150_InterruptCallback_T callback);

/**
 * @brief       Disables DRDY interrupt handling.
 *
 * @details     This function once called will disable the DRDY interrupt handling at BSP level e.g. in the NVIC and
 *              will stop forwarding events to the application through the interrupt callback.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @param[in]   deviceId    Device Id given by the BSP in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BMM150_DrdyDisable(int32_t deviceId);

/**
 * @brief       Sets Chip-Select pin to high level.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BMM150_SetCSHigh(int32_t deviceId);

/**
 * @brief       Sets Chip-Select pin to low level.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_BMM150_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BMM150_SetCSLow(int32_t deviceId);

/**@}*//* KISO_HAL_BSP_BMM150 */

#endif /*KISO_FEATURE_BSP_BMM150*/
#endif /* KISO_BSP_BMM150_H_ */
