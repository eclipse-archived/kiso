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
 *  @file
 *
 * module is an abstraction layer of an EFM32 GPIO module
 *
 * ****************************************************************************/

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
