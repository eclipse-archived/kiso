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
 * @defgroup    KISO_HAL_BSP_SARA_R4N4 Cellular Sara R4N4
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for Sara R4N4 cellular module from Ublox.
 * <A HREF="https://www.u-blox.com/sites/default/files/SARA-R4-N4_DataSheet_%28UBX-16024152%29.pdf"><B>Datasheet</B></A>
 * @details
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Cellular_SaraR4N4_Connect() function is required to map its GPIOs to their desired function (input, output, UART,
 * etc).
 *
 * A call to BSP_Cellular_SaraR4N4_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. UART). The device is then **controllable** through the microcontroller.
 *
 * BSP_Cellular_SaraR4N4_Disable() will revert back what the BSP_Tempplate_Enable() has done and set the device and its
 * related MCU resources into disabled state.
 *
 * BSP_Cellular_SaraR4N4_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_Cellular_SaraR4N4_GetUARTHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the accelerometer. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Template_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Template_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Template_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Template_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Template_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Template_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Template_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_CELLULAR_SARAR4N4_H_
#define KISO_BSP_CELLULAR_SARAR4N4_H_

#include "Kiso_HAL.h"

#if KISO_FEATURE_BSP_CELLULAR_SARAR4N4

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function maps the GPIO pins to their desired functions to drive the  cellular module.
 *              Basic input/output operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_Connect(void);

/**
 * @brief       Enables the control over the device.
 *
 * @details     This function will execute the required sequence to enable the device and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_Template_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the device and if possible it will power it off.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_Disable(void);

/**
 * @brief       Disconnects the peripheral.
 *
 * @details     This function will tristate the GPIOs of the template, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_Disconnect(void);

/**
 * @brief       Returns the handle of the UART interface used by the peripheral.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU UART driver.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation which is MCU specific.
 *
 * @return      HWHandle: Handle to the SPI interface
 */
HWHandle_T BSP_Cellular_SaraR4N4_GetUARTHandle(void);

/**
 * @brief       Returns the handle of the SPI interface used by the peripheral.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU SPI driver.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation which is MCU specific.
 *
 * @return      HWHandle: Handle to the SPI interface
 */
HWHandle_T BSP_Cellular_SaraR4N4_GetSPIHandle(void);

/**
 * @brief       Resets the peripheral.
 *
 * @details     This function will reset the cellular module peripheral by pulling up the reset pin to low level.

 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_Reset(void);

/**
 * @brief       Executes a control command for the cellular module.
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
Retcode_T BSP_Cellular_SaraR4N4_Control(uint32_t command, void *arg);

/**
 * @brief       Reads the state of the input CTS pin.
 *
 * @details     The module hardware flow control output (CTS line) is set to the ON state (low level) at UART
 *              initialization. If the hardware flow control is enabled, as it is by default, the CTS line indicates
 *              when the UART interface is enabled (data can be sent and received). The module drives the CTS line to
 *              the ON state or to the OFF state when it is either able or not able to accept data from the DTE over
 *              the UART.
 *
 * @retval      0 if pin is low
 * @retval      1 if pin is high
 * @retval      other if error (e.g. module is not connected)
 */
uint32_t BSP_Cellular_SaraR4N4_GetCTS(void);

/**
 * @brief       Sets the RTS pin to low
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_SetRTSLow(void);

/**
 * @brief       Sets the RTS pin to high
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Cellular_SaraR4N4_SetRTSHigh(void);

#endif /* KISO_FEATURE_BSP_CELLULAR_SARAR4N4 */

#endif /* KISO_BSP_CELLULAR_SARAR4N4_H_ */

/**@}*/
