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
 *  Mockup implementation for the portable.h module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_PORTABLE_TH_HH_
#define BCDS_PORTABLE_TH_HH_

/* include system header files */
#include "projdefs.h"
#include "portmacro_th.hh"
#include "mpu_wrappers.h"

/* ban  the real portable.h  header */
#define PORTABLE_H

/* Necessary typedef as original file is banned. */
typedef struct HeapRegion
{
	uint8_t *pucStartAddress;
	size_t xSizeInBytes;
} HeapRegion_t;

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(vPortDefineHeapRegions, const HeapRegion_t *);
FAKE_VALUE_FUNC(void*, pvPortMalloc, size_t);
FAKE_VOID_FUNC(vPortFree, void*);
FAKE_VOID_FUNC(vPortInitialiseBlocks);
FAKE_VALUE_FUNC(size_t, xPortGetFreeHeapSize);
FAKE_VALUE_FUNC(size_t, xPortGetMinimumEverFreeHeapSize);

FAKE_VOID_FUNC(xPortSysTickHandler);
FAKE_VALUE_FUNC(BaseType_t, xPortStartScheduler);
FAKE_VOID_FUNC(vPortEndScheduler);

#endif /* BCDS_PORTABLE_TH_HH_ */

/** ************************************************************************* */
