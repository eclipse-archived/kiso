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
 *
 *
 *  Mockup implementation for the portable.h module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef KISO_PORTABLE_TH_HH_
#define KISO_PORTABLE_TH_HH_

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
FAKE_VOID_FUNC(vPortDefineHeapRegions, const HeapRegion_t *)
FAKE_VALUE_FUNC(void *, pvPortMalloc, size_t)
FAKE_VOID_FUNC(vPortFree, void *)
FAKE_VOID_FUNC(vPortInitialiseBlocks)
FAKE_VALUE_FUNC(size_t, xPortGetFreeHeapSize)
FAKE_VALUE_FUNC(size_t, xPortGetMinimumEverFreeHeapSize)

FAKE_VOID_FUNC(xPortSysTickHandler)
FAKE_VALUE_FUNC(BaseType_t, xPortStartScheduler)
FAKE_VOID_FUNC(vPortEndScheduler)

#endif /* KISO_PORTABLE_TH_HH_ */

/** ************************************************************************* */
