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
 * Unittest for MCU STM32f7 UART
 *
 */
#include <gtest.h>


/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START
extern "C"
{

/* setup compile time configuration defines */
/* include faked interfaces */
#include "BCDS_HAL_th.hh"

#include "stm32f7xx_th.hh"
#include "stm32f7xx_hal_th.hh"
#include "stm32f7xx_hal_uart_th.hh"

#undef BCDS_FEATURE_UART
#define BCDS_FEATURE_UART 1

/* include module under test */
#include "UART.c"

} /* extern "C" */

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class HalUartTest: public testing::Test
{
public:
    HalUartTest();
    virtual ~HalUartTest();
    protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
        ;
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }

};


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

    struct MCU_UART_Handle_S m_Uart; /**< handle of MUC which address is given to user interface in form UART_T m_uart */

    USART_TypeDef m_Instance;
    DMA_HandleTypeDef m_hdmatx;
    DMA_HandleTypeDef m_hdmarx;

    DMA_Stream_TypeDef m_DmaChannel;

    uint32_t getAppInterfaceHandle();
};

UartDevice::UartDevice(enum BCDS_HAL_TransferMode_E TransferMode)
{
    m_Uart.TransferMode = TransferMode;

    m_Uart.huart.Instance = &m_Instance;


    m_Uart.RxUserSize = 0;
    m_Uart.RxState = MCU_UART_STATE_INIT;
    m_Uart.TxState = MCU_UART_STATE_INIT;
    m_Uart.AppCallback = NULL;
    m_Uart.IrqCallback = NULL;

    m_Uart.huart.hdmatx = NULL;
    m_Uart.huart.hdmarx = NULL;
    m_Uart.DmaRxCallback = NULL;
    m_Uart.DmaTxCallback = NULL;

     m_UartInterfaceParam = (intptr_t) &m_Uart;
}

UartDevice::~UartDevice()
{
    ;
}

uint32_t UartDevice::getAppInterfaceHandle()
{
    return m_UartInterfaceParam;
}


/* specify test cases ******************************************************* */

/**
 * @brief   Test public MCU_UART_Initialize
 * @detail  Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
 */
TEST_F(HalUartTest, test_MCU_UART_Initialize_001)
{
    Retcode_T rc;
    rc = MCU_UART_Initialize(NULL, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(HalUartTest, test_MCU_UART_Initialize_002)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED), rc);
}

TEST_F(HalUartTest, test_MCU_UART_Initialize_003)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    Device01.m_Uart.RxState = MCU_UART_STATE_RUNNING;
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);

    UartDevice Device02(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    Device02.m_Uart.TxState = MCU_UART_STATE_RUNNING;
    UART_T uart02 = (HWHandle_T) Device02.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart02, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}



TEST_F(HalUartTest, test_MCU_UART_Initialize_005)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
}

/**
 * @brief   Test public UART MCU interface MCU_UART_Deinitialize
 */

TEST_F(HalUartTest, test_MCU_UART_Deinitialize_001)
{
    Retcode_T rc;
    UART_T uart01 = (HWHandle_T) NULL;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(HalUartTest, test_MCU_UART_Deinitialize_003)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(HalUartTest, test_MCU_UART_Deinitialize_004)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.m_Uart.TxState = MCU_UART_STATE_RUNNING;
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);


    UartDevice Device02(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart02 = (HWHandle_T) Device02.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart02, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    Device02.m_Uart.RxState = MCU_UART_STATE_RUNNING;
    rc = MCU_UART_Deinitialize(uart02);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

/**
 * @brief   Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t len)
 */

TEST_F(HalUartTest, test_MCU_UART_Send_001)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    rc = MCU_UART_Send((UART_T)NULL, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Send(uart01, (uint8_t*)NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Send(uart01, buffer, UINT16_MAX);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

}


TEST_F(HalUartTest, test_MCU_UART_Send_002)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Transmit_fake.return_val = HAL_OK;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(Device01.m_Uart.TxState, MCU_UART_STATE_READY);
}

// HAL Send Timeout
TEST_F(HalUartTest, test_MCU_UART_Send_003)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    HAL_UART_Transmit_fake.return_val = HAL_BUSY;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_BUSY), rc);
    EXPECT_EQ(Device01.m_Uart.TxState, MCU_UART_STATE_READY);
}

TEST_F(HalUartTest, test_MCU_UART_Send_004)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.m_Uart.TxState = MCU_UART_STATE_INIT;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

/**
 * @brief   Retcode_T MCU_UART_Receive(UART_T uart, uint8_t *data, uint32_t len)
 */
TEST_F(HalUartTest, test_MCU_UART_Receive_001)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);

    rc = MCU_UART_Receive((UART_T)NULL, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Receive(uart01, (uint8_t*)NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    rc = MCU_UART_Receive(uart01, buffer, UINT16_MAX);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

}

TEST_F(HalUartTest, test_MCU_UART_Receive_002)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(Device01.m_Uart.RxState, MCU_UART_STATE_READY);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED), rc);

    Device01.m_Uart.RxState = MCU_UART_STATE_INIT;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

TEST_F(HalUartTest, test_MCU_UART_Receive_003)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, (MCU_UART_Callback_T)NULL);
    EXPECT_EQ(RETCODE_OK, rc);

    /* Request to abort inconsistent in blocking mode */
    Device01.m_Uart.RxState = MCU_UART_STATE_RUNNING;
    rc = MCU_UART_Receive(uart01, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED), rc);

    Device01.m_Uart.RxState = MCU_UART_STATE_INIT;
    rc = MCU_UART_Receive(uart01, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE), rc);
}

/**
 * @brief    Retcode_T MCU_UART_GetRxCount(UART_T uart, uint32_t* count)
 */

// Unsupported MCU_UART_GetRxCount
TEST_F(HalUartTest, test_MCU_UART_GetRxCount_001)
{
    Retcode_T rc;
    uint32_t count;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_DMA);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_GetRxCount(uart01, &count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}

/**
 * @brief    Retcode_T MapHalRetToMcuRet(HAL_StatusTypeDef halRet)
 */

TEST_F(HalUartTest, test_MapHalRetToMcuRet_001)
{
    Retcode_T rc = MapHalRetToMcuRet (HAL_OK);
    EXPECT_EQ(RETCODE_OK, rc);

    rc = MapHalRetToMcuRet (HAL_BUSY);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_BUSY), rc);

    rc = MapHalRetToMcuRet (HAL_TIMEOUT);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_TIMEOUT), rc);

    rc = MapHalRetToMcuRet ((HAL_StatusTypeDef) 100);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UART_HAL_ERROR), rc);

}
