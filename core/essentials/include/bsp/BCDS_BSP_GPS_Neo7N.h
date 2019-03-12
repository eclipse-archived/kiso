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
 * @defgroup    BCDS_HAL_BSP_GPS_NEO7N GPS NEO7N
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for NEO7N GPS engine from ublox.
 * <A HREF="https://www.u-blox.com/sites/default/files/products/documents/NEO-7_DataSheet_%28UBX-13003830%29.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_GPS_Neo7n_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_GPS_Neo7n_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_GPS_Neo7n_Disable() will revert back what the BSP_GPS_Neo7n_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_GPS_Neo7n_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_GPS_Neo7n_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the GPS contoller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_GPS_Neo7n_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_GPS_Neo7n_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_GPS_Neo7n_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_GPS_Neo7n_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_GPS_Neo7n_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_GPS_Neo7n_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_GPS_Neo7n_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_GPS_Neo7N_H
#define BCDS_BSP_GPS_Neo7N_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_GPS_NEO7N

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_GPS_Neo7n_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the GPS controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_GPS_Neo7n_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GPS_Neo7n_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the GPS controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GPS_Neo7n_Disable(void);

/**
 * @brief       Disconnects the GNSS controller.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GPS_Neo7n_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the GPS controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_GPS_Neo7n_GetUARTHandle(void);

/**
 * @brief       Resets the GPS peripheral.
 *
 * @details     This function performs a hardware reset to the GPS Peripheral.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 *
 */
Retcode_T BSP_GPS_Neo7n_Reset(void);

#endif /* BCDS_FEATURE_BSP_GPS_NEO7N */

#endif /* BCDS_BSP_GPS_Neo7N_H */

/**  @} */
