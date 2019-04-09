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


/* module includes ********************************************************** */

/* system header files */
#include "stddef.h"
#include "stdint.h"
#include "stdbool.h"

/* additional interface header files */
#include "board.h"
#include "BCDS_Assert.h"

/* own header files */
#include "rtc-board.h"

/* constants & definitions ************************************************** */

#define RTC_FREQUENCY_BASE (32768U)
#define RTC_FREQUENCY_PRESCALER (cmuClkDiv_1)
#define RTC_MINIMIUM_TIMEOUT ( (2UL * 1000UL) / RTC_FREQUENCY_BASE )
#define RTC_TICKS_TO_MS(value) (((uint64_t)(value) * 1000ULL) / RTC_FREQUENCY_BASE)
#define RTC_MS_TO_TICKS(value) (((uint64_t)(value) * RTC_FREQUENCY_BASE + (1000U / 2U)) / 1000U)
#define RTC_COUNT_MAX (0x1000000UL)
#define RTC_IntMaskGet() (RTC->IEN)

/* local variables ********************************************************** */

static void* TimerEventDispatcherCB;
static uint32_t TimerTimeStampStart;
static TimerTime_t TimerSystemTime;

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

void RTC_IRQHandler(void)
{
	uint32_t flags = RTC_IntGet();
	if (flags & RTC_IF_OF)
	{
		RTC_IntClear(RTC_IFC_OF);
		TimerSystemTime += RTC_COUNT_MAX;
	}
	if (flags & RTC_IF_COMP0)
	{
		RTC_IntDisable(RTC_IEN_COMP0);
		RTC_IntClear(RTC_IFC_COMP0);
		TimerIrqHandler();
	}
}

/* global functions ********************************************************* */

/* The API description is in the header File */
void* TimerGetEventDispatcherCB(void)
{
	return TimerEventDispatcherCB;
}

/* The API description is in the header File */
void TimerRegisterEventDispatcher(void (*eventHandler)(void* event))
{
	TimerEventDispatcherCB = (void*) eventHandler;
}

/* The API description is in the header File */
void RtcInit(void)
{
	/* select and if needed enable LFXO for LFA clock */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	CMU_ClockDivSet(cmuClock_RTC, RTC_FREQUENCY_PRESCALER);

	/* enable RTC clock */
	CMU_ClockEnable(cmuClock_RTC, true);

	/* allow HF domain access to LE peripheral registers */
	CMU_ClockEnable(cmuClock_CORELE, true);

	RTC_IntDisable(RTC_IEN_COMP0 | RTC_IEN_COMP1 | RTC_IEN_OF);
	RTC_IntClear(RTC_IFC_COMP0 | RTC_IFC_COMP1 | RTC_IFC_OF);
	NVIC_EnableIRQ(RTC_IRQn);
	RTC_IntEnable(RTC_IEN_OF);

	{
		RTC_Init_TypeDef config = {
				.enable = true,
				.debugRun = false,
				.comp0Top = false
		};
		RTC_Init(&config);
	}
}

/* The API description is in the header File */
uint32_t RtcGetMinimumTimeout(void)
{
	return RTC_MINIMIUM_TIMEOUT;
}

/* The API description is in the header File */
void RtcSetTimeout(uint32_t val)
{
	/*
	 * Reading the RTC Count value is an atomic operation.
	 * Input argument is in microseconds.
	 * The RTC resolution is
	 * The API can be called when COMP0 is already set to fire on a previously
	 * configured timer object. The upper layer does not inform the module when
	 * the active timer object becomes obsolete before it could have fired.
	 */
	uint32_t count = RTC_CounterGet();
	uint32_t delay = (uint32_t) RTC_MS_TO_TICKS(val);

	RTC_IntClear(RTC_IFC_COMP0);

	/** delay issue delay < 0x1000000ULL */
	assert(delay < 0x1000000ULL);

	TimerTimeStampStart = count;
	RTC_CompareSet(0, (delay + count));
	RTC_IntEnable(RTC_IEN_COMP0);
}


/* The API description is in the header File */
void HAL_Delay(uint32_t delay)
{
	/*
	 * The implementation is polling based without support for sleep mode.
	 * The RTC resolution is
	 * COMP1 IRQ is not in use, but the HW IF flag is polled for as software
	 * compare checks are not robust due to possible overflow events.
	 */
	uint32_t count = RTC_CounterGet();

	delay = (uint32_t) RTC_MS_TO_TICKS(delay);

	/** delay issue delay < 0x1000000ULL */
	assert(delay < 0x1000000ULL);

	RTC_IntClear(RTC_IFC_COMP1);
	RTC_CompareSet(1, (delay + count));

	while ((RTC_IntGet() & RTC_IF_COMP1) == 0)
	{
		; /* wait for time delay to pass */
	}

	RTC_IntClear(RTC_IFC_COMP1);
}
/* The API description is in the header File */
TimerTime_t RtcGetElapsedAlarmTime(void)
{
	/*
	 * The function shall only be called in a thread safe context.
	 * The return value is in microseconds.
	 * The RTC resolution is
	 */
	uint32_t elapsedTime = RTC_CounterGet();

	if (TimerTimeStampStart <= elapsedTime)
	{
		elapsedTime -= TimerTimeStampStart;
	}
	else
	{
		elapsedTime += 0x1000000UL - TimerTimeStampStart;
	}

	return (RTC_TICKS_TO_MS(elapsedTime));
}
/* The API description is in the header File */
TimerTime_t RtcComputeElapsedTime( TimerTime_t eventInTime )
{
    uint32_t elapsedTime = 0;

    /*
     * The function shall only be called in a thread safe context.
     * The return value is in microseconds.
     * The RTC resolution is
     */
    elapsedTime = RtcGetTimerValue();

    if (eventInTime <= elapsedTime)
    {
        elapsedTime -= eventInTime;
    }
    else
    {
        elapsedTime += 0x1000000UL - eventInTime;
    }

    return (elapsedTime);
}


/* The API description is in the header File */
void RtcEnterLowPowerStopMode(void)
{
    /** @todo 2015-09-04, klt2bp: WFI would break multitasking in an RTOS based environment. */
	/*Code must be changed to handle the sleep mangement for the node */
    for (;;)
    {
        ; /* wait for watchdog reset */
    }
}

/* The API description is in the header File */
TimerTime_t RtcGetTimerValue(void)
{
    /*
     * Sample the RTC Count register as late in the computation process as possible.
     */
    TimerTime_t systemTime = RTC_TICKS_TO_MS(TimerSystemTime);
    systemTime += RTC_TICKS_TO_MS(RTC_CounterGet());

    return systemTime;
}

/* The API description is in the header File */
TimerTime_t RtcComputeFutureEventTime( TimerTime_t futureEventInTime )
{
    return( RtcGetTimerValue( ) + futureEventInTime );
}

/* The API description is in the header File */
TimerTime_t RtcGetAdjustedTimeoutValue( uint32_t timeout )
{
    /* This API provided to adjust any timing deviation by external interrupt wake up.
     * Since APLM uses RTC only for Lora timing, this adjustment is not required. So this
     * API return the same value */
    return  timeout;
}
