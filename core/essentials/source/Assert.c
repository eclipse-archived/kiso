/*----------------------------------------------------------------------------*/
/*
 *  Copyright (C) Bosch Connected Devices and Solutions.
 *  All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 */
/*----------------------------------------------------------------------------*/

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

#include "BCDS_Essentials.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_MODULE_ID_ASSERT

/* own headers */
#include "BCDS_Assert.h"

/* system header files */

#if defined(ASSERT_USE_STD_EXIT)
//lint -e49 error in standard libraries suppressed
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
void Assert_Dynamic(const unsigned long line, const unsigned char * const file)
{
    if (NULL != assertCallback)
    {
    	assertCallback(line , file);
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

/** LCOV_EXCL_START : Start of section where code coverage is ignored.
 * Ignore code coverage for the deprecated APIs.*/

/* Deprecated function. See header file for more information */
Retcode_T Assert_initialize(Assert_Callback_T callback)
{
    return (Assert_Initialize(callback));
}

/* Deprecated function. See header file for more information */
void Assert_dynamic(const unsigned long line, const unsigned char * const file)
{
    Assert_Dynamic(line,file);
}

/** LCOV_EXCL_STOP : End of section where code coverage is ignored. */

#endif /* NDEBUG */

