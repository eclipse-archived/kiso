/*******************************************************************************
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
 ******************************************************************************/

#include <gtest.h>

extern "C"
{
#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_INIT

#include "Engine_th.hh"
#include "Kiso_Retcode_th.hh"

#undef KISO_MODULE_ID
#include "Init.c"
}

class TS_Smoke : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }
};

TEST_F(TS_Smoke, Compile_Success)
{
}

FAKE_VOID_FUNC(Fake_OnStateChanged, Cellular_State_T, Cellular_State_T, void *, uint32_t)

class TS_Cellular_Initialize : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(Engine_Initialize);
        RESET_FAKE(Fake_OnStateChanged);
    }
};

TEST_F(TS_Cellular_Initialize, Ok)
{
    Engine_Initialize_fake.return_val = RETCODE_OK;

    Retcode_T rc = Cellular_Initialize(Fake_OnStateChanged);

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(1U, Engine_Initialize_fake.call_count);
    EXPECT_EQ(Fake_OnStateChanged, Engine_Initialize_fake.arg0_val);
}

TEST_F(TS_Cellular_Initialize, EngineInitFail)
{
    Engine_Initialize_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    Retcode_T rc = Cellular_Initialize(Fake_OnStateChanged);

    EXPECT_EQ(Engine_Initialize_fake.return_val, rc);
    EXPECT_EQ(1U, Engine_Initialize_fake.call_count);
}

class TS_Cellular_Deinitialize : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();

        RESET_FAKE(Engine_Deinitialize);
    }
};

TEST_F(TS_Cellular_Deinitialize, Ok)
{
    Engine_Deinitialize_fake.return_val = RETCODE_OK;

    Retcode_T rc = Cellular_Deinitialize();

    EXPECT_EQ(RETCODE_OK, rc);
    EXPECT_EQ(1U, Engine_Deinitialize_fake.call_count);
}

TEST_F(TS_Cellular_Deinitialize, EngineDeinitFail)
{
    Engine_Deinitialize_fake.return_val = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    Retcode_T rc = Cellular_Deinitialize();

    EXPECT_EQ(Engine_Deinitialize_fake.return_val, rc);
    EXPECT_EQ(1U, Engine_Deinitialize_fake.call_count);
}
