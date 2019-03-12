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

/* include gtest interface */
#include <gtest.h>

/* start of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_START

/* include faked interfaces */
extern "C"
{
#include "BCDS_Retcode_th.hh"
#include "BCDS_HAL_th.hh"
#include "em_device_th.hh"
#include "em_dma_th.hh"
#include "em_int_th.hh"
#include "em_msc_th.hh"

//#include "em_msc_th.hh"

/* include module under test */
#include "Flash.c"

//uint32_t flash_data[10];
//#define FLASH_ADDRESS &flash_data[0]
}

/* end of global scope symbol and fake definitions section */
FFF_DEFINITION_BLOCK_END

class BCDS_FLASHtest: public testing::Test
{
protected:

    virtual void SetUp()
    {
        MSC_WriteWord_fake.return_val = mscReturnOk;

        FFF_RESET_HISTORY()
        ;
    }

    virtual void TearDown()
    {
        RESET_FAKE(MSC_WriteWord);
        RESET_FAKE(MSC_Init);
        RESET_FAKE(MSC_ErasePage);
        RESET_FAKE(INT_Enable);
        RESET_FAKE(INT_Disable);
        RESET_FAKE(MSC_Deinit);
        ; /* nothing to do */
    }
};
/*************************** Flash TESTS *****************************/

TEST_F(BCDS_FLASHtest, dummyTest)
{

}

/**
 * @brief   Test public  MCU_Flash_GetPageSize
 * @detail  uint32_t MCU_Flash_GetPageSize(void)
 */

TEST_F(BCDS_FLASHtest, MCU_Flash_GetPageSize)
{
    uint32_t size;
    size = MCU_Flash_GetPageSize();
    EXPECT_EQ(4096, size);
}

/**
 * @brief   Test public  MCU_Flash_ErasePage
 * @detail  Retcode_T MCU_Flash_ErasePage(uint32_t * pageAddress)
 * Dependent on Test public MCU_Flash_GetPageSize
 */

TEST_F(BCDS_FLASHtest, MCU_Flash_ErasePage_ok)
{
    uint32_t * pageAddress = (uint32_t*) 0x20000;
    MSC_ErasePage_fake.return_val = mscReturnOk;
    Retcode_T rc;

    rc = MCU_Flash_ErasePage(pageAddress);

    EXPECT_EQ(UINT32_C(1), MSC_ErasePage_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Init_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Disable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Enable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Deinit_fake.call_count);

    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_FLASHtest, MCU_Flash_EraseInvalid_addr)
{
    uint32_t * pageAddress = (uint32_t*) 0x20000;
    MSC_ErasePage_fake.return_val = mscReturnInvalidAddr;
    Retcode_T rc;

    rc = MCU_Flash_ErasePage(pageAddress);

    EXPECT_EQ(UINT32_C(1), MSC_ErasePage_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Init_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Disable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Enable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Deinit_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_HAL_FLASH_INVALID_ADDRESS), rc);
}


TEST_F(BCDS_FLASHtest, MCU_Flash_EraseInvalid_addrInput)
{
    uint32_t * pageAddress = (uint32_t*) 0x20001;
    MSC_ErasePage_fake.return_val = mscReturnInvalidAddr;
    Retcode_T rc;

    rc = MCU_Flash_ErasePage(pageAddress);

    EXPECT_EQ(UINT32_C(0), MSC_ErasePage_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MSC_Init_fake.call_count);
    EXPECT_EQ(UINT32_C(0), INT_Disable_fake.call_count);
    EXPECT_EQ(UINT32_C(0), INT_Enable_fake.call_count);
    EXPECT_EQ(UINT32_C(0), MSC_Deinit_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_HAL_FLASH_INVALID_ADDRESS), rc);
}


TEST_F(BCDS_FLASHtest, MCU_Flash_EraseAddr_Locked)
{
    uint32_t * pageAddress = (uint32_t*) 0x20000;
    MSC_ErasePage_fake.return_val = mscReturnLocked;
    Retcode_T rc;

    rc = MCU_Flash_ErasePage(pageAddress);

    EXPECT_EQ(UINT32_C(1), MSC_ErasePage_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Init_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Disable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Enable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Deinit_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_HAL_FLASH_ADDRESS_LOCKED), rc);
}

TEST_F(BCDS_FLASHtest, MCU_Flash_Erase_timeout)
{
    uint32_t * pageAddress = (uint32_t*) 0x20000;
    MSC_ErasePage_fake.return_val = mscReturnTimeOut;
    Retcode_T rc;

    rc = MCU_Flash_ErasePage(pageAddress);

    EXPECT_EQ(UINT32_C(1), MSC_ErasePage_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Init_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Disable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), INT_Enable_fake.call_count);
    EXPECT_EQ(UINT32_C(1), MSC_Deinit_fake.call_count);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_HAL_FLASH_TIMEOUT), rc);
}
/**
 * @brief   Test public  MCU_Flash_Erase
 * @detail  Retcode_T MCU_Flash_Erase(uint32_t* startAddress, uint32_t numPages)
 * Dependent on Test public MCU_Flash_GetPageSize
 */

TEST_F(BCDS_FLASHtest, MCU_Flash_Erase)
{
    uint32_t * pageAddress = (uint32_t*) 0x20000;
    uint32_t numPages = 2;

    Retcode_T rc;
    rc = MCU_Flash_Erase(pageAddress, numPages);

    EXPECT_EQ(UINT32_C(2), MSC_ErasePage_fake.call_count);
    EXPECT_EQ(RETCODE_OK, rc);
}

/**
 * @brief   Test public  MCU_Flash_Write
 * @detail  Retcode_T MCU_Flash_Write( const uint8_t* targetAddress, const uint8_t* sourceAddress,
 const uint32_t numBytes)
 * Dependent on Test public MCU_Flash_GetPageSize
 */

TEST_F(BCDS_FLASHtest, MCU_Flash_Write_ok)
{
    uint8_t tempAddr;
    uint8_t * targetAddress = &tempAddr;
    uint8_t tempAddr2;
    uint8_t * sourceAddress = &tempAddr2;

    uint32_t numBytes = 2;

    Retcode_T rc;
    rc = MCU_Flash_Write(targetAddress, sourceAddress, numBytes);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_FLASHtest, MCU_Flash_Write_fail)
{
    uint8_t tempAddr;
    uint8_t * targetAddress = &tempAddr;
    uint8_t tempAddr2;
    uint8_t * sourceAddress = &tempAddr2;

    uint32_t numBytes = 2;
    MSC_WriteWord_fake.return_val = mscReturnLocked;

    Retcode_T rc;
    rc = MCU_Flash_Write(targetAddress, sourceAddress, numBytes);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_HAL_FLASH_ADDRESS_LOCKED), rc);
}

/**
 * @brief   Test public  MCU_Flash_Read
 * @detail  Retcode_T MCU_Flash_Read( uint8_t* readFrom, uint8_t* writeTo, uint32_t numBytes)
 * Dependent on Test public MCU_Flash_GetPageSize
 */

TEST_F(BCDS_FLASHtest, MCU_Flash_Read_ok)
{
    uint8_t tempAddr;
    uint8_t * targetAddress = &tempAddr;
    uint8_t tempAddr2;
    uint8_t * sourceAddress = &tempAddr2;

    uint32_t numBytes = 2;

    Retcode_T rc;
    rc = MCU_Flash_Read(targetAddress, sourceAddress, numBytes);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(BCDS_FLASHtest, MCU_Flash_Read_readFromNull)
{
    uint8_t tempAddr2;
    uint8_t * sourceAddress = &tempAddr2;
    uint32_t numBytes = 2;

    Retcode_T rc;
    rc = MCU_Flash_Read(NULL, sourceAddress, numBytes);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}

TEST_F(BCDS_FLASHtest, MCU_Flash_Read_writeToNull)
{
    uint8_t tempAddr2;
    uint8_t * targetAddress = &tempAddr2;
    uint32_t numBytes = 2;

    Retcode_T rc;
    rc = MCU_Flash_Read(targetAddress, NULL, numBytes);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}
