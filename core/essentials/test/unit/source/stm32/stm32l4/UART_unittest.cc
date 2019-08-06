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
 * @file
 *
 * @brief
 * Unittest for MCU STM32 UART
 *
 */
#include <gtest.h>

extern "C"
{

#include "BCDS_Basics.h"
#include "BCDS_HAL_th.hh"

#if BCDS_FEATURE_UART

/* include faked interfaces */
#include "stm32l4xx_hal_th.hh"
#include "stm32l4xx_hal_dma_th.hh"
#include "stm32l4xx_hal_uart_th.hh"
#include "stm32l4xx_hal_cryp_th.hh"

/* Include module under test */
#include "UART.c"

void HAL_UART_TxCpltCallback_Peripherals(UART_HandleTypeDef *huart)
{
    BCDS_UNUSED(huart);
}

void HAL_UART_RxCpltCallback_Peripherals(UART_HandleTypeDef *huart)
{
    BCDS_UNUSED(huart);
}

void HAL_UART_ErrorCallback_Peripherals(UART_HandleTypeDef *huart)
{
    BCDS_UNUSED(huart);
}

} /* extern "C" */

/* end of global scope symbol and fake definitions section */

/*
 static Retcode_T FakeFailSend(struct MCU_UART_Driver_S* const element_ptr, uint8_t * data, uint32_t len)
 {
 BCDS_UNUSED(element_ptr);
 BCDS_UNUSED(data);
 BCDS_UNUSED(len);
 return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
 }
 */

//----------------------------------------------------------------------
void AppTestCallbackFunction_XferCpltCallback(
        struct __DMA_HandleTypeDef * hdma);
void AppTestCallbackFunction_XferHalfCpltCallback(
        struct __DMA_HandleTypeDef * hdma);
void AppTestCallbackFunction_XferErrorCallback(
        struct __DMA_HandleTypeDef * hdma);
//----------------------------------------------------------------------

class HalUartTest: public testing::Test
{
public:
    HalUartTest();
    virtual ~HalUartTest();
    protected:
    virtual void SetUp()
    {
        for (int i = 0; i < 10; i++)
        {

        }
        FFF_RESET_HISTORY()
        ;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }

public:
    static const struct MCU_UART_Event_S m_EventInit;
    static const struct MCU_UART_Event_S m_EventRxReady;
    static const struct MCU_UART_Event_S m_EventRxError;
    static const struct MCU_UART_Event_S m_EventRxComplete;
    static const struct MCU_UART_Event_S m_EventTxError;
    static const struct MCU_UART_Event_S m_EventTxComplete;

    static bool m_isCalled_cbf;
    static bool m_isCalled_XferCpltCallback;
    static bool m_isCalled_XferHalfCpltCallback;
    static bool m_isCalled_XferErrorCallback;
    static uint32_t m_uart_cbf;
    static struct MCU_UART_Event_S m_event_cbf;
};

const struct MCU_UART_Event_S HalUartTest::m_EventInit = { 0, 0, 0, 0, 0, 0, 0, 0, 0};

const struct MCU_UART_Event_S HalUartTest::m_EventRxReady = { 1, 0, 0, 0, 0, 0, 0, 0, 0};
const struct MCU_UART_Event_S HalUartTest::m_EventRxError = { 0, 1, 0, 0, 0, 0, 0, 0, 0};
const struct MCU_UART_Event_S HalUartTest::m_EventRxComplete = { 0, 0, 1, 0, 0, 0, 0, 0, 0};
const struct MCU_UART_Event_S HalUartTest::m_EventTxError = { 0, 0, 0, 1, 0, 0, 0, 0, 0};
const struct MCU_UART_Event_S HalUartTest::m_EventTxComplete = { 0, 0, 0, 0, 1, 0, 0, 0, 0};

bool HalUartTest::m_isCalled_cbf = false;
//-- dma
bool HalUartTest::m_isCalled_XferCpltCallback = false;
bool HalUartTest::m_isCalled_XferHalfCpltCallback = false;
bool HalUartTest::m_isCalled_XferErrorCallback = false;

uint32_t HalUartTest::m_uart_cbf = 0;
struct MCU_UART_Event_S HalUartTest::m_event_cbf = m_EventInit;

HalUartTest::HalUartTest()
{
    ;
}

HalUartTest::~HalUartTest()
{
    ;
}

//----------------------------------------------------------------------

class UartDevice
{
public:
    UartDevice(enum BCDS_HAL_TransferMode_E TransferMode);
    virtual ~UartDevice();

    uint32_t m_UartInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_UART_S m_Uart; /**< handle of MUC which address is given to user interface in form UART_T m_uart */

    USART_TypeDef m_Instance;
    DMA_HandleTypeDef m_hdmatx;
    DMA_HandleTypeDef m_hdmarx;

    DMA_Channel_TypeDef m_DmaChannel;

    uint32_t getAppInterfaceHandle();
};

UartDevice::UartDevice(enum BCDS_HAL_TransferMode_E TransferMode)
{
    m_Uart.TransferMode = TransferMode;

    m_Uart.huart.Instance = &m_Instance;

    m_Uart.huart.pRxBuffPtr = NULL;
    m_Uart.huart.pTxBuffPtr = NULL;
    m_Uart.huart.RxXferCount = 0;
    m_Uart.huart.RxXferSize = 0;
    m_Uart.huart.TxXferCount = 0;
    m_Uart.huart.TxXferSize = 0;
    m_Uart.huart.Mask = 0;

    m_Uart.RxUserSize = 0;
    m_Uart.isReceiving = false;
    m_Uart.isSending = false;

    m_Uart.AppCallback = NULL;
    m_Uart.IrqCallback = NULL;

    m_Uart.huart.hdmatx = NULL;
    m_Uart.huart.hdmarx = NULL;
    m_Uart.DmaRxCallback = NULL;
    m_Uart.DmaTxCallback = NULL;

    m_Uart.IrqCallback = MCU_UART_IRQ_Callback;

    switch (m_Uart.TransferMode)
    {
    case BCDS_HAL_TRANSFER_MODE_DMA:
        m_Uart.huart.hdmatx = (DMA_HandleTypeDef*) &m_hdmatx;
        m_Uart.huart.hdmarx = (DMA_HandleTypeDef*) &m_hdmarx;
        m_Uart.DmaRxCallback = MCU_UART_DMA_RX_Callback;
        m_Uart.DmaTxCallback = MCU_UART_DMA_TX_Callback;
        m_hdmatx.Instance = (DMA_Channel_TypeDef *) &m_DmaChannel;
        m_hdmatx.Parent = (void*) &m_Uart.huart;
        m_hdmatx.XferCpltCallback = AppTestCallbackFunction_XferCpltCallback;
        m_hdmatx.XferErrorCallback = AppTestCallbackFunction_XferErrorCallback;
        m_hdmatx.XferHalfCpltCallback = AppTestCallbackFunction_XferHalfCpltCallback;
        m_hdmarx.XferCpltCallback = AppTestCallbackFunction_XferCpltCallback;
        m_hdmarx.XferErrorCallback = AppTestCallbackFunction_XferErrorCallback;
        m_hdmarx.XferHalfCpltCallback = AppTestCallbackFunction_XferHalfCpltCallback;
        break;
    case BCDS_HAL_TRANSFER_MODE_INTERRUPT:
        break;
    case BCDS_HAL_TRANSFER_MODE_BLOCKING:
        break;
    default:
        break;
    }
    m_UartInterfaceParam = (intptr_t) &m_Uart;
}

UartDevice::~UartDevice()
{
    ;
}

uint32_t UartDevice::getAppInterfaceHandle()
{
    return m_UartInterfaceParam; // ((UART_T)&m_mcuHandle);
}

//----------------------------------------------------------------------

/**
 * @brief   Test public UART MCU interface HalUart_MCU_UART_Power
 * @detail  callback signature: typedef void (*MCU_UART_Callback_T)(struct MCU_UART_Event_S event);
 */
void AppTestCallbackFunction(UART_T uart, struct MCU_UART_Event_S event)
{
    HalUartTest::m_uart_cbf = (intptr_t) uart;
    HalUartTest::m_event_cbf = event;
    /*
     std::cout << "     --- Hello from AppTestCallbackFunction ---" << std::endl;
     std::cout << "     --- uart: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t) uart << std::endl;
     std::cout << "     --- event: 0x" << std::hex << std::setw(8) << std::setfill('0')
     << (uint32_t) *(uint32_t*) (struct MCU_UART_Event_S *) &event << std::endl;
     std::cout << "     --- TransferError: " << std::dec << event.TransferError << std::endl;
     std::cout << "     --- TransferComplete: " << std::dec << event.TransferComplete << std::endl;
     std::cout << "     --- Cts: " << std::dec << event.Cts << std::endl;
     std::cout << "     --- Dsr: " << std::dec << event.Dsr << std::endl;
     std::cout << "     --- Dcd: " << std::dec << event.Dcd << std::endl;
     std::cout << "     --- Ri: " << std::dec << event.Ri << std::endl;
     */
    HalUartTest::m_isCalled_cbf = true;
}

/*!< DMA transfer complete callback         */
void AppTestCallbackFunction_XferCpltCallback(
        struct __DMA_HandleTypeDef * hdma)
{
    BCDS_UNUSED(hdma);
    HalUartTest::m_isCalled_XferCpltCallback = true;
}

/*!< DMA Half transfer complete callback    */
void AppTestCallbackFunction_XferHalfCpltCallback(
        struct __DMA_HandleTypeDef * hdma)
{
    BCDS_UNUSED(hdma);
    HalUartTest::m_isCalled_XferHalfCpltCallback = true;
}

/*!< DMA transfer error callback            */
void AppTestCallbackFunction_XferErrorCallback(
        struct __DMA_HandleTypeDef * hdma)
{
    BCDS_UNUSED(hdma);
    HalUartTest::m_isCalled_XferErrorCallback = true;
}

//----------------------------------------------------------------------

/* specify test cases ******************************************************* */

/**
 * @brief   Test public MCU_UART_Initialize
 * @detail  Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
 */
TEST_F(HalUartTest, test_MCU_UART_Initialize_001)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Initialize_002)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Initialize_003)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Initialize_004)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_NE(RETCODE_OK, rc);
}

/**
 * @brief   Test public UART MCU interface MCU_UART_Deinitialize
 */
TEST_F(HalUartTest, test_MCU_UART_Deinitialize_001)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Deinitialize_002)
{
    Retcode_T rc;
    UART_T uart01 = (HWHandle_T) NULL;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Deinitialize_003)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}


/**
 * @brief   Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t len)
 */

TEST_F(HalUartTest, test_MCU_UART_Send_001)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Send_002)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
}


TEST_F(HalUartTest, test_MCU_UART_Send_003)
{
    const uint16_t size = 10;
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, NULL, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Send_004)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, buffer, 0); /* send cancel */
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Send_005)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.m_Uart.isSending = true;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_BUSY), rc);
}

TEST_F(HalUartTest, test_MCU_UART_Send_006)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Send_007)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED), rc);
}

/**
 * @brief   Retcode_T MCU_UART_Receive(UART_T uart, uint8_t *data, uint32_t len)
 */

TEST_F(HalUartTest, test_MCU_UART_Receive_001)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
}
#else
}
#endif /* BCDS_FEATURE_UART */
