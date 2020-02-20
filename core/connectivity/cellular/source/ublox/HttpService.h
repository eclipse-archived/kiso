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
 * @ingroup KISO_CELLULAR_VARIANT_UBLOX
 * @file
 * @brief Internal header for managing the u-blox http-service.
 */
#ifndef HTTPSERVICE_H_
#define HTTPSERVICE_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

/**
 * @brief Called by the URC handler to notify that a http request is done
  *
 * @param[in] profileId
 * the profileId of the received http request event
 *
 * @param[in] command
 * the method being used
 *
 * @param[in] result
 * the result of the operation (1:success, 0: failure)
 */
void HttpService_NotifyResult(AT_UHTTP_ProfileId_T profileId, AT_UHTTPC_Command_T command, AT_UHTTPC_Result_T result);

#endif //HTTPSERVICE_H_
