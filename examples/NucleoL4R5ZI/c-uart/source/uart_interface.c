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
 * @ingroup
 *
 * @defgroup
 * @{
 *
 * @brief UART Example
 *
 * @details Demo application demonstrates how to use UART from KISO.
 *
 **/

/* own header files */
#include "AppInfo.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID APP_MODULE_ID_UART



/* system header files */
#include <stdio.h>

/* additional interface header files */
#include "uart_interface.h"
#include "BSP_NucleoL4R5ZI.h"
#include "Kiso_CmdProcessor.h"
#include "Kiso_Assert.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "Kiso_Logging.h"
#include "stm32l4r5xx.h"
#include "Kiso_BSP_TestInterface.h"
#include "Kiso_BSP_LED.h"
#include "KISO_MCU_UART.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

#define UART_RX_SIZE				UINT32_C(1)
#define TIME_BETWEEN_UART_TX_MESGS	UINT32_C(1000)

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/

static void uart_interface(void* param1, uint32_t param2);
static void uartApphandler(UART_T uart, struct MCU_UART_Event_S event);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

CmdProcessor_T *AppCmdProcessor;		/* Application Command Processor Instance */
UART_T uartTesthandle;					/* UART instance */
xTimerHandle timerHandle10Sec;			/* Auto load timer instance*/

uint8_t testTxData[] = {"NUCLEOL4R5ZI UART TEST APP"};
uint8_t transactionComplete = FALSE;
uint8_t testRxData[UART_RX_SIZE] ={0};


/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

static void uartApphandler(UART_T uart, struct MCU_UART_Event_S eventField)
{
	KISO_UNUSED(uart);
	Retcode_T retcode = RETCODE_OK;

	if(1 == eventField.TxComplete)
	{
		retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_GREEN_ID, NUCLEOL4R5ZI_LED_COMMAND_TOGGLE);
	}
	else if(1 == eventField.RxComplete)
	{
		transactionComplete = TRUE;
		retcode = MCU_UART_Send(uartTesthandle, testRxData, sizeof(testRxData));
		retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_BLUE_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
	}
	else if(1 == eventField.RxError)
	{
		retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_RED_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
	}
	else if(1 == eventField.RxAborted)
	{
		retcode = BSP_LED_Switch(NUCLEOL4R5ZI_LED_RED_ID, NUCLEOL4R5ZI_LED_COMMAND_ON);
	}
	else
	{
		retcode = RETCODE(RETCODE_SEVERITY_INFO, RETCODE_INVALID_PARAM);
	}

	if(RETCODE_OK != retcode)
	{
		Retcode_RaiseError(retcode);
	}
}

static void vTimerCallback1oSecExpired(xTimerHandle pxTimer)
{
	Retcode_T retcode = RETCODE_OK;
	KISO_UNUSED(pxTimer);

	if(TRUE == transactionComplete)
	{
		xTimerStop( timerHandle10Sec, 10 );
	}
	else
	{
		retcode = MCU_UART_Send(uartTesthandle, testTxData, sizeof(testTxData));
	}

	if(RETCODE_OK != retcode)
	{
		Retcode_RaiseError(retcode);
	}
}

static void uart_interface(void* param1, uint32_t param2)
{
	KISO_UNUSED(param1);
	KISO_UNUSED(param2);

	Retcode_T retcode = RETCODE_OK;

	timerHandle10Sec = xTimerCreate("timer1Sec", pdMS_TO_TICKS(TIME_BETWEEN_UART_TX_MESGS), pdTRUE, (void*)0, vTimerCallback1oSecExpired);
	if (timerHandle10Sec==NULL)
	{
		retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
	}
	if (RETCODE_OK == retcode)
	{
		retcode = BSP_TestInterface_Connect();
	}
	if (RETCODE_OK == retcode)
	{
		retcode = BSP_TestInterface_Enable();
	}
	if (RETCODE_OK == retcode)
	{
		uartTesthandle = BSP_TestInterface_GetUARTHandle();
		retcode = MCU_UART_Initialize(uartTesthandle, uartApphandler);
	}

	xTimerStart(timerHandle10Sec, 0);

	if (RETCODE_OK == retcode)
	{
		retcode = MCU_UART_Receive(uartTesthandle, testRxData, sizeof(testRxData));
	}
	if(RETCODE_OK != retcode)
	{
		Retcode_RaiseError(retcode);
	}
}

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(void * CmdProcessorHandle, uint32_t param2)
{
	KISO_UNUSED(param2);

	AppCmdProcessor = (CmdProcessor_T *) CmdProcessorHandle;
	CmdProcessor_Enqueue((CmdProcessor_T*)AppCmdProcessor,uart_interface,(void *)AppCmdProcessor,0);
}
/**@} */

/*************************************************************************** */

