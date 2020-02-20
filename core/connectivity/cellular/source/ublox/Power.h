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
 * @brief Internal u-blox header containing variant specific calls.
 */
#ifndef POWER_H_
#define POWER_H_

#include "Kiso_Retcode.h"

#define UBLOX_MAX_PIN_LENGTH (UINT32_C(12))

/**
 * @brief Setup the initial modem configuration after modem boot-up.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Power_SetupModem(void);

/**
 * @brief Perform a soft reset and wait for the modem to be responsive again.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Power_SoftReset(void);

#endif /* POWER_H_ */
