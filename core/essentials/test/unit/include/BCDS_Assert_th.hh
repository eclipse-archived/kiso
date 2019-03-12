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
 *  @brief Mockup implementation for the Assert.c file.
 *
 **/
 
/* header definition ******************************************************** */
#ifndef BCDS_ASSERT_HH_
#define BCDS_ASSERT_HH_

#include "BCDS_Assert.h"

/* ban the real BCDS_Assert.h interface header */
#define BCDS_ASSERT_H_

/* Helper macro - direct use is forbidden! */
#define ASSERT_CONCAT_(a, b) a##b

/* Helper macro - direct use is forbidden! */
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)

#define static_assert(expression, message) \
	enum { ASSERT_CONCAT(ASSERT_LINE_, __LINE__) = 1/(!!(expression)) };

/* additional interface header files */
#include "BCDS_Retcode.h"

typedef void (*Assert_Callback_T)(const unsigned long line, const unsigned char * const file);

/* fakes for assertion related macros */
#ifndef NDEBUG

/* Faking these deprecated functions is necessary, but we don't want to spam
 * our output with deprecated warnings. */
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif /* __GNUC__ */
FAKE_VALUE_FUNC(Retcode_T,Assert_initialize,Assert_Callback_T);
FAKE_VOID_FUNC(Assert_dynamic, unsigned long, const unsigned char *);
#if __GNUC__
#pragma GCC diagnostic pop
#endif /* __GNUC__ */

FAKE_VALUE_FUNC(Retcode_T,Assert_Initialize,Assert_Callback_T);
FAKE_VOID_FUNC(Assert_Dynamic, unsigned long, const unsigned char *);
#undef assert
#define assert(condition)       ((void)0)
#endif

#endif /* BCDS_ASSERT_HH_ */

/** ************************************************************************* */
