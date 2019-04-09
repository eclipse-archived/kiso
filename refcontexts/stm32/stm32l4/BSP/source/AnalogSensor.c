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
 * @ingroup BSP_BOARD
 * @{
 * @file
 * @brief  Implementation of board BSP functions
 */

#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_ANALOGSENSOR

#include "BCDS_BSP.h"
#include "BCDS_BSP_Version.h"
#include "BoardSettings.h"

#if BSP_FEATURE_ANALOG_SENSOR

#define BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE

#include "BSP_HwConfig.h"
#include "BSP_BoardType.h"
#include "AnalogSensor.h"
#include "stm32l4xx_ll_adc.h"
#include "BSP_ModuleState.h"

static ADC_HandleTypeDef BSP_ADC1Handle;
static DMA_HandleTypeDef BSP_ADCDMAHandle;
static BSP_AnalogSensor_NotifyObject_T HeadNotifyObject;

static volatile enum BSP_ModuleState_E ModuleState_AnalogSensor = BSP_MODULE_STATE_DISCONNECTED;
static volatile uint8_t EnableCount;

/*
 * This buffer will be used by the DMA controller to periodically store the values
 * from the ADC. Values are stored in the order as they appear in BSP_AnalogSignal_E
 */
static volatile uint16_t AnalogSignalBackBuffer[BSP_ANALOG_SIGNAL_COUNT];
static int32_t ScaledAnalogSignalBackBuffer[BSP_ANALOG_SIGNAL_COUNT];

static BSP_AnalogSensor_NotifyObject_T AnalogSensorNotifyObject;
static int32_t ScaledAnalogSignals[BSP_ANALOG_SIGNAL_COUNT] = { INT32_C(0) };


/*
 * This callback is needed by the vendor driver to process the interrupt on
 * conversion complete event.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle);
static inline void AnalogSensor_ConversionCompleteCallback(void);
static void AnalogSensor_SignalScaling(void);

static Retcode_T AnalogSensorNotifyFunction(uint32_t size, int32_t buffer[]);

Retcode_T Board_ADC1_Init(void);

enum BSP_ModuleState_E AnalogSensor_getState(void)
{
    return ModuleState_AnalogSensor;
}

/*
 * @brief Initialize the analog to digital converter
 * @details This function initializes the ADC resource. It is called within the
 * Board_Initialize() function.
 *
 * @return RETCODE_OK in case of success error code otherwise.
 */
Retcode_T Board_ADC1_Init(void)
{
    Retcode_T retcode = RETCODE_OK;
    ADC_ChannelConfTypeDef sConfig;

    /* Enable the clock */
    __HAL_RCC_ADC_CLK_ENABLE();

    /* Configure the ADC */
    BSP_ADC1Handle.Instance = ADC1;
    BSP_ADC1Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    BSP_ADC1Handle.Init.Resolution = ADC_RESOLUTION_8B;
    BSP_ADC1Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    BSP_ADC1Handle.Init.ScanConvMode = ADC_SCAN_ENABLE;
    BSP_ADC1Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    BSP_ADC1Handle.Init.LowPowerAutoWait = (uint32_t) DISABLE;
#if !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE)
    BSP_ADC1Handle.Init.ContinuousConvMode = (uint32_t) ENABLE;
#else /* defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */
    BSP_ADC1Handle.Init.ContinuousConvMode = (uint32_t) DISABLE;
#endif /* !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */
    BSP_ADC1Handle.Init.NbrOfConversion = 5;
    BSP_ADC1Handle.Init.DiscontinuousConvMode = (uint32_t) DISABLE;
    BSP_ADC1Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    BSP_ADC1Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    BSP_ADC1Handle.Init.DMAContinuousRequests = (uint32_t) ENABLE;
    BSP_ADC1Handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    BSP_ADC1Handle.Init.OversamplingMode = (uint32_t) DISABLE;

    if (HAL_OK != HAL_ADC_Init(&BSP_ADC1Handle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    if (RETCODE_OK == retcode)
    {
        sConfig.Channel = ADC_CHANNEL_VREFINT;
        sConfig.Rank = (uint32_t) BSP_VREF + 1;
        sConfig.SamplingTime = HW_ADC_VREFINT_SAMPLING_TIME;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;

        if (HAL_OK != HAL_ADC_ConfigChannel(&BSP_ADC1Handle, &sConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == retcode)
    {
        sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
        sConfig.Rank = (uint32_t) BSP_TS + 1;
        sConfig.SamplingTime = HW_ADC_VTS_SAMPLING_TIME;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;

        if (HAL_OK != HAL_ADC_ConfigChannel(&BSP_ADC1Handle, &sConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == retcode)
    {
        sConfig.Channel = VBAT_SENSE_ADC_CHANNEL;
        sConfig.Rank = (uint32_t) BSP_VBAT + 1;
        sConfig.SamplingTime = HW_ADC_VBAT_SAMPLING_TIME;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_OK != HAL_ADC_ConfigChannel(&BSP_ADC1Handle, &sConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == retcode)
    {
        sConfig.Channel = VEXT_SENSE_ADC_CHANNEL;
        sConfig.Rank = (uint32_t) BSP_VEXT + 1;
        sConfig.SamplingTime = HW_ADC_VEXT_SAMPLING_TIME;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_OK != HAL_ADC_ConfigChannel(&BSP_ADC1Handle, &sConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    if (RETCODE_OK == retcode)
    {
        sConfig.Channel = AD_IBAT_CHANNEL;
        sConfig.Rank = (uint32_t) BSP_IBAT + 1;
        sConfig.SamplingTime = HW_ADC_IBAT_SAMPLING_TIME;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_OK != HAL_ADC_ConfigChannel(&BSP_ADC1Handle, &sConfig))
        {
            retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
        }
    }

    __HAL_RCC_DMA1_CLK_ENABLE();

    BSP_ADCDMAHandle.Instance = DMA1_Channel1;

    BSP_ADCDMAHandle.Init.Request = DMA_REQUEST_0;
    BSP_ADCDMAHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    BSP_ADCDMAHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    BSP_ADCDMAHandle.Init.MemInc = DMA_MINC_ENABLE;
    /* Transfer from ADC by half-word to match with ADC configuration: ADC resolution 12 bits */
    BSP_ADCDMAHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    /* Transfer to memory by half-word to match with buffer variable type: half-word */
    BSP_ADCDMAHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
#if !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE)
    /* DMA in circular mode to match with ADC configuration: DMA continuous requests */
    BSP_ADCDMAHandle.Init.Mode = DMA_CIRCULAR;
#else /* defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */
    BSP_ADCDMAHandle.Init.Mode = DMA_NORMAL;
#endif /* !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */
    BSP_ADCDMAHandle.Init.Priority = DMA_PRIORITY_HIGH;
    /* Initialize the DMA for new transfer */
    if (HAL_OK != HAL_DMA_Init(&BSP_ADCDMAHandle))
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
    }

    /* Associate the initialized DMA handle to the ADC handle */
    __HAL_LINKDMA(&BSP_ADC1Handle, DMA_Handle, BSP_ADCDMAHandle);
    /* Disable the clock */
    /** @todo 2016-11-29, BCDS/ENG1: clarify whether the below line resets the registers */
    //__HAL_RCC_ADC_CLK_DISABLE();
    return retcode;
}

Retcode_T BSP_AnalogSensor_Connect(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    Retcode_T retcode = RETCODE_OK;

    BSP_MODULE_TRY_LOCK;
    switch(ModuleState_AnalogSensor)
    {
    case BSP_MODULE_STATE_DISCONNECTED:
    case BSP_MODULE_STATE_ERROR: /*In case of error state we try to connect again to go back in a defined state, will always succeed*/
        /* Enabling GPIO clocks required by the Charger module */
        CHARGER_GPIO_CLOCK_ENABLE();

        memset(&HeadNotifyObject, 0, sizeof(BSP_AnalogSensor_NotifyObject_T));

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = VBAT_SENSE_ADC_EN_PIN;
        GPIO_InitStruct.Mode = VBAT_SENSE_ADC_EN_MODE;
        GPIO_InitStruct.Pull = VBAT_SENSE_ADC_EN_PULL;
        GPIO_InitStruct.Speed = VBAT_SENSE_ADC_EN_SPEED;
        HAL_GPIO_Init(VBAT_SENSE_ADC_EN_PORT, &GPIO_InitStruct);
        HAL_GPIO_WritePin(VBAT_SENSE_ADC_EN_PORT, VBAT_SENSE_ADC_EN_PIN, BSP_IO_PIN_LOW);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = AD_IBAT_PIN;
        GPIO_InitStruct.Mode = AD_IBAT_MODE;
        GPIO_InitStruct.Pull = AD_IBAT_PULL;
        HAL_GPIO_Init(AD_IBAT_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = VEXT_SENSE_ADC_PIN;
        GPIO_InitStruct.Mode = VEXT_SENSE_ADC_MODE;
        GPIO_InitStruct.Pull = VEXT_SENSE_ADC_PULL;
        HAL_GPIO_Init(VEXT_SENSE_ADC_PORT, &GPIO_InitStruct);

        memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
        GPIO_InitStruct.Pin = VBAT_SENSE_ADC_PIN;
        GPIO_InitStruct.Mode = VBAT_SENSE_ADC_MODE;
        GPIO_InitStruct.Pull = VBAT_SENSE_ADC_PULL;
        HAL_GPIO_Init(VBAT_SENSE_ADC_PORT, &GPIO_InitStruct);

        EnableCount = 0;
        ModuleState_AnalogSensor = BSP_MODULE_STATE_CONNECTED;
        break;
    case BSP_MODULE_STATE_CONNECTED:
        break;
    case BSP_MODULE_STATE_ENABLED:
        /* if enable, we are already connected */
        break;
    case BSP_MODULE_STATE_UNINITIALIZED: /* FALL-THROUGH */
    default:
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
        break;
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}

Retcode_T BSP_AnalogSensor_Disconnect(void)
{
    BSP_MODULE_TRY_LOCK;

    Retcode_T retcode = RETCODE_OK;

    switch(ModuleState_AnalogSensor)
    {
    case BSP_MODULE_STATE_CONNECTED:
        /* Enabling GPIO clocks required by the Charger module */
        CHARGER_GPIO_CLOCK_ENABLE();

        HAL_GPIO_DeInit(AD_IBAT_PORT, AD_IBAT_PIN);
        HAL_GPIO_DeInit(VBAT_SENSE_ADC_PORT, VBAT_SENSE_ADC_PIN);
        HAL_GPIO_DeInit(VBAT_SENSE_ADC_EN_PORT, VBAT_SENSE_ADC_EN_PIN);

        ModuleState_AnalogSensor = BSP_MODULE_STATE_DISCONNECTED;
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

Retcode_T BSP_AnalogSensor_Enable(uint32_t ids)
{
    Retcode_T retcode = RETCODE_OK;

    BCDS_UNUSED(ids);

    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_AnalogSensor)
    {
    case BSP_MODULE_STATE_CONNECTED:
        if (HAL_ADCEx_Calibration_Start(&BSP_ADC1Handle, ADC_SINGLE_ENDED) != HAL_OK)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }

        /* Start measurement */
        if (RETCODE_OK == retcode)
        {
            /* Enable VBAT measurement */
            HAL_GPIO_WritePin(VBAT_SENSE_ADC_EN_PORT, VBAT_SENSE_ADC_EN_PIN, BSP_IO_PIN_HIGH);
            HAL_Delay(12);

            /* NVIC configuration for DMA Input data interrupt */
            HAL_NVIC_SetPriority(CHARGER_DMA_IRQn, CHARGER_DMA_PREEMPT_PRIORITY, CHARGER_DMA_SUBPRIORITY);

            /* enabling interrupt requests */
            HAL_NVIC_EnableIRQ(CHARGER_DMA_IRQn);

    #if !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE)
            /* Start the signals conversion and store to memory using DMA */
            if (HAL_ADC_Start_DMA(&BSP_ADC1Handle, (uint32_t *) AnalogSignalBackBuffer, (uint32_t) BSP_ANALOG_SIGNAL_COUNT) != HAL_OK)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
    #endif /* !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */
        }

        if (RETCODE_OK == retcode)
        {
            ModuleState_AnalogSensor = BSP_MODULE_STATE_ENABLED;
        }
        else
        {
            ModuleState_AnalogSensor = BSP_MODULE_STATE_ERROR;
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

Retcode_T BSP_AnalogSensor_Disable(uint32_t ids)
{
    Retcode_T retcode = RETCODE_OK;

    BCDS_UNUSED(ids);

    BSP_MODULE_TRY_LOCK;

    switch(ModuleState_AnalogSensor)
    {
    case BSP_MODULE_STATE_CONNECTED:
        retcode = RETCODE_OK;
        break;
    case BSP_MODULE_STATE_ENABLED:
        assert(EnableCount);

        EnableCount--;

        if (0 == EnableCount)
        {
            /* Stop analog signal monitoring */
            HAL_NVIC_DisableIRQ(CHARGER_DMA_IRQn);

#if !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE)
            if (HAL_OK != HAL_ADC_Stop_DMA(&BSP_ADC1Handle))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
#endif /* !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */

            HAL_GPIO_WritePin(VBAT_SENSE_ADC_EN_PORT, VBAT_SENSE_ADC_EN_PIN, BSP_IO_PIN_LOW);
            memset((void*) AnalogSignalBackBuffer, 0, sizeof(AnalogSignalBackBuffer));

            if (RETCODE_OK == retcode)
            {
                ModuleState_AnalogSensor = BSP_MODULE_STATE_CONNECTED;
            }
            else
            {
                ModuleState_AnalogSensor = BSP_MODULE_STATE_ERROR;
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

#if defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE)
Retcode_T BSP_AnalogSensor_PollSignals(void)
{
    BSP_MODULE_TRY_LOCK;
    Retcode_T retcode = RETCODE_OK;

    if (HAL_ADC_Start_DMA(&BSP_ADC1Handle, (uint32_t *) AnalogSignalBackBuffer, (uint32_t) BSP_ANALOG_SIGNAL_COUNT) != HAL_OK)
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    else
    {
        do
        {
            ; /* KISS loop */
        } while (HAL_DMA_STATE_READY != HAL_DMA_GetState(BSP_ADC1Handle.DMA_Handle));

        if (HAL_DMA_ERROR_NONE != HAL_DMA_GetError(BSP_ADC1Handle.DMA_Handle))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
        else
        {
            AnalogSensor_ConversionCompleteCallback();
        }

        /*
         * HAL_ADC_Stop_DMA() stops the ADC and aborts the DMA. If a DMA transaction
         * is not on-going as it already concluded the API sets the ADC status to
         * HAL_ADC_STATE_ERROR_INTERNAL and the DMA error code to HAL_DMA_ERROR_NO_XFER.
         *
         * The functional behavior of the 3rd party API is questionable as the ADC
         * driver of the HAL programming model allows non continuous conversions and
         * thus the DMA can be in DMA_NORMAL mode instead of DMA_CIRCULAR.
         */
        if ((HAL_OK != HAL_ADC_Stop_DMA(&BSP_ADC1Handle)) && ((HAL_DMA_ERROR_NO_XFER | HAL_DMA_ERROR_NONE) ^ HAL_DMA_GetError(BSP_ADC1Handle.DMA_Handle)))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }

    BSP_MODULE_UNLOCK;

    return retcode;
}
#endif /* defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */

Retcode_T BSP_AnalogSensor_RegisterNotify(BSP_AnalogSensor_NotifyObject_T* object, BSP_AnalogSensor_NotifyFunction function)
{
    BSP_MODULE_TRY_LOCK;
    BSP_AnalogSensor_NotifyObject_T* iterator;

    /* search for matching object */
    for (iterator = &HeadNotifyObject; iterator != object && iterator->next != NULL; iterator = iterator->next)
    {
        ;
    }

    /* test match */
    if (iterator != object)
    {
        iterator->next = object;
        object->prev = iterator;
    }
    else
    {
        ; /* nothing to do */
    }

    assert(function);
    object->notifyFunction = function;

    BSP_MODULE_UNLOCK;

    return RETCODE_OK;
}

Retcode_T BSP_AnalogSensor_DeregisterNotify(BSP_AnalogSensor_NotifyObject_T* object)
{
    BSP_MODULE_TRY_LOCK;
    BSP_AnalogSensor_NotifyObject_T* iterator;

    /* search for matching object */
    for (iterator = &HeadNotifyObject; iterator != object && iterator->next != NULL; iterator = iterator->next)
    {
        ;
    }

    /* test match */
    if (iterator == object)
    {
        iterator->prev->next = iterator->next;
        if (iterator->next)
        {
            iterator->next->prev = iterator->prev;
        }
    }
    else
    {
        ; /* nothing to do */
    }

    BSP_MODULE_UNLOCK;
    return RETCODE_OK;
}

void BSP_AnalogSensor_ConversionCompleteISR(void)
{
    HAL_DMA_IRQHandler(BSP_ADC1Handle.DMA_Handle);
}

void AnalogSensor_SignalScaling(void)
{
    uint32_t iterator;

    for (iterator = 0; iterator < (uint32_t) BSP_ANALOG_SIGNAL_COUNT; iterator++)
    {
        ScaledAnalogSignalBackBuffer[iterator] = AnalogSignalBackBuffer[iterator];
    }

    /* implementation only supports 8 bit ADC resolution */
    {
        ScaledAnalogSignalBackBuffer[BSP_VREF] = /*lint -e{572} Macro is provided by STM32Cube library */
        __LL_ADC_CALC_VREFANALOG_VOLTAGE(ScaledAnalogSignalBackBuffer[BSP_VREF], LL_ADC_RESOLUTION_8B);
        ScaledAnalogSignalBackBuffer[BSP_TS] = /*lint -e{572,573} Macro is provided by STM32Cube library */
        __LL_ADC_CALC_TEMPERATURE(ScaledAnalogSignalBackBuffer[BSP_VREF],
                ScaledAnalogSignalBackBuffer[BSP_TS], LL_ADC_RESOLUTION_8B);

        ScaledAnalogSignalBackBuffer[BSP_VBAT] = (ScaledAnalogSignalBackBuffer[BSP_VREF]
                * ScaledAnalogSignalBackBuffer[BSP_VBAT]
                * HW_ADC_VBAT_SCALING_DIVIDEND)
                / (HW_ADC_VBAT_SCALING_DIVISOR);

        ScaledAnalogSignalBackBuffer[BSP_IBAT] = (ScaledAnalogSignalBackBuffer[BSP_VREF]
                * ScaledAnalogSignalBackBuffer[BSP_IBAT]
                * HW_ADC_IBAT_SCALING_DIVIDEND)
                / (HW_ADC_IBAT_SCALING_DIVISOR);

        ScaledAnalogSignalBackBuffer[BSP_VEXT] = (ScaledAnalogSignalBackBuffer[BSP_VREF]
                * ScaledAnalogSignalBackBuffer[BSP_VEXT]
                * HW_ADC_VEXT_SCALING_DIVIDEND)
                / (HW_ADC_VEXT_SCALING_DIVISOR);
    }
}

static inline void AnalogSensor_ConversionCompleteCallback(void)
{
    BSP_AnalogSensor_NotifyObject_T* iterator;

    AnalogSensor_SignalScaling();

    /* search for matching object */
    for (iterator = HeadNotifyObject.next; iterator != NULL; iterator = iterator->next)
    {
        Retcode_T retcode;
        if ((retcode = iterator->notifyFunction((uint32_t) BSP_ANALOG_SIGNAL_COUNT, ScaledAnalogSignalBackBuffer)) != RETCODE_OK)
        {
            Retcode_RaiseError(retcode);
        }
    }
}
/*
 * This function is executed each time the ADC has scanned the input channels
 * and data is ready in the DMA buffer
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
    BCDS_UNUSED(AdcHandle);

#if !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE)
    AnalogSensor_ConversionCompleteCallback();
#endif /* !defined(BSP_FEATURE_ANALOG_SENSOR_POLLING_HAL_INTERFACE) */
}


Retcode_T AnalogSensorNotifyFunction(uint32_t size, int32_t buffer[])
{
    memcpy((void*) ScaledAnalogSignals, buffer, size * sizeof(int32_t));

    return RETCODE_OK;
}

Retcode_T BSP_AnalogSensor_MeasureSignal(uint32_t signal, int32_t * result)
{
    Retcode_T retcode = RETCODE_OK;
    if ((BSP_ANALOG_SIGNAL_COUNT <= (BSP_AnalogSignal_T) signal) ||
        (0 == result))
    {
        retcode = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM));
    }

    enum BSP_ModuleState_E AnalogSensorState = BSP_MODULE_STATE_DISCONNECTED;
    if(RETCODE_OK == retcode)
    {
        AnalogSensorState = AnalogSensor_getState();
        if ((BSP_MODULE_STATE_UNINITIALIZED == AnalogSensorState) ||
            (BSP_MODULE_STATE_ERROR == AnalogSensorState))
        {
            retcode = (RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE));
        }
    }

    if(RETCODE_OK == retcode)
    {
        if (BSP_MODULE_STATE_DISCONNECTED == AnalogSensorState)
        {
            retcode = BSP_AnalogSensor_Connect();
        }
    }

    if(RETCODE_OK == retcode)
    {
        retcode = BSP_AnalogSensor_RegisterNotify(&AnalogSensorNotifyObject, AnalogSensorNotifyFunction);
    }

    if(RETCODE_OK == retcode)
    {
        retcode = BSP_AnalogSensor_Enable(0);
    }

    if(RETCODE_OK == retcode)
    {
        retcode = BSP_AnalogSensor_PollSignals();
    }
    else
    {
        /* Intentionally discarding return value */
        (void) BSP_AnalogSensor_DeregisterNotify(&AnalogSensorNotifyObject);
    }

    if(RETCODE_OK == retcode)
    {
        /*lint -e{661,662} The value of signal is tested against BSP_ANALOG_SIGNAL_COUNT */
        /*lint -e{613} The value of result is tested against zero */
        *result = ScaledAnalogSignals[signal];
        retcode = BSP_AnalogSensor_DeregisterNotify(&AnalogSensorNotifyObject);
    }
    else
    {
        /* Intentionally discarding return value */
        (void) BSP_AnalogSensor_Disable(0);
        (void) BSP_AnalogSensor_DeregisterNotify(&AnalogSensorNotifyObject);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = BSP_AnalogSensor_Disable(0);
    }
    else
    {
        /* Intentionally discarding return value */
        (void) BSP_AnalogSensor_Disable(0);
    }

    return retcode;
}

/**@}*//* @ingroup BSP_BOARD */
#endif /* if BSP_FEATURE_ANALOG_SENSOR */
