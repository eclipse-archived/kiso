/*----------------------------------------------------------------------------*//********************************************************************************
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
//-- instead of BCDS_BSP_BoardConfig.h mock

/* include faked interfaces */
extern "C"
{
#include "BCDS_Retcode_th.hh"
#include "BCDS_Assert_th.hh"

#include "em_device_th.hh"
#include "em_dma_th.hh"

/* BCDS_HALConfig.h file which breaks the unit test implementation.
 *
 * Below four lines hot-fix needs to be replaced by proper implementation.
 */

#include "em_usart_th.hh"

/* include module under test */
#include "SPI.c"

/* Fake error handler */

}
/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END 

class BCDS_SPItest: public testing::Test
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
    }

public:
    static const struct MCU_SPI_Event_S m_EventInit;
    static bool m_isCalled_cbf;
    static uint32_t m_spi_cbf;
    static struct MCU_SPI_Event_S m_event_cbf;
};

const struct MCU_SPI_Event_S BCDS_SPItest::m_EventInit = { 0, 0, 0, 0, 0, 0 };
bool BCDS_SPItest::m_isCalled_cbf = false;

struct MCU_SPI_Event_S BCDS_SPItest::m_event_cbf = m_EventInit;
uint32_t BCDS_SPItest::m_spi_cbf = 0;

//----------------------------------------------------------------------

class SpiDevice
{
public:
    SpiDevice(enum BCDS_HAL_TransferMode_E TransferMode);
    virtual ~SpiDevice();

    uint32_t m_SpiInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_SPI_Handle_S m_Spi; /**< handle of MCU whose address is given to user interface in form SPI_T m_spi */

    uint32_t getAppInterfaceHandle();
};

SpiDevice::SpiDevice(enum BCDS_HAL_TransferMode_E TransferMode)
{
    m_Spi.TransferMode = TransferMode;
    m_Spi.Instance = UART_TEST;

    m_Spi.IrqRxCallback = MCU_SPI_IRQ_RX_Callback;
    m_Spi.IrqTxCallback = MCU_SPI_IRQ_TX_Callback;
    m_Spi.DmaRxCallback = NULL;
    m_Spi.DmaTxCallback = NULL;

    m_SpiInterfaceParam = (intptr_t) &m_Spi;
}

SpiDevice::~SpiDevice()
{
    ;
}

uint32_t SpiDevice::getAppInterfaceHandle()
{
    return m_SpiInterfaceParam; // ((SPI_T)&m_mcuHandle);
}

/**
 * @brief   Test public SPI MCU interface
 * @detail  callback signature: typedef void (*MCU_SPI_Callback_T)(struct MCU_SPI_Event_S event);
 */
void AppTestCallbackFunction(SPI_T spi, struct MCU_SPI_Event_S event)
{
    BCDS_SPItest::m_spi_cbf = (intptr_t) spi;
    BCDS_SPItest::m_event_cbf = event;
    /*
     std::cout << "     --- Hello from AppTestCallbackFunction ---" << std::endl;
     std::cout << "     --- uart: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t) uart << std::endl;
     std::cout << "     --- event: 0x" << std::hex << std::setw(8) << std::setfill('0')
     << (uint32_t) *(uint32_t*) (struct MCU_SPI_Event_S *) &event << std::endl;
     std::cout << "     --- TransferError: " << std::dec << event.TransferError << std::endl;
     std::cout << "     --- TransferComplete: " << std::dec << event.TransferComplete << std::endl;
     std::cout << "     --- Cts: " << std::dec << event.Cts << std::endl;
     std::cout << "     --- Dsr: " << std::dec << event.Dsr << std::endl;
     std::cout << "     --- Dcd: " << std::dec << event.Dcd << std::endl;
     std::cout << "     --- Ri: " << std::dec << event.Ri << std::endl;
     */
    BCDS_SPItest::m_isCalled_cbf = true;
}

TEST_F(BCDS_SPItest, dummyTest)
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

/*************************** SPI TESTS *****************************/
/**
 * @brief   Test public MCU_SPI_Initialize
 * @detail  Retcode_T MCU_SPI_Initialize(SPI_T spi, MCU_SPI_Callback_T callback);
// */
TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_ok)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_nullSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = 0;
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_nullCallbackAndSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = 0;
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_nullCallback)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_wrongTransferMode)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}

/**
 * @brief   Test public MCU_SPI_Deinitialize
 * @detail  Retcode_T MCU_SPI_Deinitialize(SPI_T spi)
 * Dependent on Test public MCU_SPI_Initialize
 */
TEST_F(BCDS_SPItest, test_MCU_SPI_Deinitialize_ok)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Deinitialize_nullSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(0);
    EXPECT_NE(RETCODE_OK, rc);
}

/**
 * @brief   Test public SPI MCU interface MCU_SPI_Send
 * @detail  Retcode_T MCU_SPI_Send(SPI_T spi, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_SPI_Initialize and public MCU_SPI_Deinitialize
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_nullBuffer)
{
    const uint16_t size = 10;
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, NULL, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_sendCancel)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, 0); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}
/**
 * @brief   Test public SPI MCU interface MCU_SPI_Receive
 * @detail  Retcode_T MCU_SPI_Receive(SPI_T spi, uint8_t * buffer, uint32_t len)
 * Dependent on Test public MCU_SPI_Initialize and public MCU_SPI_Deinitialize
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_ok)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_nullBuffer)
{
    const uint16_t size = 10;
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, 0, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_receiveCancel)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, 0); /* receive cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

/**
 * @brief    Test public SPI MCU interface MCU_SPI_Transfer
 * @detail   MCU_SPI_Transfer(SPI_T spi, uint8_t* data_out, uint8_t* data_in, uint32_t numTransfer);
 *           Dependent on Test public MCU_SPI_Initialize, MCU_SPI_Send and MCU_SPI_Receive
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_ok)
{
    const uint32_t size = 1;
    Retcode_T rc;
    uint8_t buffer[ size ] = {0x01};
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);

}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_nullBufferIn)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, NULL, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_nullBufferOut)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer, NULL, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_cancel)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[ size ];
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

/**
 * @brief    Test public SPI MCU interface MCU_SPI_GetDataCount
 * @detail   MCU_SPI_GetDataCount(SPI_T spi)
 * Dependent on Test public MCU_SPI_Initialize, MCU_SPI_Send and MCU_SPI_Receive
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_GetDataCount_ok)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);

    struct MCU_SPI_Handle_S* spi_ptr = (struct MCU_SPI_Handle_S*) spi01;
    spi_ptr->TxXferCount = 2;
    spi_ptr->XferSize = 10;
    uint32_t count = MCU_SPI_GetDataCount(spi01);
    EXPECT_EQ(8, count);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_GetDataCount_nullSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_TRANSFER_MODE_INTERRUPT);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    uint32_t count = MCU_SPI_GetDataCount(0);
    EXPECT_EQ(0, count);
}
