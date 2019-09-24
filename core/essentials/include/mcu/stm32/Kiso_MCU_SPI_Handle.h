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

/**
 * @file
 * @brief
 * Defines MCU spi handle
 *
 * @details
 * MCU functions do always have at least an integer parameter of type SPI_T.
 * This integer is casted by MCU to a pointer to MCU_SPI_handle_T.
 * The data pointed to by SPI_TypeDef* is managed in BSP.
 */

#ifndef KISO_MCU_SPI_HANDLE_H_
#define KISO_MCU_SPI_HANDLE_H_

#if defined(stm32l4)
#include "stm32l4/Kiso_MCU_STM32L4_SPI_Handle.h"
#elif defined(stm32f7)
#include "stm32f7/Kiso_MCU_STM32F7_SPI_Handle.h"
#else
#error "The STM32 family is not defined or not supported."
#endif

#endif /* KISO_MCU_SPI_HANDLE_H_ */
