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
#define BCDS_MODULE_ID 0

#include "BCDS_Tests.h"
#include "BCDS_Retcode.h"
#include "BCDS_CmdProcessor.h"

#include "FreeRTOS.h"
#include "semphr.h"

#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#include "BSP_BoardType.h"
#include "BCDS_MCU_SPI.h"
#include "BCDS_MCU_SPI_Handle.h"

#include "TestSuite_SPI.h"

#include <stdlib.h>

#define TEST_CASE_2_a_ID   1


#define SPI_PORT                                                     USART2
#define SPI_CLOCK                                                       (cmuClock_USART2)
#define SPI_PERCLK_FREQUENCY                                            HSE_VALUE
#define SPI_BAUDRATE                                                    UINT32_C(1000000)

#define SPI_RX_IRQN                                                         USART2_RX_IRQn
#define SPI_RX_INTERRUPT_PRIORITY                                           UINT32_C(5)

#define SPI_TX_IRQN                                                         USART2_TX_IRQn
#define SPI_TX_INTERRUPT_PRIORITY                                           UINT32_C(6)

static Retcode_T TestCase_2_a_setup(CCMsg_T* ccmsg);
static void TestCase_2_a_run(CCMsg_T* ccmsg);
static Retcode_T TestCase_2_a_teardown(CCMsg_T* ccmsg);

Retcode_T BSP_SPITest_SetCSLow(void);
Retcode_T BSP_SPITest_SetCSHigh(void);

/*
 * enumeration of custom return codes
 */
enum
{
    RETCODE_BSP_SPI_TEST_SERIAL_NOT_INITIALIZED = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_BSP_SPI_TEST_DISABLED,
    RETCODE_BSP_SPI_TEST_ENABLE_NOT_COMPLETED,
    RETCODE_BSP_SPI_TEST_DETECT_LINE_BUSY,
    RETCODE_BSP_SPI_TEST_NOT_INSERTED,
    RETCODE_BSP_SPI_TEST_CANNOT_BE_ENABLED,
    RETCODE_BSP_SPI_TEST_SPI_ERROR,
};

struct MCU_SPI_Handle_S BSP_SPITest_SPI_Handle;

static SPI_IRQ_Callback_T SPITest_SPI_RxISRCallback;

static SPI_IRQ_Callback_T SPITest_SPI_TxISRCallback;

MCU_SPI_Callback_T SPICallback;

Retcode_T Board_USART2_SPI_Init(void);

static void BSP_SPITest_Connect(void);
static void BSP_SPITest_Disconnect(void);

static void SPITestEnable(void);
static void SPITestDisable(void);
static Retcode_T BSP_SPITest_Enable(void);
static void SPI_Test_Callback(SPI_T spi, struct MCU_SPI_Event_S event);
static uint32_t MsToTicks(uint32_t miliseconds);

static HWHandle_T BSP_SPITest_GetSPIHandle(void);

static Retcode_T CreateBinarySemaphore(SemaphoreHandle_t* semaphoreToCreate);
static Retcode_T DeleteSemaphore(SemaphoreHandle_t* semaphoreToDelete);

#define SPI_BUFFER_LEN                      5
#define DATA_TRANSFER_TIMEOUT_MS            UINT32_C(1000)

static SPI_T BSP_SPI_Handle = 0;
static xSemaphoreHandle SPIBusLock;

uint8_t transfer_count = 0;
uint8_t receive_count = 0;


//static bool SPITestDetectInterruptEnaled;
static bool SPITestEnabled = false;

Retcode_T TestSuite_SPI_initialize(uint8_t testSuiteId)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_RegisterTestSuite(testSuiteId, TestCase_2_a_setup, TestCase_2_a_teardown);

    if (RETCODE_OK == retcode)
    {
        retcode = Tests_RegisterTestCase(testSuiteId, TEST_CASE_2_a_ID, NULL, TestCase_2_a_run, NULL);
    }
    return retcode;
}

static Retcode_T TestCase_2_a_setup(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T retcode = RETCODE_FAILURE;

    retcode = Board_USART2_SPI_Init();

    retcode = CreateBinarySemaphore(&SPIBusLock);

    BSP_SPITest_Connect();

    BSP_SPI_Handle = BSP_SPITest_GetSPIHandle();
    SPICallback = SPI_Test_Callback;

    retcode = MCU_SPI_Initialize((SPI_T) BSP_SPI_Handle, SPICallback);

    retcode = BSP_SPITest_Enable();

    return retcode;
}

static void TestCase_2_a_run(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);

    Retcode_T retcode = RETCODE_OK;
    uint8_t data[SPI_BUFFER_LEN];
    uint8_t data_in[SPI_BUFFER_LEN] = { 0 };
    uint8_t count = 0;
    for (uint8_t i = 0; i < SPI_BUFFER_LEN; i++)
    {
        data[i] = i;
    }
    BSP_SPITest_SetCSHigh();
    BSP_Board_Delay(20);
    BSP_SPITest_SetCSLow();
retcode = MCU_SPI_Transfer(BSP_SPI_Handle, data, data_in, SPI_BUFFER_LEN);
    if (pdTRUE == xSemaphoreTake(SPIBusLock, (TickType_t )(MsToTicks(DATA_TRANSFER_TIMEOUT_MS))))
    {
        /* We were able to obtain the semaphore and can now access the
         shared resource. */
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR);
        Tests_SendReport(retcode, "Failed to send Byte over SPI");
        return;
    }

    for (uint8_t i = 0; i < SPI_BUFFER_LEN; i++)
    {
        if (data_in[i] == i)
        {
            count++;
        }
    }

    if (SPI_BUFFER_LEN == count)
    {
        Tests_SendReport(retcode, "Success SPI Byte send");
        return;
    }

    Tests_SendReport(retcode, "Failed to transmit Bytes over SPI");
    return;
}

static Retcode_T TestCase_2_a_teardown(CCMsg_T* ccmsg)
{
    BCDS_UNUSED(ccmsg);
    Retcode_T Rc = RETCODE_OK;

    Rc = DeleteSemaphore(&SPIBusLock);
    Rc = MCU_SPI_Deinitialize(BSP_SPI_Handle);

    SPITestDisable();

    BSP_SPITest_Disconnect();

    return Rc;
}

// ########################################################################################
Retcode_T Board_USART2_SPI_Init(void)
{
    /* enable the clock to access the registers */
    CMU_ClockEnable(cmuClock_USART2, true);
    /* Setting baudrate */
    SPI_PORT->CLKDIV = 256 * (SPI_PERCLK_FREQUENCY / 2 * SPI_BAUDRATE - 1);
    /* Configure SPI */
    /* Using synchronous (SPI) mode*/
    SPI_PORT->CTRL = USART_CTRL_SYNC;
    /* Clearing old transfers/receptions, and disabling interrupts */
    SPI_PORT->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
    SPI_PORT->IEN = 0;
    SPI_PORT->CTRL |= USART_CTRL_MSBF | USART_CTRL_TXBIL_HALFFULL;
    /* Disable the clock*/
    CMU_ClockEnable(cmuClock_USART2, false);
    return RETCODE_OK;
}

static void BSP_SPITest_Connect(void)
{
    GPIO_PinModeSet(SPI_TEST_CS_PORT, SPI_TEST_CS_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(SPI_TEST_SPI1_MISO_PORT, SPI_TEST_SPI1_MISO_PIN, gpioModeInputPull, 0);
    GPIO_PinModeSet(SPI_TEST_SPI1_MOSI_PORT, SPI_TEST_SPI1_MOSI_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(SPI_TEST_SPI1_SCK_PORT, SPI_TEST_SPI1_SCK_PIN, gpioModePushPull, 0);

    CMU_ClockEnable(cmuClock_USART2, true);
    SPI_TEST_SERIAL_PORT->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC1;
    CMU_ClockEnable(cmuClock_USART2, false);
}

static Retcode_T BSP_SPITest_Enable(void)
{
    /* Dont enable if the callback has not been registered */
    if (NULL == BSP_SPITest_SPI_Handle.IrqRxCallback || NULL == BSP_SPITest_SPI_Handle.IrqTxCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_BSP_SPI_TEST_SERIAL_NOT_INITIALIZED);
    }
            SPITestEnable();
    if (true == SPITestEnabled)
    {
        return RETCODE_OK;
    }
    else
    {
        return RETCODE_FAILURE;
    }
}



static void BSP_SPITest_Disconnect(void)
{
    GPIO_PinModeSet(SPI_TEST_CS_PORT, SPI_TEST_CS_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(SPI_TEST_SPI1_MISO_PORT, SPI_TEST_SPI1_MISO_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(SPI_TEST_SPI1_MOSI_PORT, SPI_TEST_SPI1_MOSI_PIN, gpioModeDisabled, 0);
    GPIO_PinModeSet(SPI_TEST_SPI1_SCK_PORT, SPI_TEST_SPI1_SCK_PIN, gpioModeDisabled, 0);

    CMU_ClockEnable(cmuClock_USART2, true);
    SPI_TEST_SERIAL_PORT->ROUTE = _USART_ROUTE_RESETVALUE;
    CMU_ClockEnable(cmuClock_USART2, false);
}


static HWHandle_T BSP_SPITest_GetSPIHandle(void)
{
    /*
     * Initialize the Handle and return it back to the caller
     */
    BSP_SPITest_SPI_Handle.Instance = SPI_TEST_SERIAL_PORT;
    BSP_SPITest_SPI_Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    return (HWHandle_T) &BSP_SPITest_SPI_Handle;
}

void SPITest_SPIRxISR(void)
{
    if (NULL != BSP_SPITest_SPI_Handle.IrqRxCallback)
    {
        /* All UART interrupt processing is to be done at the MCU level */
        SPITest_SPI_RxISRCallback((SPI_T) &BSP_SPITest_SPI_Handle);
    }
}

void SPITest_SPITxISR(void)
{
    if (NULL != BSP_SPITest_SPI_Handle.IrqTxCallback)
    {
        /* All UART interrupt processing is to be done at the MCU level */
        SPITest_SPI_TxISRCallback((SPI_T) &BSP_SPITest_SPI_Handle);
    }
}


Retcode_T BSP_SPITest_SetCSHigh(void)
{
    GPIO_PinOutSet(SPI_TEST_CS_PORT, SPI_TEST_CS_PIN);
    return RETCODE_OK;
}
//
///*
// * Refer to interface header for description
// */
Retcode_T BSP_SPITest_SetCSLow(void)
{
    GPIO_PinOutClear(SPI_TEST_CS_PORT, SPI_TEST_CS_PIN);
    return RETCODE_OK;
}

static void SPITestEnable(void)
{
    BoardCommon_WakeupPowerSupply2V5(EXTENSION_BOARD);
    BoardCommon_EnablePowerSupply3V3(EXTENSION_BOARD);
    /* Enable level shifter */
    /*enable the clock for the USART/spi interface*/
    CMU_ClockEnable(cmuClock_USART2, true);
    SPI_TEST_SERIAL_PORT->CMD = USART_CMD_CLEARTX;
    SPI_TEST_SERIAL_PORT->CMD = USART_CMD_CLEARRX;
    /* Set interrupt priorities */
    NVIC_SetPriority(SPI_TEST_RX_IRQN, SPI_TEST_TX_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SPI_TEST_TX_IRQN, SPI_TEST_TX_INTERRUPT_PRIORITY);
    /* clear pending interrupts */
    NVIC_ClearPendingIRQ(SPI_TEST_RX_IRQN);
    NVIC_ClearPendingIRQ(SPI_TEST_TX_IRQN);
    /* Assign the callback */
    SPITest_SPI_RxISRCallback = BSP_SPITest_SPI_Handle.IrqRxCallback;
    SPITest_SPI_TxISRCallback = BSP_SPITest_SPI_Handle.IrqTxCallback;

    SPI_TEST_SERIAL_PORT->IEN |= USART_IEN_RXDATAV;
    SPI_TEST_SERIAL_PORT->IFC = _USART_IFC_MASK;
    NVIC_EnableIRQ(SPI_TEST_RX_IRQN);
    NVIC_EnableIRQ(SPI_TEST_TX_IRQN);
    SPI_TEST_SERIAL_PORT->CMD = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;
    SPITestEnabled = true;
}

static void SPITestDisable(void)
{
    SPI_TEST_SERIAL_PORT->CMD = USART_CMD_MASTERDIS | USART_CMD_TXDIS | USART_CMD_RXDIS;
    SPI_TEST_SERIAL_PORT->IEN &= ~(USART_IEN_RXDATAV);
    CMU_ClockEnable(cmuClock_USART2, false);

    NVIC_DisableIRQ(SPI_TEST_RX_IRQN);
    NVIC_DisableIRQ(SPI_TEST_TX_IRQN);

    SPITest_SPI_RxISRCallback = NULL;
    SPITest_SPI_TxISRCallback = NULL;

    BoardCommon_EnablePowerSupply3V3(EXTENSION_BOARD);
    BoardCommon_WakeupPowerSupply2V5(EXTENSION_BOARD);
    SPITestEnabled = false;
}

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

static void SPI_Test_Callback(SPI_T spi, struct MCU_SPI_Event_S event)
{
    BCDS_UNUSED(spi);
    Retcode_T Rc = RETCODE_OK;


    if (UINT8_C(1) == event.TxComplete)
    {
        transfer_count++;



        if (RETCODE_OK == Rc)
        {
            BaseType_t higherPriorityTaskWoken = pdFALSE;

            if (NULL != SPIBusLock)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(SPIBusLock, &higherPriorityTaskWoken))
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

            if (NULL != SPIBusLock)
            {
                if (pdTRUE == xSemaphoreGiveFromISR(SPIBusLock, &higherPriorityTaskWoken))
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
        BSP_SPITest_SetCSHigh();
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
