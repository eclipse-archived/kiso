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

#ifndef AT_3GPP_27007_TH_HH_
#define AT_3GPP_27007_TH_HH_

#include "At3Gpp27007.h"

#include "gtest.h"

/* *** NETWORK COMMANDS ***************************************************** */
FAKE_VALUE_FUNC(Retcode_T, At_Set_CREG, AT_CXREG_N_T)
FAKE_VALUE_FUNC(Retcode_T, At_Set_CGREG, AT_CXREG_N_T)
FAKE_VALUE_FUNC(Retcode_T, At_Set_CEREG, AT_CXREG_N_T)
FAKE_VALUE_FUNC(Retcode_T, At_Get_CREG, AT_CREG_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Get_CGREG, AT_CGREG_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Get_CEREG, AT_CEREG_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Set_COPS, const AT_COPS_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Set_CGDCONT, const AT_CGDCONT_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Set_CGACT, const AT_CGACT_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Set_CGPADDR, const AT_CGPADDR_Param_T *, AT_CGPADDR_Resp_T *)
/* *** ERROR HANDLING ******************************************************* */
FAKE_VALUE_FUNC(Retcode_T, At_Set_CMEE, uint32_t);
/* *** SIM COMMANDS ********************************************************* */
FAKE_VALUE_FUNC(Retcode_T, At_Set_CPIN, const char *)
/* *** TE-TA INTERFACE COMMANDS ********************************************* */
FAKE_VALUE_FUNC(Retcode_T, At_Test_AT)
FAKE_VALUE_FUNC(Retcode_T, At_Set_ATE, bool)
/* *** POWER CONTROL COMMANDS *********************************************** */
FAKE_VALUE_FUNC(Retcode_T, At_Set_CFUN, const AT_CFUN_Param_T *)
FAKE_VALUE_FUNC(Retcode_T, At_Get_CFUN, AT_CFUN_Resp_T *)
/* *** URC HANDLERS ********************************************************* */
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_CREG)
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_CGREG)
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_CEREG)

#endif /* AT_3GPP_27007_TH_HH_ */
