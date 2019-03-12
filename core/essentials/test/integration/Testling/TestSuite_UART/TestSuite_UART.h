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
