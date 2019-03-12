/******************************************************************************/
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
 * ****************************************************************************/

#ifndef TEST_UNIT_INCLUDE_CDDK_PIPEANDFILTER_TH_HH_
#define TEST_UNIT_INCLUDE_CDDK_PIPEANDFILTER_TH_HH_

/* header definition ******************************************************** */
#include "BCDS_Retcode.h"

/* wrap the real interface header */
#include "CDDK_PipeAndFilter.h"

/**
 * mock-ups for the provided interfaces
 * Deprecated functions will be removed as part of PFM-659
 * Deprecated since vXXX (TODO: replace XXX with next version)
 */
FAKE_VALUE_FUNC(Retcode_T, PipeAndFilter_CreatePipe, PipeAndFilter_Pipe_T *);
FAKE_VALUE_FUNC(Retcode_T, PipeAndFilter_CreateFilter, PipeAndFilter_FilterFunction_T, PipeAndFilter_Pipe_T *, PipeAndFilter_Pipe_T *, PipeAndFilter_Filter_T *);
FAKE_VALUE_FUNC(Retcode_T, PipeAndFilter_FillPipe, PipeAndFilter_Pipe_T, uint8_t *, uint32_t);
FAKE_VALUE_FUNC(Retcode_T, PipeAndFilter_FillPipeFromISR, PipeAndFilter_Pipe_T, uint8_t *, uint32_t);

#endif /* TEST_UNIT_INCLUDE_CDDK_PIPEANDFILTER_TH_HH_ */


