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
