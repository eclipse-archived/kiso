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
#if BCDS_FEATURE_LEUART == 1
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

#include "em_device_th.hh"
#include "em_dma_th.hh"

#include "em_leuart_th.hh"

typedef HWHandle_T LEUART_T;

/* include module under test */
#include "LEUART.c"

}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END 

class BCDS_LEUARTtest: public testing::Test
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
    static const union MCU_LEUART_Event_U m_EventInit;
    static const union MCU_LEUART_Event_U m_EventRxReady;
    static const union MCU_LEUART_Event_U m_EventRxError;
    static const union MCU_LEUART_Event_U m_EventRxComplete;
    static const union MCU_LEUART_Event_U m_EventTxError;
    static const union MCU_LEUART_Event_U m_EventTxComplete;
    static bool m_isCalled_cbf;
    static uint32_t m_leuart_cbf;
    static union MCU_LEUART_Event_U m_event_cbf;
};

const union MCU_LEUART_Event_U BCDS_LEUARTtest::m_EventInit =
{   .bitfield =
    {   0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const union MCU_LEUART_Event_U BCDS_LEUARTtest::m_EventRxReady =
{   .bitfield =
    {   1, 0, 0, 0, 0, 0, 0, 0, 0}
};

const union MCU_LEUART_Event_U BCDS_LEUARTtest::m_EventRxError =
{   .bitfield =
    {   0, 1, 0, 0, 0, 0, 0, 0, 0}
};
const union MCU_LEUART_Event_U BCDS_LEUARTtest::m_EventRxComplete =
{   .bitfield =
    {   0, 0, 1, 0, 0, 0, 0, 0, 0}
};
const union MCU_LEUART_Event_U BCDS_LEUARTtest::m_EventTxError =
{   .bitfield =
    {   0, 0, 0, 1, 0, 0, 0, 0, 0}
};
const union MCU_LEUART_Event_U BCDS_LEUARTtest::m_EventTxComplete =
{   .bitfield =
    {   0, 0, 0, 0, 1, 0, 0, 0, 0}
};

bool BCDS_LEUARTtest::m_isCalled_cbf = false;

union MCU_LEUART_Event_U BCDS_LEUARTtest::m_event_cbf = m_EventInit;
uint32_t BCDS_LEUARTtest::m_leuart_cbf = 0;

//----------------------------------------------------------------------

class LeuartDevice
{
public:
    LeuartDevice(enum BCDS_HAL_TransferMode_E TransferMode);
    virtual ~LeuartDevice();

    uint32_t m_LeuartInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_LEUART_Handle_S m_Leuart; /**< handle of MCU whose address is given to user interface in form LEUART_T m_Leuart */

    LEUART_TypeDef m_Instance;

    uint32_t getAppInterfaceHandle();
};

LeuartDevice::LeuartDevice(enum BCDS_HAL_TransferMode_E TransferMode)
{
    m_Leuart.TransferMode = TransferMode;
    m_Leuart.leuart_ptr = LEUART_TEST;

    m_Leuart.IrqRxCallback = MCU_LEUART_IRQ_RX_Callback;
    m_Leuart.IrqTxCallback = MCU_LEUART_IRQ_TX_Callback;
    m_Leuart.DmaRxCallback = NULL;
    m_Leuart.DmaTxCallback = NULL;
    m_Leuart.CtsCallback = NULL;
    m_Leuart.RtsCallback = NULL;
    m_Leuart._DriverCtx.RxState = LEUART_STATE_INIT;
    m_Leuart._DriverCtx.TxState = LEUART_STATE_INIT;

    m_LeuartInterfaceParam = (intptr_t) &m_Leuart;
}

LeuartDevice::~LeuartDevice()
{
    ;
}

uint32_t LeuartDevice::getAppInterfaceHandle()
{
    return m_LeuartInterfaceParam; // ((LEUART_T)&m_mcuHandle);
}

/**
 * @brief   Test public LEUART MCU interface HalUart_MCU_UART_Power
 * @detail  callback signature: typedef void (*MCU_LEUART_Callback_T)(LEUART_T leuart, struct MCU_LEUART_Event_S event);
 *
 */
void AppTestCallbackFunction(LEUART_T leuart, struct MCU_LEUART_Event_S event)
{
    BCDS_LEUARTtest::m_leuart_cbf = (intptr_t) leuart;
    BCDS_LEUARTtest::m_event_cbf.bitfield = event;
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
    BCDS_LEUARTtest::m_isCalled_cbf = true;
}

/*************************** LEUART TESTS *****************************/
/**
 * @brief   Test public MCU_LEUART_Initialize
 * @detail  Retcode_T MCU_LEUART_Initialize(LEUART_T leuart, MCU_LEUART_Callback_T callback);
 *
 */
TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Initialize_ok)
{
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}
TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Initialize_nullLeuart)
{
    Retcode_T rc;
    LEUART_T leuart01 = 0;
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Initialize_nullCallbackandLeuart)
{
    Retcode_T rc;
    LEUART_T leuart01 = 0;
    rc = MCU_LEUART_Initialize(leuart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Initialize_nullCallback)
{
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Initialize_wrongTransferMode)
{
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
}


/**
 * @brief       Test public LEUART MCU interface MCU_UART_Deinitialize
 * @detail      MCU_LEUART_Deinitialize(LEUART_T leuart)
 * Dependent on Test public MCU_UART_Initialize
 */

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Deinitialize_ok)
{
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Deinitialize(leuart01);
    EXPECT_EQ(RETCODE_OK, rc);
}



TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Deinitialize_nullLeuart)
{
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

/**
 * @brief   Test public LEUART MCU interface MCU_LEUART_Send
 * @detail  MCU_LEUART_Send(LEUART_T leuart, uint8_t * data, uint32_t len)
 *  * Dependent on Test public MCU_UART_Initialize and MCU_UART_Deinitialize
 */
TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Send_ok)
{
    const uint32_t size = 10;
    Retcode_T rc = RETCODE_OK;
    uint8_t buffer[size];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Send(leuart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Deinitialize(leuart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
#if 0
TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Send_notInitialized)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Send(leuart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Send_nullBuffer)
{
    const uint16_t size = 10;
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Send(leuart01, NULL, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}
#endif
TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Send_sendCancel)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Send(leuart01, buffer, 0); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Deinitialize(leuart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Send_wrongTransferMode)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    rc = MCU_LEUART_Send(leuart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), rc);
}

/**
 * @brief   Test public LEUART MCU interface MCU_LEUART_Receive
 * @detail  Retcode_T MCU_LEUART_Receive(LEUART_T leuart, uint8_t* data, uint32_t len)
 * Dependent on Test public MCU_UART_Initialize and MCU_UART_Deinitialize
 */

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Receive_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Receive(leuart01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Deinitialize(leuart01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Receive_nullBuffer)
{
    const uint16_t size = 10;
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Receive(leuart01, 0, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Receive_receiveCancel)
{
    const uint16_t size = 1;
    Retcode_T rc;
    uint8_t buffer[ size ] = {0x01};
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_Receive(leuart01, buffer, 0); /* receive cancel */
    EXPECT_EQ(RETCODE_OK, (Retcode_T)rc);
    rc = MCU_LEUART_Deinitialize(leuart01);
    EXPECT_EQ(RETCODE_OK, rc);
}
#if 0
TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Receive_notInitialized)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Receive(leuart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_Receive_wrongTransferMode)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
    rc = MCU_LEUART_Receive(leuart01, buffer, size);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}
#endif

/**
 * @brief   Test public LEUART MCU interface MCU_LEUART_GetRxCount
 * @detail   MCU_UART_GetRxReadyCount(LEUART_T uart, uint32_t * count)
 * Dependent on Test public MCU_UART_Initialize and MCU_UART_Deinitialize
 */

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_GetRxCount_ok)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_GetRxCount(leuart01, count);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_GetRxCount_nullCount)
{
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_LEUART_GetRxCount(leuart01, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_GetRxCount_notInitialized)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_GetRxCount(leuart01, count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(BCDS_LEUARTtest, test_MCU_LEUART_GetRxCount_wrongTransferMode)
{
    uint32_t temp = 10;
    uint32_t * count = &temp;
    Retcode_T rc;
    LeuartDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    LEUART_T leuart01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_LEUART_Initialize(leuart01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED), rc);
    rc = MCU_LEUART_GetRxCount(leuart01, count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}
#endif /* BCDS_FEATURE_LEUART */
