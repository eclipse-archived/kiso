/**********************************************************************************************************************
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
 **********************************************************************************************************************/
/**
 * @file
 *
 * @brief
 * Unittest for MCU STM32L4 UART
 *
 */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START
extern "C"
{

/* setup compile time configuration defines */
/* include faked interfaces */
#include "Kiso_HAL_th.hh"

#if KISO_FEATURE_UART

#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_uart_th.hh"
#include "stm32l4xx_hal_dma_th.hh"

/* include module under test */
#include "UART.c"

} /* extern "C" */

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END
UART_T global_uart;
struct MCU_UART_Event_S global_event;

class STM32L4_UART_Test : public testing::Test
{
public:
    STM32L4_UART_Test();
    virtual ~STM32L4_UART_Test();

protected:
    virtual void SetUp()
    {
        RESET_FAKE(HAL_UART_IRQHandler);
        RESET_FAKE(HAL_DMA_IRQHandler);
        RESET_FAKE(HAL_UART_Transmit);
        RESET_FAKE(HAL_UART_Transmit_IT);
        RESET_FAKE(HAL_UART_Transmit_DMA);
        RESET_FAKE(HAL_UART_Receive);
        RESET_FAKE(HAL_UART_Receive_IT);
        RESET_FAKE(HAL_UART_Receive_DMA);
        FFF_RESET_HISTORY();
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

STM32L4_UART_Test::STM32L4_UART_Test()
{
    ;
}

STM32L4_UART_Test::~STM32L4_UART_Test()
{
    ;
}

//----------------------------------------------------------------------

class UartDevice
{
public:
    UartDevice(enum KISO_HAL_TransferMode_E TransferMode);
    virtual ~UartDevice();

    void *m_UartInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_UART_S m_Uart; /**< handle of MUC which address is given to user interface in form UART_T m_uart */

    USART_TypeDef m_Instance;
    DMA_HandleTypeDef m_hdmatx;
    DMA_HandleTypeDef m_hdmarx;

    void *getAppInterfaceHandle();
};

UartDevice::UartDevice(enum KISO_HAL_TransferMode_E TransferMode)
{
    m_Uart.TxMode = TransferMode;
    m_Uart.RxMode = TransferMode;

    m_Uart.huart.Instance = &m_Instance;

    // m_Uart.RxUserSize = 0;
    m_Uart.RxState = UART_STATE_INIT;
    m_Uart.TxState = UART_STATE_INIT;
    m_Uart.AppCallback = NULL;
    m_Uart.IrqCallback = NULL;

    m_Uart.huart.hdmatx = NULL;
    m_Uart.huart.hdmarx = NULL;
    m_Uart.DmaRxCallback = NULL;
    m_Uart.DmaTxCallback = NULL;

    m_UartInterfaceParam = (void *)&m_Uart;
}

UartDevice::~UartDevice()
{
    ;
}

void *UartDevice::getAppInterfaceHandle()
{
    return m_UartInterfaceParam;
}

static void UART_Callback(UART_T uart, struct MCU_UART_Event_S event)
{
    global_uart = uart;
    global_event = event;
}
/* specify test cases ******************************************************* */

/**
 * @brief   Test public MCU_UART_Initialize
 * @detail  Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
 */
TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_Invalid_Handle)
{
    Retcode_T rc;
    rc = MCU_UART_Initialize(NULL, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_InvalidState)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);

    Device01.m_Uart.TxState = UART_STATE_READY;
    Device01.m_Uart.RxState = UART_STATE_READY;

    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();

    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Uart.TxState = UART_STATE_TX;
    Device01.m_Uart.RxState = UART_STATE_RX;

    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Uart.TxState = UART_STATE_INIT;
    Device01.m_Uart.RxState = UART_STATE_RX;

    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    Device01.m_Uart.TxState = UART_STATE_TX;
    Device01.m_Uart.RxState = UART_STATE_INIT;

    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_InvalidCallback_int)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_InvalidCallback_dma)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_InvalidMode)
{
    Retcode_T rc;
    UartDevice Device01((enum KISO_HAL_TransferMode_E)5);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_Polling_Tx_Int_Rx_Fail)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Uart.RxMode = KISO_HAL_TRANSFER_MODE_INTERRUPT;
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_Polling_Tx_Dma_Rx_Fail)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Uart.RxMode = KISO_HAL_TRANSFER_MODE_DMA;
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_Polling_Tx_Invalid_Rx_Fail)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    Device01.m_Uart.RxMode = (enum KISO_HAL_TransferMode_E)5;
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_Polling_Ok)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_Int_Ok)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Initialize_DMA_Ok)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
}

/**
 * @brief   Test public UART MCU interface MCU_UART_Deinitialize
 */

TEST_F(STM32L4_UART_Test, test_MCU_UART_Deinitialize_Invalid_Handle)
{
    Retcode_T rc;
    rc = MCU_UART_Deinitialize((UART_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Deinitialize_OK)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Deinitialize_Invalid_State)
{
    Retcode_T rc;
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.m_Uart.TxState = UART_STATE_READY;
    Device01.m_Uart.RxState = UART_STATE_RX;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    Device01.m_Uart.TxState = UART_STATE_TX;
    Device01.m_Uart.RxState = UART_STATE_READY;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    Device01.m_Uart.TxState = UART_STATE_TX;
    Device01.m_Uart.RxState = UART_STATE_RX;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    rc = MCU_UART_Send((UART_T)NULL, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Send(uart01, (uint8_t *)NULL, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Send(uart01, buffer, UINT16_MAX + 1);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);

    Device01.m_Uart.TxState = UART_STATE_TX;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_Polling_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    HAL_UART_Transmit_fake.return_val = HAL_BUSY;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);

    HAL_UART_Transmit_fake.return_val = HAL_TIMEOUT;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);

    HAL_UART_Transmit_fake.return_val = HAL_ERROR;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_Int_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);

    HAL_UART_Transmit_IT_fake.return_val = HAL_BUSY;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);

    HAL_UART_Transmit_IT_fake.return_val = HAL_TIMEOUT;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);

    HAL_UART_Transmit_IT_fake.return_val = HAL_ERROR;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_DMA_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);

    HAL_UART_Transmit_DMA_fake.return_val = HAL_BUSY;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);

    HAL_UART_Transmit_DMA_fake.return_val = HAL_TIMEOUT;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);

    HAL_UART_Transmit_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_Polling_OK)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Transmit_fake.return_val = HAL_OK;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_TX);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_And_Abort_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_TX);

    // Abort sending by sending zero
    rc = MCU_UART_Send(uart01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Send_And_Abort_DMA_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Transmit_IT_fake.return_val = HAL_OK;

    rc = MCU_UART_Send(uart01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_TX);

    // Abort sending by sending zero
    rc = MCU_UART_Send(uart01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    rc = MCU_UART_Receive((UART_T)NULL, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Receive(uart01, (uint8_t *)NULL, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Receive(uart01, buffer, UINT16_MAX + 1);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    Device01.m_Uart.RxState = UART_STATE_RX;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_Polling_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    HAL_UART_Receive_fake.return_val = HAL_BUSY;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);

    HAL_UART_Receive_fake.return_val = HAL_TIMEOUT;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);

    HAL_UART_Receive_fake.return_val = HAL_ERROR;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_Int_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);

    HAL_UART_Receive_IT_fake.return_val = HAL_BUSY;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);

    HAL_UART_Receive_IT_fake.return_val = HAL_TIMEOUT;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);

    HAL_UART_Receive_IT_fake.return_val = HAL_ERROR;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_DMA_Fail)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);

    HAL_UART_Receive_DMA_fake.return_val = HAL_BUSY;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);

    HAL_UART_Receive_DMA_fake.return_val = HAL_TIMEOUT;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);

    HAL_UART_Receive_DMA_fake.return_val = HAL_ERROR;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_Polling_OK)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_POLLING);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Receive_fake.return_val = HAL_OK;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_And_Abort_Int_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Receive_IT_fake.return_val = HAL_OK;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_RX);

    // Abort receiving by sending zero
    rc = MCU_UART_Receive(uart01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);
}

TEST_F(STM32L4_UART_Test, test_MCU_UART_Receive_And_Cancel_DMA_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Receive_DMA_fake.return_val = HAL_OK;

    rc = MCU_UART_Receive(uart01, buffer, size);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_RX);

    // Abort receiving by sending zero
    rc = MCU_UART_Receive(uart01, buffer, 0);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.RxState, UART_STATE_READY);
}

/**
 * @brief    Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet)
 */

TEST_F(STM32L4_UART_Test, test_MapHalRetToMcuRet)
{
    Retcode_T rc = MapHalRetToMcuRet(HAL_OK);
    EXPECT_EQ(RETCODE_OK, rc);

    rc = MapHalRetToMcuRet(HAL_BUSY);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    rc = MapHalRetToMcuRet(HAL_TIMEOUT);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), rc);

    rc = MapHalRetToMcuRet(HAL_ERROR);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
}
/**
 * @brief void UART_IRQHandler(UART_T huart)
 */

TEST_F(STM32L4_UART_Test, test_UART_IRQHandler)
{
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    UART_IRQHandler(uart01);
    EXPECT_EQ(HAL_UART_IRQHandler_fake.call_count, 1U);
}

/**
 * @brief void UART_DMARxHandler(UART_T huart)
 */

TEST_F(STM32L4_UART_Test, test_UART_DMARxHandler)
{
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    UART_DMARxHandler(uart01);
    EXPECT_EQ(HAL_DMA_IRQHandler_fake.call_count, 1U);
}

/**
 * @brief void UART_DMATxHandler(UART_T huart)
 */

TEST_F(STM32L4_UART_Test, test_UART_DMATxHandler)
{
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    UART_DMATxHandler(uart01);
    EXPECT_EQ(HAL_DMA_IRQHandler_fake.call_count, 1U);
}

/**
 * @brief void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
 */

TEST_F(STM32L4_UART_Test, test_HAL_UART_TxCpltCallback)
{
    MCU_UART_S uart;
    uart.AppCallback = UART_Callback;
    HAL_UART_TxCpltCallback(&uart.huart);
    EXPECT_EQ(UART_STATE_READY, uart.TxState);
}

/**
 * @brief void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 */

TEST_F(STM32L4_UART_Test, test_HAL_UART_RxCpltCallback)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_UART_Receive(uart01, buffer, size);

    global_event.RxComplete = 0;
    HAL_UART_RxCpltCallback((UART_HandleTypeDef *)uart01);
    EXPECT_EQ(1U, global_event.RxComplete);
}

/**
 * @brief void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
 */

TEST_F(STM32L4_UART_Test, test_HAL_UART_ErrorCallback_simpleError)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_UART_Receive(uart01, buffer, size);
    global_event.RxError = 0;
    Device01.m_Uart.huart.ErrorCode = UART_HAL_ERROR_RECOVERABLE;

    HAL_UART_ErrorCallback((UART_HandleTypeDef *)uart01);

    EXPECT_EQ(1U, global_event.RxError);
}
/**
 * @brief void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart))
 */

TEST_F(STM32L4_UART_Test, test_HAL_UART_ErrorCallback_nonRecoverableError)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    UartDevice Device01(KISO_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T)Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, UART_Callback);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Receive_IT_fake.return_val = HAL_OK;
    rc = MCU_UART_Receive(uart01, buffer, size);
    global_event.RxAborted = 0;
    Device01.m_Uart.huart.ErrorCode = UART_HAL_ERROR_NON_RECOVERABLE;

    HAL_UART_ErrorCallback((UART_HandleTypeDef *)uart01);

    EXPECT_EQ(1U, global_event.RxAborted);
}

#endif
