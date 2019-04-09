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

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_MAIN

/* system header files */
#include <stdint.h>
#include <stdbool.h>

/* additional interface header files */
#include "BCDS_Assert.h"
#include "BCDS_BSP_Board.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "App.h"

static_assert((0 != portTICK_RATE_MS), "Tick rate MS is zero")

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

/* local functions ********************************************************** */

#ifndef NDEBUG /* valid only for debug builds */
/**
 * @brief This function is registered as a callback to the Assert module.
 *
 * @param [in]  line : The line number of the source code from where the assert() is called.
 *
 * @param [in]  file : The file name of the source code from where the assert() is called.
 */
static void AssertCallback(const unsigned long line, const unsigned char const file[])
{
    BCDS_UNUSED(line);
    BCDS_UNUSED(file);
}
#endif /* NDEBUG */

/**
 * @brief This function is registered as a callback to the Retcode module.
 *
 * @param [in]  retcode : Error code defining the package ID, severity and Error information.
 */
static void ErrorCallback(Retcode_T retcode, bool isFromIsr)
{
    BCDS_UNUSED(retcode);
    BCDS_UNUSED(isFromIsr);
}
/**
 * @brief  Application idle hook function will only get executed when there are no tasks of higher
 *  priority that are able to run.
 */
void vApplicationIdleHook(void)
{
    ;
}

int main(void)
{

    Retcode_T retcode = RETCODE_OK;
    uint32_t DefaultConf = 1;

#ifndef NDEBUG /* valid only for debug builds */
    (void) Assert_Initialize(AssertCallback);
#endif /* NDEBUG */

    retcode = Retcode_Initialize(ErrorCallback);
    if (RETCODE_OK == retcode)
    {
        retcode = BSP_Board_Initialize(DefaultConf, NULL);
    }
    if (RETCODE_OK == retcode)
    {
        retcode = App_Start();
    }

    if (RETCODE_OK != retcode)
    {
        Retcode_RaiseError(retcode);
    }

    /* Infinite loop */
    /* We never will reach this point */
    for (;;)
        ;
}
