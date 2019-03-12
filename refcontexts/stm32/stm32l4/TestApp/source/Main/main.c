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
 *
 * This is the Bike Sensor eBike integration test application.
 *
 * ****************************************************************************/

/* module includes ********************************************************** */

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_MAIN

/* system header files */
#include <stdint.h>
#include <stdbool.h>

/* additional interface header files */
#include "BCDS_Assert.h"
#include "BCDS_BSP_Board.h"
#include "stm32l4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "App.h"
#include "BCDS_HAL_CriticalSection.h"
/* The BCDS_MODULE_ID is defined inside BCDS_BSP_Board badly. Hence re-defining. */
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_MAIN
#include "BCDS_Logging.h"
#include "BCDS_MCU_Watchdog.h"

static_assert((0 != portTICK_RATE_MS), "Tick rate MS is zero")

/* own header files */

/* constant definitions ***************************************************** */

/* function prototypes ****************************************************** */

static void SysTickPreCallback(void);

extern void xPortSysTickHandler(void);
extern void vApplicationStackOverflowHook(TaskHandle_t xTask, int8_t *pcTaskName);

void vApplicationIdleHook(void);

void vApplicationStackOverflowHook(TaskHandle_t xTask, int8_t *pcTaskName)
{
    BCDS_UNUSED(xTask);
    BCDS_UNUSED(pcTaskName);
    /*TODO: we must log this error in persistent memory(external flash),
     * So that on next boot it can be sent via LW2M */

#ifndef NDEBUG
    /*Dont know behaviour of this in release build but in debug build it will stop
     * the execution here and watchdog will not trigger*/
    __ASM
    volatile("BKPT #01");
#endif
    /*Stop Indefinitely, till watch-dog reset */
    while (1)
        ;
}

/**
 * @brief   This function is the idle task called each time no other task are running.
 */
void vApplicationIdleHook(void)
{
#ifdef HAL_IWDG_MODULE_ENABLED
    (void) MCU_Watchdog_Feed();
#endif // HAL_IWDG_MODULE_ENABLED
}

/* local variables ********************************************************** */

/* global variables ********************************************************* */

#ifndef NDEBUG /* valid only for debug builds */
extern uint8_t gccWeekDefinitionLinkErrorFlag;
#endif /* NDEBUG */

uint32_t HAL_InterruptLockCount = 0UL;

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

#ifndef NDEBUG /* valid only for debug builds */

/**
 * @brief This function is registered as a callback to the Assert module.
 *
 * @param [in]	line : The line number of the source code from where the assert() is called.
 *
 * @param [in]	file : The file name of the source code from where the assert() is called.
 */
static void AssertCallback(const unsigned long line, const unsigned char const file[])
{
    uint32_t count = 0;
    /* Disable interrupts to avoid any interruption during the assert to freeze the system */
    HAL_CriticalSection_Enter(&count);

    /* @todo - Log/Trace the line and file information */
    BCDS_UNUSED(line);
    BCDS_UNUSED(file);
}
#endif /* NDEBUG */

/**
 * @brief This function is registered as a callback to the Retcode module.
 *
 * @param [in]	retcode : Error code defining the package ID, severity and Error information.
 */
static void ErrorCallback(Retcode_T retcode, bool isFromIsr)
{
    uint32_t package = Retcode_GetPackage(retcode);
    uint32_t severity = (uint32_t) Retcode_GetSeverity(retcode);
    uint32_t code = Retcode_GetCode(retcode);

    BCDS_UNUSED(package);
    BCDS_UNUSED(severity);
    BCDS_UNUSED(code);
    BCDS_UNUSED(isFromIsr);

}

/* global functions ********************************************************* */

/**
 * @brief This function is SysTick Handler.
 * This is called when ever the IRQ is hit.
 * This is a temporary implementation where
 * the SysTick_Handler() is not directly mapped
 * to xPortSysTickHandler(). Instead it is only
 * called if the scheduler has started.
 */
static void SysTickPreCallback(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

int main(void)
{
    /*
     * To suppress the warning
     *
     * @todo : 31 August 2015 : mta5cob -
     * Once the startup file is built as a achieve
     * and link order is changed such that UTILS
     * module has priority, this shall be removed.
     */
#ifndef NDEBUG /* valid only for debug builds */
    gccWeekDefinitionLinkErrorFlag = UINT8_C(1);
#endif /* NDEBUG */

    Retcode_T retcode = RETCODE_OK;

#ifndef NDEBUG /* valid only for debug builds */
    (void) Assert_Initialize(AssertCallback);
#endif /* NDEBUG */

    (void) Retcode_Initialize(ErrorCallback);

    /* Initialize the callbacks for the system tick */
    BSP_Board_OSTickInitialize(SysTickPreCallback, NULL);

	retcode = BSP_Board_Initialize(0UL, NULL);

    if (RETCODE_OK == retcode)
    {
        retcode = Logging_Init(Logging_AsyncRecorder, Logging_SwoAppender);
    }
    if (RETCODE_OK == retcode)
    {
        /* Module Id set to LOG_LEVEL_DEBUG to print all log level variants/types */
        LOG_LEVEL_SET_MODULE(BCDS_MODULE_ID, LOG_LEVEL_DEBUG);
        LOG_INFO("Log module added for debug messages");
    }

    if (RETCODE_OK == retcode)
    {
        retcode = App_Start();
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }

    vTaskStartScheduler();

    /* Infinite loop */
    for (;;)
        ;
}

/** ************************************************************************* */
