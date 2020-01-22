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

#include "protected/gpio.h"
#include "BSP_NucleoF767.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint16_t gpioPins[GPIO_PORT_LAST] = {0};

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 */
void GPIO_OpenClockGate(enum GPIO_Ports_E port, uint16_t pinNames)
{
    if (GPIO_PORT_LAST >= port)
    {
        uint32_t port_idx = (0x1UL << port);
        SET_BIT(RCC->AHB1ENR, port_idx);
        /* Delay after an RCC peripheral clock enabling */
        volatile uint32_t tmpreg = READ_BIT(RCC->AHB1ENR, port_idx);
        KISO_UNUSED(tmpreg);

        gpioPins[port] |= pinNames;
    }
}

/**
 * See API interface for function documentation
 */
void GPIO_CloseClockGate(enum GPIO_Ports_E port, uint16_t pinNames)
{
    if (GPIO_PORT_LAST >= port)
    {
        gpioPins[port] &= ~pinNames;
        if (!gpioPins[port])
        {
            uint32_t port_idx = (0x1UL << port);
            (RCC->AHB1ENR &= ~(port_idx));
        }
    }
}

/**
 * See API interface for function documentation
 */
bool GPIO_GetClockGate(enum GPIO_Ports_E port)
{
    return (bool)gpioPins[port];
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
