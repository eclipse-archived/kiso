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

extern "C"
{
#include "Kiso_HAL_th.hh"

#define KISO_MODULE_ID 0

#if KISO_FEATURE_SLEEP

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"


#include "em_device_th.hh"
#include "em_int_th.hh"
#include "em_emu_th.hh"

/* Should go to a CoreInstruction Test Header */
void __ISB(void);
void __DSB(void);
void __DMB(void);
void __NOP(void);

FAKE_VOID_FUNC(__ISB);
FAKE_VOID_FUNC(__DSB);
FAKE_VOID_FUNC(__DMB);
FAKE_VOID_FUNC(__NOP);

#include "Sleep.c"

}

/* end of global scope symbol and fake definitions section */

class HAL_MCU_Sleep_Test: public testing::Test
{
protected:

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        SleepBlockCounter[SleepMode_EM1 - 1] = UINT8_C(0);
        SleepBlockCounter[SleepMode_EM2 - 1] = UINT8_C(0);
        SleepBlockCounter[SleepMode_EM3 - 1] = UINT8_C(0);

        RESET_FAKE(EMU_EnterEM1);
        RESET_FAKE(EMU_EnterEM2);
        RESET_FAKE(EMU_EnterEM3);
        RESET_FAKE(EMU_EnterEM4);

        RESET_FAKE(EMU_EM2Block);
        RESET_FAKE(EMU_EM2UnBlock);
        RESET_FAKE(EMU_EM2Block);
        RESET_FAKE(EMU_EM2UnBlock);

        RESET_FAKE(INT_Enable);
        RESET_FAKE(INT_Disable);


    }

    virtual void TearDown()
    {
        ; /* nothing to do if clean up is not required */
    }
};

TEST_F(HAL_MCU_Sleep_Test, ModuleIdValidation)
{
    ASSERT_EQ(KISO_ESSENTIALS_MODULE_ID_SLEEP, KISO_MODULE_ID);
}


TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_Initialize)
{
    Retcode_T rc;

    rc = MCU_Sleep_Initialize();

    ASSERT_EQ(0, SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,SleepBlockCounter[SleepMode_EM3 - 1]);


    ASSERT_EQ(RETCODE_OK, rc);

}

/* ******************************************************************** */
/* Test Counter Increase-Decrease Functions */
/* ********************************************************************* */

TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_IncreaseCounter)
{
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);

    MCU_Sleep_IncreaseCounter(SleepMode_EM1);

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);

    MCU_Sleep_IncreaseCounter(SleepMode_EM1);

    ASSERT_EQ(2,  SleepBlockCounter[SleepMode_EM1 - 1]);

    SleepBlockCounter[SleepMode_EM1 - 1] = UINT8_C(0xFF);

    MCU_Sleep_IncreaseCounter(SleepMode_EM1);

    ASSERT_EQ(0xFF,  SleepBlockCounter[SleepMode_EM1 - 1]);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_DecreaseCounter)
{
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);

    MCU_Sleep_DecreaseCounter(SleepMode_EM1);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);

    SleepBlockCounter[SleepMode_EM1 - 1] = UINT8_C(1);

    MCU_Sleep_DecreaseCounter(SleepMode_EM1);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
}

/* ******************************************************************** */
/* Test Enter Sleep Mode Table */
/* ********************************************************************* */
TEST_F(HAL_MCU_Sleep_Test, MCU_Enter_SleepMode_EM0)
{
    /* Test EM0 */
    MCU_Sleep_EnterSleep(SleepMode_EM0);

    ASSERT_EQ(EMU_EnterEM1_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM2_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM3_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM4_fake.call_count, 0);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Enter_SleepMode_EM1)
{

    /* Test EM1 */
    MCU_Sleep_EnterSleep(SleepMode_EM1);

    ASSERT_EQ(EMU_EnterEM1_fake.call_count, 1);
    ASSERT_EQ(EMU_EnterEM2_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM3_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM4_fake.call_count, 0);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Enter_SleepMode_EM2)
{

    /* Test EM1 */
    MCU_Sleep_EnterSleep(SleepMode_EM2);

    ASSERT_EQ(EMU_EnterEM1_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM2_fake.call_count, 1);
    ASSERT_EQ(EMU_EnterEM3_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM4_fake.call_count, 0);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Enter_SleepMode_EM3)
{

    /* Test EM1 */
    MCU_Sleep_EnterSleep(SleepMode_EM3);

    ASSERT_EQ(EMU_EnterEM1_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM2_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM3_fake.call_count, 1);
    ASSERT_EQ(EMU_EnterEM4_fake.call_count, 0);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Enter_SleepMode_EM4)
{

    /* Test EM1 */
    MCU_Sleep_EnterSleep((enum MCU_SleepMode_E)(SleepMode_EM3 + 1));

    ASSERT_EQ(EMU_EnterEM1_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM2_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM3_fake.call_count, 0);
    ASSERT_EQ(EMU_EnterEM4_fake.call_count, 0);
}

/* ******************************************************************** */
/* Test Blocking Behaviour */
/* ********************************************************************* */
TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_BlockSleepMode_FAIL)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_BlockSleepMode((uint32_t)SleepMode_EM3 + 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2Block_fake.call_count);
}


TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_BlockSleepMode_EM0)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_BlockSleepMode((uint32_t)SleepMode_EM0);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2Block_fake.call_count);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_BlockSleepMode_EM1)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_BlockSleepMode((uint32_t)SleepMode_EM1);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2Block_fake.call_count);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_BlockSleepMode_EM2)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_BlockSleepMode((uint32_t)SleepMode_EM2);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(1, EMU_EM2Block_fake.call_count);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_BlockSleepMode_EM3)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_BlockSleepMode((uint32_t)SleepMode_EM3);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2Block_fake.call_count);
}

TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_BlockSleepMode_EM4)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_BlockSleepMode((uint32_t)SleepMode_EM3 + 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2Block_fake.call_count);
}

/* ******************************************************************** */
/* Test UnBlocking Behaviour */
/* ********************************************************************* */
TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_UnblockSleepMode_FAIL)
{
    Retcode_T rc;
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_UnblockSleepMode((uint32_t)SleepMode_EM3 + 1);

    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2UnBlock_fake.call_count);
}


TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_UnblockSleepMode_EM1)
{
    Retcode_T rc;

    SleepBlockCounter[SleepMode_EM1 - 1] = 1;
    SleepBlockCounter[SleepMode_EM2 - 1] = 1;
    SleepBlockCounter[SleepMode_EM3 - 1] = 1;

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);


    /* Test EM1 */
    rc = MCU_Sleep_UnblockSleepMode((uint32_t)SleepMode_EM1);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2UnBlock_fake.call_count);
}


TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_UnblockSleepMode_EM2)
{
    Retcode_T rc;

    SleepBlockCounter[SleepMode_EM1 - 1] = 1;
    SleepBlockCounter[SleepMode_EM2 - 1] = 2;
    SleepBlockCounter[SleepMode_EM3 - 1] = 1;


    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(2,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_UnblockSleepMode((uint32_t)SleepMode_EM2);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2UnBlock_fake.call_count);

    rc = MCU_Sleep_UnblockSleepMode((uint32_t)SleepMode_EM2);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(1, EMU_EM2UnBlock_fake.call_count);

}


TEST_F(HAL_MCU_Sleep_Test, MCU_Sleep_UnblockSleepMode_EM3)
{
    Retcode_T rc;

    SleepBlockCounter[SleepMode_EM1 - 1] = 1;
    SleepBlockCounter[SleepMode_EM2 - 1] = 1;
    SleepBlockCounter[SleepMode_EM3 - 1] = 1;

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM3 - 1]);

    /* Test EM1 */
    rc = MCU_Sleep_UnblockSleepMode((uint32_t)SleepMode_EM3);

    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM1 - 1]);
    ASSERT_EQ(1,  SleepBlockCounter[SleepMode_EM2 - 1]);
    ASSERT_EQ(0,  SleepBlockCounter[SleepMode_EM3 - 1]);

    ASSERT_EQ(0, EMU_EM2UnBlock_fake.call_count);
}


TEST_F(HAL_MCU_Sleep_Test, GetLowestSleepMode)
{

    /*
     * Reference: https://siliconlabs.github.io/Gecko_SDK_Doc/efm32gg/html/group__SLEEP.html
     */
    uint32_t mode;

     /* Step 0 */
     mode =  MCU_Sleep_GetLowestSleepMode(); /* Lowest SleepMode is 3 */
     ASSERT_EQ(SleepMode_EM3,  mode);

     /* Step 1*/
     MCU_Sleep_BlockSleepMode(SleepMode_EM3); /* Block Sleep mode EM3. Lowest Sleep mode is 2 */
     mode =  MCU_Sleep_GetLowestSleepMode();
     ASSERT_EQ(SleepMode_EM2,  mode);

     /* Step 2 */
     MCU_Sleep_BlockSleepMode(SleepMode_EM2); /* Block Sleep mode EM3. Lowest Sleep mode is 1 */
     mode =  MCU_Sleep_GetLowestSleepMode();
     ASSERT_EQ(SleepMode_EM1,  mode);

     /* Step 3*/
     MCU_Sleep_BlockSleepMode(SleepMode_EM2); /* Block Sleep mode EM2. Lowest Sleep mode is still 1 */
     mode =  MCU_Sleep_GetLowestSleepMode();
     ASSERT_EQ(SleepMode_EM1,  mode);

     /* Step 4 */
     MCU_Sleep_UnblockSleepMode(SleepMode_EM3); /* Unblock Sleep mode EM3. Lowest Sleep mode is still 1 */
     mode =  MCU_Sleep_GetLowestSleepMode();
     ASSERT_EQ(SleepMode_EM1,  mode);

     /* Step 5 */
     MCU_Sleep_UnblockSleepMode(SleepMode_EM2); /* Unblock Sleep mode EM2. Lowest Sleep mode is still 1 */
     mode =  MCU_Sleep_GetLowestSleepMode();
     ASSERT_EQ(SleepMode_EM1,  mode);

     /* Step 3 */
     MCU_Sleep_UnblockSleepMode(SleepMode_EM2); /* Unblock Sleep mode EM2. Lowest Sleep mode now 3 */
     mode =  MCU_Sleep_GetLowestSleepMode();
     ASSERT_EQ(SleepMode_EM3,  mode);

}

#else
}
#endif