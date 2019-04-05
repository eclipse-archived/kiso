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
 * This unit test header substitutes "em_wdog.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef EM_WDOG_TH_HH_
#define EM_WDOG_TH_HH_

#include "em_wdog.h"

/*Fake functions*/
FAKE_VOID_FUNC(WDOG_Enable,bool);
FAKE_VOID_FUNC(WDOG_Feed);
FAKE_VOID_FUNC(WDOG_Init,const WDOG_Init_TypeDef *);
FAKE_VOID_FUNC(WDOG_Lock);

#endif /* EM_WDOG_TH_HH_ */
/**********************************************************************************/
