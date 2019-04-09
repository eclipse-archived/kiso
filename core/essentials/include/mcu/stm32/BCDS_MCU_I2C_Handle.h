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
 *
 * @brief Declares the I2C handle used by BSP and MCU for STM32 targets
 *
 */

#ifndef BCDS_MCU_I2C_HANDLE_H_
#define BCDS_MCU_I2C_HANDLE_H_

#if defined (stm32l4)
    #include "stm32l4/BCDS_MCU_STM32L4_I2C_Handle.h"
#elif defined (stm32f7)
    #include "stm32f7/BCDS_MCU_STM32F7_I2C_Handle.h"
#else
    #error "The STM32 family is not defined or not supported."
#endif

#endif /* BCDS_MCU_I2C_HANDLE_H_ */
