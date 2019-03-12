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
 * @defgroup    BCDS_HAL_BSP_ACCEL_H3LIS Accelerometer H3LIS
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for H3LIS acceleration sensor from STMicroelectronics.
 * <A HREF="https://www.st.com/en/mems-and-sensors/h3lis331dl.html"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the accelerometer is requested a call to
 * BSP_Accel_H3LIS_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Accel_H3LIS_Enable() will execute the required sequence to enable the accelerometer and its dedicated
 * MCU resources (e.g. SPI). The accelerometer is then **controllable** through the microcontroller.
 *
 * BSP_Accel_H3LIS_Disable() will revert back what the BSP_Accel_H3LIS_Enable() has done and set the
 * accelerometer and its related MCU resources into disabled state.
 *
 * BSP_Accel_H3LIS_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance.
 *
 * BSP_Accel_H3LIS_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the accelerometer. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the H3LIS accelerometer
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Accel_H3LIS_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Accel_H3LIS_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Accel_H3LIS_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Accel_H3LIS_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Accel_H3LIS_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Accel_H3LIS_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Accel_H3LIS_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_ACCELEROMETER_H3LIS_H
#define BCDS_BSP_ACCELEROMETER_H3LIS_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_ACCEL_H3LIS

/**
 * @brief       Type definition for the interrupt callback
 *
 * @details     The application should provide a function of this signature (**void function(uint32_t, uint32_t)**)
 *              to BSP_Accel_H3LIS_IntEnable() to process interrupts coming from INT1 and/or INT2 lines.
 *
 * @param       id is the interrupt identifier.
 *
 * @param       event is the edge resulted in the interrupt (falling edge or rising).
 */
typedef void (*BSP_Accel_InterruptCallback_T)(uint32_t id, uint32_t event);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the accelerometer.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the accelerometer and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_Accel_H3LIS_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the accelerometer and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_Disable(void);

/**
 * @brief       Disconnects the accelerometer.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the accelerometer.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_Accel_H3LIS_GetSPIHandle(void);

/**
 * @brief       Enables interrupts from INT1 and/or INT2 lines.
 *
 * @details     This function allows interrupting the MCU on level change at INT1 and/or INT2 pin. It shall
 *              appropriately configure the pin, accordingly assign the application callback, and enable the associated
 *              interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.

 * @pre         BSP_Accel_H3LIS_Connect() successfully executed.
 *
 * @param       callback Function passed by the application to process interrupts coming from int1 and int2 interrupt
 *              lines.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_IntEnable(BSP_Accel_InterruptCallback_T callback);

/**
 * @brief       Disables interrupts from INT1/INT2 lines.
 *
 * @pre         BSP_Accel_H3LIS_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_IntDisable(void);

/**
 * @brief       Sets the chip select line to low volage level.
 * 
 * @details     This function sets the chip select line to low voltage level as seen by the device in order to 
 *              select it for an SPI transaction.
 *
 * @pre         BSP_Accel_H3LIS_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_SetCSLow(void);

/**
 * @brief       Sets the chip select line to high volage level.
 * 
 * @details     This function sets the chip select line to high voltage level as seen by the device in order to 
 *              deselect from the SPI bus.
 *
 * @pre         BSP_Accel_H3LIS_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Accel_H3LIS_SetCSHigh(void);

#endif /* BCDS_FEATURE_BSP_ACCEL_H3LIS */

#endif /* BCDS_BSP_ACCELEROMETER_H3LIS_H */

/**  @} */
