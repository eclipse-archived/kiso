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
 * @ingroup BSP_SENSOR_NODE
 * @{
 * @file
 * @brief  Implementation of I2C Sensor Node BSP functions.
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_BSP_MODULE_ID_SENSOR_NODE

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BCDS_BSP_SensorNode.h"

#if BCDS_FEATURE_BSP_SENSOR_NODE

#include "SensorNode.h"
#include "BoardSettings.h"
#include "LDO_tlv70233.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_I2C_Handle.h"
#include "BSP_ModuleState.h"

#ifndef HAVE_BMM150
#define HAVE_BMM150 0
#endif /* HAVE_BMM150 */

/*
 * Structure representing  peripherals using external interrupt for IO lines
 * 10 to 15.
 * It allows to know if it is used or not and which peripheral is using it.
 */
struct EXTILine1510_S
{
    uint32_t ACC_INT :1; /**< Interrupt line for accelerometer */
    uint32_t GYRO_INT :1; /**< Interrupt line for gyroscope */
    uint32_t BMM_DRDY :1; /**< Interrupt line for geomagnetic sensor data ready signal  */
};

/*
 * Union used to control the I2C1 bus. It allows to know if a
 * peripheral is using it.
 */
union EXTILine1510_U
{
    struct EXTILine1510_S Peripheral;
    uint32_t Register;
};

typedef union EXTILine1510_U EXTILine1510_T;

/*
 * Structure representing  peripherals using I2C1 bus. It allows to know
 * if it is used or not and which peripheral is using it.
 */
struct I2C1Register_S
{
    uint32_t SensorBMI160 :1;
    uint32_t SensorBMM150 :1;
    uint32_t SensorBMP280 :1;
};

/*
 * Union used to control the I2C1 bus. It allows to know if a
 * peripheral is using it.
 */
union I2C1Register_U
{
    struct I2C1Register_S Peripheral;
    uint32_t Register;
};

typedef union I2C1Register_U I2C1Register_T;

static EXTILine1510_T BSP_ExtiLine1510;
static I2C1Register_T BSP_I2C1;

static struct MCU_I2C_S BSP_I2C1Handle;

static BSP_Sensor_InterruptCallback_T SensorNode_BMI160ISRCallback;
static BSP_Sensor_InterruptCallback_T SensorNode_BMM150ISRCallback;

static volatile enum BSP_ModuleState_E ModuleState_BMI160       = BSP_MODULE_STATE_UNINITIALIZED;
static volatile enum BSP_ModuleState_E ModuleState_BMM150       = BSP_MODULE_STATE_UNINITIALIZED;
static volatile enum BSP_ModuleState_E ModuleState_BMP280       = BSP_MODULE_STATE_UNINITIALIZED;
static volatile enum BSP_ModuleState_E ModuleState_SensorNode  = BSP_MODULE_STATE_UNINITIALIZED;

/**
 * @brief Enable the I2C bus
 * @details This function enables the bus and registers the sensor in the
 * bus shared register
 * @param Sensor the sensor requesting  to be enabled
 */
static void SensorNode_EnableBus(void);
/**
 * @brief Disables the I2C bus
 * @details This function disables the bus if it is not in use by any sensor
 * @param Sensor the sensor requesting  to be enabled
 */
static void SensorNode_DisableBus(void);

enum BSP_ModuleState_E SensorNode_BMI160_getState(void)
{
    return ModuleState_BMI160;
}

enum BSP_ModuleState_E SensorNode_BMM150_getState(void)
{
    return ModuleState_BMM150;
}

enum BSP_ModuleState_E SensorNode_BMP280_getState(void)
{
    return ModuleState_BMP280;
}

/*
 * @brief Initialize the I2C1 resource used by the sensors
 * @details This function Initializes the I2C1 resource. It is called within the
 * Board_Initialize() function. It stores the desired configuration into the
 * resource registers and puts it into low power consumption configuration until
 * it is requested by the application.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_I2C1_Init(void);

Retcode_T Board_I2C1_Init(void)
{
    Retcode_T retcode = RETCODE_OK;

    /* Enable the clock */
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* Configure the I2C */
    BSP_I2C1Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    BSP_I2C1Handle.hi2c.Instance = I2C1;
    BSP_I2C1Handle.hi2c.Init.Timing = I2C1_TIMING;
    BSP_I2C1Handle.hi2c.Init.OwnAddress1 = 0;
    BSP_I2C1Handle.hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    BSP_I2C1Handle.hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
    BSP_I2C1Handle.hi2c.Init.OwnAddress2 = 0;
    BSP_I2C1Handle.hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    BSP_I2C1Handle.hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
    BSP_I2C1Handle.hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
    if (HAL_I2C_Init(&(BSP_I2C1Handle.hi2c)))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }
    /* Configure Analog filter */
    if (RETCODE_OK == retcode)
    {
        if (HAL_OK != HAL_I2CEx_AnalogFilter_Config(&(BSP_I2C1Handle.hi2c), I2C1_DIGITAL_FILTER_ENABLE))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }
    /* Disable the clock and the I2C */
    __HAL_I2C_DISABLE(&(BSP_I2C1Handle.hi2c));
    __HAL_RCC_I2C1_CLK_DISABLE();
    if (RETCODE_OK == retcode)
    {
        ModuleState_SensorNode  = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_BMI160      = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_BMM150      = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_BMP280      = BSP_MODULE_STATE_DISCONNECTED;
    }

    return retcode;
}

Retcode_T BSP_SensorNode_Connect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;
    GPIO_InitTypeDef GPIO_InitStruct;

    switch(ModuleState_SensorNode)
    {
    case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH: */
    case BSP_MODULE_STATE_DISCONNECTED:
        BSP_I2C1.Register = 0;

        /* Enabling GPIO clocks required by the Sensor Node module */
        SENSOR_NODE_GPIO_CLOCK_ENABLE();

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BMI160_EN_PIN;
        GPIO_InitStruct.Mode = BMI160_EN_MODE;
        GPIO_InitStruct.Pull = BMI160_EN_PULL;
        GPIO_InitStruct.Speed = BMI160_EN_SPEED;
        HAL_GPIO_Init(BMI160_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(BMI160_EN_PORT, BMI160_EN_PIN, BSP_IO_PIN_HIGH);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = I2C_CLK_PIN;
        GPIO_InitStruct.Mode = I2C_CLK_MODE;
        GPIO_InitStruct.Pull = I2C_CLK_PULL;
        GPIO_InitStruct.Speed = I2C_CLK_SPEED;
        GPIO_InitStruct.Alternate = I2C_CLK_ALTERNATE;
        HAL_GPIO_Init(I2C_CLK_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = I2C_DATA_PIN;
        GPIO_InitStruct.Mode = I2C_DATA_MODE;
        GPIO_InitStruct.Pull = I2C_DATA_PULL;
        GPIO_InitStruct.Speed = I2C_DATA_SPEED;
        GPIO_InitStruct.Alternate = I2C_DATA_ALTERNATE;
        HAL_GPIO_Init(I2C_DATA_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BMI160_INT1_PIN;
        GPIO_InitStruct.Mode = BMI160_INT1_MODE;
        GPIO_InitStruct.Pull = BMI160_INT1_PULL;
        GPIO_InitStruct.Speed = BMI160_INT1_SPEED;
        HAL_GPIO_Init(BMI160_INT1_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BMM_INT_PIN;
        GPIO_InitStruct.Mode = BMM_INT_MODE;
        GPIO_InitStruct.Pull = BMM_INT_PULL;
        GPIO_InitStruct.Speed = BMM_INT_SPEED;
        HAL_GPIO_Init(BMM_INT_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BMI160_INT2_PIN;
        GPIO_InitStruct.Mode = BMI160_INT2_MODE;
        GPIO_InitStruct.Pull = BMI160_INT2_PULL;
        GPIO_InitStruct.Speed = BMI160_INT2_SPEED;
        HAL_GPIO_Init(BMI160_INT2_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = BMM_DRDY_PIN;
        GPIO_InitStruct.Mode = BMM_DRDY_MODE;
        GPIO_InitStruct.Pull = BMM_DRDY_PULL;
        GPIO_InitStruct.Speed = BMM_DRDY_SPEED;
        HAL_GPIO_Init(BMM_DRDY_PORT, &GPIO_InitStruct);

        ModuleState_SensorNode  = BSP_MODULE_STATE_CONNECTED;
        ModuleState_BMI160      = BSP_MODULE_STATE_CONNECTED;
        ModuleState_BMM150      = BSP_MODULE_STATE_CONNECTED;
        ModuleState_BMP280      = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH: */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_SensorNode_Enable(uint32_t id, BSP_Sensor_InterruptCallback_T callback)
{
    BSP_MODULE_TRY_LOCK;

    Retcode_T retcode = RETCODE_OK;

    if (false == IS_SENSOR((BSP_Sensor_T )id))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if(RETCODE_OK == retcode)
    {
        switch (id)
        {
        case BSP_SENSOR_BMI160:
            switch(ModuleState_BMI160)
            {
            case BSP_MODULE_STATE_CONNECTED:
                /* Do not enable if callback is not registered */
                if(NULL == callback)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }

                if(RETCODE_OK == retcode)
                {
                    /* Enable I2C if not enabled */
                    SensorNode_EnableBus();
                    BSP_I2C1.Peripheral.SensorBMI160 = 1;

                    /* Enable Power supply */
                    HAL_GPIO_WritePin(BMI160_EN_PORT, BMI160_EN_PIN, BSP_IO_PIN_LOW);

                    /* Enable  external interrupts */
                    SensorNode_BMI160ISRCallback = callback;
                    HAL_NVIC_SetPriority(EXTI15_10_IRQn, EXTI_LINE1510_INTERRUPT_PRIO, 0);
                    BSP_ExtiLine1510.Peripheral.ACC_INT = 1;
                    BSP_ExtiLine1510.Peripheral.GYRO_INT = 1;
                    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
                    ModuleState_BMI160      = BSP_MODULE_STATE_ENABLED;
                    ModuleState_SensorNode  = BSP_MODULE_STATE_ENABLED;
                }
                else
                {
                    ModuleState_BMI160 = BSP_MODULE_STATE_ERROR;
                    ModuleState_SensorNode = BSP_MODULE_STATE_ERROR;
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
            break;
        case BSP_SENSOR_BMM150:
            switch(ModuleState_BMM150)
            {
            case BSP_MODULE_STATE_CONNECTED:
#if HAVE_BMM150
                /* Do not enable if callback is not registered */
                if(NULL == callback)
                {
                    retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
                }
                if(RETCODE_OK == retcode)
                {
                    /* Enable I2C if not enabled */
                    SensorNode_EnableBus();
                    BSP_I2C1.Peripheral.SensorBMM150 = 1;

                    /* Enable Power supply */
                    retcode = BSP_LDO_TLV70233_Enable();
                }

                if(RETCODE_OK == retcode)
                {
                    /* Enable Interrupts */
                    SensorNode_BMM150ISRCallback = callback;
                    /* Enable the interrupt on line 15 for BMM_DRDY signal */
                    if (0 == BSP_ExtiLine1510.Register)
                    {
                        HAL_NVIC_SetPriority(EXTI15_10_IRQn, EXTI_LINE1510_INTERRUPT_PRIO, 0);
                        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
                    }
                    BSP_ExtiLine1510.Peripheral.BMM_DRDY = 1;

                    /* Enable the interrupt on line 3 for BMM_INT signal */
                    HAL_NVIC_SetPriority(EXTI3_IRQn, EXTI_LINE3_INTERRUPT_PRIO, 0);
                    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
                }

                if (RETCODE_OK != retcode)
                {
                    ModuleState_BMM150 = BSP_MODULE_STATE_ERROR;
                    ModuleState_SensorNode = BSP_MODULE_STATE_ERROR;
                }
                else
                {
                    ModuleState_BMM150 = BSP_MODULE_STATE_ENABLED;
                    ModuleState_SensorNode = BSP_MODULE_STATE_ENABLED;
                }
#else /* HAVE_BMM150 */
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
#endif /* HAVE_BMM150 */
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
            break;
        case BSP_SENSOR_BMP280:
            switch(ModuleState_BMP280)
            {
            case BSP_MODULE_STATE_CONNECTED:
                /* Enable I2C if not enabled */
                SensorNode_EnableBus();
                BSP_I2C1.Peripheral.SensorBMP280 = 1;
                /* Enable the power supply*/
                retcode = BSP_LDO_TLV70233_Enable();
                if(RETCODE_OK == retcode)
                {
                    ModuleState_SensorNode = BSP_MODULE_STATE_ENABLED;
                    ModuleState_BMP280 = BSP_MODULE_STATE_ENABLED;
                }
                else
                {
                    ModuleState_SensorNode = BSP_MODULE_STATE_ERROR;
                    ModuleState_BMP280 = BSP_MODULE_STATE_ERROR;
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


        }
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_SensorNode_Disable(uint32_t SensorID)
{
    BSP_MODULE_TRY_LOCK;

    Retcode_T retcode = RETCODE_OK;
    if (false == IS_SENSOR((BSP_Sensor_T )SensorID))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }
    if(RETCODE_OK == retcode)
    {
        switch (SensorID)
        {
        case BSP_SENSOR_BMI160:
            switch(ModuleState_BMI160)
            {
            case BSP_MODULE_STATE_CONNECTED:
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_ENABLED:

                BSP_ExtiLine1510.Peripheral.ACC_INT = 0;
                BSP_ExtiLine1510.Peripheral.GYRO_INT = 0;
                if (0 == BSP_ExtiLine1510.Register)
                {
                    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
                    SensorNode_BMI160ISRCallback = NULL;
                }

                /* Disable power supply */
                HAL_GPIO_WritePin(BMI160_EN_PORT, BMI160_EN_PIN, BSP_IO_PIN_HIGH);

                BSP_I2C1.Peripheral.SensorBMI160 = 0;
                SensorNode_DisableBus();
                ModuleState_BMI160 = BSP_MODULE_STATE_CONNECTED;
                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
            break;
        case BSP_SENSOR_BMM150:
#if HAVE_BMM150
            switch(ModuleState_BMM150)
            {
            case BSP_MODULE_STATE_CONNECTED:
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_ENABLED:
                /* Disable the interrupts on the external lines 10=>15 */
                BSP_ExtiLine1510.Peripheral.BMM_DRDY = 0;
                if (0 == BSP_ExtiLine1510.Register)
                {
                    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
                    SensorNode_BMM150ISRCallback = NULL;
                }

                /* Disable the interrupt on line 3 for BMM_INT signal */
                HAL_NVIC_DisableIRQ(EXTI3_IRQn);

                /* Disable I2C*/
                BSP_I2C1.Peripheral.SensorBMM150 = 0;
                SensorNode_DisableBus();

                if ( RETCODE_OK == retcode)
                {
                    ModuleState_BMM150 = BSP_MODULE_STATE_CONNECTED;
                    /* Disable power supply */
                    retcode = BSP_LDO_TLV70233_Disable();
                }
                if ( RETCODE_OK != retcode)
                {
                    ModuleState_BMM150 = BSP_MODULE_STATE_ERROR;
                    ModuleState_SensorNode = BSP_MODULE_STATE_ERROR;
                }
                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
#else /* HAVE_BMM150 */
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
#endif /* HAVE_BMM150 */
            break;
        case BSP_SENSOR_BMP280:
            switch(ModuleState_BMP280)
            {
            case BSP_MODULE_STATE_CONNECTED:
                retcode = RETCODE_OK;
                break;
            case BSP_MODULE_STATE_ENABLED:
                /* Disable I2C */
                BSP_I2C1.Peripheral.SensorBMP280 = 0;
                SensorNode_DisableBus();

                if ( RETCODE_OK == retcode)
                {
                    ModuleState_BMP280 = BSP_MODULE_STATE_CONNECTED;
                    /* Disable Power supply*/
                    retcode = BSP_LDO_TLV70233_Disable();
                }

                if ( RETCODE_OK != retcode)
                {
                    ModuleState_SensorNode = BSP_MODULE_STATE_ERROR;
                    ModuleState_BMP280 = BSP_MODULE_STATE_ERROR;
                }
                break;
            case BSP_MODULE_STATE_DISCONNECTED: /* FALL-THROUGH */
            case BSP_MODULE_STATE_ERROR: /* FALL-THROUGH */
            case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
            default:
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
                break;
            }
            break;
        }

        if(BSP_MODULE_STATE_CONNECTED == ModuleState_BMI160  && BSP_MODULE_STATE_CONNECTED == ModuleState_BMM150 && BSP_MODULE_STATE_CONNECTED == ModuleState_BMP280)
        {
            ModuleState_SensorNode = BSP_MODULE_STATE_CONNECTED;
        }
    }

    BSP_MODULE_UNLOCK;
    return retcode;
}

Retcode_T BSP_SensorNode_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_SensorNode)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /*
        * Enable all GPIO ports necessary for the operation to make sure all the
        * commands have been executed
        */
        SENSOR_NODE_GPIO_CLOCK_ENABLE();

        BSP_I2C1.Register = 0;

        HAL_GPIO_DeInit(I2C_CLK_PORT, I2C_CLK_PIN);
        HAL_GPIO_DeInit(I2C_DATA_PORT, I2C_DATA_PIN);
        HAL_GPIO_DeInit(BMI160_INT1_PORT, BMI160_INT1_PIN);
        HAL_GPIO_DeInit(BMM_INT_PORT, BMM_INT_PIN);
        HAL_GPIO_DeInit(BMI160_INT2_PORT, BMI160_INT2_PIN);

        ModuleState_SensorNode  = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_BMI160      = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_BMM150      = BSP_MODULE_STATE_DISCONNECTED;
        ModuleState_BMP280      = BSP_MODULE_STATE_DISCONNECTED;
        break;
    case BSP_MODULE_STATE_DISCONNECTED:
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

HWHandle_T BSP_SensorNode_GetI2CHandle(void)
{
    return (HWHandle_T) &BSP_I2C1Handle;
}

void SensorNode_I2CEvISR(void)
{
    BSP_I2C1Handle.IRQCallback((I2C_T) &BSP_I2C1Handle);
}

void SensorNode_I2CErISR(void)
{
    BSP_I2C1Handle.ERRCallback((I2C_T) &BSP_I2C1Handle);
}

void SensorNode_EXTI3ISR(void)
{
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != (int) BSP_IO_PIN_LOW)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);

        if (NULL != SensorNode_BMM150ISRCallback)
        {
            uint32_t Edge;

            /* All interrupt servicing is to be done at the application level */
            if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(AD_IBAT_PORT, AD_IBAT_PIN))
            {
                Edge = (uint32_t) BSP_FALLING_EDGE;
            }
            else
            {
                Edge = (uint32_t) BSP_RISING_EDGE;
            }
            SensorNode_BMM150ISRCallback((uint32_t) BSP_INTERRUPT_INDEX_2, Edge);
        }
    }
}

static void SensorNode_EnableBus(void)
{
    if (0 == BSP_I2C1.Register)
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_I2C_ENABLE(&(BSP_I2C1Handle.hi2c));

        assert(BSP_I2C1Handle.IRQCallback);
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, SENSOR_NODE_I2C_EVENT_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        assert(BSP_I2C1Handle.IRQCallback);
        HAL_NVIC_SetPriority(I2C1_ER_IRQn, SENSOR_NODE_I2C_ERROR_PRIO, 0);
        HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
    }
}

static void SensorNode_DisableBus(void)
{

    if (0 == BSP_I2C1.Register)
    {
        HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
        HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
        __HAL_I2C_DISABLE(&(BSP_I2C1Handle.hi2c));
        __HAL_RCC_I2C1_CLK_DISABLE();
    }
}

void SensorNode_EXTI15_10ISR(void)
{
    /* EXTI interrupt detected on BMI160_INT1 (accelerometer interrupt) */
    if (__HAL_GPIO_EXTI_GET_IT(BMI160_INT1_PIN) != (int) BSP_IO_PIN_LOW)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BMI160_INT1_PIN);
        if (BSP_ExtiLine1510.Peripheral.ACC_INT)
        {
            uint32_t Edge;

            if (NULL != SensorNode_BMI160ISRCallback)
            {
                /* All interrupt servicing is to be done at the application level */
                if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(BMI160_INT1_PORT, BMI160_INT1_PIN))
                {
                    Edge = (uint32_t) BSP_FALLING_EDGE;
                }
                else
                {
                    Edge = (uint32_t) BSP_RISING_EDGE;
                }
                SensorNode_BMI160ISRCallback((uint32_t) BSP_INTERRUPT_INDEX_1, Edge);
            }
        }
    }

    /* EXTI interrupt detected on BMI160_INT2 (Gyroscope interrupt) */
    if (__HAL_GPIO_EXTI_GET_IT(BMI160_INT2_PIN) != (int) BSP_IO_PIN_LOW)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BMI160_INT2_PIN);
        if (BSP_ExtiLine1510.Peripheral.GYRO_INT)
        {
            if (NULL != SensorNode_BMI160ISRCallback)
            {
                uint32_t Edge;

                /* All interrupt servicing is to be done at the application level */
                if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(BMI160_INT2_PORT, BMI160_INT2_PIN))
                {
                    Edge = (uint32_t) BSP_FALLING_EDGE;
                }
                else
                {
                    Edge = (uint32_t) BSP_RISING_EDGE;
                }
                SensorNode_BMI160ISRCallback((uint32_t) BSP_INTERRUPT_INDEX_2, Edge);
            }
        }
    }

    /* EXTI interrupt detected on BMM_DRDY (accelerometer interrupt) */
    if (__HAL_GPIO_EXTI_GET_IT(BMM_DRDY_PIN) != (int) BSP_IO_PIN_LOW)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BMM_DRDY_PIN);
        if (BSP_ExtiLine1510.Peripheral.BMM_DRDY)
        {
            if (NULL != SensorNode_BMM150ISRCallback)
            {
                uint32_t Edge;

                /* All interrupt servicing is to be done at the application level */
                if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(BMM_DRDY_PORT, BMM_DRDY_PIN))
                {
                    Edge = (uint32_t) BSP_FALLING_EDGE;
                }
                else
                {
                    Edge = (uint32_t) BSP_RISING_EDGE;
                }
                SensorNode_BMM150ISRCallback((uint32_t) BSP_INTERRUPT_INDEX_1, Edge);
            }
        }
    }
}

#endif /* BCDS_FEATURE_BSP_SENSOR_NODE */

/**@}*//* @ingroup BSP_SENSOR_NODE */
