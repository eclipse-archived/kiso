/*
 * pinmapping.h
 *
 *  Created on: 12.11.2014
 *      Author: wen8rt
 */

#ifndef PINMAPPING_H_
#define PINMAPPING_H_

#include "gpio.h"
#include "PTD_portDriver_ih.h"

typedef struct LoraEFM32GpioPin_s {
	GPIO_Port_TypeDef port;
	uint8_t pin;
} LoRaEFM32GpioPin_t;

LoRaEFM32GpioPin_t loraEfm32PinMap[LORA_PIN_COUNT];

void loraPinMapInit(void);

#endif /* PINMAPPING_H_ */
