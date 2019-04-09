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

 * @file
 *
 * @brief Configuration header for the Logging module.
 *
 */

#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

/* Configuration for the Asyncronous Recorder */
#define LOG_BUFFER_SIZE             (UINT16_C(256))
#define LOG_QUEUE_BUFFER_SIZE       (UINT16_C(2048))
#define LOG_TASK_STACK_SIZE         (UINT16_C(128))
#define LOG_TASK_PRIORITY           (UINT8_C(4))
#define LOG_FILTER_ITEM_COUNT       (UINT8_C(8))

/* Configuration for the UART Appender */
#define LOG_GPIO_TX_PRT             (gpioPortB)
#define LOG_GPIO_TX_PIN             (UINT8_C(3))
#define LOG_GPIO_TX_EN              (gpioModePushPullDrive)
#define LOG_GPIO_TX_DIS             (gpioModeDisabled)
#define LOG_SER_HWTYPE              (SER_HWTYPE_USART)
#define LOG_SER_PORT                (USART2)
#define LOG_SER_ROUTE_LOCATION      (USART_ROUTE_LOCATION_LOC1)
#define LOG_SER_PROTOCOL            (SER_UART_PROTOCOL)
#define LOG_SER_BAUDRATE            (UINT16_C(115200))
#define LOG_SER_HW_FLOW_CTRL        (UINT8_C(0))
#define LOG_SER_PARITY              (usartNoParity)
#define LOG_SER_DATABITS            (usartDatabits8)
#define LOG_SER_STOPBITS            (usartStopbits1)
#define LOG_SER_CMU_CLOCK           (cmuClock_USART2)
#define LOG_DMA_CHAN                (UINT8_C(0))

/* Default log level for package and module */
#define LOG_LEVEL_PACKAGE_DEFAULT   (LOG_LEVEL_INFO)
#define LOG_LEVEL_MODULE_DEFAULT    (LOG_LEVEL_INFO)

#endif /* LOG_CONFIG_H */
