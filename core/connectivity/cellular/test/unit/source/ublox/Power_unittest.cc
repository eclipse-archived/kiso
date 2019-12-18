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
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_POWER

#include "FreeRTOS_th.hh"
#include "task_th.hh"
#include "Engine_th.hh"
#include "AtResponseQueue_th.hh"
#include "Hardware_th.hh"
#include "At3Gpp27007_th.hh"
#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "Kiso_Logging_th.hh"

#undef LOG_ERROR
#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARNING

#define LOG_INFO(...)
#define LOG_ERROR(...)
#define LOG_DEBUG(...)
#define LOG_WARNING(...)

#undef KISO_MODULE_ID
#include "Power.c"
}

class TS_Power : public testing::Test
{
protected:
    virtual void SetUp()
    {
        RESET_FAKE(At_Set_UTEST);
        RESET_FAKE(At_Set_COPS);
        RESET_FAKE(At_Set_CEREG);
        RESET_FAKE(At_Set_CGREG);
        RESET_FAKE(At_Set_CMEE);
        RESET_FAKE(At_Set_UDCONF);
        RESET_FAKE(At_Test_AT);
        RESET_FAKE(vTaskDelay);
        RESET_FAKE(Engine_SetFlukeCharFilterEnabled);
        RESET_FAKE(Engine_Dispatch);
        FFF_RESET_HISTORY();
    }
    const char *TestPin = "00001200034";
    const char *TestPinNone = "";
    void *dummyParameter = 0U;
    uint32_t dummyParameterLength = 0U;
};

TEST_F(TS_Power, Power_SetupModem_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(1U, vTaskDelay_fake.call_count);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_SetupModem_At_Set_UTEST_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, rc);
}

TEST_F(TS_Power, Power_SetupModem_AT_Set_CPIN_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SetupModem_At_Set_COPS_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SetupModem_At_Set_CEREG_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SetupModem_At_Set_CGREG_Fail)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SetupModem_At_Set_CMEE_Fail)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SetupModem_At_Set_UDCONF_Fail)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    rc = Power_SetupModem();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_ProbeForResponsiveness_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Test_AT_fake.return_val = RETCODE_OK;
    rc = ProbeForResponsiveness();
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_ProbeForResponsiveness_Fail)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Test_AT_fake.return_val = RETCODE_FAILURE;
    rc = ProbeForResponsiveness();
    EXPECT_EQ(RETCODE_FAILURE, rc);
}

TEST_F(TS_Power, Power_Cellular_PowerOn_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    const Cellular_PowerUpParameters_T *dummyParameters = (const Cellular_PowerUpParameters_T *)TestPin;
    Engine_Dispatch_fake.return_val = RETCODE_OK;
    rc = Cellular_PowerOn(dummyParameters);
    EXPECT_NE((Cellular_PowerUpParameters_T *)NULL, dummyParameters);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_Cellular_PowerOn_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    rc = Cellular_PowerOn(NULL);
    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(TS_Power, Power_CheckCellular_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Test_AT_fake.return_val = RETCODE_OK;
    IsPoweredOn = false;
    rc = CheckCellular(&dummyParameter, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_CheckCellular_isPowered_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Test_AT_fake.return_val = RETCODE_OK;
    bool PowerState = true;
    IsPoweredOn = true;
    rc = CheckCellular(&PowerState, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_CheckCellular_isPowered_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Test_AT_fake.return_val = RETCODE_OK;
    bool PowerState = false;
    IsPoweredOn = false;
    rc = CheckCellular(&PowerState, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_ShutdownCellular_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Hardware_Shutdown_fake.return_val = RETCODE_OK;
    rc = ShutdownCellular(&dummyParameter, dummyParameterLength);
    EXPECT_EQ(1U, Engine_NotifyNewState_fake.call_count);
    EXPECT_EQ(false, IsPoweredOn);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_ShutdownCellular_HW_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    RESET_FAKE(Engine_NotifyNewState);
    Hardware_Shutdown_fake.return_val = RETCODE_FAILURE;
    rc = ShutdownCellular(&dummyParameter, dummyParameterLength);
    EXPECT_NE(1U, Engine_NotifyNewState_fake.call_count);
    EXPECT_EQ(RETCODE_FAILURE, rc);
}

TEST_F(TS_Power, Power_Cellular_PowerOff_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Engine_Dispatch_fake.return_val = RETCODE_OK;
    rc = Cellular_PowerOff();
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_Cellular_PowerOff_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Engine_Dispatch_fake.return_val = RETCODE_FAILURE;
    rc = Cellular_PowerOff();
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_Cellular_Reset_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    const Cellular_PowerUpParameters_T *dummyParameters = (const Cellular_PowerUpParameters_T *)TestPin;
    rc = Cellular_Reset(dummyParameters);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_Cellular_Reset_InvalidParam_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    rc = Cellular_Reset(NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_Cellular_Reset_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Engine_Dispatch_fake.return_val = RETCODE_FAILURE;
    const Cellular_PowerUpParameters_T *dummyParameters = (const Cellular_PowerUpParameters_T *)TestPin;
    rc = Cellular_Reset(dummyParameters);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_StartupCellular_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);

    Cellular_PowerUpParameters_T *goodParam = (Cellular_PowerUpParameters_T *)Pin;
    dummyParameterLength = sizeof(goodParam);
    goodParam->SimPin = "00001200034";
    Hardware_Startup_fake.return_val = RETCODE_OK;
    At_Test_AT_fake.return_val = RETCODE_OK;
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    rc = StartupCellular(goodParam, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_StartupCellular_Pin_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Cellular_PowerUpParameters_T *badParam = (Cellular_PowerUpParameters_T *)Pin;
    dummyParameterLength = sizeof(badParam);
    badParam->SimPin = NULL;
    Hardware_Startup_fake.return_val = RETCODE_OK;
    At_Test_AT_fake.return_val = RETCODE_OK;
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    rc = StartupCellular(badParam, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_StartupCellular_AT_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    dummyParameterLength = sizeof(TestPin);
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    Hardware_Startup_fake.return_val = RETCODE_OK;
    At_Test_AT_fake.return_val = RETCODE_INVALID_PARAM;
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    rc = StartupCellular(&TestPin, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_StartupCellular_Power_SetupModem_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    At_Set_UTEST_fake.return_val = RETCODE_FAILURE;
    At_Set_COPS_fake.return_val = RETCODE_FAILURE;
    At_Set_CEREG_fake.return_val = RETCODE_FAILURE;
    At_Set_CGREG_fake.return_val = RETCODE_FAILURE;
    At_Set_CMEE_fake.return_val = RETCODE_FAILURE;
    At_Set_UDCONF_fake.return_val = RETCODE_FAILURE;
    At_Set_CPIN_fake.return_val = RETCODE_FAILURE;
    strncpy(Pin, TestPin, sizeof(Pin) - 1);
    dummyParameterLength = sizeof(TestPin);
    Hardware_Startup_fake.return_val = RETCODE_OK;
    rc = StartupCellular(&TestPin, dummyParameterLength);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_StartupCellular_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    dummyParameterLength = sizeof(TestPin);
    Hardware_Startup_fake.return_val = RETCODE_FAILURE;
    rc = StartupCellular(&TestPin, dummyParameterLength);
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_Cellular_IsPoweredOn_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    rc = Cellular_IsPoweredOn(&IsPoweredOn);
    Engine_Dispatch_fake.return_val = RETCODE_OK;
    EXPECT_NE((bool *)NULL, &IsPoweredOn);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_Cellular_IsPoweredOn_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    rc = Cellular_IsPoweredOn(NULL);
    EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SoftReset_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    RESET_FAKE(At_Test_AT);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_CFUN_fake.return_val = RETCODE_OK;
    Engine_SetFlukeCharFilterEnabled_fake.arg0_val = true;
    At_Test_AT_fake.return_val = RETCODE_OK;
    rc = Power_SoftReset();
    EXPECT_EQ(1U, At_Test_AT_fake.call_count);
    EXPECT_EQ(RETCODE_OK, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SoftReset_At_Set_CFUN_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    RESET_FAKE(At_Test_AT);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_CFUN_fake.return_val = RETCODE_OUT_OF_RESOURCES;
    At_Test_AT_fake.return_val = RETCODE_OK;
    rc = Power_SoftReset();
    EXPECT_EQ(RETCODE_OUT_OF_RESOURCES, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_SoftReset_At_Test_AT_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    RESET_FAKE(At_Test_AT);
    At_Set_UTEST_fake.return_val = RETCODE_OK;
    At_Set_COPS_fake.return_val = RETCODE_OK;
    At_Set_CEREG_fake.return_val = RETCODE_OK;
    At_Set_CGREG_fake.return_val = RETCODE_OK;
    At_Set_CMEE_fake.return_val = RETCODE_OK;
    At_Set_UDCONF_fake.return_val = RETCODE_OK;
    At_Set_CPIN_fake.return_val = RETCODE_OK;
    At_Set_CFUN_fake.return_val = RETCODE_OK;
    At_Test_AT_fake.return_val = RETCODE_FAILURE;
    rc = Power_SoftReset();
    EXPECT_EQ(RETCODE_FAILURE, Retcode_GetCode(rc));
}

TEST_F(TS_Power, Power_ResetCellular_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Hardware_Reset_fake.return_val = RETCODE_OK;
    Cellular_PowerUpParameters_T *goodParam = (Cellular_PowerUpParameters_T *)Pin;
    dummyParameterLength = sizeof(goodParam);
    goodParam->SimPin = "00001200034";
    rc = ResetCellular(&goodParam, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_ResetCellular_badSimPin_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Hardware_Reset_fake.return_val = RETCODE_OK;
    At_Test_AT_fake.return_val = RETCODE_OK;
    Cellular_PowerUpParameters_T *badParam = {NULL};
    rc = ResetCellular(&badParam, 0);
    EXPECT_EQ('\0', Pin[0]);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_ResetCellular_badParam_Success)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Hardware_Reset_fake.return_val = RETCODE_OK;
    Cellular_PowerUpParameters_T *badParam = (Cellular_PowerUpParameters_T *)Pin;
    dummyParameterLength = sizeof(badParam);
    badParam->SimPin = NULL;
    rc = ResetCellular(&badParam, dummyParameterLength);
    EXPECT_EQ(RETCODE_OK, rc);
}

TEST_F(TS_Power, Power_ResetCellular_HW_Failed)
{
    Retcode_T rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
    Hardware_Reset_fake.return_val = RETCODE_FAILURE;
    Cellular_PowerUpParameters_T *goodParam = {NULL};
    rc = ResetCellular(&goodParam, 0);
    EXPECT_EQ(false, IsPoweredOn);
    EXPECT_EQ(RETCODE_FAILURE, rc);
}