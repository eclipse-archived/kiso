/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/

#ifndef CORE_CM4_TH_HH_
#define CORE_CM4_TH_HH_

/* Include the needed headers */
#include "gtest.h"
#include "stm32l4xx_th.hh"
/* include or ban the real interface header */
#include "core_cm4.h"

/* NOTE: As core_cm4 defines the functions INSIDE the .h file, we have to apply
 * some hacks to be able to use fff-fakes instead.
 *
 * What we do here is, first include the original header like it is, then we
 * define all function calls to the original functions to go to a "_fff"
 * version.
 * Below we then define fakes for these "_fff" versions, so all calls to e.g.
 * NVIC_SetPriorityGrouping in our C.U.T will actually go to
 * NVIC_SetPriorityGrouping_fff, which is the faked version. The second set of
 * macros is there to enable us to use the NVIC_SetPriorityGrouping_fake and
 * _reset inside our unit-test file.
 */
#define NVIC_SetPriorityGrouping NVIC_SetPriorityGrouping_fff
#define NVIC_SetPriorityGrouping_fake NVIC_SetPriorityGrouping_fff_fake
#define NVIC_SetPriorityGrouping_reset NVIC_SetPriorityGrouping_fff_reset

#define NVIC_GetPriorityGrouping NVIC_GetPriorityGrouping_fff
#define NVIC_GetPriorityGrouping_fake NVIC_GetPriorityGrouping_fff_fake
#define NVIC_GetPriorityGrouping_reset NVIC_GetPriorityGrouping_fff_reset

#define NVIC_EnableIRQ NVIC_EnableIRQ_fff
#define NVIC_EnableIRQ_fake NVIC_EnableIRQ_fff_fake
#define NVIC_EnableIRQ_reset NVIC_EnableIRQ_fff_reset

#define NVIC_DisableIRQ NVIC_DisableIRQ_fff
#define NVIC_DisableIRQ_fake NVIC_DisableIRQ_fff_fake
#define NVIC_DisableIRQ_reset NVIC_DisableIRQ_fff_reset

#define NVIC_GetPendingIRQ NVIC_GetPendingIRQ_fff
#define NVIC_GetPendingIRQ_fake NVIC_GetPendingIRQ_fff_fake
#define NVIC_GetPendingIRQ_reset NVIC_GetPendingIRQ_fff_reset

#define NVIC_SetPendingIRQ NVIC_SetPendingIRQ_fff
#define NVIC_SetPendingIRQ_fake NVIC_SetPendingIRQ_fff_fake
#define NVIC_SetPendingIRQ_reset NVIC_SetPendingIRQ_fff_reset

#define NVIC_ClearPendingIRQ NVIC_ClearPendingIRQ_fff
#define NVIC_ClearPendingIRQ_fake NVIC_ClearPendingIRQ_fff_fake
#define NVIC_ClearPendingIRQ_reset NVIC_ClearPendingIRQ_fff_reset

#define NVIC_GetActive NVIC_GetActive_fff
#define NVIC_GetActive_fake NVIC_GetActive_fff_fake
#define NVIC_GetActive_reset NVIC_GetActive_fff_reset

#define NVIC_SetPriority NVIC_SetPriority_fff
#define NVIC_SetPriority_fake NVIC_SetPriority_fff_fake
#define NVIC_SetPriority_reset NVIC_SetPriority_fff_reset

#define NVIC_GetPriority NVIC_GetPriority_fff
#define NVIC_GetPriority_fake NVIC_GetPriority_fff_fake
#define NVIC_GetPriority_reset NVIC_GetPriority_fff_reset

#define NVIC_EncodePriority NVIC_EncodePriority_fff
#define NVIC_EncodePriority_fake NVIC_EncodePriority_fff_fake
#define NVIC_EncodePriority_reset NVIC_EncodePriority_fff_reset

#define NVIC_DecodePriority NVIC_DecodePriority_fff
#define NVIC_DecodePriority_fake NVIC_DecodePriority_fff_fake
#define NVIC_DecodePriority_reset NVIC_DecodePriority_fff_reset

#define NVIC_SystemReset NVIC_SystemReset_fff
#define NVIC_SystemReset_fake NVIC_SystemReset_fff_fake
#define NVIC_SystemReset_reset NVIC_SystemReset_fff_reset

#if (__Vendor_SysTickConfig == 0U)
#define SysTick_Config SysTick_Config_fff
#define SysTick_Config_fake SysTick_Config_fff_fake
#define SysTick_Config_reset SysTick_Config_fff_reset

#endif /* (__Vendor_SysTickConfig == 0U) */
#define ITM_SendChar ITM_SendChar_fff
#define ITM_SendChar_fake ITM_SendChar_fff_fake
#define ITM_SendChar_reset ITM_SendChar_fff_reset

#define ITM_ReceiveChar ITM_ReceiveChar_fff
#define ITM_ReceiveChar_fake ITM_ReceiveChar_fff_fake
#define ITM_ReceiveChar_reset ITM_ReceiveChar_fff_reset

#define ITM_CheckChar ITM_CheckChar_fff
#define ITM_CheckChar_fake ITM_CheckChar_fff_fake
#define ITM_CheckChar_reset ITM_CheckChar_fff_reset

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
FAKE_VOID_FUNC(NVIC_SetPriorityGrouping, uint32_t);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetPriorityGrouping);
FAKE_VOID_FUNC(NVIC_EnableIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_DisableIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetPendingIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_SetPendingIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_ClearPendingIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetActive, IRQn_Type);
FAKE_VOID_FUNC(NVIC_SetPriority, IRQn_Type, uint32_t);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetPriority, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_EncodePriority, uint32_t, uint32_t, uint32_t);
FAKE_VOID_FUNC(NVIC_DecodePriority, uint32_t, uint32_t, uint32_t*, uint32_t*);
FAKE_VOID_FUNC(NVIC_SystemReset);
#if (__Vendor_SysTickConfig == 0U)
FAKE_VALUE_FUNC(uint32_t, SysTick_Config, uint32_t);
#endif /* (__Vendor_SysTickConfig == 0U) */
FAKE_VALUE_FUNC(uint32_t, ITM_SendChar, uint32_t);
FAKE_VALUE_FUNC(int32_t, ITM_ReceiveChar);
FAKE_VALUE_FUNC(int32_t, ITM_CheckChar);

/* Mock-ups for the provided interfaces */

#endif /* CORE_CM4_TH_HH_ */
