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
 * @brief Contains a default configurations of buttons on XDK extension board
 */
#ifndef BCDS_BOARD_EXTENSION_H_
#define BCDS_BOARD_EXTENSION_H_

/* Extenstion Button Settings */
#if BCDS_FEATURE_BSP_BUTTON_EXT

#if BSP_XDK_EXT_BUTTON_A1
#define EXT_BUTTON_A1_PIN                                                                (0)
#define EXT_BUTTON_A1_PORT                                                               (gpioPortA)
#define EXT_BUTTON_A1_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A1_PULL                                                               (1)
#define EXT_BUTTON_A1_EDGE_RISING                                                        true
#define EXT_BUTTON_A1_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A1 */

#if BSP_XDK_EXT_BUTTON_A2
#define EXT_BUTTON_A2_PIN                                                                (0)
#define EXT_BUTTON_A2_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A2_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A2_PULL                                                               (1)
#define EXT_BUTTON_A2_EDGE_RISING                                                        true
#define EXT_BUTTON_A2_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A2 */

#if BSP_XDK_EXT_BUTTON_A3
#define EXT_BUTTON_A3_PIN                                                                (1)
#define EXT_BUTTON_A3_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A3_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A3_PULL                                                               (1)
#define EXT_BUTTON_A3_EDGE_RISING                                                        true
#define EXT_BUTTON_A3_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A3 */

#if BSP_XDK_EXT_BUTTON_A4
#define EXT_BUTTON_A4_PIN                                                                (2)
#define EXT_BUTTON_A4_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A4_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A4_PULL                                                               (1)
#define EXT_BUTTON_A4_EDGE_RISING                                                        true
#define EXT_BUTTON_A4_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A4 */

#if BSP_XDK_EXT_BUTTON_A5
#define EXT_BUTTON_A5_PIN                                                                (3)
#define EXT_BUTTON_A5_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A5_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A5_PULL                                                               (1)
#define EXT_BUTTON_A5_EDGE_RISING                                                        true
#define EXT_BUTTON_A5_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A5 */

#if BSP_XDK_EXT_BUTTON_A6
#define EXT_BUTTON_A6_PIN                                                                (4)
#define EXT_BUTTON_A6_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A6_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A6_PULL                                                               (1)
#define EXT_BUTTON_A6_EDGE_RISING                                                        true
#define EXT_BUTTON_A6_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A6 */

#if BSP_XDK_EXT_BUTTON_A7
#define EXT_BUTTON_A7_PIN                                                                (8)
#define EXT_BUTTON_A7_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A7_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A7_PULL                                                               (1)
#define EXT_BUTTON_A7_EDGE_RISING                                                        true
#define EXT_BUTTON_A7_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A7 */

#if BSP_XDK_EXT_BUTTON_A8
#define EXT_BUTTON_A8_PIN                                                                (9)
#define EXT_BUTTON_A8_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A8_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A8_PULL                                                               (1)
#define EXT_BUTTON_A8_EDGE_RISING                                                        true
#define EXT_BUTTON_A8_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A8 */

#if BSP_XDK_EXT_BUTTON_A9
#define EXT_BUTTON_A9_PIN                                                                (10)
#define EXT_BUTTON_A9_PORT                                                               (gpioPortC)
#define EXT_BUTTON_A9_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A9_PULL                                                               (1)
#define EXT_BUTTON_A9_EDGE_RISING                                                        true
#define EXT_BUTTON_A9_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A9 */

#if BSP_XDK_EXT_BUTTON_A10
#define EXT_BUTTON_A10_PIN                                                                (6)
#define EXT_BUTTON_A10_PORT                                                               (gpioPortD)
#define EXT_BUTTON_A10_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A10_PULL                                                               (1)
#define EXT_BUTTON_A10_EDGE_RISING                                                        true
#define EXT_BUTTON_A10_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A10 */

#if BSP_XDK_EXT_BUTTON_A11
#define EXT_BUTTON_A11_PIN                                                                (5)
#define EXT_BUTTON_A11_PORT                                                               (gpioPortD)
#define EXT_BUTTON_A11_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A11_PULL                                                               (1)
#define EXT_BUTTON_A11_EDGE_RISING                                                        true
#define EXT_BUTTON_A11_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A11 */

#if BSP_XDK_EXT_BUTTON_A12
#define EXT_BUTTON_A12_PIN                                                                (1)
#define EXT_BUTTON_A12_PORT                                                               (gpioPortA)
#define EXT_BUTTON_A12_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A12_PULL                                                               (1)
#define EXT_BUTTON_A12_EDGE_RISING                                                        true
#define EXT_BUTTON_A12_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A12 */

#if BSP_XDK_EXT_BUTTON_A13
#define EXT_BUTTON_A13_PIN                                                                (2)
#define EXT_BUTTON_A13_PORT                                                               (gpioPortE)
#define EXT_BUTTON_A13_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_A13_PULL                                                               (1)
#define EXT_BUTTON_A13_EDGE_RISING                                                        true
#define EXT_BUTTON_A13_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_A13 */

#if BSP_XDK_EXT_BUTTON_B1
#define EXT_BUTTON_B1_PIN                                                                (9)
#define EXT_BUTTON_B1_PORT                                                               (gpioPortB)
#define EXT_BUTTON_B1_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B1_PULL                                                               (1)
#define EXT_BUTTON_B1_EDGE_RISING                                                        true
#define EXT_BUTTON_B1_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B1 */

#if BSP_XDK_EXT_BUTTON_B2
#define EXT_BUTTON_B2_PIN                                                                (10)
#define EXT_BUTTON_B2_PORT                                                               (gpioPortB)
#define EXT_BUTTON_B2_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B2_PULL                                                               (1)
#define EXT_BUTTON_B2_EDGE_RISING                                                        true
#define EXT_BUTTON_B2_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B2 */

#if BSP_XDK_EXT_BUTTON_B3
#define EXT_BUTTON_B3_PIN                                                                (2)
#define EXT_BUTTON_B3_PORT                                                               (gpioPortB)
#define EXT_BUTTON_B3_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B3_PULL                                                               (1)
#define EXT_BUTTON_B3_EDGE_RISING                                                        true
#define EXT_BUTTON_B3_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B3 */

#if BSP_XDK_EXT_BUTTON_B4
#define EXT_BUTTON_B4_PIN                                                                (6)
#define EXT_BUTTON_B4_PORT                                                               (gpioPortF)
#define EXT_BUTTON_B4_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B4_PULL                                                               (1)
#define EXT_BUTTON_B4_EDGE_RISING                                                        true
#define EXT_BUTTON_B4_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B4 */

#if BSP_XDK_EXT_BUTTON_B5
#define EXT_BUTTON_B5_PIN                                                                (4)
#define EXT_BUTTON_B5_PORT                                                               (gpioPortB)
#define EXT_BUTTON_B5_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B5_PULL                                                               (1)
#define EXT_BUTTON_B5_EDGE_RISING                                                        true
#define EXT_BUTTON_B5_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B5 */

#if BSP_XDK_EXT_BUTTON_B6
#define EXT_BUTTON_B6_PIN                                                                (3)
#define EXT_BUTTON_B6_PORT                                                               (gpioPortB)
#define EXT_BUTTON_B6_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B6_PULL                                                               (1)
#define EXT_BUTTON_B6_EDGE_RISING                                                        true
#define EXT_BUTTON_B6_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B6 */

#if BSP_XDK_EXT_BUTTON_B7
#define EXT_BUTTON_B7_PIN                                                                (5)
#define EXT_BUTTON_B7_PORT                                                               (gpioPortB)
#define EXT_BUTTON_B7_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B7_PULL                                                               (1)
#define EXT_BUTTON_B7_EDGE_RISING                                                        true
#define EXT_BUTTON_B7_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B7 */

#if BSP_XDK_EXT_BUTTON_B8
#define EXT_BUTTON_B8_PIN                                                                (8)
#define EXT_BUTTON_B8_PORT                                                               (gpioPortD)
#define EXT_BUTTON_B8_MODE                                                               (gpioModeInputPullFilter)
#define EXT_BUTTON_B8_PULL                                                               (1)
#define EXT_BUTTON_B8_EDGE_RISING                                                        true
#define EXT_BUTTON_B8_EDGE_FALLING                                                       true
#endif /* BSP_XDK_EXT_BUTTON_B8 */

#endif /* #if BCDS_FEATURE_BSP_BUTTON_EXT */

/* Extenstion LED Settings */
#if BCDS_FEATURE_BSP_LED_EXT

#if BSP_XDK_EXT_LED_A1
#define EXT_LED_A1_PIN                                                                (0)
#define EXT_LED_A1_PORT                                                               (gpioPortA)
#define EXT_LED_A1_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A1_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A1 */

#if BSP_XDK_EXT_LED_A2
#define EXT_LED_A2_PIN                                                                (0)
#define EXT_LED_A2_PORT                                                               (gpioPortC)
#define EXT_LED_A2_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A2_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A2 */

#if BSP_XDK_EXT_LED_A3
#define EXT_LED_A3_PIN                                                                (1)
#define EXT_LED_A3_PORT                                                               (gpioPortC)
#define EXT_LED_A3_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A3_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A3 */

#if BSP_XDK_EXT_LED_A4
#define EXT_LED_A4_PIN                                                                (2)
#define EXT_LED_A4_PORT                                                               (gpioPortC)
#define EXT_LED_A4_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A4_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A4 */

#if BSP_XDK_EXT_LED_A5
#define EXT_LED_A5_PIN                                                                (3)
#define EXT_LED_A5_PORT                                                               (gpioPortC)
#define EXT_LED_A5_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A5_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A5 */

#if BSP_XDK_EXT_LED_A6
#define EXT_LED_A6_PIN                                                                (4)
#define EXT_LED_A6_PORT                                                               (gpioPortC)
#define EXT_LED_A6_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A6_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A6 */

#if BSP_XDK_EXT_LED_A7
#define EXT_LED_A7_PIN                                                                (8)
#define EXT_LED_A7_PORT                                                               (gpioPortC)
#define EXT_LED_A7_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A7_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A7 */

#if BSP_XDK_EXT_LED_A8
#define EXT_LED_A8_PIN                                                                (9)
#define EXT_LED_A8_PORT                                                               (gpioPortC)
#define EXT_LED_A8_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A8_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A8 */

#if BSP_XDK_EXT_LED_A9
#define EXT_LED_A9_PIN                                                                (10)
#define EXT_LED_A9_PORT                                                               (gpioPortC)
#define EXT_LED_A9_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A9_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A9 */

#if BSP_XDK_EXT_LED_A10
#define EXT_LED_A10_PIN                                                                (6)
#define EXT_LED_A10_PORT                                                               (gpioPortD)
#define EXT_LED_A10_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A10_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A10 */

#if BSP_XDK_EXT_LED_A11
#define EXT_LED_A11_PIN                                                                (5)
#define EXT_LED_A11_PORT                                                               (gpioPortD)
#define EXT_LED_A11_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A11_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A11 */

#if BSP_XDK_EXT_LED_A12
#define EXT_LED_A12_PIN                                                                (1)
#define EXT_LED_A12_PORT                                                               (gpioPortA)
#define EXT_LED_A12_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A12_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A12 */

#if BSP_XDK_EXT_LED_A13
#define EXT_LED_A13_PIN                                                                (2)
#define EXT_LED_A13_PORT                                                               (gpioPortE)
#define EXT_LED_A13_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_A13_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_A13 */

#if BSP_XDK_EXT_LED_B1
#define EXT_LED_B1_PIN                                                                (9)
#define EXT_LED_B1_PORT                                                               (gpioPortB)
#define EXT_LED_B1_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B1_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B1 */

#if BSP_XDK_EXT_LED_B2
#define EXT_LED_B2_PIN                                                                (10)
#define EXT_LED_B2_PORT                                                               (gpioPortB)
#define EXT_LED_B2_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B2_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B2 */

#if BSP_XDK_EXT_LED_B3
#define EXT_LED_B3_PIN                                                                (2)
#define EXT_LED_B3_PORT                                                               (gpioPortB)
#define EXT_LED_B3_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B3_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B3 */

#if BSP_XDK_EXT_LED_B4
#define EXT_LED_B4_PIN                                                                (6)
#define EXT_LED_B4_PORT                                                               (gpioPortF)
#define EXT_LED_B4_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B4_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B4 */

#if BSP_XDK_EXT_LED_B5
#define EXT_LED_B5_PIN                                                                (4)
#define EXT_LED_B5_PORT                                                               (gpioPortB)
#define EXT_LED_B5_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B5_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B5 */

#if BSP_XDK_EXT_LED_B6
#define EXT_LED_B6_PIN                                                                (3)
#define EXT_LED_B6_PORT                                                               (gpioPortB)
#define EXT_LED_B6_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B6_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B6 */

#if BSP_XDK_EXT_LED_B7
#define EXT_LED_B7_PIN                                                                (5)
#define EXT_LED_B7_PORT                                                               (gpioPortB)
#define EXT_LED_B7_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B7_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B7 */

#if BSP_XDK_EXT_LED_B8
#define EXT_LED_B8_PIN                                                                (8)
#define EXT_LED_B8_PORT                                                               (gpioPortD)
#define EXT_LED_B8_MODE                                                               (gpioModePushPullDrive)
#define EXT_LED_B8_PULL                                                               (0)
#endif /* BSP_XDK_EXT_LED_B8 */

#endif /* #if BCDS_FEATURE_BSP_LED_EXT */

#endif /*BCDS_BOARD_EXTENSION_H_*/
