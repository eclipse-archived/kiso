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
 *  @file        pinName-board.h
 *
 *  This module is used to initialize pinNames for the board
 * ****************************************************************************/

#ifndef GPIO_PIN_NAME_BOARD_H_
#define GPIO_PIN_NAME_BOARD_H_

#include <stdint.h>

/*
 * Can be moved to adapter implementation (lib/lora/adapter or whatever the path will be), because it will only change with change in semtech code
 */

#define MCU_PINS \
	RADIO_RESET, \
	RADIO_NSS, \
	RADIO_MOSI, \
	RADIO_MISO, \
	RADIO_SCLK, \
	RADIO_DIO_0, \
	RADIO_DIO_1, \
	RADIO_DIO_2, \
	RADIO_DIO_3, \
	LED_1, \
	LED_2, \
	LORA_PIN_COUNT

#endif /* GPIO_PIN_NAME_BOARD_H_ */
