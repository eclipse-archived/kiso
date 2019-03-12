/******************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 * SystemStartup is the module where the routine "Main" is defined and
 * Initializes the Application Specific clock,I/o Ports etc and Software routines
 *
 * ****************************************************************************/

/* module includes ********************************************************** */

#include "BCDS_CmdProcessor.h"
#include "BCDS_Retcode.h"
#include "BCDS_Tests.h"

#include "TestSection_A.h"

#include "WDG_watchdog_ih.h"
//#include "PTD_portDriver_ih.h"
#include "BCDS_PowerManager_ph.h"
#include "FreeRTOS.h"
#include "task.h"
//#include "PTD_portDriver_ih.h"

/* own header files */
#include "SystemStartupConfig.h"
#include "SystemStartup.h"

/* test whether we are using latest FreeRTOS version i.e. 8.0.1 */
#if (tskKERNEL_VERSION_MAJOR != 8)
# error " Please use Latest FreeRTOS Major version i.e. 8"
#elif (tskKERNEL_VERSION_MINOR !=0 )
# error " Please use Latest FreeRTOS Minor version 0"
#elif (tskKERNEL_VERSION_BUILD != 1)
# error " Please use Latest FreeRTOS version with BUILD version 1"
#endif

/* local variables ********************************************************** */

/** command Processor handle which holds the  commandprocessor name, Task and Queues Handles */
static CmdProcessor_T SytemStartup_cmdHandle;
static Retcode_T LastRaisedRetcode;
static uint32_t CounterRaisedRetcodes;

/* local functions ********************************************************** */
static void ErrorHandlingFunc(Retcode_T Error);
void vApplicationStackOverflowHook(void* *pxTask, signed char *pcTaskName);
void vApplicationIdleHook(void);

/**
 * @brief        System Error Handling routine
 *
 * @param [in]  Error: Error Information from Basic
 *
 */

static void ErrorHandlingFunc(Retcode_T Error)
{
    Retcode_Severity_T Severity;
    LastRaisedRetcode = Error;
    CounterRaisedRetcodes++;

    Severity = Retcode_GetSeverity(Error);
    if ((RETCODE_SEVERITY_FATAL == Severity) || (RETCODE_SEVERITY_ERROR == Severity))
    {
        while (1)
            ;
    }
    else
    {
        /* Ignore Info and warning messages */
    }
}
/**
 * @brief This function is called when is stack overflows and the program gets into infinite loop.
 *
 * @param[in]   Task handle
 * @param[in]   Task name.
 *
 */
void vApplicationStackOverflowHook(void* *pxTask, signed char *pcTaskName)
{
    BCDS_UNUSED(pxTask); /* To suppress "unused parameter" warning*/
    BCDS_UNUSED(pcTaskName);

    /* This function will be called if a task overflows its stack, if
     configCHECK_FOR_STACK_OVERFLOW != 0.  It might be that the function
     parameters have been corrupted, depending on the severity of the stack
     overflow.  When this is the case pxCurrentTCB can be inspected in the
     debugger to find the offending task. */
    for (;;)
    {
        ;
    }

}

/**
 * @brief  Application idle hook function will only get executed when there are no tasks of higher
 *  priority that are able to run.
 */
void vApplicationIdleHook(void)
{
    /* feed the Watch dog*/
    WDG_feedingWatchdog();
}

/** Api decription is in header file */
static void systemClockInit(void)
{

    CMU_HFRCOBandSet(cmuHFRCOBand_28MHz);

    /* Using HFRCO at 14MHz as high frequency clock, HFCLK */
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);

    /* Enable LFRCO oscillator */
    CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

    /* Select LFRCO as clock source for LFBCLK */
    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFRCO);

}

/**
 * @brief        Initialization of system
 *
 * @param [in]  *data: a pointer to any context data structure which will be passed to the function when it is invoked
 *
 * @param [in]   value: a 32 bit value  which will be passed to the function when it is invoked
 */
static void systemInit(void *data, uint32_t value)
{
    BCDS_UNUSED(data);
    BCDS_UNUSED(value);

    PTD_portInteruptConfig();

    Retcode_T retcode = TestSection_A_Initialize();

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }
}

/* global functions ********************************************************* */

extern void HwInit(void)
{
    /* Chip errata */
    CHIP_Init();

    /* set life-hold signal */
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(POWER_CHARGER_PORT, POWER_CHARGER_PIN, POWER_CHARGER_ENABLE, POWER_CHARGER_DOUT_ENABLE);
}

/**
 * @brief  Main function
 *  Here, the initialization of various modules are done, such as chip init, enabling the clocks,
 *  port driver init, configuring the interrupts and so on.
 */
int main(void)
{
    systemClockInit();

    PTD_portInit();

    /* Power manager is initialized  for the initial start of the system */

    /* Enable Battery Charger Controller Pin */
    PTD_pinModeSet(POWER_CHARGER_PORT, POWER_CHARGER_PIN,
    POWER_CHARGER_ENABLE,
    POWER_CHARGER_DOUT_ENABLE);

    /* Disable Battery Charge */
    PTD_pinModeSet(POWER_BATTERY_PORT, POWER_BATTERY_PIN,
    POWER_BATTERY_ENABLE,
    POWER_BATTERY_DOUT_ENABLE);

    /* System reset disable */
    PTD_pinModeSet(POWER_RESET_PORT,
    POWER_RESET_PIN,
    POWER_RESET_ENABLE,
    POWER_RESET_DOUT_DISABLE);

    LastRaisedRetcode = RETCODE_OK;
    CounterRaisedRetcodes = 0;

    Retcode_Initialize(&ErrorHandlingFunc);

    /* Set the number of odd pins and even pins to be configured.
     * These pins are allocated memory dynamically.
     * Needs to be in sync with other modules which use GPIO
     */
    PTD_setInterruptPinCount(SYSTEMSTARTUP_EVEN_GPIO_COUNT,
    SYSTEMSTARTUP_ODD_GPIO_COUNT);

    Retcode_T cmdProcessorRetValue =
            CmdProcessor_Initialize(&SytemStartup_cmdHandle, "Cosp Application", SYSTEMSTARTUP_CMD_TASK_PRI, SYSTEMSTARTUP_CMD_TASK_STACK, SYSTEMSTARTUP_TASK_QUEUE_LEN);

    if (RETCODE_OK == cmdProcessorRetValue)
    {
        cmdProcessorRetValue = CmdProcessor_Enqueue(&SytemStartup_cmdHandle, &systemInit, NULL, 0);

        if (RETCODE_OK != cmdProcessorRetValue)
        {
            assert(0);
        }
    }
    else
    {
        assert(0);
    }

    /* Start FreeRTOS Scheduler */
    vTaskStartScheduler();

    /* vTaskStartScheduler() never returns. FreeRTOS destroys the system stack context inside the function so it is not possible to return from it. */

    return (-1);
}
/** ************************************************************************* */
