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
 * @brief interrupt handlers used within the board support package and overwriting
 * weak definition of ARM startup file.
 *@note in order for this file to be linked correctly the option --whole-archive
 *should be used for the linker script
 *
 * Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.

 -------------------------List of available IRQs-----------------------------
 (+)Core interrupts

 (++)Reset_Handler
 (++)NMI_Handler
 (++)HardFault_Handler
 (++)MemManage_Handler
 (++)BusFault_Handler
 (++)UsageFault_Handler
 (++)Default_Handler
 (++)Default_Handler
 (++)Default_Handler
 (++)Default_Handler
 (++)SVC_Handler
 (++)DebugMon_Handler
 (++)Default_Handler
 (++)PendSV_Handler
 (++)SysTick_Handler

 (+)External interrupts
 (++)DMA_IRQHandler
 (++)GPIO_EVEN_IRQHandler
 (++)TIMER0_IRQHandler
 (++)USART0_RX_IRQHandler
 (++)USART0_TX_IRQHandler
 (++)USB_IRQHandler
 (++)ACMP0_IRQHandler
 (++)ADC0_IRQHandler
 (++)DAC0_IRQHandler
 (++)I2C0_IRQHandler
 (++)GPIO_ODD_IRQHandler
 (++)TIMER1_IRQHandler
 (++)TIMER2_IRQHandler
 (++)TIMER3_IRQHandler
 (++)USART1_RX_IRQHandler
 (++)USART1_TX_IRQHandler
 (++)LESENSE_IRQHandler
 (++)USART2_RX_IRQHandler
 (++)USART2_TX_IRQHandler
 (++)UART0_RX_IRQHandler
 (++)UART0_TX_IRQHandler
 (++)UART1_RX_IRQHandler
 (++)UART1_TX_IRQHandler
 (++)LEUART0_IRQHandler
 (++)LEUART1_IRQHandler
 (++)LETIMER0_IRQHandler
 (++)PCNT0_IRQHandler
 (++)PCNT1_IRQHandler
 (++)PCNT2_IRQHandler
 (++)RTC_IRQHandler
 (++)BURTC_IRQHandler
 (++)CMU_IRQHandler
 (++)VCMP_IRQHandler
 (++)LCD_IRQHandler
 (++)MSC_IRQHandler
 (++)AES_IRQHandler
 (++)EBI_IRQHandler
 (++)EMU_IRQHandler
 ----------------------------------------------------------------------------
 */

#ifndef IrqHandler_H
#define IrqHandler_H

/**
 * @brief  SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void);

/**
 * @brief GPIO EVEN interrupt handler.
 * @details Interrupt handler clears all IF even
 * flags and call the dispatcher passing the flags which triggered the interrupt.
 */
void GPIO_EVEN_IRQHandler(void);

/**
 * @brief  GPIO ODD interrupt handler.
 * @details Interrupt handler clears all IF odd flags
 * and call the dispatcher passing the flags which triggered the interrupt.
 */
void GPIO_ODD_IRQHandler(void);

#if BCDS_FEATURE_BSP_BT_EM9301
/**
 * @brief  UART0 Tx interrupt handler.
 * @details Interrupt handler forwards all UART transmit related interrupts to
 * the MCU driver for processing.
 */
void UART0_TX_IRQHandler(void);

/**
 * @brief  UART0 Rx interrupt handler.
 * @details Interrupt handler forwards all UART receive related interrupts to
 * the MCU driver for processing.
 */
void UART0_RX_IRQHandler(void);

#endif

#if BCDS_FEATURE_BSP_WIFI_CC3100MOD
/**
 * @brief  USART0 Tx interrupt handler.
 * @details Interrupt handler forwards all USART transmit related interrupts to
 * the MCU driver for processing.
 */
void USART0_TX_IRQHandler(void);

/**
 * @brief  USART0 Rx interrupt handler.
 * @details Interrupt handler forwards all USART receive related interrupts to
 * the MCU driver for processing.
 */
void USART0_RX_IRQHandler(void);

#endif /* BCDS_FEATURE_BSP_WIFI_CC3100MOD */

#if BCDS_FEATURE_BSP_SD_CARD
/**
 * @brief  USART1 Tx interrupt handler.
 * @details Interrupt handler forwards all USART transmit related interrupts to
 * the MCU driver for processing.
 */
void USART1_TX_IRQHandler(void);

/**
 * @brief  USART1 Rx interrupt handler.
 * @details Interrupt handler forwards all USART receive related interrupts to
 * the MCU driver for processing.
 */
void USART1_RX_IRQHandler(void);

#endif /* BCDS_FEATURE_BSP_SD_CARD */

#if BCDS_FEATURE_BSP_SENSOR_NODE
/**
 * @brief  I2C0 interrupt handler.
 * @details Interrupt handler forwards all I2C receive related interrupts to
 * the MCU driver for processing.
 */
void I2C0_IRQHandler(void);
#endif /*BCDS_FEATURE_BSP_SENSOR_NODE*/

#if BCDS_FEATURE_BSP_EXTENSIONPORT
/**
* @brief  I2C1 interrupt handler.
 * @details Interrupt handler forwards all I2C receive related interrupts to
 * the MCU driver for processing.
 * @note I2C1_IRQHandler is used for the XDK extension port
 */
void I2C1_IRQHandler(void);

/**
 * @brief  USART2 Tx interrupt handler.
 * @details Interrupt handler forwards all USART transmit related interrupts to
 * the MCU driver for processing.
 * @note USART2_TX_IRQHandler is used for the Extension Bus SPI module in XDK
 */
void USART2_TX_IRQHandler(void);

/**
 * @brief  USART2 Rx interrupt handler.
 * @details Interrupt handler forwards all USART receive related interrupts to
 * the MCU driver for processing.
 * @note USART2_RX_IRQHandler is used for the Extension Bus SPI module in XDK
 */
void USART2_RX_IRQHandler(void);

/**
 * @brief  UART1 Tx interrupt handler.
 * @details Interrupt handler forwards all UART transmit related interrupts to
 * the MCU driver for processing.
 */
void UART1_TX_IRQHandler(void);

/**
 * @brief  UART1 Rx interrupt handler.
 * @details Interrupt handler forwards all UART transmit related interrupts to
 * the MCU driver for processing.
 */
void UART1_RX_IRQHandler(void);

#endif

/*@todo workaround to address weak linkage of SysTick_Handler defined in IrqHandler.c file */
Retcode_T IrqHandler_SysTickDummyHandler(void);

#endif /* IrqHandler_H */

