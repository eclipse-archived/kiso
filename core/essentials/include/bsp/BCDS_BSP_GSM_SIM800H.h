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
 * @defgroup    BCDS_HAL_BSP_GSM_SIM800H GSM SIM800H
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for SIM800H GSM controller from SIMCOM
 * <A HREF="https://cdn-shop.adafruit.com/datasheets/sim800h_hardware_design_v1.00.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Accel_ADXL362_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Accel_ADXL362_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_Accel_ADXL362_Disable() will revert back what the BSP_Accel_ADXL362_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_Accel_ADXL362_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_GSM_SIM800H_GetUARTHandle() will return a pointer to the MCU UART resource structure created and initialized by
 * the BSP implementation. This structure is MCU specific and its proper initialization is mandatory for the proper
 * function of the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * From the datasheet of the device there was necessity to complement those
 * functions with other minimum service functions:
 *
 * - BSP_GSM_SIM800H_Wakeup() wakes up the peripheral from Sleep mode.
 *
 * - BSP_GSM_SIM800H_Sleep() puts the peripheral into sleep mode.
 *
 * - BSP_GSM_SIM800H_Restart() Restarts the peripheral.
 *
 * - BSP_GSM_SIM800H_Reset() attempts to reset the peripheral if the commands fail to execute.
 *
 * The following diagram describes the states and transitions for the GSM controller device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Accel_ADXL362_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Accel_ADXL362_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Accel_ADXL362_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Accel_ADXL362_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Accel_ADXL362_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Accel_ADXL362_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Accel_ADXL362_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_GSM_SIM800H_H
#define BCDS_BSP_GSM_SIM800H_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_GSM_SIM800H

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_GSM_SIM800H_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the GSM controller and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_GSM_SIM800H_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the GSM controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Disable(void);

/**
 * @brief       Disconnects the GSM controller.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the GSM controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_GSM_SIM800H_GetUARTHandle(void);

/**
 * @brief       Restarts the peripheral.
 *
 * @details     This function once called will restart the  GSM controller by executing the appropriate restart sequence.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Restart(void);

/**
 * @brief       Resets the peripheral.
 *
 * @details     This function once called will reset the  SIMCOM(R) SIM800H GSM peripheral by pulling up the reset pin
 *              to low level.
 *
 * @attention   This function used as an emergency reset only when AT-command AT+CPOWD=1 and the PWRKEY pin has no
 *              effect.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Reset(void);

/**
 * @brief       Wakes-up the GSM peripheral.
 *
 * @details     This function once called will wake-up the GSM controller from sleep.
 *              It is useful to request a wake-up each time a transmission is desired because the GSM peripheral goes
 *              automatically into sleep mode if not busy.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Wakeup(void);

/**
 * @brief       Sets the GSM peripheral into sleep mode.
 *
 * @details     This function once called will  force the  GSM controller into sleep mode for further information refer
 *              to the datasheet of the peripheral.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_GSM_SIM800H_Sleep(void);

#endif /* BCDS_FEATURE_BSP_GSM_SIM800H */

#endif /* BCDS_BSP_GSM_SIM800H_H */

/**  @} */
