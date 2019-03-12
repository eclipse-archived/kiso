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
 *  @brief Mockup implementation for the BCDS_Retcode module.
 *
 **/

/* header definition ******************************************************** */
#ifndef BCDS_RETCODE_HH_
#define BCDS_RETCODE_HH_

/* include system header files */

/* wrap the real interface header */
#include "BCDS_Retcode.h"

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
