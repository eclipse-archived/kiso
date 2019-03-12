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
 * @defgroup    BCDS_HAL_BSP_Mic_AKU340 Microphone AKU340
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       Hardware abstraction layer for the AKU340 microphone functionalities
 * <A HREF="https://www.mouser.com/ds/2/720/DS26-1.04%20AKU340%20Datasheet-770074.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Mic_AKU340_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Mic_AKU340_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_Mic_AKU340_Disable() will revert back what the BSP_Mic_AKU340_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_Mic_AKU340_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_Mic_AKU340_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the microphone
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Mic_AKU340_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Mic_AKU340_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Mic_AKU340_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Mic_AKU340_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Mic_AKU340_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Mic_AKU340_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Mic_AKU340_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_Mic_AKU340_H
#define BCDS_BSP_Mic_AKU340_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_MIC_AKU340

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_Mic_AKU340_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will enable the microphone.
 *
 * @pre         BSP_Mic_AKU340_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Mic_AKU340_Enable(void);

/**
 * @brief       Disables the control over the AKU340.
 *
 * @details     This function once called will disable control over the microphone and if possible power it off.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Mic_AKU340_Disable(void);

/**
 * @brief       Disconnects the microphone.
 *
 * @details     This function will tristate the GPIOs of the microphone, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Mic_AKU340_Disconnect(void);

/**
 * @deprecated  This API is deprecated.
 *
 * @brief       Senses the AKU340 Microphone signal
 *
 * @details     This function is called to trigger an ADC Conversion at the microphone pin and return the measured value.
 *
 * @return      INT32_MIN.
 */
BCDS_DEPRECATED(int32_t BSP_Mic_AKU340_Sense(void));

#endif /* BCDS_FEATURE_BSP_MIC_AKU340*/

#endif /* BCDS_BSP_Mic_AKU340_H */

/**  @} */
