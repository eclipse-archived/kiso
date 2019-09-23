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
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_SOCKET_SERVICE

#include "UBloxUtils_th.hh"
#include "Engine_th.hh"
#include "AT_UBlox_th.hh"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

#undef KISO_MODULE_ID
#include "SocketService.c"
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
