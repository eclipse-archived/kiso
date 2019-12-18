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
 *  @file
 *
 *  @brief The program assertion module implements several debug features including an
 *  assertion library, which can be used to verify assumptions made by the
 *  program and send this information to the user and stop program
 *  execution if this assumption fails.
 *
 *  There are two important assertion types: Static compilation time assertions,
 *  and dynamic runtime assertions.
 *
 *  The program assertion library behaves differently in debug and release builds.
 *  Static assertions are always enabled, but all other features of the library is
 *  disabled in release builds.
 *
 *  Release builds must be built with the NDEBUG symbol defined in the
 *  makefile.
 *
 **/

#ifndef NDEBUG /* valid only for debug builds */

#include "Kiso_Essentials.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_ASSERT

/* own headers */
#include "Kiso_Assert.h"

/* system header files */

#if defined(ASSERT_USE_STD_EXIT)

#include <stdlib.h>
#endif

/* local variables ********************************************************** */

static Assert_Callback_T assertCallback = NULL; /*< Variable to store the callback function pointer */

/* Static assert to test the compiler capabilities */
static_assert((1 != 0), "Testing static assert");

/* global functions ********************************************************* */

/* The description of the function is available in header file */
Retcode_T Assert_Initialize(Assert_Callback_T callback)
{
    Retcode_T retcode = RETCODE_OK;

    assertCallback = callback;

    if (NULL == assertCallback)
    {
        retcode = RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_INVALID_PARAM);
    }

    return (retcode);
}

/* The description of the function is available in header file */
void Assert_Dynamic(const unsigned long line, const unsigned char *const file)
{
    if (NULL != assertCallback)
    {
        assertCallback(line, file);
    }
#if defined(ASSERT_USE_STD_EXIT)
    /* exit application */
    exit(EXIT_FAILURE);
#else
    for (;;)
    {
        ; /* block application */
    }
#endif
}

#endif /* NDEBUG */
