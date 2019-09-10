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

#ifndef AT_UBLOX_TH_HH_
#define AT_UBLOX_TH_HH_

#include "AT_UBlox.h"

#include "gtest.h"

/* *** RADIO CONFIGURATION ************************************************** */
FAKE_VALUE_FUNC(Retcode_T, At_Set_URAT, const AT_URAT_Param_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_UMNOPROF, AT_UMNOPROF_Mno_T)
FAKE_VALUE_FUNC(Retcode_T, At_Get_UMNOPROF, AT_UMNOPROF_Mno_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_UBANDMASK, const AT_UBANDMASK_Param_T*)
/* *** SOCKET SERVICE ******************************************************* */
FAKE_VALUE_FUNC(Retcode_T, At_Set_USOCR, const AT_USOCR_Param_T*, uint32_t*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USOCO, const AT_USOCO_Param_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USOWR, const AT_USOWR_Param_T*, AT_USOWR_Resp_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USOST, const AT_USOST_Param_T*, AT_USOST_Resp_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USORD, const AT_USORD_Param_T*, AT_USORD_Resp_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USORF, const AT_USORF_Param_T*, AT_USORF_Resp_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USOLI, const AT_USOLI_Param_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_USOCL, const AT_USOCL_Param_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Set_UDCONF, const AT_UDCONF_Param_T*)
FAKE_VALUE_FUNC(Retcode_T, At_Get_UDCONF, AT_UDCONF_Param_T*)
/* *** SIM SERVICE ********************************************************** */
FAKE_VALUE_FUNC(Retcode_T, At_Get_CCID, AT_CCID_Resp_T*)
/* *** DNS SERVICE ********************************************************** */
FAKE_VALUE_FUNC(Retcode_T, At_Set_UDNSRN, const AT_UDNSRN_Param_T*, AT_UDNSRN_Resp_T*)
/* *** URC HANDLERS ********************************************************* */
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_UUSORD)
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_UUSORF)
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_UUSOLI)
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_UUSOCL)
FAKE_VALUE_FUNC(Retcode_T, At_HandleUrc_UUHTTPCR)

#endif /* AT_UBLOX_TH_HH_ */
