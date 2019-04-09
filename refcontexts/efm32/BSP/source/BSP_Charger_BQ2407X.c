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
 * @ingroup BSP_XDK110_Charger_MODULE
 * @{
 */
/**
 * @file
 * @brief  Implementation of Charger BSP function.
 *
 */

/* This is used to define the correct module ID for generation of module
 * error codes
 */
#include "BCDS_BSP.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_CHARGER_BQ2407X

#include "BCDS_HAL.h"
#if BCDS_FEATURE_BSP_CHARGER_BQ2407X

#include "BSP_BoardType.h"
#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#include "BCDS_BSP_Charger_BQ2407X.h"
#include "BCDS_Assert.h"

static bool chargerEnabled = false;
/*
 * Refer to interface header for description
 */
Retcode_T BSP_Charger_BQ2407X_Connect(void)
{
    GPIO_PinModeSet(CHARGING_SELECT_PORT, CHARGING_SELECT_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(VOUT_SENSE_ADC_PORT, VOUT_SENSE_ADC_PIN, gpioModeDisabled, 0);
    return RETCODE_OK;
}

/*
 * refer to interface for description
 */
Retcode_T BSP_Charger_BQ2407X_Enable(uint32_t control)
{
    Retcode_T retcode = RETCODE_OK;

    if ((uint32_t)BSP_XDK_CHARGING_SPEED_2 == control)
    {
        GPIO_PinOutSet(CHARGING_SELECT_PORT, CHARGING_SELECT_PIN);

    }
    else if ((uint32_t)BSP_XDK_CHARGING_SPEED_1 == control)
    {
        GPIO_PinOutClear(CHARGING_SELECT_PORT, CHARGING_SELECT_PIN);
        chargerEnabled = true;
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        chargerEnabled = true;

    }
    return retcode;
}
/*
 * Refer interface header for description.
 */
Retcode_T BSP_Charger_BQ2407X_Disable(void)
{
    chargerEnabled = false;
    GPIO_PinOutClear(CHARGING_SELECT_PORT, CHARGING_SELECT_PIN);
    return RETCODE_OK;
}

/*
 * Refer interface header for description.
 */
Retcode_T BSP_Charger_BQ2407X_MeasureSignal(uint32_t signal, uint32_t* output)
{
    BCDS_UNUSED(signal);
    Retcode_T retval = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_CHARGER_NOT_ENABLED);
    ADC_InitSingle_TypeDef singleInit = ADC_INITSINGLE_DEFAULT;
    int32_t sample = 0;
    int32_t voltage = 0;
    if(NULL == output)
    {
    	return(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER));
    }
    if (chargerEnabled)
    {
        /* Init for single conversion use, measure VDD/3 with 1.25 reference. */
        singleInit.reference = adcRef2V5;
        singleInit.input = adcSingleInpCh7;
        singleInit.resolution = adcRes12Bit;
        singleInit.acqTime = adcAcqTime64;
        while ((ADC0->STATUS & (ADC_STATUS_SINGLEACT | ADC_STATUS_SCANACT)) && (BSP_UNLOCKED == ADCLock));
        (void)INT_Disable(); /* Ignoring the return purposefully */
        ADCLock = BSP_LOCKED;
        (void)INT_Enable(); /* Ignoring the return purposefully */
        ADC_InitSingle(ADC0, &singleInit);
        ADC_Start(ADC0, adcStartSingle);

        /* Wait while conversion is active */
        while (ADC0->STATUS & (ADC_STATUS_SINGLEACT | ADC_STATUS_SCANACT));
        sample = ADC_DataSingleGet(ADC0);
        (void)INT_Disable(); /* Ignoring the return purposefully */
        ADCLock = BSP_UNLOCKED;
        (void)INT_Enable(); /* Ignoring the return purposefully */
        voltage = (2 * sample * 2500) / 4096; // the volt value equals the sample integer value times the bit resolution in Volts/bit
        retval = RETCODE_OK;
    }
    *output = voltage;
    return retval;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_Charger_BQ2407X_GetStatus(BSP_ChargeState_T* state)
{
	Retcode_T retval = RETCODE_OK;
    uint32_t BatteryVoltage = 0;
    *state = BSP_XDK_CHARGE_STATUS_UNDEFINED;
    if(NULL == state)
    {
    	retval = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    if(RETCODE_OK == retval)
    {
        retval = BSP_Charger_BQ2407X_MeasureSignal(0, &BatteryVoltage); /* input signal is not applicable for Efm32 Ref boards */
    }
    if(RETCODE_OK == retval)
    {
    	if ((BatteryVoltage >= CHARGE_CRITICAL_LOWER_LIMIT) && (BatteryVoltage <= CHARGE_CRITICAL_UPPER_LIMIT)) //refer to charger and battery specification for threshold information
    	{
    		*state = BSP_XDK_CHARGE_STATUS_CRITICAL;
    	}
    	else if ((BatteryVoltage > CHARGE_LOW_LOWER_LIMIT) && (BatteryVoltage < CHARGE_LOW_UPPER_LIMIT)) //refer to charger and battery specification for threshold information
    	{
    		*state = BSP_XDK_CHARGE_STATUS_LOW;
    	}
    	else if ((BatteryVoltage >= CHARGE_NORMAL_LOWER_LIMIT) && (BatteryVoltage <= CHARGE_NORMAL_UPPER_LIMIT)) //refer to charger and battery specification for threshold information
    	{
    		*state = BSP_XDK_CHARGE_STATUS_NORMAL;
    	}
    	else if ((BatteryVoltage > CHARGE_HIGH_LOWER_LIMIT) && (BatteryVoltage < CHARGE_HIGH_UPPER_LIMIT)) //refer to charger and battery specification for threshold information
    	{
    		*state = BSP_XDK_CHARGE_STATUS_FULL;
    	}
    	else if ((BatteryVoltage >= CHARGE_ON_LOWER_LIMIT) && (BatteryVoltage <= CHARGE_ON_UPPER_LIMIT))
    	{
    		*state = BSP_XDK_CHARGE_STATUS_ON;
    	}
    	else
    	{
    		;/* state = BSP_XDK_CHARGE_STATUS_UNDEFINED; */
    	}
    }
    return retval;
}

/*
 * Refer interface header for description.
 */
Retcode_T BSP_Charger_BQ2407X_Disconnect(void)
{
    GPIO_PinModeSet(CHARGING_SELECT_PORT, CHARGING_SELECT_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(VOUT_SENSE_ADC_PORT, VOUT_SENSE_ADC_PIN, gpioModeDisabled, 0);
    return RETCODE_OK;
}

/*
 * Refer interface header for description.
 */
Retcode_T BSP_Charger_BQ2407X_SetSysoffLow(void)
{
    return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_NOT_SUPPORTED);
}

/*
 * Refer interface header for description.
 */
Retcode_T BSP_Charger_BQ2407X_SetSysoffHigh(void)
{
    return RETCODE(RETCODE_SEVERITY_INFO, RETCODE_NOT_SUPPORTED);
}

#endif

/**@}*/
