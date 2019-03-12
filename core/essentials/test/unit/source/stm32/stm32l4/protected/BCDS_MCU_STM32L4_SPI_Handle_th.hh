/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file 
 *
 *
 *  Mockup implementation for the BCDS_MCU_SPI_Handle_th module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_MCU_STM32L4_SPI_HANDLE_TH_HH_
#define BCDS_MCU_STM32L4_SPI_HANDLE_TH_HH_

#if BCDS_FEATURE_SPI && BCDS_SPI_COUNT

/* Include the needed headers */
#include "BCDS_MCU_SPI_th.hh"
#include "stm32l4xx_hal_dma_th.hh"
#include "stm32l4xx_hal_spi_th.hh"

#endif /* BCDS_FEATURE_SPI && BCDS_SPI_COUNT */

/* wrap the real interface header */
#include "BCDS_MCU_SPI_Handle.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#endif /* BCDS_MCU_STM32L4_SPI_HANDLE_TH_HH_ */

/* ************************************************************************** */
