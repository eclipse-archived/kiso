/*******************************************************************************
 * Copyright (c) 2010-2020 Robert Bosch GmbH
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

/**
 * @file
 *
 * @brief
 * Implements the following functionalities specified in template.h
 */
#include "Kiso_Testing.h"
#include "TestSuiteUart.h"

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_MODULE_ID_TEST_ENTRY

#define TEST_ENTRY_ID 1

Retcode_T TestEntry_Initialize(void *param1, uint32_t param2);
static Retcode_T TestEntry_Setup(CCMsg_T *ccmsg);
static Retcode_T TestEntry_Teardown(CCMsg_T *ccmsg);

Retcode_T TestEntry_Initialize(void *param1, uint32_t param2)
{
    KISO_UNUSED(param1);
    KISO_UNUSED(param2);

    Retcode_T retcode = RETCODE_OK;
    retcode = Tests_Initialize(TEST_ENTRY_ID, TestEntry_Setup, TestEntry_Teardown);
    if (RETCODE_OK == retcode)
    {
        retcode = TestSuiteUart_Initialize((uint8_t)1);
    }
    return retcode;
}

static Retcode_T TestEntry_Setup(CCMsg_T *ccmsg)
{
    KISO_UNUSED(ccmsg);
    return RETCODE_OK;
}

static Retcode_T TestEntry_Teardown(CCMsg_T *ccmsg)
{
    KISO_UNUSED(ccmsg);
    return RETCODE_OK;
}
