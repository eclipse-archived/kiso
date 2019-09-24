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
 * @file
 * @defgroup    BASICS Basics
 * @ingroup     KISO_ESSENTIALS
 * @{
 *
 * @brief       Basic macro definitions and standard imports
 *
 * @details
 * This file provides the standard int and bool header support apart from compiler abstraction library.
 *  - The library provides macros to abstract from non standard C compiler
 *    switches like GCC's __attribute__ keyword.
 *  - abstraction macros for all attributes including always_inline,
 *    section, unused arguments and variable macros.
 *
 **/

#ifndef KISO_BASICS_H_
#define KISO_BASICS_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#ifndef false
// clang-format off
#define false ((bool)0)
// clang-format on
#endif
#ifndef true
// clang-format off
#define true ((bool)1)
// clang-format on
#endif

#ifndef TRUE
#define TRUE ((bool)1)
#endif
#ifndef FALSE
#define FALSE ((bool)0)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef null
#define null ((void *)0)
#endif

/**
 * @brief       Macro to inform the compiler that a variable is intentionally not in use.
 *
 * @param[in]   variableName : The unused variable.
 */
#define KISO_UNUSED(variableName) ((void)variableName)

/**
 * @brief       Macro to inform the compiler that a function or type is deprecated.
 *
 * @param[in]   param : function or type to deprecate.
 */
#define KISO_DEPRECATED(param) param __attribute__((deprecated))

/**
 * @brief       Macro to inform the compiler that a function is intentionally not in use.
 *
 * @details     in GCC the compiler option -Wunused (-Wall) generates warnings if a function is not in use. Using
 *              the KISO_UNUSED_FUNC() macro means that the function is meant to be unused. GCC will not produce
 *              a warning for this function.
 *
 * @code
 * void KISO_UNUSED_FUNC(FOO_foo(uint8_t param1, uint8_t param2)){ ; }
 * @endcode
 *
 * @param[in]   function : The unused function with its arguments list.
 */
#ifdef __GNUC__
#define KISO_UNUSED_FUNC(function) __attribute__((__unused__)) function
#else
#define KISO_UNUSED_FUNC(function) function
#endif

/**
 * @brief       Macro to inform the compiler that this section is a part of the provided linker section.
 *
 * @code
 * void KISO_SECTION(section_name)
 * @endcode
 *
 * @param[in]  sectionName : The section name that has been provided in the Linker file.
 */
#ifdef __GNUC__
#define KISO_SECTION(sectionName) __attribute__((section(#sectionName)))
#else
#define KISO_SECTION(sectionName)
#endif

/**
 * @brief       Macro to force the compiler to always inline an inline function.
 *
 * @details     In GCC, the inline keyword is not always regarded by the compiler. Based on the situation the compiler
 *              can decide to emit a real function instead of the function body only. Defining a function as
 *              always_inline makes it madatory for the compiler to inline it always.
 *
 * @code
 * KISO_ALWAYS_INLINE void FOO_foo(void){ ; }
 * @endcode
 *
 * @note        Inlining makes C level debugging hard as the inlined code is not aligned with the source file. To
 *              eliminate this issue, in debug builds the macro is disabled.
 */
#if defined(__GNUC__) && defined(NDEBUG)
#define KISO_ALWAYS_INLINE __attribute__((always_inline))
#else
#define KISO_ALWAYS_INLINE
#endif

/**
 * @brief       Macro to inline a function.
 *
 * @details     GCC: in GCC, the inline keyword is not always regarded by the compiler. Based on the situation the
 *              compiler can decide to emit a real function instead of the function body only. Inline is not supported
 *              by C89 or before C standards.
 * @code
 * KISO_INLINE void FOO_foo(void){ ; }
 * @endcode
 * @note        Inlining makes C level debugging hard as the inlined code is not aligned with the source file.
 */
#if (__STDC_VERSION__ >= 199901L) /* check to C99 mode */
#define KISO_INLINE inline
#else
#define KISO_INLINE __attribute__((__unused__))
#endif

#endif /* KISO_BASICS_H_ */
/**@} */
