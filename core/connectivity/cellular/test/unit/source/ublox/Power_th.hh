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

#ifndef POWER_TH_HH_
#define POWER_TH_HH_

#include <fff.h>

#include "Power.h"

FAKE_VALUE_FUNC(Retcode_T, Power_SetupModem);
FAKE_VALUE_FUNC(Retcode_T, Power_SoftReset);

#endif
