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
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Provides functions internally exposed from board module to other BSP modules.
 *
 * @details
 */
#ifndef PROTECTED_BOARD_H_
#define PROTECTED_BOARD_H_

#include "BCDS_HAL.h"
/*
 * Returns true if BSP_Board_Initialize() successfully executed.
 */
bool Board_IsInitDone(void);

#endif /* PROTECTED_BOARD_H_ */

