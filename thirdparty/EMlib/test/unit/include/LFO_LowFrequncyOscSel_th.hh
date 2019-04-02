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
 * This unit test header substitutes "??"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef LFO_LOWFREQUNCYOSCSEL_TH_HH_
#define LFO_LOWFREQUNCYOSCSEL_TH_HH_


#include "macros_th.hh"

#define __STATIC_INLINE static inline
#define LFO_LOWFREQUNCYOSCSEL_IH_H_
typedef enum LFO_lowFreqOsc_e
{
	LFO_NONE,
	LFO_LFXO,
	LFO_LFRCO
} LFO_lowFreqOsc_t;

/*Fake function*/
FAKE_VALUE_FUNC(LFO_lowFreqOsc_t,LFO_getLowFreqOsc);
FAKE_VOID_FUNC(LFO_SelectLowFrequncyOscillator);
#endif /* LFO_LOWFREQUNCYOSCSEL_TH_HH_ */
/*********************************************************************************/
