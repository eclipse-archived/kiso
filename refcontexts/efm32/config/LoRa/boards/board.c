/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Target board general functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
/*****************************************************
 * This file has Modified as per the BCDS-APLM requirements
 *****************************************************/
#include "board.h"
#include "sx1272-board.h"
#include "PTD_portDriver_ih.h"
#include "rtc-board.h"
#include "timer.h"
#include "PTD_portDriver_ph.h"

/* instance of LED GPIO pins objects */
Gpio_t Led1;
Gpio_t Led2;
Gpio_t Led3;

 /* Flag to indicate if the MCU is Initialized */
static bool McuInitialized = false;

/* Radio hardware and global parameters */
extern SX1272_t SX1272;

/* API documentation is in the header file */
void BoardInitPeriph( void )
{

}

/* API documentation is in the header file */
void BoardInitMcu( void )
{
    if( McuInitialized == false )
    {
        SpiInit( &SX1272.Spi, RADIO_MOSI, RADIO_MISO, RADIO_SCLK, NC );
        SX1272IoInit( );
        BoardInitPeriph( );
    	RtcInit();
        McuInitialized = true;
   }
}

/* API documentation is in the header file */
void BoardDeInitMcu( void )
{
    SpiDeInit( &SX1272.Spi );
    SX1272IoDeInit( );
    McuInitialized = false;
}

/* API documentation is in the header file */
void BoardGetUniqueId( uint8_t *id )
{
    id[0] =  ( *( uint32_t* )ID1 ) >> 24;
    id[1] =  ( *( uint32_t* )ID1 ) >> 16;
    id[2] =  ( *( uint32_t* )ID1 ) >> 8;
    id[3] =  ( *( uint32_t* )ID1 );
    id[4] =  ( *( uint32_t* )ID2 ) >> 24;
    id[5] =  ( *( uint32_t* )ID2 ) >> 16;
    id[6] =  ( *( uint32_t* )ID2 ) >> 8;
    id[7] =  ( *( uint32_t* )ID2);
}

/* API documentation is in the header file */
uint8_t BoardMeasureBatterieLevel( void ) {
 return (true);
}
