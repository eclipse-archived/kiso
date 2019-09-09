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

#include "protected/board.h"
#include "protected/gpio.h"
#include "protected/power_supply.h"
#include "BSP_CommonGateway.h"
#include "Kiso_BSP_Board.h"
#include "Kiso_HAL_Delay.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#undef KISO_MODULE_ID
#define KISO_MODULE_ID  MODULE_BSP_POWER_SUPPLY

#define POWER_SUPPLY_3V9GSM_ON_DELAY_MS     UINT32_C(100)
#define POWER_SUPPLY_2V8GPS_ON_DELAY_MS     UINT32_C(10)
#define POWER_SUPPLY_2V8BLE_ON_DELAY_MS     UINT32_C(10)
#define POWER_SUPPLY_2V8SENS_ON_DELAY_MS    UINT32_C(10)
#define POWER_SUPPLY_2V8MEM_ON_DELAY_MS     UINT32_C(10)
#define POWER_SUPPLY_5VCAN_ON_DELAY_MS      UINT32_C(10)
#define POWER_SUPPLY_5VA_ON_DELAY_MS        UINT32_C(10)
#define POWER_SUPPLY_5VB_ON_DELAY_MS        UINT32_C(10)

#define POWER_SUPPLY_3V9GSM_OFF_DELAY_MS     UINT32_C(100)
#define POWER_SUPPLY_2V8GPS_OFF_DELAY_MS     UINT32_C(100)
#define POWER_SUPPLY_2V8BLE_OFF_DELAY_MS     UINT32_C(100)
#define POWER_SUPPLY_2V8SENS_OFF_DELAY_MS    UINT32_C(100)
#define POWER_SUPPLY_2V8MEM_OFF_DELAY_MS     UINT32_C(10)
#define POWER_SUPPLY_5VCAN_OFF_DELAY_MS      UINT32_C(10)
#define POWER_SUPPLY_5VA_OFF_DELAY_MS        UINT32_C(10)
#define POWER_SUPPLY_5VB_OFF_DELAY_MS        UINT32_C(10)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

static uint8_t powerSupply5VCAN = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply3V9GSM = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply2V8GPS = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply2V8BLE = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply2V8Sensor = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply2V8Memory = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply5VA = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */
static uint8_t powerSupply5VB = 0x00; /**< Each bit corresponds to a peripheral supplied from the power supply */

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

Retcode_T PowerSupply_EnablePower3V9GSM(void)
{
    Retcode_T retcode = RETCODE_OK;
    /* Power Supply relies on board to configure the power GPIOs */
    if (!Board_IsInitDone())
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_A, PINA_EN_POW_GSM);
        /* Set power enable pin*/
        HAL_GPIO_WritePin(GPIOA, PINA_EN_POW_GSM, GPIO_PIN_SET);
        /* Wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_3V9GSM_ON_DELAY_MS);
        powerSupply3V9GSM = 1;
    }
    return retcode;
}

Retcode_T PowerSupply_EnablePower2V8GPS(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* Power Supply relies on BSP_Board_Initialize to configure the power GPIOs */
    if (!Board_IsInitDone())
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_A, PINA_EN_POW_GPS);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOA, PINA_EN_POW_GPS, GPIO_PIN_RESET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8GPS_ON_DELAY_MS);
        powerSupply2V8GPS = 1;
    }
    return retcode;
}

Retcode_T PowerSupply_EnablePower2V8BLE(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* PowerSupply relies on BSP_Board_Initialize to configure the power GPIOs*/
    if (!Board_IsInitDone())
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_B, PINB_EN_POW_BLE);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOB, PINB_EN_POW_BLE, GPIO_PIN_RESET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8BLE_ON_DELAY_MS);
        powerSupply2V8BLE = 1;
    }
    return retcode;
}

Retcode_T PowerSupply_EnablePower2V8Sensor(enum PowerSupply_Sensors_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_SENSOR_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        /* PowerSupply relies on BSP_Board_Initialize to configure the power GPIOs*/
        if (!Board_IsInitDone())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_A, PINA_EN_POW_SENS);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOA, PINA_EN_POW_SENS, GPIO_PIN_RESET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8SENS_ON_DELAY_MS);
        /* check power good pin and return error if not set*/
        powerSupply2V8Sensor |= 1 << peripheral;
    }
    return retcode;
}

Retcode_T PowerSupply_EnablePower2V8Memory(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* PowerSupply relies on BSP_Board_Initialize to configure the power GPIOs*/
    if (!Board_IsInitDone())
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_B, PINB_EN_POW_MEM);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOB, PINB_EN_POW_MEM, GPIO_PIN_RESET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8MEM_ON_DELAY_MS);
        powerSupply2V8Memory = 1;
    }
    return retcode;
}

/**
 * Enables power supply 5V for can and others
 * @param peripheral
 * @return
 */
Retcode_T PowerSupply_EnablePower5VCAN(enum PowerSupply_CAN_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_CAN_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        /* PowerSupply relies on BSP_Board_Initialize to configure the power GPIOs*/
        if (!Board_IsInitDone())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_E, PINE_EN_POW_CAN);
        GPIO_OpenClockGate(GPIO_PORT_D, PIND_PG_5V0);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOE, PINE_EN_POW_CAN, GPIO_PIN_SET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_5VCAN_ON_DELAY_MS);
        /* check power good pin and return error if not set*/
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOD, PIND_PG_5V0))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT);
        }
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply5VCAN |= 1 << peripheral;
    }
    return retcode;
}

Retcode_T PowerSupply_EnablePower5VA(enum PowerSupply_5VA_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_5VA_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        /* PowerSupply relies on BSP_Board_Initialize to configure the power GPIOs*/
        if (!Board_IsInitDone())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_EnablePower5VCAN(POWER_SUPPLY_5VA);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_C, PINC_EN_POW_5V_A);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOC, PINC_EN_POW_5V_A, GPIO_PIN_RESET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_5VA_ON_DELAY_MS);
        /* check power good pin and return error if not set*/
        powerSupply5VA |= 1 << peripheral;
    }
    return retcode;
}

Retcode_T PowerSupply_EnablePower5VB(enum PowerSupply_5VB_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_5VB_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        /* PowerSupply relies on BSP_Board_Initialize to configure the power GPIOs*/
        if (!Board_IsInitDone())
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_EnablePower5VCAN(POWER_SUPPLY_5VB);
    }
    if (RETCODE_OK == retcode)
    {
        GPIO_OpenClockGate(GPIO_PORT_C, PINC_EN_POW_5V_B);
        /*set power enable pin*/
        HAL_GPIO_WritePin(GPIOC, PINC_EN_POW_5V_B, GPIO_PIN_RESET);
        /* wait for setup time */
        HAL_Delay_WaitMs(POWER_SUPPLY_5VB_ON_DELAY_MS);
        /* check power good pin and return error if not set*/
        powerSupply5VB |= 1 << peripheral;
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower3V9GSM(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!powerSupply3V9GSM)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply3V9GSM = 0;
        /* Set power enable pin*/
        HAL_GPIO_WritePin(GPIOA, PINA_EN_POW_GSM, GPIO_PIN_RESET);
        /* Capacitor discharge time */
        HAL_Delay_WaitMs(POWER_SUPPLY_3V9GSM_OFF_DELAY_MS);
        GPIO_CloseClockGate(GPIO_PORT_A, PINA_EN_POW_GSM);
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower2V8GPS(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!powerSupply2V8GPS)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply2V8GPS = 0;
        /* Set power enable pin*/
        HAL_GPIO_WritePin(GPIOA, PINA_EN_POW_GPS, GPIO_PIN_SET);
        /* Capacitor discharge time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8GPS_OFF_DELAY_MS);
        GPIO_CloseClockGate(GPIO_PORT_A, PINA_EN_POW_GPS);
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower2V8BLE(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!powerSupply2V8BLE)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply2V8BLE = 0;
        /* Set power enable pin*/
        HAL_GPIO_WritePin(GPIOB, PINB_EN_POW_BLE, GPIO_PIN_SET);
        /* Capacitor discharge time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8BLE_OFF_DELAY_MS);
        GPIO_CloseClockGate(GPIO_PORT_B, PINB_EN_POW_BLE);

    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower2V8Sensor(enum PowerSupply_Sensors_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_SENSOR_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (!powerSupply2V8Sensor)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply2V8Sensor &= ~(1 << peripheral);
        if (0 == powerSupply2V8Sensor)
        {
            /*set power enable pin*/
            HAL_GPIO_WritePin(GPIOA, PINA_EN_POW_SENS, GPIO_PIN_SET);
            /* capacitor discharge time */
            HAL_Delay_WaitMs(POWER_SUPPLY_2V8SENS_OFF_DELAY_MS);
            GPIO_CloseClockGate(GPIO_PORT_A, PINA_EN_POW_SENS);
        }
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower2V8Memory(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (!powerSupply2V8Memory)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply2V8Memory = 0;
        /* Set power enable pin*/
        HAL_GPIO_WritePin(GPIOB, PINB_EN_POW_MEM, GPIO_PIN_SET);
        /* Capacitor discharge time */
        HAL_Delay_WaitMs(POWER_SUPPLY_2V8MEM_OFF_DELAY_MS);
        GPIO_CloseClockGate(GPIO_PORT_B, PINB_EN_POW_MEM);
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower5VCAN(enum PowerSupply_CAN_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_CAN_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (!powerSupply5VCAN)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply5VCAN &= ~(1 << peripheral);
        if (0 == powerSupply5VCAN)
        {
            /* Reset power enable pin */
            HAL_GPIO_WritePin(GPIOE, PINE_EN_POW_CAN, GPIO_PIN_RESET);
            /* Capacitor discharge time */
            HAL_Delay_WaitMs(POWER_SUPPLY_5VCAN_OFF_DELAY_MS);
            GPIO_CloseClockGate(GPIO_PORT_E, PINE_EN_POW_CAN);

        }
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower5VA(enum PowerSupply_5VA_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_5VA_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (!powerSupply5VA)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply5VA &= ~(1 << peripheral);
        if (0 == powerSupply5VA)
        {
            /* Reset power enable pin */
            HAL_GPIO_WritePin(GPIOC, PINC_EN_POW_5V_A, GPIO_PIN_SET);
            /* Capacitor discharge time */
            HAL_Delay_WaitMs(POWER_SUPPLY_5VA_OFF_DELAY_MS);
            GPIO_CloseClockGate(GPIO_PORT_C, PINC_EN_POW_5V_A);

        }
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_DisablePower5VCAN(POWER_SUPPLY_5VA);
    }
    return retcode;
}

Retcode_T PowerSupply_DisablePower5VB(enum PowerSupply_5VB_E peripheral)
{
    Retcode_T retcode = RETCODE_OK;

    if (POWER_SUPPLY_5VB_COUNT <= peripheral)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if (RETCODE_OK == retcode)
    {
        if (!powerSupply5VB)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
    }
    if (RETCODE_OK == retcode)
    {
        powerSupply5VB &= ~(1 << peripheral);
        if (0 == powerSupply5VB)
        {
            /* Reset power enable pin */
            HAL_GPIO_WritePin(GPIOC, PINC_EN_POW_5V_B, GPIO_PIN_SET);
            /* Capacitor discharge time */
            HAL_Delay_WaitMs(POWER_SUPPLY_5VB_OFF_DELAY_MS);
            GPIO_CloseClockGate(GPIO_PORT_C, PINC_EN_POW_5V_B);
        }
    }
    if (RETCODE_OK == retcode)
    {
        retcode = PowerSupply_DisablePower5VCAN(POWER_SUPPLY_5VB);
    }
    return retcode;
}

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/
