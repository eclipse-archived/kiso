/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH.
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
 * @defgroup    BCDS_HAL_BSP_BLE_CC2640 BLE CC2640
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for CC2640 BLE module from TI.
 * <A HREF=""><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_BLE_CC2640_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_BLE_CC2640_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_BLE_CC2640_Disable() will revert back what the BSP_BLE_CC2640_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_BLE_CC2640_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_BLE_CC2640_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the BLE controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_BLE_CC2640_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_BLE_CC2640_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_BLE_CC2640_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_BLE_CC2640_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_BLE_CC2640_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_BLE_CC2640_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_BLE_CC2640_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BSP_BLE_CC2640_H_
#define BSP_BLE_CC2640_H_

/* Include all headers which are needed by this file. */
#include "BCDS_HAL.h"

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_BLE_CC2640_Connect(void);

/**
 * @brief       Disconnects the BLE controller.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BLE_CC2640_Disconnect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the GPS controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_GPS_CC2640_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BLE_CC2640_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the GPS controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_BLE_CC2640_Disable(void);

/**
 * @brief       Returns the handle of the communication interface used by the BLE controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_BLE_CC2640_GetUARTHandle(void);

#endif /* BSP_BLE_CC2640_H_ */

