/****************************************************************************/
/**
 *  Copyright (C) Robert Bosch GmbH. All Rights Reserved. Confidential.
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
 *  Mockup implementation for the Lean B2CAP module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_LEANB2CAP_TH_HH_
#define BCDS_LEANB2CAP_TH_HH_

/* wrap the real BCDS_LeanB2CAP.h interface header */
#include "BCDS_LeanB2CAP.h"

/* fakes for assertion related macros */
FAKE_VALUE_FUNC(Retcode_T, LeanB2CAP_Initialize, LeanB2CAP_HandlePtr_T * , LeanB2CAP_FrameData_T * , LeanB2CAPCallback );
FAKE_VALUE_FUNC(Retcode_T, LeanB2CAP_RxDataProcessor, LeanB2CAP_HandlePtr_T * , LeanB2CAP_FrameData_T * );
FAKE_VALUE_FUNC(Retcode_T, LeanB2CAP_Builder, LeanB2CAP_FrameData_T * , LeanB2CAP_RawData_T * );
FAKE_VALUE_FUNC(Retcode_T, LeanB2CAP_OverheadBuilder, LeanB2CAP_Overhead_T * ,LeanB2CAP_RawData_T *);
FAKE_VALUE_FUNC(Retcode_T, LeanB2CAP_ResetRxDataProcessor, LeanB2CAP_HandlePtr_T * );
FAKE_VALUE_FUNC(Retcode_T, LeanB2CAP_Deinitialize, LeanB2CAP_HandlePtr_T * );

#endif /* BCDS_LEANB2CAP_TH_HH_ */

/** ************************************************************************* */
