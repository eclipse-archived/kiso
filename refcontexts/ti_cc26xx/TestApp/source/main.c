/*******************************************************************************
 @file  main.c
 @brief main entry of the BLE stack sample application.

 Copyright 2013 - 2014 Texas Instruments Incorporated. All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License").  You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product.  Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED ``AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 NOTE :
 According to BCDS standards, the file has been modified.

 *******************************************************************************/

/* module includes ********************************************************** */
#include "BCDS_TestAppInfo.h"
#ifdef BCDS_MODULE_ID
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_IROLL_APP_MODULE_ID_MAIN
#endif

/* system header files */
/* additional interface header files */

#include "std.h"
#include "Error.h"
#include "System.h"
#include "Power.h"
#include  "BIOS.h"
#ifdef POWER_SAVING
#include "Hwi.h"
#include "Clock.h"
#include "Task.h"
#include "aon_rtc.h"
#include "aon_wuc.h"
#include "aux_wuc.h"
#endif
#include "sys_ctrl.h"
#include "ioc.h"
#include "icall.h"
#include "Board.h"
#include "bcomdef.h"
#include "peripheral.h"
#include "BCDS_BSP_Board.h"
#include "App.h"

/* Header files required to enable instruction fetch cache */
#include <ti/drivers/SPI.h>


#ifndef USE_DEFAULT_USER_CFG
#include "BleUserConfig.h"



/**
 * Important Note :
 * As Per the Reference manual icall_api.h interface Header should be added at the end.
 * */
#include "icall_ble_api.h"


/* BLE user defined configuration */
#ifdef ICALL_JT
icall_userCfg_t user0Cfg = BLE_USER_CFG;
#else
bleUserCfg_t user0Cfg = BLE_USER_CFG;
#endif

#endif /* USE_DEFAULT_USER_CFG */

/* functions */

/** Exception handler */
void exceptionHandler()
{
    volatile uint8_t i = 1;
    while (i)
    {
    }
}

extern PIN_Config BoardGpioInitTableTest[];
/** Main function */
Void main()
{
Retcode_T retcode = RETCODE_OK;
#ifdef BSP_SUPPORT
   retcode = BSP_Board_Initialize(NULL,NULL);
#else
    /*port pin configuration*/
   PIN_init(BoardGpioInitTable);
#endif

 if (RETCODE_OK != retcode)
 {
	 Retcode_RaiseError(retcode);
 }

#ifndef POWER_SAVING
    /* Set constraints for Standby, powerdown and idle mode */
    Power_setConstraint(PowerCC26XX_SB_DISALLOW);
    Power_setConstraint(PowerCC26XX_IDLE_PD_DISALLOW);
#endif /* POWER_SAVING*/
#ifdef ICALL_JT
    user0Cfg.appServiceInfo->timerTickPeriod = Clock_tickPeriod;
    user0Cfg.appServiceInfo->timerMaxMillisecond  = ICall_getMaxMSecs();
#endif /* POWER_SAVING*/

/* 	This macro is added to avoid including ICall related tasks for Integration tests that do not require BLE. This saves some space. 
	For eg, to run IT test cases in DRIVERS repo, BLE is not required. In that case, DISABLE_ICALL_TASK macro must to be added in TestSection.mk  */
#ifndef DISABLE_ICALL_TASK
    /* Initialize ICall module */
    ICall_init();

    /* Start tasks of external images - Priority 5 */
  	ICall_createRemoteTasks();

    /* Kick off profile - Priority 3 */
    GAPRole_createTask();
#endif /* DISABLE_ICALL_TASK*/
  
    /*Test app task - Priority 1 */
    TestApp_init();

    /* enable interrupts and start SYS/BIOS */
    BIOS_start();

}

/** Error handled to be hooked into TI-RTOS */
Void smallErrorHook(Error_Block *eb)
{
    for (;;)
        ;
}

/** HAL assert handler required by OSAL memory module */
void halAssertHandler(void)
{
    for (;;)
        ;
}
