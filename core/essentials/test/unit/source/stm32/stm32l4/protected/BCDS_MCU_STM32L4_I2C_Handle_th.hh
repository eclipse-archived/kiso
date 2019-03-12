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

#ifndef BCDS_MCU_STM32L4_I2C_HANDLE_TH_HH
#define BCDS_MCU_STM32L4_I2C_HANDLE_TH_HH

/* Include the needed headers */
#include "BCDS_MCU_I2C_th.hh"

#if BCDS_FEATURE_I2C &&  BCDS_I2C_COUNT

#include "stm32l4xx_hal_dma_th.hh"
#include "stm32l4xx_hal_i2c_th.hh"

#endif /* BCDS_FEATURE_I2C && BCDS_I2C_COUNT */

/* wrap the real interface header */
#include "BCDS_MCU_I2C_Handle.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */


#endif /* BCDS_MCU_STM32L4_I2C_HANDLE_TH_HH */
