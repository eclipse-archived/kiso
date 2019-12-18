/*******************************************************************************
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
 ******************************************************************************/

#ifndef UBLOXUTILS_TH_HH_
#define UBLOXUTILS_TH_HH_

#include <fff.h>

#include "UBloxUtils.h"

FAKE_VALUE_FUNC(Retcode_T, CelToUbloxAddr, const Cellular_IpAddress_T *, AT_UBlox_Address_T *);
FAKE_VALUE_FUNC(Retcode_T, UbloxToCelAddr, const AT_UBlox_Address_T *, Cellular_IpAddress_T *);

#endif
