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
 *
 * @brief Unit Test Mock-up Header
 *
 * This unit test header substitutes "em_rmu.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_RMU_TH_HH_
#define EM_RMU_TH_HH_
#define __EM_RMU_H
#define __STATIC_INLINE static inline

/*Fake functions*/

FAKE_VALUE_FUNC(uint32_t, RMU_ResetCauseGet);
FAKE_VOID_FUNC(RMU_ResetCauseClear);
#endif /* EM_RMU_TH_HH_ */

/**************************************************************************************/
