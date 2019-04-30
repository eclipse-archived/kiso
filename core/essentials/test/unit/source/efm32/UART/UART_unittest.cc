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

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START

/* setup compile time configuration defines */

/* include faked interfaces */
extern "C"
{
#include "BCDS_Retcode_th.hh"
#include "BCDS_HAL_th.hh"

/**
 * BCDS_HALConfig.h file which breaks the unit test implementation.
 *
 * Below four lines hot-fix needs to be replaced by proper implementation.
 */

#include "em_usart_th.hh"
#include "em_dma_th.hh"

typedef HWHandle_T UART_T;

/* include module under test */
#include "UART.c"

}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class BCDS_UARTtest: public testing::Test
{
protected:

    virtual void SetUp()
    {
        DeInit();

        for (int i = 0; i < 10; i++)
        {

        }

        FFF_RESET_HISTORY()
    }

    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }

    void DeInit()
    {
        ;/* nothing to do if clean up is not required */
    }

public:
    static const union MCU_UART_Event_U m_EventInit;
    static const union MCU_UART_Event_U m_EventRxReady;
    static const union MCU_UART_Event_U m_EventRxError;
    static const union MCU_UART_Event_U m_EventRxComplete;
    static const union MCU_UART_Event_U m_EventTxError;
    static const union MCU_UART_Event_U m_EventTxComplete;
    static bool m_isCalled_cbf;
    static uint32_t m_uart_cbf;
    static union MCU_UART_Event_U m_event_cbf;
};

const union MCU_UART_Event_U BCDS_UARTtest::m_EventInit =
{   .bitfield =
    {   0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const union MCU_UART_Event_U BCDS_UARTtest::m_EventRxReady =
{   .bitfield =
    {   1, 0, 0, 0, 0, 0, 0, 0, 0}
};

const union MCU_UART_Event_U BCDS_UARTtest::m_EventRxError =
{   .bitfield =
    {   0, 1, 0, 0, 0, 0, 0, 0, 0}
};
const union MCU_UART_Event_U BCDS_UARTtest::m_EventRxComplete =
{   .bitfield =
    {   0, 0, 1, 0, 0, 0, 0, 0, 0}
};
const union MCU_UART_Event_U BCDS_UARTtest::m_EventTxError =
{   .bitfield =
    {   0, 0, 0, 1, 0, 0, 0, 0, 0}
};
const union MCU_UART_Event_U BCDS_UARTtest::m_EventTxComplete =
{   .bitfield =
    {   0, 0, 0, 0, 1, 0, 0, 0, 0}
};

bool BCDS_UARTtest::m_isCalled_cbf = false;

union MCU_UART_Event_U BCDS_UARTtest::m_event_cbf = m_EventInit;
uint32_t BCDS_UARTtest::m_uart_cbf = 0;

//----------------------------------------------------------------------

class UartDevice
{
public:
    UartDevice(enum BCDS_HAL_TransferMode_E TransferMode);
    virtual ~UartDevice();

    uint32_t m_UartInterfaceParam; /**< user interface handle, simple integer */
    struct MCU_UART_Handle_S m_Uart; /**< handle of MCU whose address is given to user interface in form UART_T m_uart */

    uint32_t getAppInterfaceHandle();
};

UartDevice::UartDevice(enum BCDS_HAL_TransferMode_E TransferMode)
{
    m_Uart.TransferMode = TransferMode;
    m_Uart.Index = (0x2BAD);
    m_Uart.Uart_ptr = UART_TEST;

    m_Uart.IrqRxCallback = MCU_UART_IRQ_RX_Callback;
    m_Uart.IrqTxCallback = MCU_UART_IRQ_TX_Callback;
    m_Uart.DmaRxCallback = NULL;
    m_Uart.DmaTxCallback = NULL;
    m_Uart.CtsCallback = NULL;
    m_Uart.RtsCallback = NULL;
    m_Uart._DriverCtx.TxState = UART_STATE_INIT;
	m_Uart._DriverCtx.RxState = UART_STATE_INIT;

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

/**
 * @brief   Test public UART MCU interface HalUart_MCU_UART_Power
 * @detail  callback signature: typedef void (*MCU_UART_Callback_T)(struct MCU_UART_Event_S event);
 */
void AppTestCallbackFunction(UART_T uart, struct MCU_UART_Event_S event)
{
    BCDS_UARTtest::m_uart_cbf = (intptr_t) uart;
    BCDS_UARTtest::m_event_cbf.bitfield = event;
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
    BCDS_UARTtest::m_isCalled_cbf = true;
}

TEST_F(BCDS_UARTtest, dummyTest)
{
    /** @testcase{ <FIXTURE_NAME>::<TEST_NAME>: }
     *
     */

    /* SETUP: Declare and initialize local variables required only by this
	 * test case */

    /* EXECISE: call relevant production code Interface with appropriate test
	 * inputs  */

    /* VERIFY : Compare the expected with actual */

    /* CLEAN UP: Perform dynamic memory deallocation steps and similar. */

}

/*************************** UART TESTS *****************************/
/**
 * @brief   Test public MCU_UART_Initialize
 * @detail  Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
 */
TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_ok)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}
TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_nullUart)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_nullCallbackandUart)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_nullCallback)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_wrongTransferMode)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED)
, rc);
}

/**
 * @brief   Test public UART MCU interface MCU_UART_Deinitialize
 * @detail  Retcode_T MCU_UART_Deinitialize(UART_T uart)
 * Dependent on Test public MCU_UART_Initialize
 */
TEST_F(BCDS_UARTtest, test_MCU_UART_Deinitialize_ok)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

#if 0
TEST_F(BCDS_UARTtest, test_MCU_UART_Deinitialize_notInitialized)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED)
, rc);
}
#endif

TEST_F(BCDS_UARTtest, test_MCU_UART_Deinitialize_nullUart)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED)
, rc);
}

/**
 * @brief Test public UART MCU interface MCU_UART_Send
 * @detail  Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_UART_Initialize and public MCU_UART_Deinitialize
 */

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_ok)
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
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
#if 0
TEST_F(BCDS_UARTtest, test_MCU_UART_Send_notInitialized)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_nullBuffer)
{
    const uint16_t size = 10;
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}
#endif

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_sendCancel)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, buffer, 0); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_wrongTransferMode)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED)
, rc);
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

/**
 * @brief Test public UART MCU interface MCU_UART_Receive
 * @detail   Retcode_T MCU_UART_Receive(UART_T uart, uint8_t *data, uint32_t len)
 * Dependent on Test public MCU_UART_Initialize and public MCU_UART_Deinitialize
 */

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_ok)
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
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}


TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_receiveCancel)
{
    const uint16_t size = 1;
    const uint16_t size2 = 0;
    Retcode_T rc;
    uint8_t buffer[ size ] = {0x01};
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Receive(uart01, buffer, size2); /* receive cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
#if 0

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_nullBuffer)
{
    const uint16_t size = 10;
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Receive(uart01, 0, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_notInitialized)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}
#endif
TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_wrongTransferMode)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED)
, rc);
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

/**
 * @brief Test public UART MCU interface MCU_UART_GetRxCount
 * @detail   MCU_UART_GetRxCount(UART_T uart, uint32_t * count)
 * Dependent on Test public MCU_UART_Initialize and public MCU_UART_Deinitialize
 */

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_ok)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_GetRxCount(uart01, count);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_nullCount)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_GetRxCount(uart01, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_notInitialized)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_GetRxCount(uart01, count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_wrongTransferMode)
{
uint32_t temp = 10;
uint32_t * count = &temp;
Retcode_T rc;
UartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
rc = MCU_UART_GetRxCount(uart01, count);
EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}
