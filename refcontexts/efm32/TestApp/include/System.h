/*****************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *
 *  The interface header exports the following features:
 *  - System_Initialize
 *  - System_Init
 *  - System_deintialize
 *  - System_FactoryReset
 *
 * ****************************************************************************/

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
