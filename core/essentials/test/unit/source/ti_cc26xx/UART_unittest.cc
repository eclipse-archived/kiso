/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

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

#include "drivers\UART.h"

/* Add vendor specific fake functions needed */
FAKE_VALUE_FUNC(int_fast32_t, UART_write,UART_Handle, const void *,size_t)
;
FAKE_VOID_FUNC(UART_writeCancel, UART_Handle)
;
FAKE_VALUE_FUNC(int_fast32_t, UART_read, UART_Handle,void *,size_t)
;
FAKE_VOID_FUNC(UART_readCancel, UART_Handle)
;

typedef HWHandle_T UART_T;

#undef BCDS_FEATURE_UART
#define BCDS_FEATURE_UART 1

/* include module under test */
#include "BCDS_MCU_UART.c"

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
    UartDevice(enum BCDS_HAL_CallMode_E CallMode);
    virtual ~UartDevice();

    uint32_t m_UartInterfaceParam; /**< user interface handle, simple integer */
    struct MCU_UART_Handle_S m_Uart; /**< handle of MCU whose address is given to user interface in form UART_T m_uart */

    uint32_t getAppInterfaceHandle();

    void setDriveHandle(UART_Handle handle);
    void setTxState(enum MCU_UARTState_E state);
    void setTxCount(uint32_t count);
    void setRxCount(uint32_t count);

};

UartDevice::UartDevice(enum BCDS_HAL_CallMode_E CallMode)
{

    m_Uart.handle = NULL;
    m_Uart.callMode = CallMode;

    m_Uart.rxCallback = NULL;
    m_Uart.txCallback = NULL;
    m_Uart.appCallback = NULL;

    m_Uart.rxFunc = NULL;
    m_Uart.txFunc = NULL;
    m_Uart.abortRxFunc = NULL;

    m_Uart.rxBuffer = NULL;
    m_Uart.rxCount = 0;
    m_Uart.rxSize = 0;
    m_Uart.txCount = 0;
    m_Uart.txSize = 0;

    m_Uart.txState = MCU_UART_STATE_INIT;
    m_Uart.rxState = MCU_UART_STATE_INIT;

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

void UartDevice::setDriveHandle(UART_Handle handle)
{
    m_Uart.handle = handle;
}
void UartDevice::setTxState(enum MCU_UARTState_E state)
{
    m_Uart.txState = state;
}
void UartDevice::setTxCount(uint32_t count)
{
    m_Uart.txCount = count;
}

void UartDevice::setRxCount(uint32_t count)
{
    m_Uart.rxCount = count;
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
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public MCU_UART_Initialize
 * @detail  Retcode_T MCU_UART_Initialize(UART_T uart, MCU_UART_Callback_T callback)
 */
TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_ok_interrupt)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_ok_blocking)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_dopple)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_nullUart)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_nullCallbackandUart)
{
    Retcode_T rc;
    UART_T uart01 = 0;
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_nullCallback)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
            , rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Initialize_wrongCallMode)
{
    Retcode_T rc;
    UartDevice Device01((enum BCDS_HAL_CallMode_E)0);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED)
            , rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public UART MCU interface MCU_UART_Deinitialize
 * @detail  Retcode_T MCU_UART_Deinitialize(UART_T uart)
 * Dependent on Test public MCU_UART_Initialize
 */
TEST_F(BCDS_UARTtest, test_MCU_UART_Deinitialize_ok)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Deinitialize_notInitialized)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Deinitialize_nullUart)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Deinitialize(0);
    EXPECT_NE(RETCODE_OK, rc);
}

/*----------------------------------------------------------------------------*/
/**
 * @brief Test public UART MCU interface MCU_UART_Send
 * @detail  Retcode_T MCU_UART_Send(UART_T uart, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_UART_Initialize and public MCU_UART_Deinitialize
 */
TEST_F(BCDS_UARTtest, test_MCU_UART_Send_ok)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTxCount(size);
    MCU_UARTTxCallback(uart01);
    EXPECT_TRUE(m_isCalled_cbf);
    EXPECT_EQ(1, m_event_cbf.bitfield.TxComplete);
    EXPECT_EQ(0, m_event_cbf.bitfield.TxError);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
TEST_F(BCDS_UARTtest, test_MCU_UART_Send_ok_Blocking)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = size;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_Error)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    uint32_t sizeFailedSend = 2;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTxCount(sizeFailedSend);
    MCU_UARTTxCallback(uart01);
    EXPECT_TRUE(m_isCalled_cbf);
    EXPECT_EQ(0, m_event_cbf.bitfield.TxComplete);
    EXPECT_EQ(1, m_event_cbf.bitfield.TxError);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
TEST_F(BCDS_UARTtest, test_MCU_UART_Send_Fail)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    uint32_t sizeFailedSend = 2;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = sizeFailedSend;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_fail_Blocking)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    uint32_t sizeFailedSend = 2;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = sizeFailedSend;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_notInitialized)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_nullBuffer)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_null_DriveHandle)
{
    UART_Handle uartHandle = (UART_Handle) NULL;
    uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_nullHandle)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T)  Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(NULL, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_invalid_length)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 0x0001FFFF;
    Retcode_T rc;
    uint8_t buffer[size];
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_sendCancel)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Send(uart01, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTxCount(2);
    MCU_UARTTxCallback(uart01);
    EXPECT_FALSE(m_isCalled_cbf);
    EXPECT_EQ(0, m_event_cbf.bitfield.TxComplete);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Send_sendCancel_fail)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_write_fake.return_val = 0;
    rc = MCU_UART_Send(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTxCount(size);
    MCU_UARTTxCallback(uart01);
    EXPECT_TRUE(m_isCalled_cbf);
    EXPECT_EQ(1, m_event_cbf.bitfield.TxComplete);
    EXPECT_EQ(0, m_event_cbf.bitfield.TxError);
    rc = MCU_UART_Send(uart01, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Test public UART MCU interface MCU_UART_Receive
 * @detail  Retcode_T MCU_UART_Receive(UART_T uart, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_UART_Initialize and public MCU_UART_Deinitialize
 */
TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_ok)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setRxCount(size);
    MCU_UARTRxCallback(uart01);
    EXPECT_TRUE(m_isCalled_cbf);
    EXPECT_EQ(1, m_event_cbf.bitfield.RxComplete);
    EXPECT_EQ(0, m_event_cbf.bitfield.RxError);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_ok_Blocking)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = size;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_Error)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    uint32_t sizeFailedReceive = 2;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setRxCount(sizeFailedReceive);
    MCU_UARTRxCallback(uart01);
    EXPECT_TRUE(m_isCalled_cbf);
    EXPECT_EQ(0, m_event_cbf.bitfield.RxComplete);
    EXPECT_EQ(1, m_event_cbf.bitfield.RxError);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_fail)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    uint32_t sizeFailedReceive = 2;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = sizeFailedReceive;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_fail_Blocking)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    uint32_t sizeFailedSend = 2;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = sizeFailedSend;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_notInitialized)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_nullBuffer)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_null_DriveHandle)
{
    UART_Handle uartHandle = (UART_Handle) NULL;
    uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_nullHandle)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T)  Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(NULL, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_invalid_length)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 0x0001FFFF;
    Retcode_T rc;
    uint8_t buffer[size];
    m_isCalled_cbf = false;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_sendCancel)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_Receive(uart01, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setRxCount(2);
    MCU_UARTRxCallback(uart01);
    EXPECT_FALSE(m_isCalled_cbf);
    EXPECT_EQ(0, m_event_cbf.bitfield.RxComplete);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_Receive_sendCancel_fail)
{
    UART_Handle uartHandle = (UART_Handle) 0xC0DE;
    uint32_t size = 10;
    Retcode_T rc;
    m_isCalled_cbf = false;
    m_event_cbf.registerValue = 0;
    uint8_t buffer[size];
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(uartHandle);
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    UART_read_fake.return_val = 0;
    rc = MCU_UART_Receive(uart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setRxCount(size);
    MCU_UARTRxCallback(uart01);
    EXPECT_TRUE(m_isCalled_cbf);
    EXPECT_EQ(1, m_event_cbf.bitfield.RxComplete);
    EXPECT_EQ(0, m_event_cbf.bitfield.RxError);
    rc = MCU_UART_Receive(uart01, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
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
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_GetRxCount(uart01, count);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_nullCount)
{
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_UART_GetRxCount(uart01, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM)
            , rc);
    rc = MCU_UART_Deinitialize(uart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_notInitialized)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    UartDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_GetRxCount(uart01, count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}

TEST_F(BCDS_UARTtest, test_MCU_UART_GetRxCount_wrongCallMode)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    UartDevice Device01((enum BCDS_HAL_CallMode_E)0);
    UART_T uart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_UART_Initialize(uart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    rc = MCU_UART_GetRxCount(uart01, count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE), rc);
}
