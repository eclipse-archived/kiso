/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 * @defgroup    BCDS_HAL_BSP_WIFI_CC3100MOD WiFi CC3100MOD
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for CC3100MOD WiFi controller.
 * <A HREF="http://www.ti.com/lit/ds/swrs161/swrs161.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_WiFi_CC3100MOD_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_WiFi_CC3100MOD_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_WiFi_CC3100MOD_Disable() will revert back what the BSP_WiFi_CC3100MOD_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_WiFi_CC3100MOD_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_WiFi_CC3100MOD_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the UMTS controller:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_WiFi_CC3100MOD_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_WiFi_CC3100MOD_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_WiFi_CC3100MOD_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_WiFi_CC3100MOD_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_WiFi_CC3100MOD_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_WiFi_CC3100MOD_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_WiFi_CC3100MOD_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_WiFi_CC3100MOD_H
#define BCDS_BSP_WiFi_CC3100MOD_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_WIFI_CC3100MOD

/**
 * @brief       Type definition for the wlan interrupt callback.
 *
 * @details     The application should then pass a callback function of this signature (**void function(void)**) to
 *              BSP_WiFi_CC3100MOD_Enable() function. This function will be called whenever a change occurs in the
 *              module's interrupt line.
 *              Refer to the datasheet for more details.
 */
typedef void (*BSP_WiFi_CC3100MOD_InterruptCallback_T)(void);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the WiFi controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_WiFi_CC3100MOD_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Enable(BSP_WiFi_CC3100MOD_InterruptCallback_T callback);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the WiFi controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Disable(void);

/**
 * @brief       Disconnects the WiFi controller.
 *
 * @details     This function will tristate the GPIOs of the WiFi controller, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the WiFi controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_WiFi_CC3100MOD_GetSPIHandle(void);

/**
 * @brief       Returns the handle of the communication interface used by the WiFi controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_WiFi_CC3100MOD_GetUartHandle(void);

/**
 * @brief       Wakes the module from hibernate mode.
 *
 * @details     If the WIFI peripheral is hibernated this function will execute the wake-up sequence with respect to
 *              the data-sheet.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Wakeup(void);

/**
 * @brief       Hibernates the WIFI module.
 *
 * @details     This function will execute the instruction sequence in order to put the WIFI module into hibernated state.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Hibernate(void);

/**
 * @brief       Resets the WiFi module.
 *
 * @details     This function will execute the sequence to reset the WiFi controller.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_Reset(void);

/**
 * @brief       Sets the CS pin to low voltage level i.e. device selected.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_WiFi_CC3100MOD_Connect() successfully executed
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_SetCSLow(void);

/**
 * @brief       Sets the CS pin to high voltage level i.e. device deselected.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre          BSP_WiFi_CC3100MOD_Connect() successfully executed
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_WiFi_CC3100MOD_SetCSHigh(void);

#endif /* BCDS_FEATURE_BSP_WIFI_CC3100MOD */

#endif /* BCDS_BSP_WiFi_CC3100MOD_H */

/**  @} */
