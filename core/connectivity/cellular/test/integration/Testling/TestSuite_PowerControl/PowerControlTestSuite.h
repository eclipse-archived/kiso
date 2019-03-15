/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file	Tests power ON,OFF and RESET conditions.
 *
 * The positive condition related to Cellular Power control Module(ON,OFF,Reset)
 * and corresponding response is validated.
 *
 */

#ifndef PowerControlTestSuite_H
#define PowerControlTestSuite_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

#define DELAY_COUNT_VALUE                       UINT8_C(26)

#define DELAY_15_SECS                           UINT32_C(15000)
#define DELAY_60_SECS                           UINT32_C(60000)

/* Put the function declarations here */

/**
 * @brief  Registers the power control suite and the respective test cases for this suite
 *
 * @retval Status of the Power control test suite operation, RETCODE_OK on success or some
 * error code otherwise.
 */
Retcode_T PowerControlTestSuite_Init(void);

#endif /* PowerControlTestSuite_H */

