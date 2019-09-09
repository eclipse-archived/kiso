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
 * @defgroup    KISO_HAL_BSP_GNSS_G7020 GNSS G7020
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for G7020 GNSS engine from ublox.
 * <A HREF="https://www.u-blox.com/sites/default/files/products/documents/u-blox7-V14_ReceiverDescriptionProtocolSpec_%28GPS.G7-SW-12001%29_Public.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_GNSS_G7020_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_GNSS_G7020_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_GNSS_G7020_Disable() will revert back what the BSP_GNSS_G7020_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_GNSS_G7020_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_GNSS_G7020_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the GNSS controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_GNSS_G7020_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_GNSS_G7020_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_GNSS_G7020_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_GNSS_G7020_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_GNSS_G7020_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_GNSS_G7020_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_GNSS_G7020_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_GNSS_G7020_H
#define KISO_BSP_GNSS_G7020_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_GNSS_G7020

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_GNSS_G7020_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the GNSS controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_GNSS_G7020_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GNSS_G7020_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the GNSS controller and if possible will power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GNSS_G7020_Disable(void);

/**
 * @brief       Disconnects the GNSS controller.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GNSS_G7020_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the GNSS controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_GNSS_G7020_GetUARTHandle(void);

/**
 * @brief       Resets the GNSS peripheral.
 *
 * @details     This function performs a hardware reset to the GNSS Peripheral by pulling the reset pin to low voltage
 *              level.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GNSS_G7020_Reset(void);

#endif /* KISO_FEATURE_BSP_GNSS_G7020 */
#endif /* KISO_BSP_GNSS_G7020_H */

/**  @} */
