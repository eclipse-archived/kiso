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
#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcu.h"
#include "em_cmu.h"
#include "em_rtc.h"
#include "em_int.h"
#include "pinmapping.h"
#include "string.h"
#include "rtc-board.h"
#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "delay.h"
#include "radio.h"
#include "sx1272.h"

/**
* SWAP function to convert controller endian format
*/
#define SWAP32(x)     ((((x) >> 24)&0xff) | (((x)<<8)&0xff0000) \
                            | (((x)>>8)&0xff00) | (((x)<<24)&0xff000000))

/**
* Unique Devices IDs register set (EFM32)
*/
#define ID1             (0x0FE081F0)
#define ID2             (0x0FE081F4)
#define SET				UINT8_C(1)
#define CLEAR			UINT8_C(0)

/**
* Random seed generated using the MCU Unique ID
*/
#define RAND_SEED      (( *( uint32_t* )ID1 ) ^ \
                           (*( uint32_t* )ID2 ))
/**
 * @brief   Function to find the SEED for randam number calculation.
 *
 * @param[in] seed : unique ID of the EFM32 controller
 *
 */
void srand(unsigned int seed);

enum BoardPowerSource
{
    USB_POWER = 0,
    BATTERY_POWER
};

/*!
 * \brief Initializes the target board peripherals.
 */
void BoardInitMcu( void );

/*!
 * \brief Initializes the boards peripherals.
 */
void BoardInitPeriph( void );

/*!
 * \brief De-initializes the target board peripherals to decrease power
 *        consumption.
 */
void BoardDeInitMcu( void );

/*!
 * \brief Measure the Battery level
 *
 * \retval value  battery level ( 0: very low, 254: fully charged )
 */
uint8_t BoardMeasureBatterieLevel( void );

/*!
 * \brief Gets the board 64 bits unique ID
 *
 * \param [IN] id Pointer to an array that will contain the Unique ID
 */
void BoardGetUniqueId( uint8_t *id );

/*!
 * \brief Get the board power source
 *
 * \retval value  power source ( 0: USB_POWER,  1: BATTERY_POWER )
 */
uint8_t GetBoardPowerSource( void );

#endif /* BOARD_H_ */
