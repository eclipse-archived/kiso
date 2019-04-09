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
 * @defgroup    BCDS_HAL_BSP_LORA_RN2xx3 LoRa RN2483/RN2903
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for LoRa modules RN2483/RN2903 from Microchip.
 * <A HREF="http://ww1.microchip.com/downloads/en/devicedoc/50002346c.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_LoRa_RN2xx3_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_LoRa_RN2xx3_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_LoRa_RN2xx3_Disable() will revert back what the BSP_LoRa_RN2xx3_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_LoRa_RN2xx3_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_LoRa_RN2xx3_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the LoRa controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_LoRa_RN2xx3_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_LoRa_RN2xx3_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_LoRa_RN2xx3_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_LoRa_RN2xx3_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_LoRa_RN2xx3_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_LoRa_RN2xx3_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_LoRa_RN2xx3_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_LoRa_RN2xx3_H
#define BCDS_BSP_LoRa_RN2xx3_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_LORA_RN2XX3

/**
 * enumeration representing return codes from the LoRa module BSP interface
 */
enum BSP_LoRa_RN2xx3_Retcode_E
{
    RETCODE_BSP_LORA_SERIAL_NOT_INITIALIZED = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_BSP_LORA_DISABLED,
};

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_LoRa_RN2xx3_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the LoRa controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_LoRa_RN2xx3_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_RN2xx3_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the LoRa controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_RN2xx3_Disable(void);

/**
 * @brief       Disconnects the LoRa controller.
 *
 * @details     This function will tristate the GPIOs of the LoRa module, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_RN2xx3_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the LoRa controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_LoRa_RN2xx3_GetUartHandle(void);

/**
 * @brief       Sets the reset pin of the module to the desired state Low or High
 *
 * @details     This function sets the reset line of the device to low/high voltage level e.g 0.0V/3V. The implementation
 *              shall take into account the hardware design and not only applying logic 0 at the GPIO output of the MCU.
 *
 * @param       state: voltage level to set:
 *              - 0 for Low
 *              - 1 for High
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_RN2xx3_SetResetPin(uint32_t state);

/**
 * @brief       Changes the baudrate of the communication interface
 *
 * @param       baudrate: The desired baudrate.
 *              -if 0 will keep the current baudrate and send a break command with
 *              synchronization frame could be used to wakeup the module from sleep according to datasheet.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_RN2xx3_ChangeBaudrate(uint32_t baudrate);

/**
 * @brief       Sends wake-up signal to the LoRa module
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_RN2xx3_WakeUp(void);

#endif /* BCDS_FEATURE_BSP_LORA_RN2XX3 */

#endif /* BCDS_BSP_LoRa_RN2xx3_H */

/**  @} */
