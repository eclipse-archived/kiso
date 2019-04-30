/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
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
