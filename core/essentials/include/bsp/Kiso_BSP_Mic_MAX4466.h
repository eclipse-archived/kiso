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
 * @defgroup    KISO_HAL_BSP_Mic_MAX4466 Microphone MAX4466
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for MAX4466 microphone from Maxim Integrated.
 * <A HREF="https://datasheets.maximintegrated.com/en/ds/MAX4465-MAX4469.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Mic_MAX4466_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Mic_MAX4466_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_Mic_MAX4466_Disable() will revert back what the BSP_Mic_MAX4466_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_Mic_MAX4466_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_Mic_MAX4466_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the microphone device:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Mic_MAX4466_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Mic_MAX4466_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Mic_MAX4466_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Mic_MAX4466_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Mic_MAX4466_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Mic_MAX4466_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Mic_MAX4466_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_Mic_MAX4466_H
#define KISO_BSP_Mic_MAX4466_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_MIC_MAX4466

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_Mic_MAX4466_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will enable the microphone.
 *
 * @pre         BSP_Mic_AKU340_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Mic_MAX4466_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the microphone and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Mic_MAX4466_Disable(void);

/**
 * @brief       Disconnects the microphone.
 *
 * @details     This function will tristate the GPIOs of the microphone, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Mic_MAX4466_Disconnect(void);

#endif /* KISO_FEATURE_BSP_MIC_MAX4466*/

#endif /* KISO_BSP_Mic_MAX4466_H */

/**  @} */
