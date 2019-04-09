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


/* header definition ******************************************************** */
#ifndef SYSTEM_H_
#define SYSTEM_H_

/* additional interface header files */
#include "BCDS_Retcode.h"
#include "BCDS_CmdProcessor.h"

/* public interface declaration */

/* public type and macro definitions */

/* public function prototype declarations */

/**
 * @brief This function is used to initialize the following modules
 * in the provided command processor handle which is saved until updated.
 * - User Interface
 * - CAN
 * - BLE
 * - Serval/GSM
 *
 * @param [in] cmdHandle
 * Pointer to the valid application command processor.
 *
 * @retval	Retcode_T
 * RETCODE_OK on success. Any other Return Code if not successful.
 */
Retcode_T System_Initialize(CmdProcessor_T * cmdHandle);

/**
 * @brief This function is used to trigger a system factory reset.
 * The following modules are deinitialized before software reset.
 * - CAN
 * - BLE
 * - Serval/GSM
 * - User Interface
 * - SysOff Control line
 *
 * @note System_Initialize must have been called prior to this.
 *
 * @retval	Retcode_T
 * RETCODE_OK on success. Any other Return Code if not successful.
 */
Retcode_T System_FactoryReset(void);

/*
 * @brief This function API for reading BSE ID/DMC No.
 */
Retcode_T System_GetEepromBseId(uint8_t *BseId);

/*
 * @brief This function API shuts down the BSE-device
 */
Retcode_T SystemShutDownTriggerFromeBike(void);

#endif /* SYSTEM_H_ */

/** ************************************************************************* */
