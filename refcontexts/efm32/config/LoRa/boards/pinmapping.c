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


/* system header files */

/* additional interface header files */

/* own header files */
#include "pinmapping.h"

/* functions */
/*
 * mapping of lora pins to actual efm32 gpio pins
 */
void loraPinMapInit(void)
{
#if defined (A1_SAMPLE)
    loraEfm32PinMap[RADIO_RESET].port = gpioPortC;
    loraEfm32PinMap[RADIO_RESET].pin = 1;
    loraEfm32PinMap[RADIO_NSS].port = gpioPortC;
    loraEfm32PinMap[RADIO_NSS].pin = 5;
    loraEfm32PinMap[RADIO_MOSI].port = gpioPortC;
    loraEfm32PinMap[RADIO_MOSI].pin = 2;

    loraEfm32PinMap[RADIO_MISO].port = gpioPortC;
    loraEfm32PinMap[RADIO_MISO].pin = 3;

    loraEfm32PinMap[RADIO_SCLK].port = gpioPortC;
    loraEfm32PinMap[RADIO_SCLK].pin = 4;

    loraEfm32PinMap[RADIO_DIO_0].port = gpioPortC;
    loraEfm32PinMap[RADIO_DIO_0].pin = 0;

    loraEfm32PinMap[RADIO_DIO_1].port = gpioPortB;
    loraEfm32PinMap[RADIO_DIO_1].pin = 14;

    loraEfm32PinMap[RADIO_DIO_2].port = gpioPortB;
    loraEfm32PinMap[RADIO_DIO_2].pin = 13;

    loraEfm32PinMap[RADIO_DIO_3].port = gpioPortB;
    loraEfm32PinMap[RADIO_DIO_3].pin = 12;

    loraEfm32PinMap[LED_1].port = gpioPortA;
    loraEfm32PinMap[LED_1].pin = 8;

    loraEfm32PinMap[LED_2].port = gpioPortA;
    loraEfm32PinMap[LED_2].pin = 9;
#elif defined (R1_SAMPLE) || defined (R3_SAMPLE) || defined (R4_SAMPLE) || defined (R5_SAMPLE)
    loraEfm32PinMap[RADIO_RESET].port = gpioPortC;
    loraEfm32PinMap[RADIO_RESET].pin = 1;

    loraEfm32PinMap[RADIO_NSS].port = gpioPortC;
    loraEfm32PinMap[RADIO_NSS].pin = 5;

    loraEfm32PinMap[RADIO_MOSI].port = gpioPortC;
    loraEfm32PinMap[RADIO_MOSI].pin = 2;

    loraEfm32PinMap[RADIO_MISO].port = gpioPortC;
    loraEfm32PinMap[RADIO_MISO].pin = 3;

    loraEfm32PinMap[RADIO_SCLK].port = gpioPortC;
    loraEfm32PinMap[RADIO_SCLK].pin = 4;

    loraEfm32PinMap[RADIO_DIO_0].port = gpioPortC;
    loraEfm32PinMap[RADIO_DIO_0].pin = 0;

    loraEfm32PinMap[RADIO_DIO_1].port = gpioPortB;
    loraEfm32PinMap[RADIO_DIO_1].pin = 14;

    loraEfm32PinMap[RADIO_DIO_2].port = gpioPortB;
    loraEfm32PinMap[RADIO_DIO_2].pin = 13;

    loraEfm32PinMap[RADIO_DIO_3].port = gpioPortB;
    loraEfm32PinMap[RADIO_DIO_3].pin = 12;
#endif
}
