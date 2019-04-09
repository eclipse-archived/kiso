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
 * @defgroup    BCDS_HAL_BSP_UMTS_QUECTELUC20 UMTS QUECTELUC20
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       Generic interface to the UMTS/HSPA peripheral QUECTEL UC20.
 * <A HREF="https://www.quectel.com/product/uc20.htm"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_UMTS_QuectelUC20_Connect() function is required to map its GPIOs to their desired function (input, output, SPI,
 * etc).
 *
 * A call to BSP_UMTS_QuectelUC20_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_UMTS_QuectelUC20_Disable() will revert back what the BSP_UMTS_QuectelUC20_Enable() has done and set the device
 * and its related MCU resources into disabled state.
 *
 * BSP_UMTS_QuectelUC20_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_UMTS_QuectelUC20_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the UMTS controller:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_UMTS_QuectelUC20_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_UMTS_QuectelUC20_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_UMTS_QuectelUC20_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_UMTS_QuectelUC20_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_UMTS_QuectelUC20_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_UMTS_QuectelUC20_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_UMTS_QuectelUC20_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_UMTS_QUECTELUC20_H
#define BCDS_BSP_UMTS_QUECTELUC20_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_UMTS_QUECTELUC20

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_UMTS_QuectelUC20_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the UMTS controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_UMTS_QuectelUC20_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_QuectelUC20_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the UMTS controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_QuectelUC20_Disable(void);

/**
 * @brief       Disconnects the UMTS controller.
 *
 * @details     This function will tristate the GPIOs of the UMTS controller, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_QuectelUC20_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the UMTS controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_UMTS_QuectelUC20_GetUARTHandle(void);

/**
 * @brief       Power on the peripheral.
 *
 * @details     This function once called will power on the UMTS controller by applying power on sequence on the power
 *              pin.
 */
void BSP_UMTS_QuectelUC20_PowerOn(void);

/**
 * @brief       Power off the peripheral.
 *
 * @details     This function once called will power off the UMTS controller by applying power off  sequence on the
 *              power pin.
 */
void BSP_UMTS_QuectelUC20_PowerOff(void);

/**
 * @brief       Resets the peripheral.
 *
 * @details     This function once called will reset the UMTS controller by pulling up the reset pin to low level.
 */
void BSP_UMTS_QuectelUC20_Reset(void);

#endif /* BCDS_FEATURE_BSP_UMTS_QUECTELUC20 */
#endif /* BCDS_BSP_UMTS_QUECTELUC20_H */

/**  @} */
