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
 * @defgroup    KISO_HAL_BSP_BME280 Environmental Sensor BME280
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for BME280 Environmental sensor from Bosch Sensortec.
 * <A HREF="https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the sensor is requested a call to
 * BSP_BME280_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_BME280_Enable() will execute the required sequence to enable the sensor and its dedicated
 * MCU resources (e.g. SPI). The sensor is then **controllable** through the microcontroller.
 *
 * BSP_BME280_Disable() will revert back what the BSP_BME280_Enable() has done and set the
 * sensor and its related MCU resources into disabled state.
 *
 * BSP_BME280_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_BME280_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the sensor. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the H3LIS sensor
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_BME280_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_BME280_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_BME280_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_BME280_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_BME280_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_BME280_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_BME280_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_BME280_H_
#define KISO_BSP_BME280_H_

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_BME280

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the sensor.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @param[in]   deviceId: Id of the targeted device. The device id is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BME280_Connect(int32_t deviceId);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the sensor and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_BME280_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. This id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is reserved to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BME280_Enable(int32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the sensor and if possible power it off.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BME280_Disable(int32_t deviceId);

/**
 * @brief       Disconnects the sensor.
 *
 * @details     This function will tristate the GPIOs of the sensor, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_BME280_Disconnect(int32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the sensor.
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
HWHandle_T BSP_BME280_GetHandle(int32_t deviceId);

/**
 * @brief       Executes a control command for the BME280 sensor.
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
Retcode_T BSP_BME280_Control(uint32_t command, void *arg);

/**
 * @brief       Sets Chip-Select pin to high level.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_BME280_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BME280_SetCSHigh(int32_t deviceId);

/**
 * @brief       Sets Chip-Select pin to low level.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_BME280_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BME280_SetCSLow(int32_t deviceId);

#endif /*KISO_FEATURE_BSP_BME280*/
#endif /* KISO_BSP_BME280_H_ */

/**@}*/ /* KISO_HAL_BSP_BME280 */
