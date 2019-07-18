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
 *  @file
 *
 *  Module test specification for the SPI_unittest module.
 *
 * The unit test file template follows the Four-Phase test pattern. 
 *
 * ****************************************************************************/

/* include gtest interface */
#include <gtest.h>

/* include module under test */
extern "C"
{
#include "BCDS_Basics.h"

#if BCDS_FEATURE_SPI
/* include faked interfaces */
#include "BCDS_HAL_th.hh"
#include "stm32l4xx_th.hh"
#include "stm32l4xx_hal_dma_th.hh"
#include "stm32l4xx_hal_spi_th.hh"

/* Include unit under test */
#include "SPI.c"

} /* extern "C"*/

class SPITestContext
{
public:
    SPITestContext()
    {
        mTestAppCallbackCount = 0;
        // Configure DMA mode by default
        mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;
        SPITestContext::TestContext = this;
    }
    // A static pointer that will be set upon setup to be able to access
    // the test instance from within static functions
    static SPITestContext* TestContext;

    // Declare a static function used as application callback for SPI
    static void TestAppCallback(SPI_T spi, struct MCU_SPI_Event_S event);

    uint32_t mTestAppCallbackCount;
    SPI_HandleTypeDef mSTMHandleSPI;
    struct MCU_SPI_S mBSPHandleSPI;
    struct MCU_SPI_Event_S mLastEvent;
};

// static member variable
SPITestContext* SPITestContext::TestContext = NULL;
// static member function
void SPITestContext::TestAppCallback(SPI_T spi, struct MCU_SPI_Event_S event)
{
    if (TestContext)
    {
        UNUSED(spi);
        TestContext->mLastEvent = event;
        // Do something useful, e.g. increment a counter
        TestContext->mTestAppCallbackCount++;
    }
}

uint32_t HAL_SPI_GetErrorCustomFake(SPI_HandleTypeDef *hspi)
{
  /* Return SPI ErrorCode */
  return hspi->ErrorCode;
}

// Create an instance for the test context
SPITestContext testContext;
SPI_HandleTypeDef spiHandle;
SPI_TypeDef spiDef;

class BCDS_SPItest: public testing::Test
{
public:

protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        memset(&spiHandle, 0, sizeof(spiHandle));
        memset(&spiDef, 0, sizeof(spiDef));
        spiDef.SR = 0x00;
        spiHandle.Instance = &spiDef;
        testContext.mBSPHandleSPI.hspi.Instance = &spiDef;
    }

    virtual void TearDown()
    {
        RESET_FAKE(HAL_SPI_Init);
        RESET_FAKE(HAL_SPI_DeInit);
        //RESET_FAKE(HAL_SPI_GetState);
        RESET_FAKE(HAL_SPI_Transmit_IT);
        RESET_FAKE(HAL_SPI_Receive_IT);
        RESET_FAKE(HAL_SPI_TransmitReceive_IT);
        RESET_FAKE(HAL_SPI_Transmit_DMA);
        RESET_FAKE(HAL_SPI_Receive_DMA);
        RESET_FAKE(HAL_SPI_TransmitReceive_DMA);
        RESET_FAKE(HAL_SPI_GetError);
        RESET_FAKE(HAL_SPI_IRQHandler);
    }
};



TEST_F(BCDS_SPItest, testSPI_initialize)
{
    /* here we test the MCU_SPI_Initialize function */

    /* call initialize function without a valid SPI handler */
    Retcode_T rc = MCU_SPI_Initialize(0, SPITestContext::TestAppCallback);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_INVALID_PARAM), rc);

    /* call initialize function with missing callback function  */
    rc = MCU_SPI_Initialize((SPI_T) &testContext.mBSPHandleSPI, NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_INVALID_PARAM), rc);

    /* call initialize function with correct parameters but unsupported transfer modes */
    testContext.mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_BLOCKING;
    rc = MCU_SPI_Initialize((SPI_T) &testContext.mBSPHandleSPI, &SPITestContext::TestAppCallback);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL,RETCODE_NOT_SUPPORTED), rc);

    testContext.mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;
    /* call initialize function with correct parameters  */
    rc = MCU_SPI_Initialize((SPI_T) &testContext.mBSPHandleSPI, &SPITestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK, rc);

    testContext.mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT_RX_DMA_TX;
    /* call initialize function with correct parameters  */
    rc = MCU_SPI_Initialize((SPI_T) &testContext.mBSPHandleSPI, &SPITestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_SEVERITY_WARNING, Retcode_GetSeverity(rc));
    EXPECT_EQ(RETCODE_NOT_SUPPORTED, Retcode_GetCode(rc));

    /* make copy of SPI object */
    SPITestContext testContextBackup = testContext;
    rc = MCU_SPI_Deinitialize((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(RETCODE_OK, rc);

    /* copy object back */
    testContext = testContextBackup;

    testContext.mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;
    /* call initialize function with correct parameters  */
    rc = MCU_SPI_Initialize((SPI_T) &testContext.mBSPHandleSPI, &SPITestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK, rc);

    testContext.mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

    /* make copy of SPI object */
    testContextBackup = testContext;
    rc = MCU_SPI_Deinitialize((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(RETCODE_OK, rc);

    /* copy object back */
    testContext = testContextBackup;

    /* restore initial state */
    testContext.mBSPHandleSPI.TransferMode = BCDS_HAL_TRANSFER_MODE_DMA;
    /* call initialize function with correct parameters in DMA mode for further tests  */
    rc = MCU_SPI_Initialize((SPI_T) &testContext.mBSPHandleSPI, &SPITestContext::TestAppCallback);
    EXPECT_EQ(RETCODE_OK, rc);

}

TEST_F(BCDS_SPItest, testSPI_Send)
{
    uint32_t len = 1024;
    uint8_t buffer[len];

    /* First try with wrong handle */
    Retcode_T rc = MCU_SPI_Send(0, buffer, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_INVALID_PARAM), rc);

    /* Then use a valid handle, but invalid buffer */
    rc = MCU_SPI_Send((SPI_T) &testContext.mBSPHandleSPI, NULL, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Then use a valid handle, valid buffer but invalid len */
    rc = MCU_SPI_Send((SPI_T) &testContext.mBSPHandleSPI, buffer, UINT16_MAX + 1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Then use a valid handle, valid buffer but invalid len */
    rc = MCU_SPI_Send((SPI_T) &testContext.mBSPHandleSPI, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* STM32 transmit function return is valid */
    HAL_SPI_Transmit_DMA_fake.return_val = HAL_OK;
    /* Send with all valid parameters */
    rc = MCU_SPI_Send((SPI_T) &testContext.mBSPHandleSPI, buffer, len);
    EXPECT_EQ(RETCODE_OK, rc);

    /* STM32 transmit function return is not valid */
    HAL_SPI_Transmit_DMA_fake.return_val = HAL_ERROR;
    rc = MCU_SPI_Send((SPI_T) &testContext.mBSPHandleSPI, buffer, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);

}

TEST_F(BCDS_SPItest, testSPI_Receive)
{
    uint32_t len = 1024;
    uint8_t buffer[len];

    /* First try with wrong handle */
    Retcode_T rc = MCU_SPI_Receive(0, NULL, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_INVALID_PARAM), rc);

    /* Then use a valid handle but invalid buffer and length */
    rc = MCU_SPI_Receive((SPI_T) &testContext.mBSPHandleSPI, NULL, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Then use a valid handle and buffer but invalid length */
    rc = MCU_SPI_Receive((SPI_T) &testContext.mBSPHandleSPI, buffer, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Then use a valid handle, valid buffer but invalid length */
    rc = MCU_SPI_Receive((SPI_T) &testContext.mBSPHandleSPI, buffer, UINT16_MAX + 1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* STM32 receive function return is valid */
    HAL_SPI_Receive_DMA_fake.return_val = HAL_OK;
    /* Receive with valid parameters */
    rc = MCU_SPI_Receive((SPI_T) &testContext.mBSPHandleSPI, buffer, len);
    EXPECT_EQ(RETCODE_OK, rc);

    /* STM32 receive function return is not valid */
    HAL_SPI_Receive_DMA_fake.return_val = HAL_ERROR;
    /* Receive with valid parameters */
    rc = MCU_SPI_Receive((SPI_T) &testContext.mBSPHandleSPI, buffer, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR,RETCODE_FAILURE), rc);

}

TEST_F(BCDS_SPItest, testSPI_Transfer)
{
    uint32_t len = 1024;
    uint8_t data_in[len];
    uint8_t data_out[len];

    /* First try with wrong handle */
    Retcode_T rc = MCU_SPI_Transfer(0, NULL, NULL, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_INVALID_PARAM), rc);

    /* Then use a valid handle but invalid data buffers and length */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, NULL, NULL, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Try with valid handle, valid data_out and length, invalid data_in */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, data_out, NULL, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Try with valid handle, valid data_in and length, invalid data_in */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, NULL, data_in, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Try with valid handle, valid buffers, invalid length */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, data_out, data_in, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* Try with valid handle, valid buffers, invalid length */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, data_out, data_in, UINT16_MAX + 1);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

    /* STM32 receive function return is invalid */
    HAL_SPI_TransmitReceive_DMA_fake.return_val = HAL_ERROR;
    /* Try with valid parameters */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, data_out, data_in, len);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE), rc);

    /* STM32 receive function return is valid */
    HAL_SPI_TransmitReceive_DMA_fake.return_val = HAL_OK;
    /* Try with valid parameters */
    rc = MCU_SPI_Transfer((SPI_T) &testContext.mBSPHandleSPI, data_out, data_in, len);
    EXPECT_EQ(RETCODE_OK, rc);

}

TEST_F(BCDS_SPItest, testSPI_GetDataCount)
{
    // First try with wrong handle */
    uint32_t count = MCU_SPI_GetDataCount(0);
    EXPECT_EQ(UINT32_C(0), count);

    // Then use a valid handle
    count = MCU_SPI_GetDataCount((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(UINT32_C(0), count);

    HAL_SPI_GetState_fake.return_val = HAL_SPI_STATE_BUSY_RX;
    // Then use a valid handle
    count = MCU_SPI_GetDataCount((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(UINT32_C(0), count);

    HAL_SPI_GetState_fake.return_val = HAL_SPI_STATE_BUSY_TX;
    // Then use a valid handle
    count = MCU_SPI_GetDataCount((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(UINT32_C(0), count);

    HAL_SPI_GetState_fake.return_val = HAL_SPI_STATE_BUSY_TX_RX;
    // Then use a valid handle
    count = MCU_SPI_GetDataCount((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(UINT32_C(0), count);

}

/**
 * This test is currently just used to call the SPI interrupt callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_MCU_BSP_SPI_IRQ_Callback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_SPI_IRQHandler);
    MCU_BSP_SPI_IRQ_Callback(0);
    EXPECT_EQ(0, HAL_SPI_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_SPI_IRQ_Callback((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(1, HAL_SPI_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_SPI_IRQHandler);
}

/**
 * This test is currently just used to call the SPI DMA Rx callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_MCU_BSP_SPI_DMA_RX_Callback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_DMA_IRQHandler);
    MCU_BSP_SPI_DMA_RX_Callback(0);
    EXPECT_EQ(0, HAL_DMA_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_SPI_DMA_RX_Callback((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(1, HAL_DMA_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_DMA_IRQHandler);
}

/**
 * This test is currently just used to call the SPI DMA Tx callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_MCU_BSP_SPI_DMA_TX_Callback)
{
    /* First try with totally wrong parameters */
	RESET_FAKE(HAL_DMA_IRQHandler);
    MCU_BSP_SPI_DMA_TX_Callback(0);
    EXPECT_EQ(0, HAL_DMA_IRQHandler_fake.call_count);
    /* Then use a valid handle */
    MCU_BSP_SPI_DMA_TX_Callback((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(1, HAL_DMA_IRQHandler_fake.call_count);
    RESET_FAKE(HAL_DMA_IRQHandler);
}

/**
 * This test is currently just used to call the SPI error callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_HAL_SPI_ErrorCallback)
{
    /* struct MCU_SPI_Event_S Events = { 0, 0, 0, 0, 0, 0 }; */

//#define HAL_SPI_ERROR_NONE   (uint32_t)0x00000000  /*!< No error                          */
//#define HAL_SPI_ERROR_MODF   (uint32_t)0x00000001  /*!< MODF error                        */
//#define HAL_SPI_ERROR_CRC    (uint32_t)0x00000002  /*!< CRC error                         */
//#define HAL_SPI_ERROR_OVR    (uint32_t)0x00000004  /*!< OVR error                         */
//#define HAL_SPI_ERROR_FRE    (uint32_t)0x00000008  /*!< FRE error                         */
//#define HAL_SPI_ERROR_DMA    (uint32_t)0x00000010  /*!< DMA transfer error                */
//#define HAL_SPI_ERROR_FLAG   (uint32_t)0x00000020  /*!< Error on BSY/TXE/FTLVL/FRLVL Flag */
//#define HAL_SPI_ERROR_UNKNOW (uint32_t)0x00000040  /*!< Unknown error                     */
	//HAL_SPI_GetError_fake.custom_fake = HAL_SPI_GetErrorCustomFake;
    /* First try with totally wrong parameters */
    HAL_SPI_ErrorCallback(NULL);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_NONE;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(0,testContext.mLastEvent.RxReady);
    EXPECT_EQ(0,testContext.mLastEvent.RxError);
    EXPECT_EQ(0,testContext.mLastEvent.RxComplete);
    EXPECT_EQ(0,testContext.mLastEvent.TxError);
    EXPECT_EQ(0,testContext.mLastEvent.TxComplete);
    EXPECT_EQ(0,testContext.mLastEvent.DataLoss);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_MODF;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.DataLoss);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_CRC;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.DataLoss);
    EXPECT_EQ(1,testContext.mLastEvent.RxError);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_OVR;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.DataLoss);
    EXPECT_EQ(1,testContext.mLastEvent.RxError);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_FRE;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.DataLoss);
    EXPECT_EQ(1,testContext.mLastEvent.RxError);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_DMA;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.DataLoss);
    EXPECT_EQ(1,testContext.mLastEvent.RxError);
    EXPECT_EQ(1,testContext.mLastEvent.TxError);

    /* Fake return of STM32 SPI GetError function */
    HAL_SPI_GetError_fake.return_val = HAL_SPI_ERROR_FLAG;
    /* Then use a valid handle */
    HAL_SPI_ErrorCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.DataLoss);
    EXPECT_EQ(1,testContext.mLastEvent.RxError);
    EXPECT_EQ(1,testContext.mLastEvent.TxError);

}

/**
 * This test is currently just used to call the SPI Tx complete callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_HAL_SPI_TxCpltCallback)
{
    /* First try with totally wrong parameters */
	testContext.mTestAppCallbackCount = 0;
    HAL_SPI_TxCpltCallback(NULL);
    EXPECT_EQ(0,testContext.mTestAppCallbackCount);
    /* Then use a valid handle */
    HAL_SPI_TxCpltCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.TxComplete);
    EXPECT_EQ(1,testContext.mTestAppCallbackCount);
}

/**
 * This test is currently just used to call the SPI Rx complete callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_HAL_SPI_RxCpltCallback)
{
    /* First try with totally wrong parameters */
	testContext.mTestAppCallbackCount = 0;
    HAL_SPI_RxCpltCallback(NULL);
    EXPECT_EQ(0,testContext.mTestAppCallbackCount);
    /* Then use a valid handle */
    HAL_SPI_RxCpltCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.RxComplete);
    EXPECT_EQ(1,testContext.mTestAppCallbackCount);
}

/**
 * This test is currently just used to call the SPI Tx/Rx complete callback
 * and to see if it can be called without problems
 */
TEST_F(BCDS_SPItest, testSPI_HAL_SPI_TxRxCpltCallback)
{
    /* First try with totally wrong parameters */
	testContext.mTestAppCallbackCount = 0;
    HAL_SPI_TxRxCpltCallback(NULL);
    EXPECT_EQ(0,testContext.mTestAppCallbackCount);
    /* Then use a valid handle */
    HAL_SPI_TxRxCpltCallback(&testContext.mBSPHandleSPI.hspi);
    EXPECT_EQ(1,testContext.mLastEvent.RxComplete);
    EXPECT_EQ(1,testContext.mLastEvent.TxComplete);
    EXPECT_EQ(1,testContext.mTestAppCallbackCount);
}

/**
 * @attention This should be the last test case executed because it deintializes
 * the local SPI Handle of the testContext.
 * After that test it is necessary to call MCU_SPI_Initialize again if more tests
 * should be run after this one.
 */
TEST_F(BCDS_SPItest, testSPI_DeInitializer)
{
    /* First try with totally wrong parameters */
    Retcode_T rc = MCU_SPI_Deinitialize(0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_INVALID_PARAM), rc);
    /* Then use the correct handle */
    rc = MCU_SPI_Deinitialize((SPI_T) &testContext.mBSPHandleSPI);
    EXPECT_EQ(RETCODE_OK, rc);
    /* check if the pointers are reset to default */
    EXPECT_EQ((uint32_t)testContext.mBSPHandleSPI.AppCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleSPI.IRQCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleSPI.DmaTxCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleSPI.DmaRxCallback,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleSPI.RxFunPtr,NULL);
    EXPECT_EQ((uint32_t)testContext.mBSPHandleSPI.TxFunPtr,NULL);
    /* After this any call except to initalize should fail */
    rc = MCU_SPI_Send((SPI_T) &testContext.mBSPHandleSPI, NULL, 0);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_WARNING,RETCODE_NOT_SUPPORTED), rc);

}
/*****************************************************************************************/
#else
}
#endif /* BCDS_FEATURE_SPI */
