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
 * @brief The program retcode module implements support functionalities
 *  for the generic return code to be used across all the packages
 *  and components.
 *
 **/

#include "BCDS_Essentials.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_ESSENTIALS_MODULE_ID_RETCODE

/* own header files */
#include "BCDS_Retcode.h"
#include "BCDS_Assert.h"

/* system header files */

/* additional headers */

/* constant definitions ***************************************************** */

/* function prototypes ****************************************************** */

static bool IsRetcodeModuleInitialized = false;

/* local variables ********************************************************** */

static Retcode_ErrorHandlingFunc_T errorHandlingFunc;

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* global functions ********************************************************* */

/* The description of the function is available in header file */
Retcode_T Retcode_Initialize(Retcode_ErrorHandlingFunc_T func)
{
    Retcode_T Rc;

    if(IsRetcodeModuleInitialized)
    {
        /* Already initialized. */
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSITENT_STATE);
    }

    if(NULL == func)
    {
        Rc = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }
    else
    {
        errorHandlingFunc = func;
        IsRetcodeModuleInitialized = true;
        Rc = RETCODE_OK;
    }
    return Rc;
}

/* The description of the function is available in header file */
void Retcode_RaiseError(Retcode_T error)
{
    /* If it asserts here, Retcode_initialize() is not done */
    assert(errorHandlingFunc);

	errorHandlingFunc(error, false);
}

/* The description of the function is available in header file */
void Retcode_RaiseErrorFromIsr(Retcode_T error)
{
    /* If it asserts here, Retcode_initialize() is not done */
    assert(errorHandlingFunc);

    /* The flag which indicates the error is from ISR is set here. */
    errorHandlingFunc(error, true);
}

/** ************************************************************************* */
