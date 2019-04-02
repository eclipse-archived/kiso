/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

#include "BCDS_BSP_LED.h"

#if BCDS_FEATURE_BSP_LED

#include "BSP_Sensgate.h"
#include "protected/gpio.h"
#include "protected/power_supply.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID MODULE_BSP_API_LED

#define PIN_LED_ALL     (PIND_LED_R | PIND_LED_G | PIND_LED_B)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static Retcode_T LED_Off(uint32_t id);
static Retcode_T LED_On(uint32_t id);
static Retcode_T LED_Toggle(uint32_t id);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint8_t bspState = (uint8_t) BSP_STATE_INIT; /**< BSP state of the LEDs */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow connecting.
 */
Retcode_T BSP_LED_Connect(void)
{
    GPIO_InitTypeDef GPIO_Init;
    Retcode_T retcode = RETCODE_OK;
    if (!(bspState & (uint8_t) BSP_STATE_TO_CONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_D, PIN_LED_ALL);
        GPIO_Init.Pin = PIN_LED_ALL;
        GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_Init.Pull = GPIO_PULLDOWN;
        GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOD, &GPIO_Init);

        bspState = (uint8_t) BSP_STATE_CONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow enabling.
 */
Retcode_T BSP_LED_Enable(uint32_t id)
{
    BCDS_UNUSED(id);
    Retcode_T retcode;
    if (!(bspState & (uint8_t) BSP_STATE_TO_ENABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_EnablePower5VCAN(POWER_SUPPLY_LED);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_ENABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 */
Retcode_T BSP_LED_Disable(uint32_t id)
{
    BCDS_UNUSED(id);
    Retcode_T retcode;
    if (!(bspState & (uint8_t) BSP_STATE_TO_DISABLED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = LED_Off(SENSGATE_LED_ALL);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_DisablePower5VCAN(POWER_SUPPLY_LED);
    }
    if (RETCODE_OK == retcode)
    {
        bspState = (uint8_t) BSP_STATE_DISABLED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disconnecting.
 */
Retcode_T BSP_LED_Disconnect(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!(bspState & (uint8_t) BSP_STATE_TO_DISCONNECTED))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        uint32_t pins = PIN_LED_ALL;
        HAL_GPIO_DeInit(GPIOD, pins);
        GPIO_CloseClockGate(GPIO_PORT_D, PIN_LED_ALL);
        bspState = (uint8_t) BSP_STATE_DISCONNECTED;
    }
    return retcode;
}

/**
 * See API interface for function documentation
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INCONSISTENT_STATE in case the module is not in a state to allow disabling.
 * @retval RETCODE_INVALID_PARAM in case the passed id or command are not valid.
 */
Retcode_T BSP_LED_Switch(uint32_t id, uint32_t command)
{
    Retcode_T retcode = RETCODE_OK;
    if ((uint8_t) BSP_STATE_ENABLED == bspState)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        switch (command)
        {
            case SENSGATE_LED_COMMAND_OFF:
                retcode = LED_Off(id);
                break;
            case SENSGATE_LED_COMMAND_ON:
                retcode = LED_On(id);
                break;
            case SENSGATE_LED_COMMAND_TOGGLE:
                retcode = LED_Toggle(id);
                break;
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                break;
        }
    }
    return retcode;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/**
 * Switches off an LED
 * @param id
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INVALID_PARAM in case the passed id is not valid.
 */
static Retcode_T LED_Off(uint32_t id)
{
    Retcode_T retcode = RETCODE_OK;
    switch (id)
    {
        case SENSGATE_LED_ALL:
            HAL_GPIO_WritePin(GPIOD, PIN_LED_ALL, GPIO_PIN_RESET);
            break;
        case SENSGATE_LED_RED_ID:
            HAL_GPIO_WritePin(GPIOD, PIND_LED_B, GPIO_PIN_RESET);
            break;
        case SENSGATE_LED_GREEN_ID:
            HAL_GPIO_WritePin(GPIOD, PIND_LED_B, GPIO_PIN_RESET);
            break;
        case SENSGATE_LED_BLUE_ID:
            HAL_GPIO_WritePin(GPIOD, PIND_LED_B, GPIO_PIN_RESET);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
    }
    return retcode;
}

/**
 * Switches on an LED
 * @param id
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INVALID_PARAM in case the passed id is not valid.
 */
static Retcode_T LED_On(uint32_t id)
{
    Retcode_T retcode = RETCODE_OK;
    switch (id)
    {
        case SENSGATE_LED_ALL:
            HAL_GPIO_WritePin(GPIOD, PIN_LED_ALL, GPIO_PIN_SET);
            break;
        case SENSGATE_LED_RED_ID:
            HAL_GPIO_WritePin(GPIOD, PIND_LED_B, GPIO_PIN_SET);
            break;
        case SENSGATE_LED_GREEN_ID:
            HAL_GPIO_WritePin(GPIOD, PIND_LED_B, GPIO_PIN_SET);
            break;
        case SENSGATE_LED_BLUE_ID:
            HAL_GPIO_WritePin(GPIOD, PIND_LED_B, GPIO_PIN_SET);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
    }
    return retcode;
}

/**
 * Toggles an LED
 * @param id
 * @retval RETCODE_OK in case of success.
 * @retval RETCODE_INVALID_PARAM in case the passed id is not valid.
 */
static Retcode_T LED_Toggle(uint32_t id)
{
    Retcode_T retcode = RETCODE_OK;
    switch (id)
    {
        case SENSGATE_LED_ALL:
            HAL_GPIO_TogglePin(GPIOD, PIN_LED_ALL);
            break;
        case SENSGATE_LED_RED_ID:
            HAL_GPIO_TogglePin(GPIOD, PIND_LED_B);
            break;
        case SENSGATE_LED_GREEN_ID:
            HAL_GPIO_TogglePin(GPIOD, PIND_LED_B);
            break;
        case SENSGATE_LED_BLUE_ID:
            HAL_GPIO_TogglePin(GPIOD, PIND_LED_B);
            break;
        default:
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
            break;
    }
    return retcode;
}

#endif /* BCDS_FEATURE_BSP_LED */
