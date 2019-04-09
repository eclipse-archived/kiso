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


/* header definition ******************************************************** */
#ifndef APLM_SPI_CH_H_
#define APLM_SPI_CH_H_

#include "SPI_ih.h"

/* local type and macro definitions ***************************************** */
#define RING_BUFFER_SIZE        UINT32_C(50)           /**< defines ring buffer size used for SPI driver>*/
#define SPI_LOW_BITRATE         UINT32_C(7000000)      /**< defines Speed of the SPI drivers>*/
#define LOW                     UINT8_C(0)             /**< Defines active LOW */
#define HIGH                    UINT8_C(1)             /**< Defines active HIGH */

/** 
  * @brief Serial Peripheral Interface
  */

typedef struct __SPI_HandleTypeDef
{
	uint32_t *Instance;    /*!< SPI registers base address */
}SPI_HandleTypeDef;

/**
 *  @brief
 *      Describes structure of a SPI device handle.
 *
 */
struct Spi_s
{
    SPI_HandleTypeDef Spi;
    Gpio_t Mosi;
    Gpio_t Miso;
    Gpio_t Sclk;
    Gpio_t Nss;
};

#endif /* APLM_SPI_CH_H_ */
