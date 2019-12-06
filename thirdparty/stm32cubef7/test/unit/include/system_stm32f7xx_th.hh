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
 *
 * @file
 *
 * Mock-up implementation for the system_stm32f7xx module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef SYSTEM_STM32F7XX_TH_HH
#define SYSTEM_STM32F7XX_TH_HH

/* Include the needed headers */
#include "gtest.h"

/* Include or ban the real interface header */
#include "system_stm32f7xx.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

FAKE_VOID_FUNC(SystemInit);

FAKE_VOID_FUNC(SystemCoreClockUpdate);

# endif /* SYSTEM_STM32F7XX_TH_HH */
