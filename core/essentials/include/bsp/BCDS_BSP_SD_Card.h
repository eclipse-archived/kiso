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
 * @defgroup    BCDS_HAL_BSP_SDCARD SD Card
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for Secure Digital SDCard over SPI
 * <A HREF=""><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_SDCard_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_SDCard_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_SDCard_Disable() will revert back what the BSP_SDCard_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_SDCard_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_SDCard_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the SD Card:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_SDCard_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_SDCard_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_SDCard_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_SDCard_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_SDCard_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_SDCard_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_SDCard_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_SD_Card_H
#define BCDS_BSP_SD_Card_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_SD_CARD

/**
 * @brief       Type definition for the callback to be triggered upon detection of the SD card
 *
 * @param       Event reported by the BSP upon SD-card detection event. The event could be:
 *               - 0 if error,
 *               - 1 if card inserted,
 *               - 2 if card ejected.
 */
typedef void (*BSP_SDCardAutoDetectCallback_T)(uint32_t);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_SDCard_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function once called will allow the SD memory peripheral to receive, execute commands and transmit
 *              process data.
 *
 * @pre         BSP_SDCard_Connect() successfully executed.
 *
 * @param       autoDetectCallback: In case auto detection is implemented this function will be called upon SD-card
 *              detection event.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SDCard_Enable(BSP_SDCardAutoDetectCallback_T autoDetectCallback);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the SD memory card and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SDCard_Disable(void);

/**
 * @brief       Disconnects the SD-card.
 *
 * @details     This function will tristate the GPIOs of the SD-card, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SDCard_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the SD memory card.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_SDCard_GetSPIHandle(void);

/**
 * @brief       Sets the SD card chip select pin to high voltage level.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_SDCard_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SDCard_SetCSHigh(void);

/**
 * @brief       Sets the SD chip select pin to low voltage level.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_SDCard_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_SDCard_SetCSLow(void);

#endif /* BCDS_FEATURE_BSP_SDCARD */

#endif /* BCDS_BSP_SD_Card_H */

/**  @} */
