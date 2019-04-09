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
