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
 *  @brief Mockup implementation for the Assert.c file.
 *
 **/

/* header definition ******************************************************** */
#ifndef KISO_ASSERT_HH_
#define KISO_ASSERT_HH_

#include "Kiso_Assert.h"

/* ban the real Kiso_Assert.h interface header */
#define KISO_ASSERT_H_

/* Helper macro - direct use is forbidden! */
#define ASSERT_CONCAT_(a, b) a##b

/* Helper macro - direct use is forbidden! */
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)

#define static_assert(expression, message) \
	enum { ASSERT_CONCAT(ASSERT_LINE_, __LINE__) = 1/(!!(expression)) };

/* additional interface header files */
#include "Kiso_Retcode.h"

typedef void (*Assert_Callback_T)(const unsigned long line, const unsigned char * const file);

/* fakes for assertion related macros */
#ifndef NDEBUG

/* Faking these deprecated functions is necessary, but we don't want to spam
 * our output with deprecated warnings. */
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif /* __GNUC__ */
FAKE_VALUE_FUNC(Retcode_T,Assert_initialize,Assert_Callback_T)
FAKE_VOID_FUNC(Assert_dynamic, unsigned long, const unsigned char *)
#if __GNUC__
#pragma GCC diagnostic pop
#endif /* __GNUC__ */

FAKE_VALUE_FUNC(Retcode_T,Assert_Initialize,Assert_Callback_T)
FAKE_VOID_FUNC(Assert_Dynamic, unsigned long, const unsigned char *)
#undef assert
#define assert(condition)       ((void)0)
#endif

#endif /* KISO_ASSERT_HH_ */

/** ************************************************************************* */
