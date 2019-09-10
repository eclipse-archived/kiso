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
 * @ingroup    KISO_ESSENTIALS
 * @defgroup   ASSERT Assertion
 * @{
 *
 * @brief      Enhanced Assertion functionality
 *
 * @details
 * This module offers assertion function for:
 * - static assertion, i.e. at compile time
 * - dynamic assertion, i.e. during run time, customizable with user defined callback.
 *
 * @file
 */

/* header definition ******************************************************** */
#ifndef KISO_ASSERT_H_
#define KISO_ASSERT_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Essentials.h"

/*
 * Check for usage of the C99 standard in order to provide
 * support of the static assertion functionality
 */
#if (__STDC_VERSION__ <= 199901L)

#ifndef __FILE__
#define __FILE_    "Filename not defined"
#endif

#ifndef __LINE__
#define __LINE__    0
#endif

/**
 * @warning The direct use of this macro is forbidden!
 */
#define ASSERT_CONCAT_(a, b) a##b

/**
 * @warning The direct use of this macro is forbidden!
 */
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)

/**
 * @brief       Static (compile time) assertion macro.
 *
 * @details     The C99 standard does not support static assertions, nor the sizeof() keyword in preprocessor macros.
 *              To be able to perform compilation time variable size related checks, this macro is provided as a
 *              workaround.
 *@note         The the macro has some limitations.
 *
 *              To check if the int integer type is 4 bytes long, the following
 *              static assertion can be used.
 * @code
 *  static_assert(sizeof(int) == 4, "The int type shall be 32 bits long.");
 * @endcode
 *
 * @warning     This macro can't be used twice on the same __LINE__ number. The user must ensure that even in header
 *              files the __LINE__ number is always unique. Protect header files against multiple time inclusions and
 *              if possible put static assertions into C files instead of headers.
 *
 *
 * @param[in]   expression : A logical statement which can be true or false.
 *
 * @param[in]   message : A string about the test case - note that it is not compiled into the code.
 */
#define static_assert(expression, message) \
   enum { ASSERT_CONCAT(ASSERT_LINE_, __LINE__) = 1/(!!(expression)) };
#else

/* Expand the C11 Keyword _Static_assert if not included already by the compiler */
#ifndef static_assert
#define static_assert _Static_assert
#endif

#endif /* __STDC_VERSION__ */

#ifndef NDEBUG /* valid only for debug builds */

#undef assert /* removing the default compiler assert function */

/**
 * @brief       Program assertion macro.
 *
 * @details     The program assertion macro implements an assertion, which can be used to verify assumptions made by
 *              the program and get notified of this information through a callback if this assumption is false and
 *              stop program execution.
 *
 * @note        Program assertion is behaving differently in debug and release builds.
 *
 * @note        Release builds must be built with the NDEBUG symbol defined.
 *
 * @param[in]   Expression : A logical statement which can be true or false.
 */
#define assert(expression) \
   do{ if (!(expression)) { Assert_Dynamic(__LINE__, (const unsigned char*)__FILE__); } } while (0)


/**
 * @brief       Type definition for the assertion callback.
 *
 * @details     Upon initialization of Assert module the user should pass a reference to a function of this signature
 *              (**void function(const unsigned long, const unsigned char * const)**), handling dynamic assertions.
 *
 * @param[in]   line : The line number of the source code from where the assert() is called.
 *
 * @param[in]   file : The file name of the source code from where the assert() is called.
 */
typedef void (*Assert_Callback_T)(const unsigned long line, const unsigned char * const file);

/**
 * @brief       Initializes the Assert module.
 *
 * @details     If Assert_Dynamic() is to be used (runtime assertion), this function initializes the assert module. It
 *              registers a callback function which gets called when an assertion is hit.
 *
 * @note        Release builds must be built with the NDEBUG symbol defined. This API should be called before making
 *              use of any one of the assertion API's except for the static_assert().
 *
 * @param [in]  callback : A callback function which is executed when an assertion is triggered.
 *
 * @retval      #RETCODE_OK on success
 *
 * @retval      #RETCODE_INVALID_PARAM if callback is not valid
 */
Retcode_T Assert_Initialize(Assert_Callback_T callback);

/**
 * @brief       Implements runtime assertion.
 *
 * @details     This function can be used wherever an assertion during runtime is needed. The function calls the
 *              previously initialized callback and then blocks the system infinitely.
 *
 * @pre         Assert_Initialize() successfully executed.
 *
 * @note        Release builds must be built with the NDEBUG symbol defined. User shall not use this API and rather
 *              call the assert() macro with required assertion expression.
 *
 * @note        Defining the ASSERT_USE_STD_EXIT macro in CFLAGS_DEBUG make variable would use C standard's exit() API.
 *
 * @param[in]   line : The line number of the source code from where the assert() is called.
 *
 * @param[in]   file : The file name of the source code from where the assert() is called.
 */
void Assert_Dynamic(const unsigned long line, const unsigned char * const file);

#else /* valid only for release builds */

/**
 * @note        For release builds where NDEBUG flag is defined
 *              dynamic assertion and Assert initialize function
 *              call are disabled. The Callback function is made void
 *              to suppress unused function warning.
 */
#undef assert /* removing the default compiler assert function */
#define assert(expression)                     ((void)0)
#define Assert_Initialize(callback)               ((Retcode_T)0)

#endif /* NDEBUG */

#endif /* KISO_ASSERT_H_ */
/**@} */
