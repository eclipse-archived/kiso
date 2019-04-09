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


/* module includes ********************************************************** */

/* system header files */
#include <stdint.h>
#include <stdlib.h>

/* additional interface header files */
#include "em_cmu.h"
#include "PTD_portDriver_ih.h"
#include "BCDS_LoRaConfig.h"
#include "gpio.h"
#include "PTD_portDriver_ph.h"

/* own header files */
#include "spi.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */
static uint8_t transmitRingBuffer_gmd[RING_BUFFER_SIZE];              /**< Ring Buffer for low level SPI interface */
static uint8_t receiveRingBuffer_gmd[RING_BUFFER_SIZE];               /**< Ring Buffer for low level SPI interface */
static SPI_device_t spiHandle;                                    /**< Global SPI driver handle */

/* global variables ********************************************************* */

/* API documentation is in the header file */
void spiClockInit(void)
{
    /* Enable clock to USART1/SPI module */
    CMU_ClockEnable(cmuClock_USART2, true);

    /* Enable clock to CPIO  */
    CMU_ClockEnable(cmuClock_GPIO, true);
}

/* API documentation is in the header file */
void SpiInit( Spi_t *obj, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{
    SPI_initParams_t initParams;
    (void)miso ;
    (void)mosi ;
    (void)sclk ;
    (void)nss;
    obj->Spi.Instance = ( uint32_t *)  USART2_BASE;
    initParams.portNumber     = SPI_PORT_NUMBER;
    initParams.clockMode      = SPI_CLK_MODE;
    initParams.routeLocation  = SPI_ROUTE_LOCATION;
    initParams.txBuf_p        = transmitRingBuffer_gmd;
    initParams.rxBuf_p        = receiveRingBuffer_gmd;
    initParams.txBufSize      = RING_BUFFER_SIZE;
    initParams.rxBufSize      = RING_BUFFER_SIZE;
    initParams.baudrate       = SPI_LOW_BITRATE;

    spiClockInit();

    /* pin setting for chip select of SD card */
    PTD_pinModeSet(PTD_PORT_SPI_CS, PTD_PIN_SPI_CS, PTD_MODE_SPI_CS,
        PTD_DOUT_SPI_CS);

    /* MOSI configuration */
    PTD_pinModeSet(PTD_PORT_SPI_MOSI, PTD_PIN_SPI_MOSI, PTD_MODE_SPI_MOSI,
        PTD_DOUT_SPI_MOSI);

    /* MISO configuration */
    PTD_pinModeSet(PTD_PORT_SPI_MISO, PTD_PIN_SPI_MISO, PTD_MODE_SPI_MISO,
        PTD_DOUT_SPI_MISO);

    /* Clock configuration */
    PTD_pinModeSet(PTD_PORT_SPI_CLK, PTD_PIN_SPI_CLK, PTD_MODE_SPI_CLK,
        PTD_DOUT_SPI_CLK);

    SPI_init(&spiHandle,&initParams);
}

/* API documentation is in the header file*/
void SpiDeInit( Spi_t *obj )
{
    if(NULL != obj)
    {
        SPI_deinit(&spiHandle);
    }
}

/* API documentation is in the header file*/
uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
    obj = obj;
    uint8_t data = (uint8_t)outData;
    uint8_t value = 0 ;

    data = SPI_writeReadInOut(&spiHandle,&data,&value);
    return (value) ;
}

/** ************************************************************************* */
