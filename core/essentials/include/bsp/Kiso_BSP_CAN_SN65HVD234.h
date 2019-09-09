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
 * @defgroup    KISO_HAL_BSP_CAN_SN65HVD234 CAN Transceiver SN65HVD234
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for SN65HVD234 CAN transceiver from TI.
 * <A HREF="http://www.ti.com/lit/ds/symlink/sn65hvd234.pdf"><B>Datasheet</B></A>
 *  * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_CAN_SN65HVD234_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_CAN_SN65HVD234_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The device is then **controllable** through the microcontroller.
 *
 * BSP_CAN_SN65HVD234_Disable() will revert back what the BSP_CAN_SN65HVD234_Enable() has done and set the device and
 * its related MCU resources into disabled state.
 *
 * BSP_CAN_SN65HVD234_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_CAN_SN65HVD234_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * One possible use sequence can be as follows:
 * -# Enable the CAN interface by calling BSP_CAN_SN65HVD234_Enable()
 * -# Initialize the CAN driver by calling the Can_Init() from AE_EB_LIB library.
 * -# Call BSP_CAN_SN65HVD234_EnableIRQ() enable the CAN interrupt processing.
 *
 * The following diagram describes the states and transitions for the CAN controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_CAN_SN65HVD234_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_CAN_SN65HVD234_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_CAN_SN65HVD234_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_CAN_SN65HVD234_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_CAN_SN65HVD234_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_CAN_SN65HVD234_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_CAN_SN65HVD234_Disconnect()"]
 *  }
 * @enddot
 *
 */

#ifndef KISO_BSP_CAN_SN65HVD234_H
#define KISO_BSP_CAN_SN65HVD234_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_CAN_SN65HVD234

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_CAN_SN65HVD234_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the CAN transceiver and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_CAN_SN65HVD234_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_CAN_SN65HVD234_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the accelerometer and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_CAN_SN65HVD234_Disable(void);

/**
 * @brief       Disconnects the CAN transceiver.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_CAN_SN65HVD234_Disconnect(void);

/**
 * @brief       Enables the Interrupt requests from CAN
 *
 * @details     A prior call to Can_Init() from AE-EB library is necessary for this function in order to execute
 *              properly. The MCU can after calling this function process CAN interrupt requests.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 *
 */
Retcode_T BSP_CAN_SN65HVD234_EnableIRQ(void);

/**
 * @brief       Disables the interrupt requests from CAN
 *
 * @details     The MCU can not anymore receive interrupt requests from CAN after calling this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 *
 */
Retcode_T BSP_CAN_SN65HVD234_DisableIRQ(void);

#endif /* KISO_FEATURE_BSP_CAN_SN65HVD234 */
#endif /* KISO_BSP_CAN_SN65HVD234_H */

/**  @} */
