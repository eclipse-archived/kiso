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
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_URC

#include "AtResponseQueue_th.hh"
#include "At3Gpp27007_th.hh"
#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "Kiso_Logging_th.hh"

#undef KISO_MODULE_ID
#include "Urc.c"
}

class TS_URC : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY();
    }
};

TEST_F(TS_URC, Urc_HandleResponses_Success)
{
    Retcode_T retcode;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = RETCODE_OK;
    At_HandleUrc_CGREG_fake.return_val = RETCODE_OK;
    At_HandleUrc_CEREG_fake.return_val = RETCODE_OK;
    At_HandleUrc_UUSOCL_fake.return_val = RETCODE_OK;
    At_HandleUrc_UUSOLI_fake.return_val = RETCODE_OK;
    At_HandleUrc_UUSORD_fake.return_val = RETCODE_OK;
    At_HandleUrc_UUSORF_fake.return_val = RETCODE_OK;
    At_HandleUrc_UUHTTPCR_fake.return_val = RETCODE_OK;

    retcode = Urc_HandleResponses();

    EXPECT_EQ(RETCODE_OK, retcode);
}

TEST_F(TS_URC, Urc_HandleResponses_Fail)
{
    Retcode_T retcode;
    AtResponseQueue_WaitForNamedCmd_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_CGREG_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_CEREG_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_UUSOCL_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_UUSOLI_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_UUSORD_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_UUSORF_fake.return_val = (Retcode_T)~RETCODE_OK;
    At_HandleUrc_UUHTTPCR_fake.return_val = (Retcode_T)~RETCODE_OK;

    retcode = Urc_HandleResponses();

    EXPECT_NE(RETCODE_OK, retcode);
}
