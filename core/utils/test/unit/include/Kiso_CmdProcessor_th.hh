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
 * @file        CmdProcessor_th.hh
 *
 *  Mockup implementation for the CmdProcessor module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef KISO_CMDPROCESSOR_TH_HH_
#define KISO_CMDPROCESSOR_TH_HH_

/* include system header files */
#include "gtest.h"
/* wrap the real  interface header */
#include "Kiso_CmdProcessor.h"
#include "Kiso_Retcode.h"

/* faked variables needs to be initialized by the test fixture */
typedef void (*CmdProcessor_Func_T)(void *param1, uint32_t param2);

/* mock-ups for the provided interfaces */

FAKE_VOID_FUNC(CmdProcessor_Suspend, CmdProcessor_T*)
FAKE_VOID_FUNC(CmdProcessor_Resume, CmdProcessor_T*)
FAKE_VALUE_FUNC(Retcode_T, CmdProcessor_Initialize, CmdProcessor_T*, const char*, uint32_t, uint32_t, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, CmdProcessor_Enqueue, CmdProcessor_T*, CmdProcessor_Func_T, void*, uint32_t)
FAKE_VALUE_FUNC(Retcode_T, CmdProcessor_EnqueueFromIsr, CmdProcessor_T*, CmdProcessor_Func_T, void*, uint32_t)

#endif /* KISO_CMDPROCESSOR_TH_HH_ */

/** ************************************************************************* */
