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
 * @brief u-blox specific utilities.
 */
#ifndef UBLOXUTILS_H_
#define UBLOXUTILS_H_

#include "Kiso_Cellular.h"
#include "AT_UBlox.h"

#include "Kiso_Retcode.h"

/**
 * @brief Translate #Cellular_IpAddress_T to #AT_UBlox_Address_T.
 */
Retcode_T CelToUbloxAddr(const Cellular_IpAddress_T *from, AT_UBlox_Address_T *to);

/**
 * @brief Translate #AT_UBlox_Address_T to #Cellular_IpAddress_T.
 */
Retcode_T UbloxToCelAddr(const AT_UBlox_Address_T *from, Cellular_IpAddress_T *to);

#endif /* UBLOXUTILS_H_ */
