/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Bleeper board GPIO driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
/*****************************************************
 * This file has Modified as per the BCDS-APLM requirements
 *****************************************************/
#include "board.h"
#include "gpio-board.h"

/* API documentation is in the header file */
void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
    obj->pinIndex = loraEfm32PinMap[pin].pin;
    obj->portIndex = loraEfm32PinMap[pin].port;
    (void)config;
    (void)type;

    if( mode == PIN_OUTPUT )
    {
        PTD_pinModeSet((GPIO_Port_TypeDef)obj->portIndex, obj->pinIndex, gpioModePushPullDrive, value);
		GpioWrite( obj, value );
    }
    else if( mode == PIN_INPUT )
    {
        PTD_pinModeSet((GPIO_Port_TypeDef)obj->portIndex, obj->pinIndex, gpioModeInput, 0);
    }
    else
    {
        PTD_pinModeSet((GPIO_Port_TypeDef)obj->portIndex, obj->pinIndex, gpioModePushPullDrive, value);
		GpioWrite( obj, value );
    }
}

/* API documentation is in the header file */
void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
	uint8_t risingedge = false;
	    uint8_t fallingedge = false;
	    (void)obj;
	    (void)irqPriority;

	    /* The interrupt pin is configured in the PTD module for the Lora */
	    if (IRQ_RISING_EDGE == irqMode)
	    {
	        risingedge=true;
	        fallingedge=false;
	    }
	    else if(IRQ_FALLING_EDGE == irqMode)
	    {
	        risingedge=false ;
	        fallingedge=true;
	    }
	    else if (IRQ_RISING_FALLING_EDGE == irqMode)
	    {
	        risingedge=true;
	        fallingedge=true;
	    }
	    else
	    {
	        /**do nothing  */
	    }
	    PTD_intConfig((GPIO_Port_TypeDef)obj->portIndex ,obj->pinIndex,risingedge, fallingedge,true,irqHandler);
}

/* API documentation is in the header file */
void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
 (void)obj;
}

/* API documentation is in the header file */
void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
    if (true == value )
    {
        PTD_pinOutSet((GPIO_Port_TypeDef)obj->portIndex, obj->pinIndex);
    }
    else
    {
        PTD_pinOutClear((GPIO_Port_TypeDef)obj->portIndex, obj->pinIndex);
    }
}

/* API documentation is in the header file */
uint32_t GpioMcuRead( Gpio_t *obj )
{
	 (void)obj;
    return 0;
}

