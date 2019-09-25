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
 *
 * @brief
 *      Module test specification for the I2CTranseiver_unittest.cc module.
 * 
 * @detail
 *      The unit test file template follows the Four-Phase test pattern.
 * 
 * @file
 */

/* Include gtest interface */
#include <gtest.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
    /* Setup compile time configuration defines */

#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_I2C_TRANSCEIVER

#if KISO_FEATURE_I2CTRANSCEIVER
/* Include faked interfaces */
#include "Kiso_Retcode_th.hh"
#include "Kiso_MCU_I2C_th.hh"
#include "FreeRTOS_th.hh"
#include "semphr_th.hh"

    uint32_t tempI2CHandle = 0x55;
    I2C_T I2CHandle = (I2C_T)&tempI2CHandle;

/* Include module under test */
#include "I2CTransceiver.c"

    /* End of global scope symbol and fake definitions section */
}
/* Create test fixture initializing all variables automatically */
class I2CTransceiver : public testing::Test
{
public:
    I2cTranceiverHandle_T TranceiverHandle;

protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        TranceiverHandle.I2CBusSync = (xSemaphoreHandle)0x1234;
        TranceiverHandle.I2CHandle = I2CHandle;
        TranceiverHandle.I2CMutexLock = (xSemaphoreHandle)0x1234;
        TranceiverHandle.I2cTransferStatusFlag = INT8_C(0);
        TranceiverHandle.InitializationStatus = false;
        RESET_FAKE(xSemaphoreCreateMutex);
        RESET_FAKE(xSemaphoreCreateBinary);
        RESET_FAKE(xSemaphoreTake);
        RESET_FAKE(xSemaphoreGive);
        RESET_FAKE(MCU_I2C_ReadRegister);
        RESET_FAKE(MCU_I2C_Send);
        RESET_FAKE(MCU_I2C_WriteRegister);
        RESET_FAKE(MCU_I2C_Deinitialize);
        RESET_FAKE(Retcode_RaiseErrorFromIsr);
        RESET_FAKE(xSemaphoreGiveFromISR);

        FFF_RESET_HISTORY();
    }
};

/* Specify test cases ******************************************************* */

TEST_F(I2CTransceiver, I2CTransceiverInitSuccess)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverInitSuccess: }
     * This test case tests I2CTransceiver_Init() for success case
     */

    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateBinary_fake.return_val = (xSemaphoreHandle)0x124;
    xSemaphoreCreateMutex_fake.return_val = (xSemaphoreHandle)0x124;
    TranceiverHandle.I2CHandle = I2CHandle;
    retVal = I2CTransceiver_Init(&TranceiverHandle, I2CHandle);

    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
    EXPECT_EQ((xSemaphoreHandle)0x124, TranceiverHandle.I2CBusSync);
    EXPECT_TRUE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverInitNULLHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverInitNULLHandle: }
     * This test case tests I2CTransceiver_Init() when TranceiverHandle is passed as NULL
     */

    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateBinary_fake.return_val = (xSemaphoreHandle)0x1234;
    xSemaphoreCreateMutex_fake.return_val = (xSemaphoreHandle)0x1234;

    retVal = I2CTransceiver_Init(NULL, I2CHandle);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverInitNULLI2CHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverInitNULLI2CHandle: }
     * This test case tests I2CTransceiver_Init() when I2CHandle is passed as NULL
     */

    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateBinary_fake.return_val = (xSemaphoreHandle)0x1234;
    xSemaphoreCreateMutex_fake.return_val = (xSemaphoreHandle)0x1234;

    retVal = I2CTransceiver_Init(&TranceiverHandle, NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverInitDoppleInit)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverInitDoppleInit: }
     * This test case tests I2CTransceiver_Init() when it is already initialzed
     */

    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateBinary_fake.return_val = (xSemaphoreHandle)0x1234;
    xSemaphoreCreateMutex_fake.return_val = (xSemaphoreHandle)0x1234;
    TranceiverHandle.InitializationStatus = true;

    retVal = I2CTransceiver_Init(&TranceiverHandle, I2CHandle);

    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreCreateMutex_fake.call_count);
    EXPECT_TRUE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverInitSemaphoreFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverInitSemaphoreFail: }
     * This test case tests I2CTransceiver_Init() when  xSemaphoreCreateBinary() is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateBinary_fake.return_val = (xSemaphoreHandle)NULL;
    xSemaphoreCreateMutex_fake.return_val = (xSemaphoreHandle)0x1234;

    retVal = I2CTransceiver_Init(&TranceiverHandle, I2CHandle);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverInitMutexFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverInitMutexFail: }
     * This test case tests I2CTransceiver_Init() when  xSemaphoreCreateMutex() is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    xSemaphoreCreateBinary_fake.return_val = (xSemaphoreHandle)0x1234;
    xSemaphoreCreateMutex_fake.return_val = (xSemaphoreHandle)NULL;

    retVal = I2CTransceiver_Init(&TranceiverHandle, I2CHandle);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_OUT_OF_RESOURCES), retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateBinary_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreCreateMutex_fake.call_count);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverReadZeroByte)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadZeroByte: }
     * This test case tests I2CTransceiver_Read() when user tries to read zero bytes
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 5;
    uint8_t NbrBytesToRead = 0;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
    EXPECT_TRUE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverReadNullReg)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadNullReg: }
     * This test case tests I2CTransceiver_Read() when Null is passed for RegData
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t *RegData = NULL;
    uint8_t NbrBytesToRead = 10;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
    EXPECT_TRUE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverReadNullTransceiver)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadNullTransceiver: }
     * This test case tests I2CTransceiver_Read() when Null is passed for TranceiverHandle
     */

    Retcode_T retVal = RETCODE_FAILURE;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 5;
    uint8_t NbrBytesToRead = 10;

    retVal = I2CTransceiver_Read(NULL, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadNullBusSyn)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadNullBusSyn: }
     * This test case tests I2CTransceiver_Read() when Null is passed as any one of the argument
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.I2CBusSync = NULL;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadNullMutex)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadNullMutex: }
     * This test case tests I2CTransceiver_Read() when Null is passed as any one of the argument
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.I2CMutexLock = NULL;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadNullI2cHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadNullI2cHandle: }
     * This test case tests I2CTransceiver_Read() when Null is passed as any one of the argument
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.I2CHandle = NULL;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadUnInitialized)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadUnInitialized: }
     * This test case tests I2CTransceiver_Read() when it is called without initialization
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = false;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadMutexTakeFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadMutexTakeFail: }
     * This test case tests I2CTransceiver_Read() when xSemaphoreTake is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    xSemaphoreTake_fake.return_val = pdFALSE;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR), retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadFail: }
     * This test case tests I2CTransceiver_Read() when  MCU_I2C_ReadRegister is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_ReadRegister_fake.return_val = RETCODE_FAILURE;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE_FAILURE, retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverSynFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverSynFail: }
     * This test case tests I2CTransceiver_Read() when  xSemaphoreTake is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    BaseType_t SemaphoreRetcode[2] = {pdTRUE, pdFALSE};
    xSemaphoreTake_fake.return_val_seq = SemaphoreRetcode;
    xSemaphoreTake_fake.return_val_seq_len = 2;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_ReadRegister_fake.return_val = RETCODE_OK;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverSendFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverSendFail: }
     * This test case tests I2CTransceiver_Read() when  xSemaphoreTake is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    BaseType_t SemaphoreRetcode[2] = {pdTRUE, pdFALSE};
    xSemaphoreTake_fake.return_val_seq = SemaphoreRetcode;
    xSemaphoreTake_fake.return_val_seq_len = 2;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_ReadRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_FAILURE;

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE_FAILURE, retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadErrorTransfer)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadErrorTransfer: }
     * This test case tests I2CTransceiver_Read() when Error is updated in status flag
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_ReadRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_OK;
    TranceiverHandle.I2cTransferStatusFlag = INT8_C(-1);

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR), retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadSemaphoreGiveFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadSemaphoreGiveFail: }
     * This test case tests I2CTransceiver_Read() when xSemaphoreGive is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdFALSE;
    MCU_I2C_ReadRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_OK;
    TranceiverHandle.I2cTransferStatusFlag = INT8_C(0);

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_SEMAPHORE_ERROR), retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverReadSuccess)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverReadSuccess: }
     * This test case tests I2CTransceiver_Read() for success case
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToRead = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_ReadRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_OK;
    TranceiverHandle.I2cTransferStatusFlag = INT8_C(0);

    retVal = I2CTransceiver_Read(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToRead);

    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_ReadRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteZeroByte)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteZeroByte: }
     * This test case tests I2CTransceiver_Write() when user tries to Write zero bytes
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 5;
    uint8_t NbrBytesToWrite = 0;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
    EXPECT_TRUE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteNullReg)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteNullReg: }
     * This test case tests I2CTransceiver_Write() when Null is passed for RegData
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t *RegData = NULL;
    uint8_t NbrBytesToWrite = 10;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
    EXPECT_TRUE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteNullTransceiver)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteNullTransceiver: }
     * This test case tests I2CTransceiver_Write() when Null is passed for TranceiverHandle
     */

    Retcode_T ReturnValue = RETCODE_FAILURE;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 5;
    uint8_t NbrBytesToWrite = 10;

    ReturnValue = I2CTransceiver_Write(NULL, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), ReturnValue);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteNullBusSyn)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteNullBusSyn: }
     * This test case tests I2CTransceiver_Write() when Null is passed as any one of the argument
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.I2CBusSync = NULL;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteNullMutex)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteNullMutex: }
     * This test case tests I2CTransceiver_Write() when Null is passed as any one of the argument
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.I2CMutexLock = NULL;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteNullI2cHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteNullI2cHandle: }
     * This test case tests I2CTransceiver_Write() when Null is passed as any one of the argument
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.I2CHandle = NULL;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteUnInitialized)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteUnInitialized: }
     * This test case tests I2CTransceiver_Write() when it is called without initialization
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = false;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), retVal);
    EXPECT_EQ(UINT32_C(0), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteMutexTakeFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteMutexTakeFail: }
     * This test case tests I2CTransceiver_Write() when xSemaphoreTake is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    xSemaphoreTake_fake.return_val = pdFALSE;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_SEMAPHORE_ERROR), retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteFail: }
     * This test case tests I2CTransceiver_Write() when  MCU_I2C_WriteRegister is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_WriteRegister_fake.return_val = RETCODE_FAILURE;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE_FAILURE, retVal);
    EXPECT_EQ(UINT32_C(1), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteSynFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverSynFail: }
     * This test case tests I2CTransceiver_Write() when  xSemaphoreTake is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    BaseType_t SemaphoreRetcode[2] = {pdTRUE, pdFALSE};
    xSemaphoreTake_fake.return_val_seq = SemaphoreRetcode;
    xSemaphoreTake_fake.return_val_seq_len = 2;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_WriteRegister_fake.return_val = RETCODE_OK;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_TIMEOUT), retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteSendFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteSendFail: }
     * This test case tests I2CTransceiver_Write() when  xSemaphoreTake is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    BaseType_t SemaphoreRetcode[2] = {pdTRUE, pdFALSE};
    xSemaphoreTake_fake.return_val_seq = SemaphoreRetcode;
    xSemaphoreTake_fake.return_val_seq_len = 2;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_WriteRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_FAILURE;

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE_FAILURE, retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteErrorTransfer)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteErrorTransfer: }
     * This test case tests I2CTransceiver_Write() when Error is updated in status flag
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_WriteRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_OK;
    TranceiverHandle.I2cTransferStatusFlag = INT8_C(-1);

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR), retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteSemaphoreGiveFail)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteSemaphoreGiveFail: }
     * This test case tests I2CTransceiver_Write() when xSemaphoreGive is returning failure
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdFALSE;
    MCU_I2C_WriteRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_OK;
    TranceiverHandle.I2cTransferStatusFlag = INT8_C(0);

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_SEMAPHORE_ERROR), retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverWriteSuccess)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverWriteSuccess: }
     * This test case tests I2CTransceiver_Write() for success case
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CHandle = I2CHandle;
    uint8_t I2Caddr = 0x68;
    uint8_t Reg = 0x80;
    uint8_t RegData = 4;
    uint8_t NbrBytesToWrite = 10;
    xSemaphoreTake_fake.return_val = pdTRUE;
    xSemaphoreGive_fake.return_val = pdTRUE;
    MCU_I2C_WriteRegister_fake.return_val = RETCODE_OK;
    MCU_I2C_Send_fake.return_val = RETCODE_OK;
    TranceiverHandle.I2cTransferStatusFlag = INT8_C(0);

    retVal = I2CTransceiver_Write(&TranceiverHandle, I2Caddr, Reg, &RegData, NbrBytesToWrite);

    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_EQ(UINT32_C(2), xSemaphoreTake_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MCU_I2C_WriteRegister_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MCU_I2C_Send_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGive_fake.call_count);
}

TEST_F(I2CTransceiver, I2CTransceiverDeInitNULLHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverDeInitNULLHandle: }
     * This test case tests I2CTransceiver_Deinit() when TranceiverHandle is passed as NULL
     */

    Retcode_T retVal = RETCODE_FAILURE;

    retVal = I2CTransceiver_Deinit(NULL);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverDeInitNULLSynHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverDeInitNULLSynHandle: }
     * This test case tests I2CTransceiver_Deinit() when any of the parameter is passed as NULL
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CBusSync = NULL;
    retVal = I2CTransceiver_Deinit(&TranceiverHandle);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
}

TEST_F(I2CTransceiver, I2CTransceiverDeInitNULLMutexHandle)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverDeInitNULLMutexHandle: }
     * This test case tests I2CTransceiver_Deinit() when any of the parameter is passed as NULL
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CMutexLock = NULL;
    retVal = I2CTransceiver_Deinit(&TranceiverHandle);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), retVal);
}

TEST_F(I2CTransceiver, I2CTransceiverDeInitUnintialized)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverDeInitUnintialized: }
     * This test case tests I2CTransceiver_Deinit() when it is called without initialization
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = false;

    retVal = I2CTransceiver_Deinit(&TranceiverHandle);

    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverDeInitSuccess)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverDeInitSuccess: }
     * This test case tests I2CTransceiver_Deinit() for success
     */

    Retcode_T retVal = RETCODE_FAILURE;
    TranceiverHandle.InitializationStatus = true;
    retVal = I2CTransceiver_Deinit(&TranceiverHandle);

    EXPECT_EQ(RETCODE_OK, retVal);
    EXPECT_FALSE(TranceiverHandle.InitializationStatus);
}

TEST_F(I2CTransceiver, I2CTransceiverLoopCallbackNullError)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverLoopCallbackNullError: }
     * This test case tests I2CTransceiver_LoopCallback() when Transceiver handle is null
     */

    struct MCU_I2C_Event_S event;

    I2CTransceiver_LoopCallback(NULL, event);

    EXPECT_EQ(UINT32_C(0), xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseErrorFromIsr_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Retcode_RaiseErrorFromIsr_fake.arg0_val);
}

TEST_F(I2CTransceiver, I2CTransceiverLoopCallbackUninitialized)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverLoopCallbackUninitialized: }
     * This test case tests I2CTransceiver_LoopCallback() when it is called without initialization
     */

    struct MCU_I2C_Event_S event;
    TranceiverHandle.InitializationStatus = false;

    I2CTransceiver_LoopCallback(&TranceiverHandle, event);

    EXPECT_EQ(UINT32_C(0), xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(UINT32_C(1), Retcode_RaiseErrorFromIsr_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED), Retcode_RaiseErrorFromIsr_fake.arg0_val);
}

TEST_F(I2CTransceiver, I2CTransceiverLoopCallbackSyncNull)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverLoopCallbackSyncNull: }
     * This test case tests I2CTransceiver_LoopCallback() when semaphore handle is null
     */

    struct MCU_I2C_Event_S event;
    TranceiverHandle.InitializationStatus = true;
    TranceiverHandle.I2CBusSync = NULL;

    I2CTransceiver_LoopCallback(&TranceiverHandle, event);

    EXPECT_EQ(UINT32_C(1), Retcode_RaiseErrorFromIsr_fake.call_count);
    EXPECT_EQ(UINT32_C(0), xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), Retcode_RaiseErrorFromIsr_fake.arg0_val);
}

TEST_F(I2CTransceiver, I2CTransceiverLoopCallbackTransferError)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverLoopCallbackTransferError: }
     * This test case tests I2CTransceiver_LoopCallback() when there is an error in transfer
     */

    struct MCU_I2C_Event_S event;
    TranceiverHandle.InitializationStatus = true;
    event.TransferError = 1;
    event.TxComplete = 0;

    I2CTransceiver_LoopCallback(&TranceiverHandle, event);

    EXPECT_EQ(UINT32_C(1), Retcode_RaiseErrorFromIsr_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGiveFromISR_fake.call_count);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_I2CTRANSCEIVER_TRANSFER_ERROR), Retcode_RaiseErrorFromIsr_fake.arg0_val);
}

TEST_F(I2CTransceiver, I2CTransceiverLoopCallbackTransferSuccess)
{
    /** @testcase{ I2CTransceiver::I2CTransceiverLoopCallbackTransferSuccess: }
     * This test case tests I2CTransceiver_LoopCallback() when the transfer is success
     */

    struct MCU_I2C_Event_S event;
    TranceiverHandle.InitializationStatus = true;
    event.TransferError = 0;
    event.TxComplete = 1;

    I2CTransceiver_LoopCallback(&TranceiverHandle, event);

    EXPECT_EQ(UINT32_C(0), Retcode_RaiseErrorFromIsr_fake.call_count);
    EXPECT_EQ(UINT32_C(1), xSemaphoreGiveFromISR_fake.call_count);
}

#else
}
#endif /* #if KISO_FEATURE_I2CTRANSCEIVER */
