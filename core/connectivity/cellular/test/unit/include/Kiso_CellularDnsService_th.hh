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

#ifndef KISO_CELLULARDNSSERVICE_TH_HH_
#define KISO_CELLULARDNSSERVICE_TH_HH_

#include "Kiso_CellularDnsService.h"

#include "gtest.h"

FAKE_VALUE_FUNC(Retcode_T, CellularDns_ResolveDomain, const Cellular_DataContext_T*, const char*, Cellular_IpAddress_T*)

#endif /* KISO_CELLULARDNSSERVICE_TH_HH_ */
