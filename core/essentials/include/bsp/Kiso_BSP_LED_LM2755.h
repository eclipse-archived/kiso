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
 * @defgroup    KISO_HAL_BSP_LED_LM2755  LED Controller LM2755
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for LM2755 charge pump LED controller from Texas Instruments.
 * <A HREF="http://www.ti.com/lit/ds/symlink/lm2755.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_LED_LM2755_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_LED_LM2755_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_LED_LM2755_Disable() will revert back what the BSP_LED_LM2755_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_LED_LM2755_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_LED_LM2755_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the LED controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_LED_LM2755_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_LED_LM2755_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_LED_LM2755_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_LED_LM2755_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_LED_LM2755_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_LED_LM2755_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_LED_LM2755_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_LED_LM2755_H_
#define KISO_BSP_LED_LM2755_H_

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_LED_LM2755

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_LED_LM2755_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the LED driver and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_LED_LM2755_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_LM2755_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the LED controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_LM2755_Disable(void);

/**
 * @brief       Disconnects the LED controller.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LED_LM2755_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the LED controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_LED_LM2755_GetI2CHandle(void);

#endif /* KISO_FEATURE_BSP_LED_LM2755 */

#endif /* KISO_BSP_LED_LM2755_H_ */

/**  @} */
