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
 * This unit test header substitutes "em_emu.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef BCDS_EM_EMU_TH_HH
#define BCDS_EM_EMU_TH_HH

/**
 * Banning the real Header
 */
#define __EM_EMU_H

FAKE_VOID_FUNC(EMU_EnterEM1);
FAKE_VOID_FUNC(EMU_EnterEM2, bool);
FAKE_VOID_FUNC(EMU_EnterEM3, bool);
FAKE_VOID_FUNC(EMU_EnterEM4);

FAKE_VOID_FUNC(EMU_MemPwrDown, uint32_t);
FAKE_VOID_FUNC(EMU_UpdateOscConfig);

FAKE_VOID_FUNC(EMU_EM4Lock, bool);

FAKE_VOID_FUNC(EMU_BUReady);
FAKE_VOID_FUNC(EMU_BUPinEnable, bool);

FAKE_VOID_FUNC(EMU_Lock);
FAKE_VOID_FUNC(EMU_Unlock);

FAKE_VOID_FUNC(EMU_EM2Block);
FAKE_VOID_FUNC(EMU_EM2UnBlock);


#if 0
FAKE_VOID_FUNC(EMU_EM4Init, EMU_EM4Init_TypeDef *em4init);
FAKE_VOID_FUNC(EMU_BUPDInit, EMU_BUPDInit_TypeDef *bupdInit);
FAKE_VOID_FUNC(EMU_BUThresholdSet, EMU_BODMode_TypeDef mode, uint32_t value);
FAKE_VOID_FUNC(EMU_BUThresRangeSet, EMU_BODMode_TypeDef mode, uint32_t value);
#endif

#endif /* BCDS_EM_EMU_TH_HH */
