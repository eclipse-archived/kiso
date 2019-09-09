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
 * @defgroup    KISO_HAL_BSP_LORA_SX1272 LoRa SX1272
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for LoRa module SX1272 from Semtech.
 * <A HREF="https://www.semtech.com/uploads/documents/sx1272.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_LoRa_SX1272_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_LoRa_SX1272_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_LoRa_SX1272_Disable() will revert back what the BSP_LoRa_SX1272_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_LoRa_SX1272_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration
 * e.g. high impedance.
 *
 * BSP_LoRa_SX1272_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the LoRa controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_LoRa_SX1272_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_LoRa_SX1272_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_LoRa_SX1272_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_LoRa_SX1272_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_LoRa_SX1272_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_LoRa_SX1272_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_LoRa_SX1272_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_LoRa_SX1272_H
#define KISO_BSP_LoRa_SX1272_H

#include "Kiso_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_LORA_SX1272

/**
 *  Enumeration of the available IO pins 
 */
enum BSP_LoRa_SX1272_IO_E
{
    BSP_LoRa_SX1272_IO0,
    BSP_LoRa_SX1272_IO1,
    BSP_LoRa_SX1272_IO2,
    BSP_LoRa_SX1272_IO3,
    BSP_LoRa_SX1272_IO4,
    BSP_LoRa_SX1272_IO5,
};

/**
 * @brief       Type definition for the interrupt callback.
 *
 * @details     The application should pass a function pointer of the same signature (**void function(void)**) to
 *              BSP_LoRa_SX1272_EnableInterruptPin() in order to handle interrupts coming from the specified interrupt
 *              line.
 */
typedef void (*BSP_LoRa_SX1272_InterruptCallback_T)(void);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_LoRa_SX1272_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the LoRa controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_LoRa_SX1272_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the LoRa controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_Disable(void);

/**
 * @brief       Disconnects the LoRa controller.
 *
 * @details     This function will tristate the GPIOs of the LoRa module, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the SX1272 controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_LoRa_SX1272_GetSPIHandle(void);

/**
 * @brief       Enables interrupt.
 *
 * @details     This function allows interrupting the MCU on level change at one of the IO pins. It shall appropriately
 *              configure the pin, accordingly assign the application callback, and enable the associated interrupt
 *              controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @param       pinName: pin to be enabled from the enumeration BSP_LoRa_SX1272_IO_E
 *
 * @param       callback : function to be called whenever an interrupt related to the pin occures
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_EnableInterruptPin(enum BSP_LoRa_SX1272_IO_E pinName, BSP_LoRa_SX1272_InterruptCallback_T callback);

/**
 * @brief       Disables interrupt.
 *
 * @details     The function disables interrupting the microcontroller upon state change of the IO pin.
 * 
 * @param       pinName: pin to be enabled from the enumeration BSP_LoRa_SX1272_IO_E
 * 
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_DisableInterruptPin(enum BSP_LoRa_SX1272_IO_E pinName);

/**
 * @brief       Enables the radio antenna.
 *
 * @details     This function will enable the radio antenna for transmission and reception.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_EnableAntenna(void);

/**
 * @brief       Disables the radio antenna.
 *
 * @details     This function will disable the radio antenna for transmission and reception.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_DisableAntenna(void);

/**
 * @brief       Resets the LORA module.
 *
 * @details     This function will execute the sequence to reset the SX1272 device.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_Reset(void);

/**
 * @brief       Sets the CS pin to Low voltage level.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_LoRa_SX1272_Connect() successfully executed. *
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_SetCSLow(void);

/**
 * @brief       Sets the CS pin to high voltage level.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_LoRa_SX1272_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_LoRa_SX1272_SetCSHigh(void);

#endif /* KISO_FEATURE_BSP_LORA_SX1272 */

#endif /* KISO_BSP_LoRa_SX1272_H */

/**  @} */
