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
 *  Mockup implementation for the em_rmu module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
#ifndef BOOTLOADERAPP_EMRMU_TH_HH_
#define BOOTLOADERAPP_EMRMU_TH_HH_

/* include system header files */
#define EM_RMU_H

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(uint32_t, RMU_ResetCauseGet);
FAKE_VOID_FUNC(RMU_ResetCauseClear);

#endif
