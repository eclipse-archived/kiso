/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
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

#include "BCDS_Utils.h"

#ifdef BCDS_MODULE_ID
#undef BCDS_MODULE_ID
#endif
#define BCDS_MODULE_ID BCDS_UTILS_MODULE_ID_SLEEPCONTROL

#include "BCDS_SleepControl.h"

#if BCDS_FEATURE_SLEEPCONTROL

/* Include the SysTick */
#include "BCDS_BSP_Board.h"

/* FreeRTOS Configuration and information about task scheduler */
#include "FreeRTOS.h"
#include "task.h"

/* Detect if FreeRTOS is configured for Tickless idle */
#if( configUSE_TICKLESS_IDLE != 0 )
#define BCDS_FREERTOS_TICKLESS_IDLE 1
#else
#define BCDS_FREERTOS_TICKLESS_IDLE 0
#endif

/**
 * Default SleepControl Callback
 *
 * @param[in] timer : Timer context
 * @param[in] event : Event extracted by ISR handler
 *
 */
static void SleepControl_Callback(Timer_T timer, struct MCU_Timer_Event_S event);

/**
 * Calculate the Wakeup Timer Sleep Counts
 *
 * @param[in] sleepTimeMs : Amount of time to sleep in OS ticks
 * @param[in] timerFrequency : Clock frequency of timer in Hz
 * @param[in] maxCounts : Maximum amount of counts supported by the timer
 *
 * @returns Number of counts of sleep time
 */
static BCDS_INLINE uint32_t CalculateSleepCounts(uint32_t sleepTimeInTicks,
        uint32_t timerFrequency, uint32_t maxCounts);

#if( BCDS_FREERTOS_TICKLESS_IDLE )
/**
 * Calculates elapsed time in ms
 *
 * @param[in] elapsedCount : Number of count ticks from Wakeup Timer
 * @param[in] timerFrequency : Timer clock frequency in Hz
 *
 * @param[out] Elapsed Time in OS ticks
 */
static BCDS_INLINE uint32_t CalculateElapsedTimeInTicks(uint32_t elapsedCount,
        uint32_t timerFrequency);

/**
 * Get and translate the lowest sleep mode allowed by the RTOS
 *
 * @returns rtosSleepMode as uint32_t
 */
static BCDS_INLINE uint32_t GetLowestRtosSleepMode(void);

/**
 * Advance the RTOS tick for the amount of time spend
 * inside the SleepControl_SleepTickless function
 *
 * @param[in] elapsedTimeInTicks : Time elapsed since the wakeup timer
 *      was activated
 */
static BCDS_INLINE void AdvanceRtosTick(uint32_t elapsedTimeInTicks);

#endif /* BCDS_FREERTOS_TICKLESS_IDLE */

/* PreSleep Callback */
static SleepControl_Hook_T myPreSleepHook = NULL;

/* Post Sleep Callback */
static SleepControl_Hook_T myPostSleepHook = NULL;

/* Wakeup Timer to use */
static Timer_T myWakeupTimer = NULL;

/* Initialize Sleep Control */
Retcode_T SleepControl_Initialize(SleepControl_Hook_T preSleepHook,
        SleepControl_Hook_T postSleepHook, Timer_T wakeupTimer)
{
    Retcode_T rc = RETCODE_OK;
    /* Sleep hooks may be NULL, but this is not recomended */
    myPreSleepHook = preSleepHook;
    myPostSleepHook = postSleepHook;

    if (NULL != wakeupTimer)
    {
        myWakeupTimer = wakeupTimer;

        /* Initialize the Wakeup Timer */
        rc = MCU_Timer_Initialize(myWakeupTimer, SleepControl_Callback);
    }
    else
    {
        rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Sleep_Initialize();
    }

    return rc;
}

/* Go to sleep into the tickless mode */
Retcode_T SleepControl_SleepTickless(uint32_t sleepTimeInTicks)
{
    Retcode_T rc = RETCODE_OK;
    uint32_t timerFrequency = UINT32_C(0); /* Clock Frequency from timer */
    uint32_t sleepMode = UINT32_C(0); /* Sleep mode */
    uint32_t sleepCounts = UINT32_C(0); /* Amount of counts to sleep */
    uint32_t elapsedCount = UINT32_C(0); /* Elapsed counts since the wakeup timer started */

    uint32_t maxCount = UINT32_C(0); /* Maximum wakeup timer counts */

#if(BCDS_FREERTOS_TICKLESS_IDLE)
    uint32_t elapsedTimeInTicks = UINT32_C(0); /* Elapsed time in ms */
    uint32_t rtosSleepMode = UINT32_C(0); /* RTOS Sleep suggestion */
#endif

    /* Get maximum number of timer counts */
    rc = MCU_Timer_GetMaxCount(myWakeupTimer, &maxCount);

    /* Get Timer Clock Frequency */
    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_GetClockFrequency(myWakeupTimer, &timerFrequency);
    }

    /* Calculate Sleep Counts with given information */
    if (RETCODE_OK == rc)
    {
        sleepCounts = CalculateSleepCounts(sleepTimeInTicks, timerFrequency, maxCount);
    }

    /* Suspend the SysTick */
    BSP_Board_DisableSysTick();

    /* Set compare match value and enable alarm */
    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_SetCompareValue(myWakeupTimer, 0, sleepCounts);
    }

    if (RETCODE_OK == rc)
    {
        rc = MCU_Timer_Start(myWakeupTimer);
    }

    /* Get lowest sleep mode allowed by the application */
    if (RETCODE_OK == rc)
    {
        /* Get lowest sleep mode from HAL MCU */
        sleepMode = MCU_Sleep_GetLowestSleepMode();

#if(BCDS_FREERTOS_TICKLESS_IDLE)
        /* Get lowest sleep mode from RTOS */
        rtosSleepMode = GetLowestRtosSleepMode();

        /* Negotiation. The lowest sleep mode is set */
        if (rtosSleepMode < sleepMode)
        {
            sleepMode = rtosSleepMode;
        }
#endif

        if (UINT32_C(0) == sleepMode)
        { /* No sleep is possible  */
            rc = RETCODE(RETCODE_SEVERITY_INFO,
                    RETCODE_SLEEPCONTROL_NOSLEEP);
        }
    }

    /* Run Pre-Sleep Hook */
    if (RETCODE_OK == rc)
    {
        if (NULL != myPreSleepHook)
        {
            rc = myPreSleepHook(sleepMode);
        }
    }

    /* Actual MCU Sleep */
    if (RETCODE_OK == rc)
    {
        MCU_Sleep_EnterSleep(sleepMode);
    }

    /* Run Post-Sleep Hook */
    if (RETCODE_OK == rc)
    {
        if (NULL != myPostSleepHook)
        {
            rc = myPostSleepHook(sleepMode);
        }
    }

    /* Get count value and disable the wakeup timer */
    if ((RETCODE_OK == rc) ||
            (RETCODE(RETCODE_SEVERITY_INFO, RETCODE_SLEEPCONTROL_NOSLEEP) == rc))
    {
        (void) MCU_Timer_GetCountValue(myWakeupTimer, &elapsedCount);
        (void) MCU_Timer_Stop(myWakeupTimer);

#if(BCDS_FREERTOS_TICKLESS_IDLE)
        /* Convert Timer Counts to OS Ticks */
        elapsedTimeInTicks = CalculateElapsedTimeInTicks(elapsedCount, timerFrequency);
#endif
    }

    /* Leaves the sleep section and re-enable the SysTick */
    (void) BSP_Board_EnableSysTick();

#if(BCDS_FREERTOS_TICKLESS_IDLE)
    AdvanceRtosTick(elapsedTimeInTicks);
#endif

    return (rc);
}

static BCDS_INLINE uint32_t CalculateSleepCounts(uint32_t sleepTimeInTicks,
        uint32_t timerFrequency, uint32_t maxCounts)
{
    uint32_t sleepCounts = UINT32_C(0);

    /* Step 1: Calculate max Sleep Time before the timer saturates */
    uint32_t maxSleepTimeInMs = configTICK_RATE_HZ * (maxCounts / timerFrequency); /*lint !e414 timerFrequency will never be 0,
     unless the system is broken.
     Hand over to hardfault then */

    if (sleepTimeInTicks >= maxSleepTimeInMs)
    {
        sleepCounts = maxCounts; /* Sleep Counts Saturates */
    }
    else if (sleepTimeInTicks >= configTICK_RATE_HZ)
    {
        sleepCounts = (sleepTimeInTicks / configTICK_RATE_HZ) * timerFrequency; /* Sleep Time > 1000 */
    }
    else if (timerFrequency >= configTICK_RATE_HZ)
    {
        sleepCounts = sleepTimeInTicks * (timerFrequency / configTICK_RATE_HZ); /* timerFrequency > 1000 */
    }
    else
    {
        sleepCounts = (sleepTimeInTicks * timerFrequency) / configTICK_RATE_HZ;
    }

    return sleepCounts;
}

#if(BCDS_FREERTOS_TICKLESS_IDLE)
static BCDS_INLINE uint32_t CalculateElapsedTimeInTicks(uint32_t elapsedCount,
        uint32_t timerFrequency)
{
    uint32_t elapsedTimeInTicks = UINT32_C(0);

    /* Convert Wakeup Timer Count to ms */
    if (elapsedCount < ((UINT32_MAX)/(configTICK_RATE_HZ)))
    {
        elapsedTimeInTicks = (configTICK_RATE_HZ * elapsedCount) / timerFrequency;
    }
    else
    { /* Case to avoid overflowing */
        elapsedTimeInTicks = (elapsedCount / timerFrequency) * configTICK_RATE_HZ; /*lint !e414 timerFrequency will never be 0, unless the system is broken. Hand over to hardfault then */
    }

    return elapsedTimeInTicks;
}

static BCDS_INLINE uint32_t GetLowestRtosSleepMode(void)
{ /* This probably belongs into the BSP */
    uint32_t rtosSleepMode = UINT32_C(0);

    /* Confirm FreeRTOS Sleep Mode Status */
    eSleepModeStatus osSleepModeStatus = eTaskConfirmSleepModeStatus();

    switch(osSleepModeStatus)
    {
        case eAbortSleep:
        rtosSleepMode = UINT32_C(0);
        break;
        case eStandardSleep:
        rtosSleepMode = UINT32_C(3);
        break;
        case eNoTasksWaitingTimeout:
        rtosSleepMode = UINT32_C(4);
        break;
        default:
        rtosSleepMode = UINT32_C(0);
        break;
    }

    return rtosSleepMode;
}

static BCDS_INLINE void AdvanceRtosTick(uint32_t elapsedTimeInTicks)
{
    /* Advance the FreeRTOS tick count */
    if (taskSCHEDULER_NOT_STARTED != xTaskGetSchedulerState())
    {
        vTaskStepTick(elapsedTimeInTicks);
    }
}

#endif /* if(BCDS_FREERTOS_TICKLESS_IDLE) */

/* Sleep Timer Diagnostics */
/*lint -esym(551,compareMatchCount) is a diagnostic variable*/
/*lint -esym(551,overflowCount) is a diagnostic variable*/
/*lint -esym(551,timerFailure) is a diagnostic variable*/
static uint32_t compareMatchCount = UINT32_C(0);
static uint32_t overflowCount = UINT32_C(0);
static uint32_t timerFailure = UINT32_C(0);

/* Default SleepControl Callback */
static void SleepControl_Callback(Timer_T timer, struct MCU_Timer_Event_S event)
{
    Retcode_T rc = RETCODE_OK;

    if (timer != myWakeupTimer)
    {
        /* Not the expected timer */
        rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNEXPECTED_BEHAVIOR);
    }

    if (true == event.CompareMatch)
    {
        compareMatchCount++;
    }

    if (true == event.Overflow)
    {
        overflowCount++;
    }

    if (event.ErrorCode)
    {
        timerFailure++;
    }

    if (RETCODE_OK != rc)
    {
        Retcode_RaiseError(rc);
    }
}

#endif /* if BCDS_FEATURE_SLEEPCONTROL */
