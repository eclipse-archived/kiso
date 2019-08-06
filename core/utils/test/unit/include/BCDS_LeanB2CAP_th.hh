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
 *  Mockup implementation for the Lean B2CAP module.
 *
 * ****************************************************************************/
 
/* header definition ******************************************************** */
#ifndef BCDS_LEANB2CAP_TH_HH_
#define BCDS_LEANB2CAP_TH_HH_

#include "gtest.h"

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
