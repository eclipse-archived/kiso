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
 *  Mockup implementation for the FreeRTOSConfig file.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_FREERTOS_CONFIG_TH_HH_
#define BCDS_FREERTOS_CONFIG_TH_HH_

/* include system header files */

/* wrap the real FreeRTOSConfig.h header */
#include "FreeRTOSConfig.h"

#undef configASSERT
#define configASSERT(expression) if (!(expression)) {assert(pdFAIL); }

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

#endif /* BCDS_FREERTOS_CONFIG_TH_HH_ */

/** ************************************************************************* */
