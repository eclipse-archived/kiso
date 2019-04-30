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
