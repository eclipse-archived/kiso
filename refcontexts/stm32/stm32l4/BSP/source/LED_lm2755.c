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

/**
 * @ingroup BSP_LED_LM2755
 * @{
 * @file
 * @brief  Implementation of Charge Pump LED Controller Module LM2755 BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_LED_LM2755

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_LED_LM2755.h"

#if BCDS_FEATURE_BSP_LED_LM2755

/** @todo 2017-02-20, BCDS/ENG1: Clarify power-on start-up time with TI. Response from TI is pending. */
#include "BSP_HwConfig.h"
#include "LED_LM2755.h"
#include "BoardSettings.h"
#include "LDO_tlv70233.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_I2C_Handle.h"
#include "BSP_ModuleState.h"

/*
 * Structure representing  peripherals using I2C2 bus. It allows to know
 * if it is used or not and which peripheral is using it.
 */
struct I2C2Register_S
{
    uint32_t PeripheralLM2755 :1;
};

/*
 * Union used to control the I2C1 bus. It allows to know if a
 * peripheral is using it.
 */
union I2C2Register_U
{
    struct I2C2Register_S Peripheral;
    uint32_t Register;
};

typedef union I2C2Register_U I2C2Register_T;

static struct MCU_I2C_S BSP_I2C2Handle;
static I2C2Register_T BSP_I2C2;
static volatile enum BSP_ModuleState_E ModuleState_LED_lm2755 = BSP_MODULE_STATE_UNINITIALIZED;
static volatile uint8_t EnableCount;

enum BSP_ModuleState_E LED_LM2755_getState(void)
{
    return ModuleState_LED_lm2755;
}

/*
 * @brief Initialize the I2C2 resource used by the sensors
 * @details This function Initializes the I2C2 resource. It is called within the
 * Board_Initialize() function. It stores the desired configuration into the
 * resource registers and puts it into low power consumption configuration until
 * it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_I2C2_Init(void);

Retcode_T Board_I2C2_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* Enable the clock */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* Configure the I2C */
    BSP_I2C2Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    BSP_I2C2Handle.hi2c.Instance = I2C2;
    BSP_I2C2Handle.hi2c.Init.Timing = I2C2_TIMING;
    BSP_I2C2Handle.hi2c.Init.OwnAddress1 = 0;
    BSP_I2C2Handle.hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    BSP_I2C2Handle.hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    BSP_I2C2Handle.hi2c.Init.OwnAddress2 = 0;
    BSP_I2C2Handle.hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    BSP_I2C2Handle.hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    BSP_I2C2Handle.hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    if (HAL_I2C_Init(&(BSP_I2C2Handle.hi2c)))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    /* Configure Analog filter */
    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_I2CEx_AnalogFilter_Config(&(BSP_I2C2Handle.hi2c), I2C2_DIGITAL_FILTER_ENABLE))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    /* Disable the clock and the I2C */
    __HAL_I2C_DISABLE(&(BSP_I2C2Handle.hi2c));
    __HAL_RCC_I2C2_CLK_DISABLE();
    if (RETCODE_OK == retcode)
    {
        ModuleState_LED_lm2755 = BSP_MODULE_STATE_DISCONNECTED;
    }

    return retcode;
}

Retcode_T BSP_LED_LM2755_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;

    switch(ModuleState_LED_lm2755)
    {
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH: */
    case BSP_MODULE_STATE_DISCONNECTED:
        /* Enabling GPIO clocks required */
        LED_LM2755_GPIO_CLOCK_ENABLE();

        BSP_I2C2.Register = 0;

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = LED_CHRGPUMP_EN_PIN;
        GPIO_InitStruct.Mode = LED_CHRGPUMP_EN_MODE;
        GPIO_InitStruct.Pull = LED_CHRGPUMP_EN_PULL;
        GPIO_InitStruct.Speed = LED_CHRGPUMP_EN_SPEED;
        HAL_GPIO_Init(LED_CHRGPUMP_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(LED_CHRGPUMP_EN_PORT, LED_CHRGPUMP_EN_PIN, BSP_IO_PIN_LOW);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = I2C2_CLK_PIN;
        GPIO_InitStruct.Mode = I2C2_CLK_MODE;
        GPIO_InitStruct.Pull = I2C2_CLK_PULL;
        GPIO_InitStruct.Speed = I2C2_CLK_SPEED;
        GPIO_InitStruct.Alternate = I2C2_CLK_ALTERNATE;
        HAL_GPIO_Init(I2C2_CLK_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = I2C2_DATA_PIN;
        GPIO_InitStruct.Mode = I2C2_DATA_MODE;
        GPIO_InitStruct.Pull = I2C2_DATA_PULL;
        GPIO_InitStruct.Speed = I2C2_DATA_SPEED;
        GPIO_InitStruct.Alternate = I2C2_DATA_ALTERNATE;
        HAL_GPIO_Init(I2C2_DATA_PORT, &GPIO_InitStruct);
        EnableCount = 0;
        ModuleState_LED_lm2755 = BSP_MODULE_STATE_CONNECTED;

        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_UNINITIALIZED:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
        break;
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_LED_LM2755_Enable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_LED_lm2755)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enable Power supply */
        retcode = BSP_LDO_TLV70233_Enable();
        if(RETCODE_OK == retcode)
        {
            /* check if the I2c bus is already configured */
            if (!BSP_I2C2.Peripheral.PeripheralLM2755)
            {
                BSP_I2C2.Peripheral.PeripheralLM2755 = TRUE;

                __HAL_RCC_I2C2_CLK_ENABLE();
                __HAL_I2C_ENABLE(&(BSP_I2C2Handle.hi2c));

                assert(BSP_I2C2Handle.IRQCallback);
                HAL_NVIC_SetPriority(I2C2_EV_IRQn, LED_LM2755_I2C_EVENT_PRIO, 0);
                HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);

                assert(BSP_I2C2Handle.ERRCallback);
                HAL_NVIC_SetPriority(I2C2_ER_IRQn, LED_LM2755_I2C_ERROR_PRIO, 0);
                HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
            }
            HAL_GPIO_WritePin(LED_CHRGPUMP_EN_PORT, LED_CHRGPUMP_EN_PIN, BSP_IO_PIN_HIGH);
            HAL_Delay(HW_LM2755_POWER_ON_TIME_DELAY);

            ModuleState_LED_lm2755 = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_LED_lm2755 = BSP_MODULE_STATE_ERROR;
        }
        break;
    case BSP_MODULE_STATE_ENABLED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    if(RETCODE_OK == retcode)
    {
        EnableCount++;
        assert(EnableCount);
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_LED_LM2755_Disable(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_LED_lm2755)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            HAL_GPIO_WritePin(LED_CHRGPUMP_EN_PORT, LED_CHRGPUMP_EN_PIN, BSP_IO_PIN_LOW);

            if (BSP_I2C2.Peripheral.PeripheralLM2755)
            {
                BSP_I2C2.Peripheral.PeripheralLM2755 = FALSE;
                HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
                HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
                __HAL_I2C_DISABLE(&(BSP_I2C2Handle.hi2c));
                __HAL_RCC_I2C2_CLK_DISABLE();
            }
            if(RETCODE_OK == retcode)
            {
                ModuleState_LED_lm2755 = BSP_MODULE_STATE_CONNECTED;
                /* Disable power supply */
                retcode = BSP_LDO_TLV70233_Disable();
            }
            if(RETCODE_OK != retcode)
            {
                ModuleState_LED_lm2755 = BSP_MODULE_STATE_ERROR;
            }
        }
        break;
    case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_LED_LM2755_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_LED_lm2755)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /*
         * Enable all GPIO ports necessary for the operation to make sure all the
         * commands have been executed
         */
        SENSOR_NODE_GPIO_CLOCK_ENABLE();

        BSP_I2C2.Register = 0;


        HAL_GPIO_DeInit(I2C2_CLK_PORT, I2C2_CLK_PIN);
        HAL_GPIO_DeInit(I2C2_DATA_PORT, I2C2_DATA_PIN);
        HAL_GPIO_DeInit(LED_CHRGPUMP_EN_PORT, LED_CHRGPUMP_EN_PIN);

        ModuleState_LED_lm2755 = BSP_MODULE_STATE_DISCONNECTED;
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED: /* FALL-THROUGH */
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

HWHandle_T BSP_LED_LM2755_GetI2CHandle(void)
{
    return (HWHandle_T) &BSP_I2C2Handle;
}

void LED_LM2755_I2CEvISR(void)
{
    BSP_I2C2Handle.IRQCallback((I2C_T) &BSP_I2C2Handle);
}


void LED_LM2755_I2CErISR(void)
{
    BSP_I2C2Handle.ERRCallback((I2C_T) &BSP_I2C2Handle);
}

#endif /* BCDS_FEATURE_BSP_LED_LM2755 */
/**@}*//* @ingroup BSP_LED_LM2755 */
