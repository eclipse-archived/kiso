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
 *  Mockup implementation for the em_msc module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
#ifndef BOOTLOADERAPP_CYCURLIB_TH_HH_
#define BOOTLOADERAPP_CYCURLIB_TH_HH_

/* include system header files */

/* wrap the real App.h interface header */
#include "em_msc.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC (MSC_Init);
FAKE_VOID_FUNC (MSC_Deinit);
FAKE_VALUE_FUNC(MSC_Status_TypeDef, MSC_WriteWordFast, uint32_t *, void const *, uint32_t);
FAKE_VALUE_FUNC(MSC_Status_TypeDef, MSC_ErasePage, uint32_t*);

#endif
