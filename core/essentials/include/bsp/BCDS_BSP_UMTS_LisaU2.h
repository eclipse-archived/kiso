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
 * @defgroup    BCDS_HAL_BSP_UMTS_LISAU2 UMTS LISAU2
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for LISAU2 UMTS/HSPA controller from Ublox.
 * <A HREF="https://www.u-blox.com/sites/default/files/LISA-U2_DataSheet_%28UBX-13001734%29.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_UMTS_LisaU2_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_UMTS_LisaU2_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_UMTS_LisaU2_Disable() will revert back what the BSP_UMTS_LisaU2_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_UMTS_LisaU2_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_UMTS_LisaU2_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * From the datasheet of the device there was necessity to complement those
 * functions with other minimum service functions:
 * - BSP_UMTS_LisaU2_Reset() attempts to reset the peripheral if the commands
 *   fail to execute.
 *
 * The following diagram describes the states and transitions for the UMTS controller:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_UMTS_LisaU2_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_UMTS_LisaU2_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_UMTS_LisaU2_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_UMTS_LisaU2_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_UMTS_LisaU2_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_UMTS_LisaU2_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_UMTS_LisaU2_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_UMTS_LisaU2_H
#define BCDS_BSP_UMTS_LisaU2_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_UMTS_LISAU2

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_Connect(void);

/**
 * @brief      Enables the control over the peripheral.
 *
 * @details    This function will execute the required sequence to enable the UMTS controller and allows it to receive,
 *             execute commands, and transmit data.
 *
 * @pre        BSP_UMTS_LisaU2_Connect() successfully executed.
 *
 * @return     RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_Enable(void);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the UMTS controller and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_Disable(void);

/**
 * @brief       Disconnects the UMTS Lisa U2.
 *
 * @details     This function will tristate the GPIOs of the UMTS controller, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_Disconnect(void);

/**
 * @brief       Returns the handle of the communication interface used by the UMTS controller interface.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_UMTS_LisaU2_GetUARTHandle(void);

/**
 * @brief       Returns the handle of the communication interface used by the UMTS controller.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_UMTS_LisaU2_GetSPIHandle(void);

/**
 * @brief       Resets the peripheral.
 *
 * @details     This function once called will reset the  UBLOX(R) LisaU2 UMTS peripheral by pulling up the reset pin
 *              to low voltage level.

 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_Reset(void);

/**
 * @brief       Reads the state of the input CTS pin
 *
 * @details     The module hardware flow control output (CTS line) is set to the ON state (low level) at UART
 *              initialization. If the hardware flow control is enabled, as it is by default, the CTS line indicates
 *              when the UART interface is enabled (data can be sent and received). The module drives the CTS line to
 *              the ON state or to the OFF state when it is either able or not able to accept data from the DTE over
 *              the UART.
 *
 * @pre         BSP_UMTS_LisaU2_Connect() successfully executed
 *
 * @return      Error (if module is not enabled) or Logic level of the input pin (high or low).
 */
uint32_t BSP_UMTS_LisaU2_GetCTS(void);

/**
 * @brief       Sets the RTS pin to Low
 *
 * @details     This function sets the RTS line to low voltage level as seen by the device.
 *
 * @pre         BSP_UMTS_LisaU2_Connect() successfully executed
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_SetRTSLow(void);

/**
 * @brief       Sets the RTS pin to high
 *
 * @details     This function sets the RTS line to high voltage level as seen by the device.
 *
 * @pre         BSP_UMTS_LisaU2_Connect() successfully executed
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_UMTS_LisaU2_SetRTSHigh(void);

#endif /* BCDS_FEATURE_BSP_UMTS_LISAU2 */

#endif /* BCDS_BSP_UMTS_LisaU2_H */

/**  @} */
