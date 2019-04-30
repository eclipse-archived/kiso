/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/
 
#ifndef CELLULAR_DNS_TH_HH
#define CELLULAR_DNS_TH_HH

/* Include the needed headers */
#include <gtest.h>
#include "BCDS_Basics.h"
#include "BCDS_Retcode_th.hh"
#include "Dns.h"

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, Cellular_ResolveDomainName, uint8_t*, uint32_t, uint32_t*);

#endif /* CELLULAR_DNS_TH_HH */
