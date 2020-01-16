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

#include "protected/bitband.h"
#include "protected/gpio.h"
#include "BSP_NucleoL4R5ZI.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint16_t gpioA = 0x0000; /**< Each bit corresponds to a GPIOA pin. Disabling the GPIOA occurs if gpioA = 0 */
static uint16_t gpioB = 0x0000; /**< Each bit corresponds to a GPIOB pin. Disabling the GPIOB occurs if gpioB = 0 */
static uint16_t gpioC = 0x0000; /**< Each bit corresponds to a GPIOC pin. Disabling the GPIOC occurs if gpioC = 0 */
static uint16_t gpioD = 0x0000; /**< Each bit corresponds to a GPIOD pin. Disabling the GPIOD occurs if gpioD = 0 */
static uint16_t gpioE = 0x0000; /**< Each bit corresponds to a GPIOE pin. Disabling the GPIOE occurs if gpioE = 0 */
static uint16_t gpioF = 0x0000; /**< Each bit corresponds to a GPIOF pin. Disabling the GPIOF occurs if gpioF = 0 */
static uint16_t gpioG = 0x0000; /**< Each bit corresponds to a GPIOG pin. Disabling the GPIOG occurs if gpioG = 0 */
static uint16_t gpioH = 0x0000; /**< Each bit corresponds to a GPIOH pin. Disabling the GPIOH occurs if gpioH = 0 */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 */
void GPIO_OpenClockGate(enum GPIO_Ports_E port, uint16_t pins)
{
    switch (port)
    {
        case GPIO_PORT_A:
            gpioA |= pins;
            BITBAND_GPIOAEN_BIT = 1;
            break;
        case GPIO_PORT_B:
            gpioB |= pins;
            BITBAND_GPIOBEN_BIT = 1;
            break;
        case GPIO_PORT_C:
            gpioC |= pins;
            BITBAND_GPIOCEN_BIT = 1;
            break;
        case GPIO_PORT_D:
            gpioD |= pins;
            BITBAND_GPIODEN_BIT = 1;
            break;
        case GPIO_PORT_E:
            gpioE |= pins;
            BITBAND_GPIOEEN_BIT = 1;
            break;
        case GPIO_PORT_F:
            gpioF |= pins;
            BITBAND_GPIOFEN_BIT = 1;
            break;
        case GPIO_PORT_G:
            gpioG |= pins;
            BITBAND_GPIOGEN_BIT = 1;
            break;
        case GPIO_PORT_H:
            gpioH |= pins;
            BITBAND_GPIOHEN_BIT = 1;
            break;
        default:
            break;
    }
}

/**
 * See API interface for function documentation
 */
void GPIO_CloseClockGate(enum GPIO_Ports_E port, uint16_t pins)
{
    switch (port)
    {
        case GPIO_PORT_A:
            gpioA &= ~pins;
            if (!gpioA)
                BITBAND_GPIOAEN_BIT = 0;
            break;
        case GPIO_PORT_B:
            gpioB &= ~pins;
            if (!gpioB)
                BITBAND_GPIOBEN_BIT = 0;
            break;
        case GPIO_PORT_C:
            gpioC &= ~pins;
            if (!gpioC)
                BITBAND_GPIOCEN_BIT = 0;
            break;
        case GPIO_PORT_D:
            gpioD &= ~pins;
            if (!gpioD)
                BITBAND_GPIODEN_BIT = 0;
            break;
        case GPIO_PORT_E:
            gpioE &= ~pins;
            if (!gpioE)
                BITBAND_GPIOEEN_BIT = 0;
            break;
        case GPIO_PORT_F:
            gpioF &= ~pins;
            if (!gpioF)
                BITBAND_GPIOFEN_BIT = 0;
            break;
        case GPIO_PORT_G:
            gpioG &= ~pins;
            if (!gpioG)
                BITBAND_GPIOGEN_BIT = 0;
            break;
        case GPIO_PORT_H:
            gpioH &= ~pins;
            if (!gpioH)
                BITBAND_GPIOHEN_BIT = 0;
            break;
        default:
            break;
    }
}

/**
 * See API interface for function documentation
 */
bool GPIO_GetClockGate(enum GPIO_Ports_E port)
{
    bool result;
    switch (port)
    {
        case GPIO_PORT_A:
            result = (bool) gpioA;
            break;
        case GPIO_PORT_B:
            result = (bool) gpioB;
            break;
        case GPIO_PORT_C:
            result = (bool) gpioC;
            break;
        case GPIO_PORT_D:
            result = (bool) gpioD;
            break;
        case GPIO_PORT_E:
            result = (bool) gpioE;
            break;
        case GPIO_PORT_F:
			result = (bool) gpioF;
			break;
        case GPIO_PORT_G:
			result = (bool) gpioG;
			break;
        case GPIO_PORT_H:
			result = (bool) gpioH;
			break;

        default:
            result = false;
    }
    return result;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
