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
 * @defgroup    BCDS_HAL_BSP_MAX31865 RTD-to-Digital Converter MAX31865
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for MAX31865 RTD-to-Digital Converter from Maxim Integrated.
 * <A HREF="https://datasheets.maximintegrated.com/en/ds/MAX31865.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Max31865_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Max31865_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_Max31865_Disable() will revert back what the BSP_Max31865_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_Max31865_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_Max31865_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the converter device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Max31865_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Max31865_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Max31865_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Max31865_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Max31865_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Max31865_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Max31865_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_MAX31865_H
#define BCDS_BSP_MAX31865_H

#include "BCDS_HAL.h"

#if BCDS_FEATURE_BSP_MAX31865

/* Put the type and macro definitions here */

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_Max31865_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the converter and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_Max31865_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Max31865_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the converter and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Max31865_Disable(void);

/**
 * @brief       Disconnects the converter.
 *
 * @details     This function will tristate the GPIOs of the converter, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Max31865_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the converter.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_Max31865_GetSPIHandle(void);

/**
 * @brief       Sets the Max31865 chip select pin to voltage level high.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_Max31865_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Max31865_SetCSHigh(void);

/**
 * @brief       Sets the Max31865 chip select pin to voltage level low.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_Max31865_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Max31865_SetCSLow(void);

/**
 * @brief       Get the Status of the peripheral(Temp Sensor).
 *
 * @param[out]  isConnected: To get the Max31865 Sensor connected status, if the sensor connected it's true otherwise false.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Max31865_GetStatus(uint8_t *isConnected);

#endif // BCDS_FEATURE_BSP_MAX31865

#endif /* BCDS_BSP_MAX31865_H */
