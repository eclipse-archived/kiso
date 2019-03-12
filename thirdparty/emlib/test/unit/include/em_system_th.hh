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
 *  Mockup implementation for the em_system module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
#ifndef BOOTLOADERAPP_EMSYSTEM_TH_HH_
#define BOOTLOADERAPP_EMSYSTEM_TH_HH_

/* include system header files */

/* wrap the real App.h interface header */
#include "em_system.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

FAKE_VOID_FUNC (SYSTEM_ChipRevisionGet,SYSTEM_ChipRevision_TypeDef*);

#endif
