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

#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID 0

#include "BCDS_Tests.h"
#include "BCDS_Retcode.h"
#include "BCDS_CmdProcessor.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef efm32
#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#endif
#include "BSP_BoardType.h"
#include "BCDS_MCU_UART.h"
#include "BCDS_MCU_UART_Handle.h"

#include "TestSuite_UART.h"

#include <stdlib.h>

#define TEST_CASE_3_a_ID   1

#define UART_STARTUP_DELAY_MS                        UINT32_C(4)
#define UART_WAKEUP_DELAY_MS                         UINT32_C(3)
#define UART_RESET_PROCESSING_DELAY_MS               UINT32_C(5)
#define UART_RESET_PULLUP_DELAY_MS                   UINT32_C(2)
#define UART_TRANSMIT_DISABLE_TIMEOUT_MS             UINT32_C(2)

#define UART_UART_RX_IRQn                          UART1_RX_IRQn
#define UART_UART_RX_PRIORITY                      UINT32_C(5)
#define UART_UART_TX_IRQn                          UART1_TX_IRQn
#define UART_UART_TX_PRIORITY                      UINT32_C(6)

#define UART_SERIAL_PORT                          UART1

#define UART_IRQ_PIN                  (8)
#define UART_IRQ_PORT                 (gpioPortD)


#define UART_UART1_RX_PIN             (10)
#define UART_UART1_RX_PORT            (gpioPortB)

#define UART_UART1_TX_PIN             (9)
#define UART_UART1_TX_PORT            (gpioPortB)


static Retcode_T TestCase_3_a_setup(CCMsg_T* ccmsg);
static void TestCase_3_a_run(CCMsg_T* ccmsg);
static Retcode_T TestCase_3_a_teardown(CCMsg_T* ccmsg);

void UART_TEST_UART_RxISR(void);
void UART_TEST_UART_TxISR(void);
HWHandle_T BSP_UART_TEST_GetUARTHandle(void);
void BSP_UART_TEST_Disconnect(void);
Retcode_T BSP_UART_TEST_Disable(void);
Retcode_T BSP_UART_TEST_Enable(void);
void BSP_UART_TEST_Connect(void);

MCU_UART_Callback_T UARTCallback;
/*
 * enumeration of custom return codes
 */
enum
{
    RETCODE_BSP_UART_SERIAL_NOT_INITIALIZED = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_BSP_UART_DISABLED,
} BSP_UART_TEST_Retcode_E;

#ifdef efm32
struct MCU_UART_Handle_S BSP_UART_TEST_UART_Handle;

static UART_IRQ_Callback_T UART_TEST_UART_RxISRCallback;
static UART_IRQ_Callback_T UART_TEST_UART_TxISRCallback;
#endif /* efm32 */

MCU_UART_Callback_T UARTCallback;

static void UART_Test_Callback(UART_T uart, struct MCU_UART_Event_S event);

static uint32_t MsToTicks(uint32_t miliseconds);

static Retcode_T CreateBinarySemaphore(SemaphoreHandle_t* semaphoreToCreate);
static Retcode_T DeleteSemaphore(SemaphoreHandle_t* semaphoreToDelete);

#define UART_BUFFER_LEN                      5
#define DATA_TRANSFER_TIMEOUT_MS            UINT32_C(1000)

static UART_T BSP_UART_Handle = 0;
static xSemaphoreHandle UARTBusLock = 0;;


Retcode_T TestSuite_UART_initialize(uint8_t testSuiteId)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_RegisterTestSuite(testSuiteId, TestCase_3_a_setup, TestCase_3_a_teardown);

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_3_a_ID, NULL, TestCase_3_a_run, NULL);
    }
    return retcode;
}

static Retcode_T TestCase_3_a_setup(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T retcode = RETCODE_FAILURE;

    CreateBinarySemaphore(&UARTBusLock);

    BSP_UART_TEST_Connect();

    BSP_UART_Handle = BSP_UART_TEST_GetUARTHandle();
    UARTCallback = UART_Test_Callback;

    retcode = MCU_UART_Initialize((UART_T) BSP_UART_Handle, UART_Test_Callback);

    retcode = BSP_UART_TEST_Enable();

    return retcode;
}

static void TestCase_3_a_run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T retcode = RETCODE_OK;
    uint8_t data[UART_BUFFER_LEN];
    uint8_t data_in[UART_BUFFER_LEN] = { 0 };
    uint8_t count = 0;
    for (uint8_t i = 0; i < UART_BUFFER_LEN; i++)
    {
        data[i] = i;
    }

    retcode = MCU_UART_Send((UART_T) BSP_UART_Handle, data, UART_BUFFER_LEN);
    if (pdTRUE == xSemaphoreTake(UARTBusLock, (TickType_t) (MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
    {
        /* We were able to obtain the semaphore and can now access the
         shared resource. */
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        Tests_SendReport(retcode, "Failed to send Byte over I2C");
        return;
    }
    retcode = MCU_UART_Receive((UART_T) BSP_UART_Handle, data_in, UART_BUFFER_LEN);

    if (pdTRUE == xSemaphoreTake(UARTBusLock, (TickType_t) (MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
    {
        /* We were able to obtain the semaphore and can now access the
         shared resource. */
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        Tests_SendReport(retcode, "Failed to send Byte over I2C");
        return;
    }

    for (uint8_t i = 0; i < UART_BUFFER_LEN; i++)
    {
        if (data_in[i] == i)
        {
            count++;
        }
    }

    if (UART_BUFFER_LEN == count)
    {
        Tests_SendReport(retcode, "Success UART Byte send");
        return;
    }

    Tests_SendReport(retcode, "Failed to transmit Bytes over UART");
    return;
}

static Retcode_T TestCase_3_a_teardown(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Rc = RETCODE_OK;

    Rc = DeleteSemaphore(&UARTBusLock);
    if (RETCODE_OK != Rc)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    Rc = MCU_UART_Deinitialize((UART_T) BSP_UART_Handle);
    if (RETCODE_OK != Rc)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    Rc = BSP_UART_TEST_Disable();
    if (RETCODE_OK != Rc)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES);
    }

    BSP_UART_TEST_Disconnect();
    return Rc;
}
// ########################################################################################

void BSP_UART_TEST_Connect(void)
{
#ifdef efm32
    GPIO_PinModeSet(UART_UART1_RX_PORT, UART_UART1_RX_PIN, gpioModeInput, 0);
    GPIO_PinModeSet(UART_UART1_TX_PORT, UART_UART1_TX_PIN, gpioModePushPull, 1);

    CMU_ClockEnable(cmuClock_UART1, true);
    UART_SERIAL_PORT->ROUTE = UART_ROUTE_RXPEN | UART_ROUTE_TXPEN | UART_ROUTE_LOCATION_LOC2;
    CMU_ClockEnable(cmuClock_UART1, false);
#endif /* efm32 */
}

void BSP_UART_TEST_Disconnect(void)
{
#ifdef efm32
    GPIO_PinModeSet(UART_UART1_RX_PORT, UART_UART1_RX_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(UART_UART1_TX_PORT, UART_UART1_TX_PIN, gpioModeDisabled, 0);

    CMU_ClockEnable(cmuClock_UART1, true);
    UART_SERIAL_PORT->ROUTE = _UART_ROUTE_RESETVALUE;
    CMU_ClockEnable(cmuClock_UART1, false);
#endif /* efm32 */
}


/*
 * Refer to interface header for description
 */
Retcode_T BSP_UART_TEST_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;
#ifdef efm32
    /* Dont enable if the callback has not been registered */
    if (NULL == BSP_UART_TEST_UART_Handle.IrqRxCallback || NULL == BSP_UART_TEST_UART_Handle.IrqTxCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_UART_SERIAL_NOT_INITIALIZED);
    }
    /* Enable 2v5 power rail if not already enabled */
    BoardCommon_WakeupPowerSupply2V5(EXTENSION_BOARD);
    if (RETCODE_OK == retcode)
    {
        /*
         * Wait for the UART module to start up
         */
        BSP_Board_Delay(UART_STARTUP_DELAY_MS);
        /*
         * Enable the USART MCU resource
         */
        CMU_ClockEnable(cmuClock_UART1, true);
        /*
         * Clear Pending Interrupt flags
         */
        /* clear interrupt flags */
        UART_SERIAL_PORT->IFC = _USART_IF_MASK;
        /*
         * Enable receive interrupt flags
         */
        /* Enable the UART RX Not Empty Interrupt */
        UART_SERIAL_PORT->IEN = USART_IEN_RXDATAV | USART_IEN_RXFULL | USART_IEN_RXOF |  USART_IEN_FERR;
        /*
         * Associate the ISR callbacks
         */
        UART_TEST_UART_RxISRCallback = BSP_UART_TEST_UART_Handle.IrqRxCallback;
        UART_TEST_UART_TxISRCallback = BSP_UART_TEST_UART_Handle.IrqTxCallback;

        /*
         * Set the priority for the UART interface
         */
        NVIC_SetPriority(UART_UART_RX_IRQn, UART_UART_RX_PRIORITY);
        NVIC_SetPriority(UART_UART_TX_IRQn, UART_UART_TX_PRIORITY);
        /*
         * Clear Pending interrupt requests
         */
        NVIC_ClearPendingIRQ(UART_UART_RX_IRQn);
        NVIC_ClearPendingIRQ(UART_UART_TX_IRQn);
        /*
         * Enable the interrupt requests
         */
        NVIC_EnableIRQ(UART_UART_RX_IRQn);
        NVIC_EnableIRQ(UART_UART_TX_IRQn);
        /* Enable the UART */
        /*Enable transmitter and receiver*/
        UART_SERIAL_PORT->CMD = (USART_CMD_RXEN | USART_CMD_TXEN);
        /*
         * Check if the USART has been enabled
         */
        BSP_Board_Delay(1);
        if (((UART_SERIAL_PORT->STATUS & _USART_STATUS_RXENS_MASK) == 0) || ((UART_SERIAL_PORT->STATUS & _USART_STATUS_TXENS_MASK) == 0))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
        }
    }
#endif /* efm32 */
    return retcode;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_UART_TEST_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;
#ifdef efm32
    /* Disable USART interface */
    UART_SERIAL_PORT->CMD = USART_CMD_RXDIS|USART_CMD_TXDIS|USART_CMD_MASTERDIS;
    /*
     * Check if the USART has been disabled
     */
    if (((UART_SERIAL_PORT->STATUS & _USART_STATUS_RXENS_MASK) != 0) || ((UART_SERIAL_PORT->STATUS & _USART_STATUS_TXENS_MASK) != 0))
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }
    if (RETCODE_OK == retcode)
    {

        NVIC_DisableIRQ(UART_UART_RX_IRQn);
        NVIC_DisableIRQ(UART_UART_TX_IRQn);
        UART_TEST_UART_RxISRCallback = NULL;
        UART_TEST_UART_TxISRCallback = NULL;
        BoardCommon_SnoozePowerSupply2V5(EXTENSION_BOARD);
    }
#endif /* efm32 */
    return retcode;
}

/*
 * Refer to interface header for description
 */
HWHandle_T BSP_UART_TEST_GetUARTHandle(void)
{
#ifdef efm32
    /*
     * Initialize the Handle and return it back to the caller
     */
    BSP_UART_TEST_UART_Handle.Uart_ptr = UART1;
    BSP_UART_TEST_UART_Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    return (HWHandle_T) &BSP_UART_TEST_UART_Handle;
#else /* efm32 */
    return (HWHandle_T) NULL; // not supported
#endif /* efm32 */
}

/*
 * Refer to interface header for description
 */
void UART_TEST_UART_TxISR(void)
{
#ifdef efm32
    if (NULL != BSP_UART_TEST_UART_Handle.IrqTxCallback)
    {
        /* All UART interrupt processing is to be done at the MCU level */
        UART_TEST_UART_TxISRCallback((UART_T) &BSP_UART_TEST_UART_Handle);
    }
#endif /* efm32 */
}

/*
 * Refer to interface header for description
 */
void UART_TEST_UART_RxISR(void)
{
#ifdef efm32
    if (NULL != BSP_UART_TEST_UART_Handle.IrqRxCallback)
    {
        /* All UART interrupt processing is to be done at the MCU level */
        UART_TEST_UART_RxISRCallback((UART_T) &BSP_UART_TEST_UART_Handle);
    }
#endif /* efm32 */
}
// ####################################################################################################

static Retcode_T CreateBinarySemaphore(SemaphoreHandle_t* semaphoreToCreate)
{
    if (NULL != *semaphoreToCreate)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
    }
    else
    {
        *semaphoreToCreate = xSemaphoreCreateBinary();
        if (NULL == semaphoreToCreate)
        {
            return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
        }
        else
        {
            return RETCODE_OK;
        }
    }
}

static Retcode_T DeleteSemaphore(SemaphoreHandle_t* semaphoreToDelete)
{
    Retcode_T Rc = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_OUT_OF_RESOURCES);
    if (NULL != *semaphoreToDelete)
    {
        vSemaphoreDelete(*semaphoreToDelete);
        *semaphoreToDelete = NULL;
        Rc = RETCODE_OK;
    }
    return Rc;
}


static void UART_Test_Callback(UART_T uart, struct MCU_UART_Event_S event)
{
    BCDS_UNUSED(uart);
    Retcode_T Rc = RETCODE_OK;


    if (UINT8_C(1) == event.TxComplete)
    {




        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != UARTBusLock)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(UARTBusLock, &higherPriorityTaskWoken))
                {
                    portYIELD_FROM_ISR(higherPriorityTaskWoken);
                }
                else
                {
                    /* ignore... semaphore has already been given */
                }
            }
            else
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
            }
        }
    }

    if (UINT8_C(1) == event.RxComplete)
    {

        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != UARTBusLock)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(UARTBusLock, &higherPriorityTaskWoken))
                {
                    portYIELD_FROM_ISR(higherPriorityTaskWoken);
                }
                else
                {
                    /* ignore... semaphore has already been given */
                }

            }
            else
            {
                Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
            }
        }

    }

    if (UINT8_C(1) == event.TxError)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    }

    if (RETCODE_OK != Rc)
    {
        Retcode_RaiseErrorFromIsr(Rc);
    }
}

static uint32_t MsToTicks(uint32_t miliseconds)
{
    uint32_t frequency = HSE_VALUE;
    uint32_t ticks = miliseconds * (frequency / 1000);
    return ticks;
}
