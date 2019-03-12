/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch.
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
 
#ifndef BCDS_MCU_TIMER_HANDLE_TH_HH_
#define BCDS_MCU_TIMER_HANDLE_TH_HH_

/* Include the needed headers */

#if defined (stm32f7)
    #include "stm32f7/BCDS_MCU_STM32F7_Timer_Handle_th.hh"
#else
    #error "The STM32 family is not defined or not supported."
#endif

/* wrap the real interface header */

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#endif /* BCDS_MCU_TIMER_HANDLE_TH_HH_ */
