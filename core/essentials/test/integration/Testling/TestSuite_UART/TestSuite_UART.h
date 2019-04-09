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

#ifndef TESTSUITE_UART_H_
#define TESTSUITE_UART_H_

enum TestSuite_3_RetCode_E
{
    TESTSUITE_3_SETUP_TRIGGERED_SEVERAL_TIMES = RETCODE_FIRST_CUSTOM_CODE,
    TESTSUITE_3_RUN_TRIGGERED_SEVERAL_TIMES,
    TESTSUITE_3_TEARDOWN_TRIGGERED_SEVERAL_TIMES,
};

Retcode_T TestSuite_UART_initialize(uint8_t testSuiteId);


//#if BCDS_FEATURE_BSP_UART_TEST

/**
 * @brief UART transmit interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void UART_TEST_UART_TxISR(void);

/**
 * @brief UART receive interrupt service routine used to capture interrupts at the UART
 * resource and to forward them to the MCU driver for processing.
 */
void UART_TEST_UART_RxISR(void);

void UART1_RX_IRQHandler(void);

void UART1_TX_IRQHandler(void);



#endif /* TESTSUITE_SPI_H_ */
