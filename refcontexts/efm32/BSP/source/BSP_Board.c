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
 * @ingroup BSP_XDK110_BOARD
 * @{
 */

/**
 * @file
 * @brief  Implementation of Board BSP functions
 */

#include "BCDS_BSP.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_BOARD

#include "BCDS_BSP_Board.h"

#include "BSP_BoardSettings.h"
#include "BSP_BoardExtSettings.h"
#include "BSP_BoardType.h"
#include "BSP_BoardShared.h"
#include "BSP_IrqHandler.h"

/*include feature specific header files*/
#if BCDS_FEATURE_BSP_USB
#include "BCDS_USB.h"
#include "usbconfig.h"
#endif

#if BCDS_FEATURE_TIMER
#include "BCDS_MCU_Timer.h"
#include "BCDS_MCU_Timer_Handle.h"
#endif

#if BCDS_FEATURE_DMA
#include "BSP_DMA.h"
#include "BCDS_MCU_DMA_Handle.h"
#endif



BSPLock_T ADCLock = BSP_UNLOCKED;/**< global scope storage for ADC lock state */

OStoBSPDelayFunc_T OStoBSPdelayFunc; /**< global scope function pointer to OS specific delay function */

static volatile uint32_t BSP_Tick;/**< local scope storage for tick count */

static GPIOINT_IrqCallbackPtr_t gpioCallbacks[16] = { 0 }; /**< array of function pointers to gpio interrupt handlers*/

#if BCDS_BOARD_XDK110
static PowerSupply3V3Register_T BSP_PowerSupply3V3;
static PowerSupply2V5Register_T BSP_PowerSupply2V5;/**< power supply 2v5 register */
#if BCDS_FEATURE_DMA
struct MCU_DMA_Controller_S DmaController;
static DMA_DESCRIPTOR_TypeDef DMA_Descriptors[1][12] __attribute__((aligned(0x100)));
#endif /* BCDS_FEATURE_DMA */
#endif /* BCDS_BOARD_XDK110 */

#if BCDS_BOARD_XDK110
/**
 * @brief Initialization of GPIO Interrupts.
 */
static void Board_GPIOInterruptInit(void);

/**
 * @brief Initialization of critical GPIO pins.
 * @
 */
static void Board_GPIO_Init(void);

/**
 * @brief Initialize the UART0
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_UART0_Init(void);

/**
 * @brief Initialize the UART1
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_UART1_Init(void);

/**
 * @brief Initialize the USART0 interface
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_USART0_Init(void);

/**
 * @brief Initialize the USART1 interface
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_USART1_Init(void);
#endif /* BCDS_BOARD_XDK110 */

#if BCDS_BOARD_APLM
/**
 * @brief Initializes the Board part of the BURTC peripheral
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_LEUART1_Init(void);

/**
 * @brief Initializes USART2 interface
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_USART2_Init(void);
#endif /* BCDS_BOARD_APLM */

#if BCDS_BOARD_XDK110
/**
 * @brief Initialize the I2C0 interface
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_I2C0_Init(void);

/**
 * @brief Initialize the I2C1 interface
 * @return RETCODE_OK in case of success error code otherwise.
 *
 */
static Retcode_T Board_I2C1_Init(void);

/**
 * @brief Initialize the ADC interface for the analog measurements
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_ADC_Init(void);

/**
 * @brief Initialize the USB interface
 * @return RETCODE_OK in case of success error code otherwise.
 */
static Retcode_T Board_USB_Init(void);

/**
 * @brief Initializes the Board part of the BURTC peripheral
 */
static Retcode_T Board_BURTC_Init(void);

/**
 * @brief Enable the 4-Bit Trace Port
 *
 * Enables the ETM peripheral to enable insights into the micro-controller state
 *
 * @note Enable the ETM port after starting the GPIO peripheral
 */
static void Board_ETM_Enable(void);

#endif /* BCDS_BOARD_XDK110 */

/**
 * @brief Initializes the Board part of the RTC peripheral
 */
static Retcode_T Board_RTC_Init(void);

#if BCDS_FEATURE_TRACEPORT

/**
 * @brief Disables the 4-bit Trace Port
 */
static void Board_ETM_Disable(void);
#endif /* BCDS_FEATURE_TRACEPORT */

#if BCDS_BOARD_XDK110
/**
 * @brief Enable the Serial Wire Viewer Port
 *
 * This enables the SWO port enabling printf-style debugging
 * and tracing using the ITM peripheral
 *
 * @note Enable the SWO port after starting the GPIO peripheral
 *
 */
static void Board_SWO_Enable(void);
#endif /* BCDS_BOARD_XDK110 */

#if BCDS_FEATURE_TRACEPORT
/**
 * @brief Disables the SWO port
 */
static void Board_SWO_Disable(void);

/**
 * @brief Enable the trace clock necessary for ETM and SWO
 */
static void Board_TraceClock_Enable(void);
#endif /* BCDS_FEATURE_TRACEPORT */

#if BCDS_BOARD_XDK110
/*
 * @brief function updating 3V3 power rail register on XDK Board
 */
static void PowerSupply3V3UpdateRegister(Peripherals_T peripheral, uint32_t updateValue);

/*
 * @brief function updating 2V5 power rail register on XDK Board
 */
static void PowerSupply2V5UpdateRegister(Peripherals_T peripheral, uint32_t updateValue);
#endif /* BCDS_BOARD_XDK110 */

/*---------------------------------------------------------------------------------------------------------------------------------------*/

/*
 * Refer to  function interface for description.
 */
void BSP_Board_OSTickInitialize(BSP_Systick_Callback_T PreCallback, BSP_Systick_Callback_T PostCallback)
{
	SysTickOsPreProcessing = PreCallback;

	SysTickOsPostProcessing = PostCallback;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_Board_Initialize(uint32_t param1, void* param2)
{
	BCDS_UNUSED(param1);
	BCDS_UNUSED(param2);
	Retcode_T retcode = RETCODE_OK;

#if BCDS_BOARD_APLM

	CHIP_Init();
	//Set high frequency clock band to 28 MHz
	CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);
	//Prescale HF clock to 28 MHz
	CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_1);
	//Prescale HFPERCLK to 14 MHz
	CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_2);
	//Select HFRCO as clock source for HF clock
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
	//Enable clock to interface of the low energy modules
	CMU_ClockEnable(cmuClock_CORELE, true);

	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	/* Enable clock to CPIO  */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Enable LFXO and wait for it to stabilize */
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

	/* Select CORELEDIV2 for LEUARTs (and wait for it to stabilize) */
	CMU_ClockSelectSet(cmuClock_LFB, BSP_LEUART_CLOCK_SELECT);

	CMU_ClockEnable(cmuClock_LFB, true);

	CMU_ClockEnable(BSP_SERIAL_LEUART_CLOCK, true);

	/* Prescale value for LEUART clock */
	CMU_ClockDivSet(BSP_SERIAL_LEUART_CLOCK, BSP_LEUART_CLOCKDIV_VALUE);

	/* Set the priority for the systick interrupt*/
	NVIC_SetPriority(SysTick_IRQn, 255);

	/* Use SysTick as time base source and configure 1ms tick (default clock after Reset is MSI) */
	/*Configure the SysTick to have interrupt in 1ms time basis*/
	retcode = BSP_Board_EnableSysTick();

	if (RETCODE_OK == retcode)
	{
		retcode = Board_USART2_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_LEUART1_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_RTC_Init();
	}


#elif BCDS_BOARD_XDK110

	CHIP_Init();
	MSC_Init();
	NVIC_SetPriorityGrouping((uint32_t) 3);/* Set priority grouping to group 4*/


#if BCDS_FEATURE_DMA
	BSP_DMA_Initialize(&DMA_Descriptors);
	BSP_DMA_Enable();
#endif

	/* Use SysTick as time base source and configure 1ms tick (default clock after Reset is MSI) */

	/* Start HFXO and wait until it is stable */
	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);

	/* Select HFXO as clock source for HFCLK */
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);

	/* Disable HFRCO */
	CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

	if (RETCODE_OK == retcode)
	{
		retcode = IrqHandler_SysTickDummyHandler();
	}
	if (RETCODE_OK == retcode)
	{
		/* Set the priority for the systick interrupt*/
		NVIC_SetPriority(SysTick_IRQn, 0);

		/* enable clock for HF peripherals */
		CMU_ClockEnable(cmuClock_HFPER, true);

		/* Starting LFXO and waiting until it is stable */
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

		/* Routing the LFXO clock to the RTC */
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

		/* Use SysTick as time base source and configure 1ms tick (default clock after Reset is MSI) */
		/*Configure the SysTick to have interrupt in 1ms time basis*/
		retcode = BSP_Board_EnableSysTick();

		/* Wake power supply up*/
		BoardCommon_WakeupPowerSupply2V5(MCU);
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_BURTC_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_RTC_Init();
	}
	if (RETCODE_OK == retcode)
	{
		Board_GPIO_Init();
		Board_GPIOInterruptInit();
		Board_SWO_Enable();
		Board_ETM_Enable();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_UART0_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_UART1_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_USART0_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_USART1_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_I2C0_Init();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = Board_I2C1_Init();
	}
	if(RETCODE_OK == retcode)
	{
		retcode = Board_ADC_Init();
	}
#if BCDS_FEATURE_BSP_USB
	if (RETCODE_OK == retcode)
	{
		retcode = Board_USB_Init();
	}
#endif
	BSP_Board_SetOStoBSPDelayFunc(BoardCommon_Delay);
#endif

	return retcode;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_Board_SwitchPowerMode(uint32_t powerMode)
{
	BCDS_UNUSED(powerMode);
	return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*
 * Refer to interface for description
 */
uint32_t BSP_Board_GetWakeupSource(void)
{
	return (uint32_t) BSP_XDK_WAKE_EVENT_NONE;
}

/*
 * Refer to interface for description
 */
uint32_t BSP_Board_DetectPowerSupply(void)
{
	return (uint32_t) POWER_SUPPLY_UNKNOWN;
}

/*
 * Refer to interface for description
 */
uint32_t BSP_Board_GetBoardRevision(void)
{
	return 0;
}

/*
 * Refer to interface for description
 */
Retcode_T BSP_Board_ControlPowerRail(uint32_t control)
{
#if BCDS_BOARD_XDK110
	Retcode_T retcode = RETCODE_OK;
	BSP_PowerSupply3V3.Register = 0;
	switch (control)
	{
	case BSP_XDK_RAIL_3V3_DISABLE:
		GPIO_PinOutClear(PWR_3V3_EN_PORT, PWR_3V3_EN_PIN);
		BSP_PowerSupply3V3.Register = 0;

		break;
	case BSP_XDK_RAIL_2V5_SNOOZE:
		GPIO_PinOutClear(PWR_2V5_SNOOZE_PORT, PWR_2V5_SNOOZE_PIN);
		BSP_PowerSupply2V5.Register = 0;
		break;
	default:
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
	}
	return retcode;
#else
	BCDS_UNUSED(control);
	return RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_NOT_SUPPORTED);
#endif
}

/*
 * Provides time delay in millisecond
 */
void BSP_Board_Delay(uint32_t delayMs)
{
	uint32_t tickstart = 0;
	tickstart = BoardCommon_GetTick();
	while ((BoardCommon_GetTick() - tickstart) < delayMs)
	{
	}
}

/*
 * Provides time delay in microseconds
 */
void BSP_Board_DelayUs(uint32_t delayInUs)
{
	BCDS_UNUSED(delayInUs);
}


/*
 * Refer to  function interface for description.
 */
void BSP_Board_SetOStoBSPDelayFunc(OStoBSPDelayFunc_T delayFunc)
{
	OStoBSPdelayFunc = delayFunc;
}

Retcode_T BSP_Board_EnableSysTick(void)
{
	Retcode_T rc = RETCODE_OK;
	uint32_t retval = 0U;

	retval = SysTick_Config(HSE_VALUE / 1000);
	if (0U != retval) /* Will return 0 is successful, or 1 upon failure */
	{
		rc = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_FAILURE);
	}
	return (rc);
}

void BSP_Board_DisableSysTick(void)
{
	/* Disable the Core SysTick*/
	SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk |
			(0 & SysTick_CTRL_TICKINT_Msk) |
			(0 & SysTick_CTRL_ENABLE_Msk));

	/* Clear Pending IRQ if applicable */
	NVIC_ClearPendingIRQ(SysTick_IRQn);
}

/*
 * Refer to  function interface for description.
 */
void BSP_Board_SoftReset(void)
{
	NVIC_SystemReset();
}

#if BCDS_BOARD_XDK110
/*----------------------------------------------------------------------------------------------------------------------------------------*/
/*
 * Refer to prototype for description
 */
static void Board_GPIOInterruptInit(void)
{
	NVIC_SetPriority(GPIO_ODD_IRQn, 5);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_SetPriority(GPIO_EVEN_IRQn, 5);
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
}

/*
 * Refer to prototype for description
 */
static Retcode_T Board_UART0_Init(void)
{
	USART_InitAsync_TypeDef BSP_UART0InitStruct = USART_INITASYNC_DEFAULT;

	CMU_ClockEnable(cmuClock_UART0, true);

	BSP_UART0InitStruct.enable = usartDisable; /* Don't enable UART upon intialization */
	BSP_UART0InitStruct.refFreq = 0; /* Provide information on reference frequency. When set to 0, the reference frequency is */
	BSP_UART0InitStruct.baudrate = 115200; /* Baud rate */
	BSP_UART0InitStruct.oversampling = usartOVS16; /* Oversampling. Range is 4x, 6x, 8x or 16x */
	BSP_UART0InitStruct.databits = usartDatabits8; /* Number of data bits. Range is 4 to 10 */
	BSP_UART0InitStruct.parity = usartNoParity; /* Parity mode */
	BSP_UART0InitStruct.stopbits = usartStopbits1; /* Number of stop bits. Range is 0 to 2 */

	/* Initialize USART with uartInit struct */
	USART_InitAsync(UART0, &BSP_UART0InitStruct);

	CMU_ClockEnable(cmuClock_UART0, false);
	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */
static Retcode_T Board_UART1_Init(void)
{
	USART_InitAsync_TypeDef BSP_UART1InitStruct = USART_INITASYNC_DEFAULT;

	CMU_ClockEnable(cmuClock_UART1, true);

	BSP_UART1InitStruct.enable = UART1_STATE; /* Don't enable UART upon intialization */
	BSP_UART1InitStruct.refFreq = UART1_REF_FREQ; /* Provide information on reference frequency. When set to 0, the reference frequency is */
	BSP_UART1InitStruct.baudrate = UART1_BAUDRATE; /* Baud rate */
	BSP_UART1InitStruct.oversampling = UART1_OVERSAMPLING; /* Oversampling. Range is 4x, 6x, 8x or 16x */
	BSP_UART1InitStruct.databits = UART1_DATABITS; /* Number of data bits. Range is 4 to 10 */
	BSP_UART1InitStruct.parity = UART1_PARITY; /* Parity mode */
	BSP_UART1InitStruct.stopbits = UART1_STOPBITS; /* Number of stop bits. Range is 0 to 2 */

	/* Initialize USART with uartInit struct */
	USART_InitAsync(UART1, &BSP_UART1InitStruct);

	CMU_ClockEnable(cmuClock_UART1, false);
	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */
static Retcode_T Board_USART0_Init(void)
{
	/* enable the clock to access the registers */
	CMU_ClockEnable(cmuClock_USART0, true);
	/* Setting baudrate */
	USART_BaudrateSyncSet(USART0_PORT_BASE, USART0_PERCLK_FREQUENCY, USART0_BAUDRATE);
	/* Configure SPI */
	/* Using synchronous (SPI) mode*/
	USART0_PORT_BASE->CTRL = USART_CTRL_SYNC;
	/* Enabling master mode SPI */
	USART0_PORT_BASE->CMD = USART_CMD_MASTEREN;
	/* Clearing old transfers/receptions, and disabling interrupts */
	USART0_PORT_BASE->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
	/* Set Most Significant Bit First */
	USART0_PORT_BASE->CTRL |= USART_CTRL_MSBF;
	/* Reset interrupt enable register */
	USART0_PORT_BASE->IEN = 0;
	/* Clear interrupt flags */
	USART0_PORT_BASE->IFC = _USART_IFC_MASK;
	/* Disable the clock*/
	CMU_ClockEnable(cmuClock_USART0, false);
	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */
static Retcode_T Board_USART1_Init(void)
{
	/* enable the clock to access the registers */
	CMU_ClockEnable(cmuClock_USART1, true);
	/* Setting baudrate */
	USART_BaudrateSyncSet(USART1_PORT_BASE, USART1_PERCLK_FREQUENCY, USART1_BAUDRATE);
	/* Configure SPI */
	/* Using synchronous (SPI) mode*/
	USART1_PORT_BASE->CTRL = USART_CTRL_SYNC;
	/* Clearing old transfers/receptions, and disabling interrupts */
	USART1_PORT_BASE->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
	USART1_PORT_BASE->IEN = 0;
	USART1_PORT_BASE->CTRL |= USART_CTRL_MSBF | USART_CTRL_TXBIL_HALFFULL;
	/* Disable the clock*/
	CMU_ClockEnable(cmuClock_USART1, false);
	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */
static void Board_GPIO_Init(void)
{
	/* enable the clock for the gpio ports.*/
	CMU_ClockEnable(cmuClock_GPIO, true);
	/*
	 * The nRESET pin of the CC3100MOD must be held low until the VBAT supply to the device is driven and stable.
	 */
	GPIO_PinModeSet(WIFI_NRESET_PORT, WIFI_NRESET_PIN, WIFI_NRESET_MODE, 0);
	/*
	 * Snooze Mode Enable Logic Input. Logic LOW disables the snooze Mode. Do not leave floating.
	 */
	GPIO_PinModeSet(PWR_2V5_SNOOZE_PORT, PWR_2V5_SNOOZE_PIN, PWR_2V5_SNOOZE_MODE, 0);
	/*
	 * 3V3 Enable Logic Input. Logic high enables the 3V3 output.
	 */
	GPIO_PinModeSet(PWR_3V3_EN_PORT, PWR_3V3_EN_PIN, PWR_3V3_EN_MODE, 0);
	/*
	 * Setting the alternative drive strength for the GPIO ports
	 */
	GPIO_DriveModeSet(gpioPortE, gpioDriveModeLow);
	GPIO_DriveModeSet(gpioPortD, gpioDriveModeHigh);
}
#endif /* BCDS_BOARD_XDK110 */

/*
 * Refer to prototype for description
 */
void BoardCommon_IncTick(void)
{
	BSP_Tick++;
}

/*
 * Refer to prototype for description
 */
uint32_t BoardCommon_GetTick(void)
{
	return BSP_Tick;
}

/*
 * Refer to prototype for description
 */
void BoardCommon_Delay(uint32_t delay)
{
	uint32_t tickstart = 0;
	tickstart = BoardCommon_GetTick();
	while ((BoardCommon_GetTick() - tickstart) < delay)
	{
	}
}

/*
 * Refer to prototype for description
 */
GPIOINT_IrqCallbackPtr_t BoardCommon_GPIOInterruptGetCallback(uint8_t pin)
{
	return gpioCallbacks[pin];
}

/*
 * Refer to prototype for description
 */
void BoardCommon_GPIOInterruptRegisterCallback(uint8_t pin, GPIOINT_IrqCallbackPtr_t callbackPtr)
{
	uint32_t tmp = INT_Disable();

	/* Dispatcher is used */
	gpioCallbacks[pin] = callbackPtr;

	tmp = INT_Enable();
	if (tmp)
	{
		__asm("nop");
	} // makes no sense just for lint
}

/*
 * Refer to prototype for description
 */
void BoardCommon_GPIOInterruptDispatch(uint32_t iflags)
{
	uint32_t irqIdx;
	GPIOINT_IrqCallbackPtr_t callback;

	/* check for all flags set in IF register */
	while (iflags)
	{
		irqIdx = __CLZ(__RBIT(iflags));

		/* clear flag*/
		iflags &= ~(1 << irqIdx);

		callback = gpioCallbacks[irqIdx];
		if (callback)
		{
			/* call user callback */
			callback();
		}
	}
}

/*
 * Refer to prototype for description
 */
void BoardCommon_WakeupPowerSupply2V5(Peripherals_T peripheral)
{
#if BCDS_BOARD_XDK110
	bool power2V5enabled = false;

	if (BSP_PowerSupply2V5.Register)
	{
		power2V5enabled = true;
	}
	PowerSupply2V5UpdateRegister(peripheral, 1);
	if (false == power2V5enabled)
	{
		GPIO_PinOutClear(PWR_2V5_SNOOZE_PORT, PWR_2V5_SNOOZE_PIN);
	}
	/*@todo. As this is called in interrupt context of SDCard detect calling board delay will cause hang in a while loop.
	 * need to find a better way to hanlde this . So as of now  commenting the board delay */
	//    Board_Delay(1);
#else
	BCDS_UNUSED(peripheral);
#endif
}

/*
 * Refer to prototype for description
 */
void BoardCommon_SnoozePowerSupply2V5(Peripherals_T peripheral)
{
#if BCDS_BOARD_XDK110
	bool power2V5Disabled = false;

	PowerSupply2V5UpdateRegister(peripheral, 0);
	if (0U == BSP_PowerSupply2V5.Register)
	{
		power2V5Disabled = true;
	}
	if (power2V5Disabled)
	{
		GPIO_PinOutSet(PWR_2V5_SNOOZE_PORT, PWR_2V5_SNOOZE_PIN);
	}
#else
	BCDS_UNUSED(peripheral);
#endif
}

/*
 * Refer to prototype for description
 */
void BoardCommon_DisablePowerSupply3V3(Peripherals_T peripheral)
{
#if BCDS_BOARD_XDK110

	bool power3V3Disabled = false;
	PowerSupply3V3UpdateRegister(peripheral, 0);
	if (0U == BSP_PowerSupply3V3.Register)
	{
		power3V3Disabled = true;
	}
	if (power3V3Disabled)
	{
		GPIO_PinOutClear(PWR_3V3_EN_PORT, PWR_3V3_EN_PIN);
	}
#else
	BCDS_UNUSED(peripheral);
#endif
}
/*
 * Refer to prototype for description
 */
void BoardCommon_EnablePowerSupply3V3(Peripherals_T peripheral)
{
#if BCDS_BOARD_XDK110
	bool power3V3enabled = false;

	if (BSP_PowerSupply3V3.Register)
	{
		power3V3enabled = true;
	}
	PowerSupply3V3UpdateRegister(peripheral, 1);
	if (false == power3V3enabled)
	{
		GPIO_PinOutSet(PWR_3V3_EN_PORT, PWR_3V3_EN_PIN);
	}
#else
	BCDS_UNUSED(peripheral);
#endif
}

#if BCDS_BOARD_XDK110
/*
 * Refer to interface for description
 */
Retcode_T Board_ADC_Init(void)
{
	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
	/* Init common settings for both single conversion and scan mode */
	init.timebase = ADC_TimebaseCalc(0);
	/* Might as well finish conversion as quickly as possibly since polling */
	/* for completion. */
	/* Set ADC clock to 7 MHz, use default HFPERCLK */
	init.prescale = ADC_PrescaleCalc(7000000, 0);

	/* WARMUPMODE must be set to Normal according to ref manual before */
	/* entering EM2. In this example, the warmup time is not a big problem */
	/* due to relatively infrequent polling. Leave at default NORMAL, */

	CMU_ClockEnable(cmuClock_ADC0, true);

	ADC_Init(ADC0, &init);

	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */
static Retcode_T Board_I2C0_Init(void)
{
	I2C_Init_TypeDef i2cInit =
	{ false, /* Enable when init done */
			true, /* Set to master mode */
			0, /* Use currently configured reference clock */
			I2C_FREQ_FAST_MAX, /* Set to fast+ rate assuring being */
			/* within I2C spec */
			i2cClockHLRStandard /* Set to use 4:4 low/high duty cycle */
	};

	CMU_ClockEnable(cmuClock_I2C0, true);

	I2C_Init(I2C0, &i2cInit);

	CMU_ClockEnable(cmuClock_I2C0, false);

	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */

static Retcode_T Board_I2C1_Init(void)
{
	I2C_Init_TypeDef i2cInit =
	{ false, /* Enable when init done */
			true, /* Set to master mode */
			0, /* Use currently configured reference clock */
			I2C_FREQ_FAST_MAX, /* Set to fast+ rate assuring being */
			/* within I2C spec */
			i2cClockHLRStandard /* Set to use 4:4 low/high duty cycle */
	};

	CMU_ClockEnable(cmuClock_I2C1, true);

	I2C_Init(I2C1, &i2cInit);

	CMU_ClockEnable(cmuClock_I2C1, false);

	return RETCODE_OK;
}

/*
 * Refer to prototype for description
 */

static Retcode_T Board_USB_Init(void)
{
	USB_lineCoding_t BSP_USBHandle;

	BSP_USBHandle.Baudrate = USB_SET_BAUDRATE;
	BSP_USBHandle.charFormat = USB_SET_STOPBITS_MODE;
	BSP_USBHandle.parityType = USB_SET_PARITY;
	BSP_USBHandle.dataBits = USB_SET_DATA_BITS;
	BSP_USBHandle.dummy = 0;
	BSP_USBHandle.usbRxCallback = NULL;

	NVIC_SetPriority(USB_IRQn, 5);
	USB_init(&BSP_USBHandle, NULL);
	return RETCODE_OK;
}
#endif /* BCDS_BOARD_XDK110 */

/* Refer to prototype for description */
static Retcode_T Board_RTC_Init(void)
{
	Retcode_T rc = RETCODE_OK;
	RTC_Init_TypeDef BSP_RtcInitStruct;

#if	BCDS_BOARD_XDK110 && BCDS_FEATURE_EFM32_RTC

	/* Prerequisites: LFA has been activated and connected to an oscillator */
	BSP_RtcInitStruct.enable = false; /* Do not enable on initialization */
	BSP_RtcInitStruct.debugRun = false; /* Do not run during debugging */
	BSP_RtcInitStruct.comp0Top = false; /* Do not use the ch0 compare match as top */

	/* Set Clock Divisor to 1 */
	CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_1);

	/* Enable RTC Clock*/
	CMU_ClockEnable(cmuClock_RTC, true);
	/* Enabling clock to the interface of the low energy modules */
	CMU_ClockEnable(cmuClock_CORELE, true);
	/* Register Initialization Struct */
	struct MCU_Timer_Handle_S * RtcTimerHandle = (struct MCU_Timer_Handle_S *)MCU_Timer_GetRtcHandle();

	/* Store init handle for RTC module */
	RtcTimerHandle->InitHandle.Rtc = &BSP_RtcInitStruct;

	/* Run dummy initialization */
	rc = MCU_Timer_Initialize((Timer_T)RtcTimerHandle, NULL);

	/* Clear Pending Interrupts */
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_EnableIRQ(RTC_IRQn);

#elif BCDS_BOARD_APLM && BCDS_FEATURE_EFM32_RTC

    /* Prerequisites: LFA has been activated and connected to an oscillator */
    BSP_RtcInitStruct.enable = true; /* Do not enable on initialization */
    BSP_RtcInitStruct.debugRun = false; /* Do not run during debugging */
    BSP_RtcInitStruct.comp0Top = false; /* Do not use the ch0 compare match as top */

    /* Set Clock Divisor to 1 */
    CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_2);

    /* Enable RTC Clock*/
    CMU_ClockEnable(cmuClock_RTC, true);
    /* Enabling clock to the interface of the low energy modules */
    CMU_ClockEnable(cmuClock_CORELE, true);
    /* Register Initialization Struct */
    struct MCU_Timer_Handle_S * RtcTimerHandle = (struct MCU_Timer_Handle_S *)MCU_Timer_GetRtcHandle();

    /* Store init handle for RTC module */
    RtcTimerHandle->InitHandle.Rtc = &BSP_RtcInitStruct;

    /* Run dummy initialization */
    rc = MCU_Timer_Initialize((Timer_T)RtcTimerHandle, NULL);

    /* Clear Pending Interrupts */
    NVIC_SetPriority(RTC_IRQn,5);
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);

    return (rc);

#else /* This feature has not been activated */
	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_DisableIRQ(RTC_IRQn);
#endif
	return (rc);
}

#if BCDS_BOARD_XDK110
/* Refer to prototype for description */
Retcode_T Board_BURTC_Init(void)
{
	Retcode_T rc = RETCODE_OK;
#if BCDS_FEATURE_EFM32_BURTC
	BURTC_Init_TypeDef BSP_BurtcInitStruct;
	BSP_BurtcInitStruct.enable = false; /* Do not enable on initialization */
	BSP_BurtcInitStruct.mode = burtcModeEM3;
	BSP_BurtcInitStruct.debugRun = false; /* Do not run while on debug */
	BSP_BurtcInitStruct.clkSel = burtcClkSelULFRCO;
	BSP_BurtcInitStruct.clkDiv = burtcClkDiv_2;
	BSP_BurtcInitStruct.lowPowerComp = 0;
	BSP_BurtcInitStruct.timeStamp = false;
	BSP_BurtcInitStruct.compare0Top = false;
	BSP_BurtcInitStruct.lowPowerMode = burtcLPDisable;

	/* Enable CORELE */
	CMU_ClockEnable(cmuClock_CORELE, true);

	/* Access the Reset Control module */
	RMU_ResetControl(rmuResetBU, false);

	/* Register Initialization Struct */
	struct MCU_Timer_Handle_S * BurtcTimerHandle = (struct MCU_Timer_Handle_S *)MCU_Timer_GetBurtcHandle();

	/* Register Initialization Struct */
	BurtcTimerHandle->InitHandle.Burtc = &BSP_BurtcInitStruct;

	/* Disable Pending Interrupt flags */
	BURTC_IntClear((BURTC_IFC_OF |BURTC_IFC_COMP0 | BURTC_IFC_LFXOFAIL));
	BURTC_IntDisable((BURTC_IF_OF | BURTC_IF_COMP0 | BURTC_IF_LFXOFAIL));

	rc = MCU_Timer_Initialize((Timer_T)BurtcTimerHandle, NULL);

	NVIC_ClearPendingIRQ(BURTC_IRQn);
	NVIC_EnableIRQ(BURTC_IRQn);
#else /* This feature has not been activated */
    NVIC_ClearPendingIRQ(BURTC_IRQn);
    NVIC_DisableIRQ(BURTC_IRQn);
#endif
	return (rc);
}
#endif /* BCDS_BOARD_XDK110 */

/*
 * Refer to interface header for description
 */
#if BCDS_BOARD_APLM
Retcode_T Board_USART2_Init(void)
{
    /* enable the clock to access the registers */
    CMU_ClockEnable(cmuClock_USART2, true);
    /* Setting baudrate */
    USART_BaudrateSyncSet(USART2_PORT_BASE, USART2_PERCLK_FREQUENCY, USART2_BAUDRATE);
    /* Configure SPI */
    /* Using synchronous (SPI) mode*/
    USART2_PORT_BASE->CTRL = USART_CTRL_SYNC;
    /* Enabling master mode SPI */
    USART2_PORT_BASE->CMD = USART_CMD_MASTEREN;
    /* Clearing old transfers/receptions, and disabling interrupts */
    USART2_PORT_BASE->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;

    /* Set Most Significant Bit First */
    USART2_PORT_BASE->CTRL |= USART_CTRL_MSBF;
    /* Reset interrupt enable register */
    USART2_PORT_BASE->IEN = 0;
    /* Clear interrupt flags */
    USART2_PORT_BASE->IFC = _USART_IFC_MASK;

    /* Disable the clock*/
    CMU_ClockEnable(cmuClock_USART2, false);

    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
static Retcode_T Board_LEUART1_Init(void)
{
	LEUART_Init_TypeDef BSP_LEUARTInitStruct;

	CMU_ClockEnable(BSP_SERIAL_LEUART_CLOCK, true);
	BSP_LEUARTInitStruct.enable = leuartDisable; /* Don't enable UART upon intialization */
	BSP_LEUARTInitStruct.refFreq = 0; /* Provide information on reference frequency. When set to 0, the reference frequency is */
	BSP_LEUARTInitStruct.baudrate = 9600; /* Baud rate */
	BSP_LEUARTInitStruct.parity = leuartNoParity; /* Parity mode */
	BSP_LEUARTInitStruct.stopbits = leuartStopbits1; /* Number of stop bits. Range is 0 to 2 */
	BSP_LEUARTInitStruct.databits = leuartDatabits8;
	/* Initialize USART with uartInit struct */
	LEUART_Init(BSP_SERIAL_LEUART_PORT, &BSP_LEUARTInitStruct);

	CMU_ClockEnable(BSP_SERIAL_LEUART_CLOCK, true);
	return RETCODE_OK;
}
#endif /* BCDS_BOARD_APLM */

#if BCDS_BOARD_XDK110
/* Enables the ETM port */
static void Board_ETM_Enable(void)
{
#if BCDS_FEATURE_TRACEPORT
	/* Enable the trace clock */
	Board_TraceClock_Enable();

	/* Set GPIO Configuration for TraceData[0..3] and TraceClk */
	GPIO_PinModeSet(ETM_TD0_PORT, ETM_TD0_PIN, gpioModePushPull, 0); /* TraceData[0] */
	GPIO_PinModeSet(ETM_TD1_PORT, ETM_TD1_PIN, gpioModePushPull, 0); /* TraceData[1] */
	GPIO_PinModeSet(ETM_TD2_PORT, ETM_TD2_PIN, gpioModePushPull, 0); /* TraceData[2] */
	GPIO_PinModeSet(ETM_TD3_PORT, ETM_TD3_PIN, gpioModePushPull, 0); /* TraceData[3] */
	GPIO_PinModeSet(ETM_TCLK_PORT, ETM_TCLK_PIN, gpioModePushPull, 0); /* TraceClk */

	/* Set ETM Location to Location 3 */
	GPIO->ROUTE = ((GPIO->ROUTE & ~_GPIO_ROUTE_ETMLOCATION_MASK) |
			GPIO_ROUTE_ETMLOCATION_LOC3);

	/* Enable ETM pins */
	GPIO->ROUTE = ((GPIO->ROUTE &
			~(_GPIO_ROUTE_TD0PEN_MASK | _GPIO_ROUTE_TD1PEN_MASK
					| _GPIO_ROUTE_TD2PEN_MASK | _GPIO_ROUTE_TD3PEN_MASK
					| _GPIO_ROUTE_TCLKPEN_MASK)) |
			(GPIO_ROUTE_TD0PEN | GPIO_ROUTE_TD1PEN |
					GPIO_ROUTE_TD2PEN | GPIO_ROUTE_TD3PEN |
					GPIO_ROUTE_TCLKPEN));
#endif /* BCDS_FEATURE_TRACEPORT */
}
#endif /* BCDS_BOARD_XDK110 */

#if BCDS_FEATURE_TRACEPORT
/* Disable the ETM port */
static void Board_ETM_Disable(void)
{
	/* Disable ETM pins using disable mask */
	GPIO->ROUTE = ((GPIO->ROUTE &
			~(_GPIO_ROUTE_TD0PEN_MASK | _GPIO_ROUTE_TD1PEN_MASK
					| _GPIO_ROUTE_TD2PEN_MASK | _GPIO_ROUTE_TD3PEN_MASK
					| _GPIO_ROUTE_TCLKPEN_MASK)));

	/* Disable pins */
	GPIO_PinModeSet(ETM_TD0_PORT, ETM_TD0_PIN, gpioModeDisabled, 0); /* TraceData[0] */
	GPIO_PinModeSet(ETM_TD1_PORT, ETM_TD1_PIN, gpioModeDisabled, 0); /* TraceData[1] */
	GPIO_PinModeSet(ETM_TD2_PORT, ETM_TD2_PIN, gpioModeDisabled, 0); /* TraceData[2] */
	GPIO_PinModeSet(ETM_TD3_PORT, ETM_TD3_PIN, gpioModeDisabled, 0); /* TraceData[3] */
	GPIO_PinModeSet(ETM_TCLK_PORT, ETM_TCLK_PIN, gpioModeDisabled, 0); /* TraceClk */
}

/* Disable the Serial Wire Viewer */
static void Board_SWO_Disable(void)
{
	GPIO_DbgSWOEnable(false);

	GPIO_PinModeSet(GPIO_SWO_PORT, GPIO_SWO_PIN, gpioModeDisabled, 0);
}

/* Enables the trace clock used by SWO and ETM */
static void Board_TraceClock_Enable(void)
{
	/* Start the AUXHFRCO oscillator */
	CMU_OscillatorEnable(cmuOsc_AUXHFRCO, true, false);

	/* Set the AUXHFRCO band to 28MHz*/
	CMU_AUXHFRCOBandSet(cmuAUXHFRCOBand_28MHz);

	/* Set the Debug Clock to use AUXHFRCO */
	CMU_ClockSelectSet(cmuClock_DBG, cmuSelect_AUXHFRCO);
}

#endif /* BCDS_FEATURE_TRACEPORT */

#if BCDS_BOARD_XDK110
/* Enable Serial Wire Viewer */
static void Board_SWO_Enable(void)
{
#if BCDS_FEATURE_TRACEPORT

    /* Enable the trace clock */
    Board_TraceClock_Enable();

    GPIO_DbgLocationSet(GPIO_ROUTE_SWLOCATION_LOC0);
    GPIO_DbgSWOEnable(true);

    GPIO_PinModeSet(GPIO_SWO_PORT, GPIO_SWO_PIN, gpioModePushPull, 0);
#endif /* BCDS_FEATURE_TRACEPORT */
}

/*
 * Refer to prototype for description
 */
void PowerSupply2V5UpdateRegister(Peripherals_T peripheral, uint32_t updateValue)
{
	switch (peripheral)
	{

	case MCU:
		BSP_PowerSupply2V5.Peripheral.MCU = updateValue;
		break;
	case MIC_AKU340:
		BSP_PowerSupply2V5.Peripheral.Mic_AKU340 = updateValue;
		break;
	case BT_EM9301:
		BSP_PowerSupply2V5.Peripheral.BT_EM9301 = updateValue;
		break;
	case WIFI_CC3100MOD:
		BSP_PowerSupply2V5.Peripheral.WiFi_CC3100MOD = updateValue;
		break;
	case SENSOR_BMA280:
		BSP_PowerSupply2V5.Peripheral.Sensor_BMA280 = updateValue;
		break;
	case SENSOR_BMG160:
		BSP_PowerSupply2V5.Peripheral.Sensor_BMG160 = updateValue;
		break;
	case SENSOR_BME280:
		BSP_PowerSupply2V5.Peripheral.Sensor_BME280 = updateValue;
		break;
	case SENSOR_BMI150:
		BSP_PowerSupply2V5.Peripheral.Sensor_BMI150 = updateValue;
		break;
	case SENSOR_MAX44009:
		BSP_PowerSupply2V5.Peripheral.Sensor_MAX44009 = updateValue;
		break;
	case SENSOR_BMI160:
		BSP_PowerSupply2V5.Peripheral.Sensor_BMI160 = updateValue;
		break;
	case EXTENSION_BOARD:
		BSP_PowerSupply2V5.Peripheral.ExtensionBoard = updateValue;
		break;
	case SD_CARD:
		BSP_PowerSupply2V5.Peripheral.SDCard = updateValue;
		break;
	case LED_ORANGE:
		BSP_PowerSupply2V5.Peripheral.LED_O = updateValue;
		break;
	case LED_YELLOW:
		BSP_PowerSupply2V5.Peripheral.LED_Y = updateValue;
		break;
	case LED_RED:
		BSP_PowerSupply2V5.Peripheral.LED_R = updateValue;
		break;
	case LED_ALL:
		BSP_PowerSupply2V5.Peripheral.LED_R = updateValue;
		BSP_PowerSupply2V5.Peripheral.LED_O = updateValue;
		BSP_PowerSupply2V5.Peripheral.LED_Y = updateValue;
		break;
	default:
		__asm("nop");
	}
}

/*
 * Refer to prototype for description
 */
void PowerSupply3V3UpdateRegister(Peripherals_T peripheral, uint32_t updateValue)
{
	switch (peripheral)
	{

	case MCU:
		BSP_PowerSupply3V3.Peripheral.MCU = updateValue;
		break;
	case SD_CARD:
		BSP_PowerSupply3V3.Peripheral.SDCard = updateValue;
		break;
	case EXTENSION_BOARD:
		BSP_PowerSupply3V3.Peripheral.ExtensionBoard = updateValue;
		break;
	default:
		__asm("nop");
	}
}
#endif /* BCDS_BOARD_XDK110 */

/**@}*/
