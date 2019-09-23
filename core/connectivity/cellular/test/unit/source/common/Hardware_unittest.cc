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
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_HARDWARE

#include "Kiso_BSP_Cellular_SaraR4N4_th.hh"
#include "Kiso_BSP_UMTS_LisaU2_th.hh"

#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "Kiso_MCU_UART_th.hh"

#undef KISO_MODULE_ID
#include "Hardware.c"
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
