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
 *
 *  Mockup implementation for the BCDS_MCU_SPI_Handle_th module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_MCU_STM32F7_SPI_HANDLE_TH_HH_
#define BCDS_MCU_STM32F7_SPI_HANDLE_TH_HH_

#if BCDS_FEATURE_SPI && BCDS_SPI_COUNT

/* Include the needed headers */
#include "BCDS_MCU_SPI_th.hh"
#include "stm32f7xx_hal_dma_th.hh"
#include "stm32f7xx_hal_spi_th.hh"

#endif /* BCDS_FEATURE_SPI && BCDS_SPI_COUNT */

/* wrap the real interface header */
#include "BCDS_MCU_SPI_Handle.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#endif /* BCDS_MCU_STM32F7_SPI_HANDLE_TH_HH_ */

/* ************************************************************************** */
