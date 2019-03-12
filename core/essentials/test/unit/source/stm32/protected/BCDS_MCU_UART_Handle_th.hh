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

#ifndef BCDS_MCU_UART_HANDLE_TH_HH
#define BCDS_MCU_UART_HANDLE_TH_HH

/* Include the needed headers */

#if defined (stm32l4)
    #include "stm32l4/BCDS_MCU_STM32L4_UART_Handle_th.hh"
#elif defined (stm32f7)
     #include "stm32f7/BCDS_MCU_STM32F7_UART_Handle_th.hh"
#else
    #error "The STM32 family is not defined or not supported."
#endif

/* wrap the real interface header */

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#endif /* BCDS_MCU_UART_HANDLE_TH_HH */
