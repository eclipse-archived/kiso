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
 * @brief Defines MCU Timer handle
 *
 *
 * @details
 *
 */
#ifndef BCDS_MCU_TIMER_HANDLE_H_
#define BCDS_MCU_TIMER_HANDLE_H_

/* Include all headers which are needed by this file. */
#if defined (stm32f7)
    #include "stm32f7/BCDS_MCU_STM32F7_Timer_Handle.h"
#else
    #error "The STM32 family is not defined or not supported."
#endif
/* Put the type and macro definitions here */

/* Put the function declarations here */

#endif /* BCDS_MCU_TIMER_HANDLE_H_ */
