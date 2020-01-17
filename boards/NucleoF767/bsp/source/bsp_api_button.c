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

#include "Kiso_BSP_Button.h"

#if KISO_FEATURE_BSP_BUTTON

#include "BSP_NucleoF767.h"
#include "protected/gpio.h"
#include "Kiso_BSP_LED.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID MODULE_BSP_BUTTON

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

void EXTI15_10_IRQHandler(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static BSP_Button_Callback_T ButtonCallback = null;
static uint8_t bspState = (uint8_t)BSP_STATE_INIT; /**< BSP state of the button */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/*
 * Refer to interface header for description
 */
Retcode_T BSP_Button_Connect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }

    if (RETCODE_OK == retcode)
    {
        GPIO_InitTypeDef GPIO_Init;

        GPIO_Init.Pin = PINC_USR_BUTTON;
        GPIO_Init.Mode = GPIO_MODE_INPUT;
        GPIO_Init.Pull = GPIO_NOPULL;
        GPIO_Init.Speed = GPIO_SPEED_FAST;

        GPIO_OpenClockGate(GPIO_PORT_C, PINC_USR_BUTTON);
        HAL_GPIO_Init(GPIOC, &GPIO_Init);

        bspState = (uint8_t)BSP_STATE_CONNECTED;
    }

    return retcode;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_Button_Enable(uint32_t buttonId, BSP_Button_Callback_T callback)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }

    if (RETCODE_OK == retcode)
    {
        switch (buttonId)
        {
        case NUCLEOF767_BUTTON_USER:
            if (NULL == callback)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            }
            else
            {
                GPIO_InitTypeDef GPIO_Init;

                ButtonCallback = callback;

                GPIO_Init.Pin = PINC_USR_BUTTON;
                GPIO_Init.Pull = GPIO_NOPULL;
                GPIO_Init.Speed = GPIO_SPEED_FAST;
                GPIO_Init.Mode = GPIO_MODE_IT_RISING_FALLING;
                HAL_GPIO_Init(GPIOC, &GPIO_Init);

                /* Enable and set Button EXTI Interrupt to the lowest priority */
                HAL_NVIC_SetPriority((IRQn_Type)(EXTI15_10_IRQn), 0x0F, 0x00);
                HAL_NVIC_EnableIRQ((IRQn_Type)(EXTI15_10_IRQn));

                bspState = (uint8_t)BSP_STATE_ENABLED;
            }

            break;

        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
        }
    }

    return retcode;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_Button_Disable(uint32_t buttonId)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }

    if (RETCODE_OK == retcode)
    {
        switch (buttonId)
        {
        case NUCLEOF767_BUTTON_USER:
        {
            GPIO_InitTypeDef GPIO_Init;

            HAL_NVIC_DisableIRQ((IRQn_Type)(EXTI15_10_IRQn));

            // The way stm32cube is working:
            // First DeInit the pin
            HAL_GPIO_DeInit(GPIOC, PINC_USR_BUTTON);

            // Next, re-init it in normal input mode
            GPIO_Init.Pin = PINC_USR_BUTTON;
            GPIO_Init.Mode = GPIO_MODE_INPUT;
            GPIO_Init.Pull = GPIO_NOPULL;
            GPIO_Init.Speed = GPIO_SPEED_FAST;

            HAL_GPIO_Init(GPIOC, &GPIO_Init);

            ButtonCallback = null;
            bspState = (uint8_t)BSP_STATE_DISABLED;

            break;
        }

        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
        }
    }

    return retcode;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_Button_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t)BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }

    if (RETCODE_OK == retcode)
    {
        HAL_GPIO_DeInit(GPIOC, PINC_USR_BUTTON);
        GPIO_CloseClockGate(GPIO_PORT_C, PINC_USR_BUTTON);
        bspState = (uint8_t)BSP_STATE_DISCONNECTED;
    }

    return retcode;
}

/*
 * Refer to interface header for description
 */
uint32_t BSP_Button_GetState(uint32_t buttonId)
{
    enum BSP_ButtonStatus_E status = BSP_BUTTON_ERROR;

    if (((uint8_t)BSP_STATE_CONNECTED == bspState) || ((uint8_t)BSP_STATE_ENABLED == bspState))
    {
        switch (buttonId)
        {
        case NUCLEOF767_BUTTON_USER:
            if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOC, PINC_USR_BUTTON))
            {
                status = BSP_BUTTON_RELEASED;
            }
            else
            {
                status = BSP_BUTTON_PRESSED;
            }

            break;

        default:
            break;
        }
    }

    return (uint32_t)status;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

void EXTI15_10_IRQHandler(void)
{
    // Get which line triggered this interrupt
    uint16_t line = (EXTI->PR & GPIO_PIN_All);
    HAL_GPIO_EXTI_IRQHandler(line);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == PINC_USR_BUTTON)
    {
        if (((uint8_t)BSP_STATE_ENABLED == bspState) && (null != ButtonCallback))
        {
            if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOC, PINC_USR_BUTTON))
            {
                ButtonCallback((uint32_t)BSP_BUTTON_RELEASED);
            }
            else
            {
                ButtonCallback((uint32_t)BSP_BUTTON_PRESSED);
            }
        }
        else
        {
            assert(0);
        }
    }
}
#endif
