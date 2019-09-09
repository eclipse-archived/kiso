/**
 * Generated by Eclipse Mita 0.2.0.
 * @date 2019-04-24 17:25:19
 */


#include <Kiso_Basics.h>
#include <FreeRTOS.h>
#include <Kiso_Retcode.h>
#include <stdio.h>
#include <Kiso_CmdProcessor.h>
#include <timers.h>
#include "../../../../boards/CommonGateway/bsp/include/BSP_CommonGateway.h"
#include "application.h"
#include "SensorEnvironment.h"
#include "SensorAccelerometer.h"
#include "ConnectivityLEDLeds.h"
#include "PlatformCGW.h"
#include "MitaTime.h"
#include "MitaEvents.h"
#include "MitaExceptions.h"

static Retcode_T Mita_initialize(void* userParameter1, uint32_t userParameter2);
static Retcode_T Mita_goLive(void* userParameter1, uint32_t userParameter2);

CmdProcessor_T Mita_EventQueue;

/**
 * The priority of the event loop task. The default of 1 is just above the idle task
 * priority. Note that the platform may use tasks of higher prio, but XDK LIVE programs
 * are solely executed in this event loop context.
 */
#define TASK_PRIO_EVENT_LOOP         (UINT32_C(1))

/**
 * The stack size of the event loop task in 32-bit words. If you don't know what this
 * means, be careful when you change this value. More information can be found here:
 * http://www.freertos.org/FAQMem.html#StackSize
 */
#define TASK_STACK_SIZE_EVENT_LOOP   (UINT16_C(2000))

/**
 * The maximum number of events the event queue can hold. The default value should
 * be sufficient in most cases. If you need to handle a lot of events and have some
 * long running computations, you might want to increase this number if you find
 * events are missed.
 */
#define TASK_Q_LEN_EVENT_LOOP        (UINT32_C(10))

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
#ifndef NDEBUG /* valid only for debug builds */
/**
 * @brief This API is called when function enters an assert
 *
 * @param[in] line : line number where asserted
 * @param[in] file : file name which is asserted
 *
 */

void assertIndicationMapping(const unsigned long line, const unsigned char * const file)
{
    /* Switch on the LEDs */
    Retcode_T retcode = RETCODE_OK;

    retcode = BSP_LED_Switch(COMMONGATEWAY_LED_ALL,COMMONGATEWAY_LED_COMMAND_ON);

    printf("asserted at Filename %s , line no  %ld \n\r", file, line);

    if (retcode != RETCODE_OK)
    {
        printf("LED's ON failed during assert");
    }

}
#endif
Retcode_T systemStartup(void)
{
    Retcode_T returnValue = RETCODE_OK;
    uint32_t param1 = 0;
    void* param2 = NULL;

    /* Initialize the callbacks for the system tick */
    BSP_Board_OSTickInitialize(SysTickPreCallback, NULL);

#ifndef NDEBUG /* valid only for debug builds */
    if (RETCODE_OK == returnValue)
    {
        returnValue = Assert_Initialize(assertIndicationMapping);
    }
#endif
    /* First we initialize the board. */
    if (RETCODE_OK == returnValue)
    {
        returnValue = BSP_Board_Initialize(param1, param2);
    }

    return returnValue;
}
void ErrorHandler(Retcode_T error, bool isfromIsr)
{
    if (false == isfromIsr)
    {
        /** @TODO ERROR HANDLING SHOULD BE DONE FOR THE ERRORS RAISED FROM PLATFORM */
        uint32_t PackageID = Retcode_GetPackage(error);
        uint32_t ErrorCode = Retcode_GetCode(error);
        uint32_t ModuleID = Retcode_GetModuleId(error);
        Retcode_Severity_T SeverityCode = Retcode_GetSeverity(error);

        if (RETCODE_SEVERITY_FATAL == SeverityCode)
            printf("Fatal error from package %u , Error code: %u and module ID is :%u \r\n",(unsigned int) PackageID ,(unsigned int) ErrorCode, (unsigned int) ModuleID);

        if (RETCODE_SEVERITY_ERROR == SeverityCode)
            printf("Severe error from package %u , Error code: %u and module ID is :%u \r\n",(unsigned int) PackageID , (unsigned int) ErrorCode, (unsigned int) ModuleID);
    }
    else
    {
        BSP_LED_Switch(COMMONGATEWAY_LED_RED_ID, COMMONGATEWAY_LED_COMMAND_ON);
    }
}

int main(void)
{
	/* Mapping Default Error Handling function */
	Retcode_T returnValue = Retcode_initialize(ErrorHandler);
	if (RETCODE_OK == returnValue)
	{
	    returnValue = systemStartup();
	}
	if (RETCODE_OK == returnValue)
	{
		returnValue = CmdProcessor_Initialize(&Mita_EventQueue, (char *) "EventQueue", TASK_PRIO_EVENT_LOOP, TASK_STACK_SIZE_EVENT_LOOP, TASK_Q_LEN_EVENT_LOOP);
	}
	if (RETCODE_OK == returnValue)
	{
		returnValue = CmdProcessor_Enqueue(&Mita_EventQueue, Mita_initialize, NULL, UINT32_C(0));
	}
	if (RETCODE_OK == returnValue)
	{
	    returnValue = CmdProcessor_Enqueue(&Mita_EventQueue, Mita_goLive, NULL, UINT32_C(0));
	}
	if (RETCODE_OK != returnValue)
	{
	    printf("System Startup failed");
	    assert(false);
	}
	/* start scheduler */
	vTaskStartScheduler();
}

Retcode_T Mita_initialize(void* userParameter1, uint32_t userParameter2)
{
	KISO_UNUSED(userParameter1);
	KISO_UNUSED(userParameter2);
	Retcode_T exception = NO_EXCEPTION;
	

	// setup resources
	exception = PlatformCGW_Setup();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	
	exception = ConnectivityLEDLeds_Setup();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	
	exception = SensorAccelerometer_Setup();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	
	exception = SensorEnvironment_Setup();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	

	// setup time
	exception = SetupTime();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	return exception;
}

Retcode_T Mita_goLive(void* userParameter1, uint32_t userParameter2)
{
	KISO_UNUSED(userParameter1);
	KISO_UNUSED(userParameter2);
	Retcode_T exception = NO_EXCEPTION;
	

	exception = EnableTime();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	
	exception = PlatformCGW_Enable();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	exception = ConnectivityLEDLeds_Enable();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	exception = SensorAccelerometer_Enable();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	exception = SensorEnvironment_Enable();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	exception = initGlobalVariables_application();
	if(exception == NO_EXCEPTION)
	{
	}
	else
	{
		return exception;
	}
	return NO_EXCEPTION;
}

