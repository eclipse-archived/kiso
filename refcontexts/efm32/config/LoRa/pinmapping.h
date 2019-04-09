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
