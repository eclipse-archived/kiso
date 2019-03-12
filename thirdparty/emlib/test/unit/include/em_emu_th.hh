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
 *  Mockup implementation for the em_emu module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
#ifndef EM_EMU_TH_HH_
#define EM_EMU_TH_HH_

/* include system header files */

/* wrap the real App.h interface header */
#include "em_emu.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC (EMU_MemPwrDown,uint32_t);

#endif
