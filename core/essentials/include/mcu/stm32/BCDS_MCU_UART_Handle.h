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
 * @brief
 * Defines MCU uart handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type UART_T.
 * This integer is casted by MCU to a pointer to MCU_UART_handle_T.
 * The data pointed to by USART_TypeDef* is managed in BSP.
 */

#ifndef BCDS_MCU_UART_HANDLE_H_
#define BCDS_MCU_UART_HANDLE_H_

#if defined (stm32l4)
    #include "stm32l4/BCDS_MCU_STM32L4_UART_Handle.h"
#elif defined (stm32f7)
 	 #include "stm32f7/BCDS_MCU_STM32F7_UART_Handle.h"
#else
    #error "The STM32 family is not defined or not supported."
#endif

#endif /* BCDS_MCU_UART_HANDLE_H_ */
