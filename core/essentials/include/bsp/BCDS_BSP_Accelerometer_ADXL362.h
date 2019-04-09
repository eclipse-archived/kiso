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
 * @defgroup    BCDS_HAL_BSP_ACCEL_ADXL362 Accelerometer ADXL362
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for ADXL362 acceleration sensor from Analog Devices.
 * <A HREF="https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL362.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the accelerometer is requested a call to
 * BSP_Accel_ADXL362_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Accel_ADXL362_Enable() will execute the required sequence to enable the accelerometer and its dedicated
 * MCU resources (e.g. SPI). The accelerometer is then **controllable** through the microcontroller.
 *
 * BSP_Accel_ADXL362_Disable() will revert back what the BSP_Accel_ADXL362_Enable() has done and set the
 * accelerometer and its related MCU resources into disabled state.
 *
 * BSP_Accel_ADXL362_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_Accel_ADXL362_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the accelerometer. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the ADXL362 accelerometer
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Accel_ADXL362_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Accel_ADXL362_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Accel_ADXL362_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Accel_ADXL362_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Accel_ADXL362_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Accel_ADXL362_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Accel_ADXL362_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_ACCEL_ADXL362_H
#define BCDS_BSP_ACCEL_ADXL362_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_ACCEL_ADXL362

/**
 * @brief       Type definition for the interrupt callback.
 *
 * @details     The application should pass a function pointer of the same signature (**void function(void)**) to
 *              BSP_Accel_ADXL362_Int1Enable() and BSP_Accel_ADXL362_Int2Enable() in order to handle interrupts coming
 *              from int1 or int2 lines.
 */
typedef void (*BSP_Accel_InterruptCallback_T)(void);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function maps the GPIO pins to their desired functions to drive the ADXL362 accelerometer.
 *              Basic input/output operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the accelerometer and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the accelerometer and if possible power it off.
 *
 * @pre         BSP_Accel_ADXL362_Enable() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Disable(void);

/**
 * @brief       Disconnects the accelerometer.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the accelerometer.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_Accel_ADXL362_GetSPIHandle(void);

/**
 * @brief       Sets to high the accelerometer chip select pin.
 *
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_SetCSHigh(void);

/**
 * @brief       Sets to low accelerometer chip select pin.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_SetCSLow(void);

/**
 * @brief       Enables interrupts from INT1 line.
 *
 * @details     This function allows interrupting the MCU on level change at INT1 pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @param       callback: function passed by the application to process interrupts coming from INT1 interrupt line.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Int1Enable(BSP_Accel_InterruptCallback_T callback);

/**
 * @brief       Disables interrupts from INT1 line.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Int1Disable(void);

/**
 * @brief       Enables interrupts from INT2 line.
 *
 * @details     This function allows interrupting the MCU on level change at INT2 pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @param       callback: function passed by the application to process interrupts coming from INT2 interrupt line.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Int2Enable(BSP_Accel_InterruptCallback_T callback);

/**
 * @brief       Disables interrupts from INT2 line.
 *
 * @pre         BSP_Accel_ADXL362_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_ADXL362_Int2Disable(void);

#endif /* BCDS_FEATURE_BSP_ACCEL_ADXL362 */

#endif /* BCDS_BSP_ACCEL_ADXL362_H */
/**  @} */
