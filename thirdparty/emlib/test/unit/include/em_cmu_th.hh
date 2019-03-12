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
 *  @file
 *
 *
 *  Mockup implementation for the em_cmu module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
#ifndef BOOTLOADERAPP_EMCMU_TH_HH_
#define BOOTLOADERAPP_EMCMU_TH_HH_

/* include system header files */

/* wrap the real App.h interface header */
#include "em_cmu.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC (CMU_ClockDivSet,CMU_Clock_TypeDef,CMU_ClkDiv_TypeDef);
FAKE_VOID_FUNC (CMU_HFRCOBandSet,CMU_HFRCOBand_TypeDef);
FAKE_VOID_FUNC (CMU_ClockSelectSet,CMU_Clock_TypeDef,CMU_Select_TypeDef);
FAKE_VOID_FUNC (CMU_ClockEnable,CMU_Clock_TypeDef, bool);

#endif
