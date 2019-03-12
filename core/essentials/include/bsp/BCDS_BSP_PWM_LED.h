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
 * @defgroup    BCDS_HAL_BSP_PWM_LED PWM LED
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API PWM controlled LEDs.
 *
 * @details
 * The application needs to call BSP_PWM_LED_Connect() to establish
 * the link between the timer (internal MCU resource) and the LED peripheral
 * then it enables it via the BSP_PWM_LED_Enable() or BSP_PWM_LED_EnableAll()
 * then it is possible to drive it using BSP_PWM_LED_Switch() which takes as
 * input the brightness of the LED.
 *
 * The application can then disable the LED and disconnect it from the MCU
 * via BSP_PWM_LED_Disable() or BSP_PWM_LED_DisableAll() and
 * BSP_PWM_LED_Disconnect() functions.
 */

#ifndef BCDS_BSP_PWM_LED_H
#define BCDS_BSP_PWM_LED_H

#include "BCDS_HAL.h"

/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_PWM_LED

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_PWM_LED_Connect(void);

/**
 * @brief       Enables the control over a particular LED.
 *
 * @details     This function once called will allow the specified LED to receive brightness commands otherwise no
 *              effect for those commands.
 *
 * @pre         BSP_PWM_LED_Connect() successfully executed.
 *
 * @param       id: identifier of the desired LED.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_Enable(uint32_t id);

/**
 * @brief       Disables control over a particular LED.
 *
 * @param       id : identifier of the LED.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_Disable(uint32_t id);

/**
 * @brief       Disconnects the LEDs.
 *
 * @details     This function will tristate the GPIOs of the LEDs, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_Disconnect(void);

/**
 * @brief       Enables the control over all LEDs.
 *
 * @details     This function once called will allow to control all LEDs e.g controlling brightness.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_EnableAll(void);

/**
 * @brief       Switches on/off a particular LED.
 *
 * @details     This function is called to change the brightness of an LED from 0% to 100%.
 *
 * @param       id : identifier of the desired LED.
 *
 * @param       brightnessPercent
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_Switch(uint32_t id, uint32_t brightnessPercent);

/**
 * @brief       Switches on/off all LEDs.
 *
 * @details     This function is called to change the brightness of all LEDs from 0% to 100%.
 *
 * @param       brightnessPercent
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_SwitchAll(uint32_t brightnessPercent);

/**
 * @brief       Disables control over all LEDs.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_PWM_LED_DisableAll(void);

#endif /* BCDS_FEATURE_BSP_PWM_LED */

#endif /* BSP_PWM_LED_H */

/**  @} */
