/*----------------------------------------------------------------------------*//*----------------------------------------------------------------------------*/
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

#include "drivers\SPI.h"

/* Add vendor specific fake functions needed */
FAKE_VALUE_FUNC(bool, SPI_transfer,SPI_Handle , SPI_Transaction *)
;
FAKE_VOID_FUNC(SPI_transferCancel, SPI_Handle)
;

#undef BCDS_FEATURE_SPI
#define BCDS_FEATURE_SPI 1

/* include module under test */
#include "BCDS_MCU_SPI.c"

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
    SpiDevice(enum BCDS_HAL_CallMode_E CallMode);
    virtual ~SpiDevice();

    uint32_t m_SpiInterfaceParam; /**< user interface handle, simple integer */

    struct MCU_SPI_Handle_S m_Spi; /**< handle of MCU whose address is given to user interface in form SPI_T m_spi */

    uint32_t getAppInterfaceHandle();
    void setDriveHandle(SPI_Handle handle);
    void setTransactionStatus(SPI_Status status);

};

SpiDevice::SpiDevice(enum BCDS_HAL_CallMode_E CallMode)
{
    m_Spi.handle = NULL;
    m_Spi.callMode = CallMode;
    m_Spi.state = MCU_SPI_STATE_INIT;

    m_Spi.readFunc = NULL;
    m_Spi.writeFunc = NULL;
    m_Spi.transferFunc = NULL;
    m_Spi.abortFunc = NULL;

    m_Spi.appCallback = NULL;

    m_Spi.transferCallback = NULL;

    m_Spi.count = 0;
    m_Spi.transaction.count = 0;
    m_Spi.transaction.txBuf = NULL;
    m_Spi.transaction.rxBuf = NULL;
    m_Spi.transaction.arg = NULL;
    m_Spi.transaction.status = SPI_TRANSFER_COMPLETED;

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

void SpiDevice::setDriveHandle(SPI_Handle handle)
{
    m_Spi.handle = handle;
}

void SpiDevice::setTransactionStatus(SPI_Status status)
{
    m_Spi.transaction.status = status;
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

/*----------------------------------------------------------------------------*/

/**
 * @brief   Test public MCU_SPI_Initialize
 * @detail  Retcode_T MCU_SPI_Initialize(SPI_T spi, MCU_SPI_Callback_T callback);
 // */
TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_ok_interruptMode)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_ok_blockingMode)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_doppleInit_interruptMode)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}
TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_doppleInit_blockingMode)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_nullSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = 0;
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_nullCallbackAndSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = 0;
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_nullCallback)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, NULL);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Initialize_wrongCallMode)
{
    Retcode_T rc;
    SpiDevice Device01((enum BCDS_HAL_CallMode_E)0);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_NE(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public MCU_SPI_Deinitialize
 * @detail  Retcode_T MCU_SPI_Deinitialize(SPI_T spi)
 * Dependent on Test public MCU_SPI_Initialize
 */
TEST_F(BCDS_SPItest, test_MCU_SPI_Deinitialize_ok)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Deinitialize_dopple)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Deinitialize_nullSpi)
{
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(0);
    EXPECT_NE(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public SPI MCU interface MCU_SPI_Send
 * @detail  Retcode_T MCU_SPI_Send(SPI_T spi, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_SPI_Initialize and public MCU_SPI_Deinitialize
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_ok_interrupt)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_not_completed)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTransactionStatus(SPI_TRANSFER_CANCELED);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_failure)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = false;
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_ok_blocking)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, 0);
    EXPECT_NE(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_invalidLength)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Send(spi01, buffer, 0x0001FFFF);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_nullHandle)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle((SPI_Handle) NULL);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_nullBuffer)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Send(spi01, NULL, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_nullspi)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    uint8_t buffer[size];
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Send(NULL, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_invalidState)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Send(spi01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_sendCancel)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, 10); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, 0); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Send_sendCancel_invalidState)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Send(spi01, buffer, 10); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Send(spi01, buffer, 0); /* send cancel */
    EXPECT_NE(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief   Test public SPI MCU interface MCU_SPI_Receive
 * @detail  Retcode_T MCU_SPI_Receive(SPI_T spi, uint8_t * data, uint32_t len)
 * Dependent on Test public MCU_SPI_Initialize and public MCU_SPI_Deinitialize
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_ok_interrupt)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_not_completed)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTransactionStatus(SPI_TRANSFER_CANCELED);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_failure)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = false;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_ok_blocking)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, 0);
    EXPECT_NE(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_invalidLength)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Receive(spi01, buffer, 0x0001FFFF);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_nullHandle)
{
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle((SPI_Handle) NULL);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_nullBuffer)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Receive(spi01, NULL, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_nullspi)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    uint8_t buffer[size];
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Receive(NULL, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_invalidState)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Receive(spi01, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_sendCancel)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, 10); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, 0); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Receive_sendCancel_invalidState)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Receive(spi01, buffer, 10); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Receive(spi01, buffer, 0); /* send cancel */
    EXPECT_NE(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief    Test public SPI MCU interface MCU_SPI_Transfer
 * @detail   MCU_SPI_Transfer(SPI_T spi, uint8_t* data_out, uint8_t* data_in, uint32_t numTransfer);
 *           Dependent on Test public MCU_SPI_Initialize, MCU_SPI_Send and MCU_SPI_Receive
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_ok_interrupt)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint32_t size = 1;
    Retcode_T rc;
    uint8_t buffer[size] = { 0x01 };
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}


TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_not_completed)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Transfer(spi01, buffer,buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setTransactionStatus(SPI_TRANSFER_CANCELED);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}


TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_ok_blocking)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint32_t size = 1;
    Retcode_T rc;
    uint8_t buffer[size] = { 0x01 };
    SpiDevice Device01(BCDS_HAL_CALL_MODE_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, 0);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_failure)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = false;
    rc = MCU_SPI_Transfer(spi01, buffer,buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_invalidLength)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint32_t size = 1;
    Retcode_T rc;
    uint8_t buffer[size] = { 0x01 };
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, 0x0001FFFF);
    EXPECT_NE(RETCODE_OK, rc);
}


TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_nullhandle)
{
    SPI_Handle spiHandle = (SPI_Handle) NULL;
    const uint32_t size = 1;
    Retcode_T rc;
    uint8_t buffer[size] = { 0x01 };
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}


TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_nullspi)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint32_t size = 1;
    Retcode_T rc;
    uint8_t buffer[size] = { 0x01 };
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(NULL, buffer, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_nullBufferIn)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, NULL, buffer, size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_nullBufferOut)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, buffer, NULL, size);
    EXPECT_NE(RETCODE_OK, rc);
}


TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_invalidState)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    SPI_transfer_fake.return_val = true;
    rc = MCU_SPI_Transfer(spi01, buffer, buffer ,size);
    EXPECT_NE(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_cancel)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, size);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer, buffer, 0);
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_Transfer_cancel_invalidState)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    const uint16_t size = 10;
    Retcode_T rc;
    uint8_t buffer[size];
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    Device01.setDriveHandle(spiHandle);
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    rc = MCU_SPI_Transfer(spi01, buffer,buffer, 10); /* send cancel */
    EXPECT_EQ(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Transfer(spi01, buffer,buffer, 0); /* send cancel */
    EXPECT_NE(RETCODE_OK, rc);
    MCU_SPICallback(spi01);
    rc = MCU_SPI_Deinitialize(spi01);
    EXPECT_EQ(RETCODE_OK, rc);
}
/*----------------------------------------------------------------------------*/
/**
 * @brief    Test public SPI MCU interface MCU_SPI_GetDataCount
 * @detail   MCU_SPI_GetDataCount(SPI_T spi)
 * Dependent on Test public MCU_SPI_Initialize, MCU_SPI_Send and MCU_SPI_Receive
 */

TEST_F(BCDS_SPItest, test_MCU_SPI_GetDataCount_ok)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    struct MCU_SPI_Handle_S* spi_ptr = (struct MCU_SPI_Handle_S*) spi01;
    spi_ptr->count = 2;
    uint32_t count = MCU_SPI_GetDataCount(spi01);
    EXPECT_EQ(2, count);
}

TEST_F(BCDS_SPItest, test_MCU_SPI_GetDataCount_nullSpi)
{
    SPI_Handle spiHandle = (SPI_Handle) 0xC0DE;
    Retcode_T rc;
    SpiDevice Device01(BCDS_HAL_CALL_MODE_NON_BLOCKING);
    SPI_T spi01 = (HWHandle_T) Device01.getAppInterfaceHandle();
    rc = MCU_SPI_Initialize(spi01, AppTestCallbackFunction);
    EXPECT_EQ(RETCODE_OK, rc);
    Device01.setDriveHandle(spiHandle);
    uint32_t count = MCU_SPI_GetDataCount(0);
    EXPECT_EQ(0, count);
}
