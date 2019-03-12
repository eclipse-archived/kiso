/******************************************************************************/
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
 *  @file        spi-board.h
 *
 *  Configuration header for the spi Module.
 *
 * ****************************************************************************/

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
