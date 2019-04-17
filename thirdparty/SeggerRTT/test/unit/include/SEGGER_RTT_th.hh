/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file SEGGER_RTT_th.hh
 *
 *
 *  Mockup implementation for SEGGER_RTT
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_SEGGER_RTT_TH_HH_
#define BCDS_SEGGER_RTT_TH_HH_

/* include system header files */
#include <gtest.h>

/* wrap the real event_groups.h header */
#include "SEGGER_RTT.h"

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(SEGGER_RTT_Init);
FAKE_VALUE_FUNC(unsigned, SEGGER_RTT_Read, unsigned , void* , unsigned);
FAKE_VALUE_FUNC(unsigned, SEGGER_RTT_Write, unsigned , const void* , unsigned);
/* @todo: add missing headers */



#endif /* BCDS_SEGGER_RTT_TH_HH_ */

/** ************************************************************************* */
