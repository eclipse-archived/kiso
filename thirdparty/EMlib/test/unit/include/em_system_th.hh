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
 * This unit test header substitutes "em_system.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef BCDS_EM_SYSTEM_TH_HH_
#define BCDS_EM_SYSTEM_TH_HH_

/* include system header files */

/* ban  the real em_chip_ih.h interface header */
#define __EM_SYSTEM_H
#define __STATIC_INLINE    static inline

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(uint16_t,SYSTEM_GetFlashSize);
FAKE_VALUE_FUNC(uint64_t,SYSTEM_GetUnique);
FAKE_VALUE_FUNC(uint8_t,SYSTEM_GetCalibrationTemperature);

#endif /* BCDS_EM_SYSTEM_TH_HH_ */

/** ************************************************************************* */
