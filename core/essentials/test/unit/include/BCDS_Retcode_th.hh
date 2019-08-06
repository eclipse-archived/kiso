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
 *  @brief Mockup implementation for the BCDS_Retcode module.
 *
 **/

/* header definition ******************************************************** */
#ifndef BCDS_RETCODE_HH_
#define BCDS_RETCODE_HH_

/* include system header files */

/* wrap the real interface header */
#include "BCDS_Retcode.h"
#include "gtest.h"
/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

/* Faking these deprecated functions is necessary, but we don't want to spam
 * our output with deprecated warnings. */
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif /* __GNUC__ */
FAKE_VALUE_FUNC(Retcode_T, Retcode_initialize, Retcode_ErrorHandlingFunc_T);
FAKE_VOID_FUNC(Retcode_raiseError, Retcode_T);
FAKE_VOID_FUNC(Retcode_raiseErrorFromIsr, Retcode_T);
#if __GNUC__
#pragma GCC diagnostic pop
#endif /* __GNUC__ */

FAKE_VALUE_FUNC(Retcode_T, Retcode_Initialize, Retcode_ErrorHandlingFunc_T);
FAKE_VOID_FUNC(Retcode_RaiseError, Retcode_T);
FAKE_VOID_FUNC(Retcode_RaiseErrorFromIsr, Retcode_T);

#endif /* BCDS_RETCODE_HH_ */

/** ************************************************************************* */
