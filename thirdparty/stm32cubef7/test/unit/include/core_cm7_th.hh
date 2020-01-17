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
 
#ifndef CORE_CM7_TH_HH_
#define CORE_CM7_TH_HH_

/* Include the needed headers */
#include "gtest.h"
#include "stm32f7xx_th.hh"

/* Include or ban the real interface header */
#include "core_cm7.h"

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for MCU core registers */

uint8_t fakeSbcReg[sizeof(SCB_Type)] __attribute__((unused)) = {0};

uint8_t fakeSysTickReg[sizeof(SysTick_Type)] __attribute__((unused)) = {0};

uint8_t fakeNvicReg[sizeof(NVIC_Type)] __attribute__((unused)) = {0};

uint8_t fakeItmReg[sizeof(ITM_Type)] __attribute__((unused)) = {0};

uint8_t fakeDwtReg[sizeof(DWT_Type)] __attribute__((unused))= {0};

uint8_t fakeTpiReg[sizeof(TPI_Type)] __attribute__((unused)) = {0};

uint8_t fakeCoreDebugReg[sizeof(CoreDebug_Type)] __attribute__((unused)) = {0};

/** System control Register not in SCB */
#undef SCnSCB
#define SCnSCB      ((SCnSCB_Type*)&fakeSbcReg)

/** SCB configuration structure */
#undef SCB
#define SCB         ((SCB_Type*)&fakeSbcReg)

/** SysTick configuration structure */
#undef SysTick
#define SysTick     ((SysTick_Type*)&fakeSysTickReg)

/** NVIC configuration structure */
#undef NVIC
#define NVIC        ((NVIC_Type*)&fakeNvicReg)

/** ITM configuration structure */
#undef ITM
#define ITM         (ITM_Type*)&fakeItmReg)

/** DWT configuration structure */
#undef DWT
#define DWT         ((DWT_Type*)&fakeDwtReg)

/** TPI configuration structure */
#undef TPI
#define TPI         ((TPI_Type*)&fakeTpiReg)

/** Core Debug configuration structure */
#undef CoreDebug
#define CoreDebug   ((CoreDebug_Type *)&fakeCoreDebugReg)

/* Mock-ups for the provided interfaces */

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

#define SCB_GetFPUType SCB_GetFPUType_fff
#define SCB_GetFPUType_fake SCB_GetFPUType_fff_fake
#define SCB_GetFPUType_reset SCB_GetFPUType_fff_reset

#define SCB_EnableICache SCB_EnableICache_fff
#define SCB_EnableICache_fake SCB_EnableICache_fff_fake
#define SCB_EnableICache_reset SCB_EnableICache_fff_reset

#define SCB_DisableICache SCB_DisableICache_fff
#define SCB_DisableICache_fake SCB_DisableICache_fff_fake
#define SCB_DisableICache_reset SCB_DisableICache_fff_reset

#define SCB_InvalidateICache SCB_InvalidateICache_fff
#define SCB_InvalidateICache_fake SCB_InvalidateICache_fff_fake
#define SCB_InvalidateICache_reset SCB_InvalidateICache_fff_reset

#define SCB_EnableDCache SCB_EnableDCache_fff
#define SCB_EnableDCache_fake SCB_EnableDCache_fff_fake
#define SCB_EnableDCache_reset SCB_EnableDCache_fff_reset

#define SCB_DisableDCache SCB_DisableDCache_fff
#define SCB_DisableDCache_fake SCB_DisableDCache_fff_fake
#define SCB_DisableDCache_reset SCB_DisableDCache_fff_reset

#define SCB_InvalidateDCache SCB_InvalidateDCache_fff
#define SCB_InvalidateDCache_fake SCB_InvalidateDCache_fff_fake
#define SCB_InvalidateDCache_reset SCB_InvalidateDCache_fff_reset

#define SCB_CleanDCache SCB_CleanDCache_fff
#define SCB_CleanDCache_fake SCB_CleanDCache_fff_fake
#define SCB_CleanDCache_reset SCB_CleanDCache_fff_reset

#define SCB_CleanInvalidateDCache SCB_CleanInvalidateDCache_fff
#define SCB_CleanInvalidateDCache_fake SCB_CleanInvalidateDCache_fff_fake
#define SCB_CleanInvalidateDCache_reset SCB_CleanInvalidateDCache_fff_reset

#define SCB_InvalidateDCache_by_Addr SCB_InvalidateDCache_by_Addr_fff
#define SCB_InvalidateDCache_by_Addr_fake SCB_InvalidateDCache_by_Addr_fff_fake
#define SCB_InvalidateDCache_by_Addr_reset SCB_InvalidateDCache_by_Addr_fff_reset

#define SCB_CleanDCache_by_Addr SCB_CleanDCache_by_Addr_fff
#define SCB_CleanDCache_by_Addr_fake SCB_CleanDCache_by_Addr_fff_fake
#define SCB_CleanDCache_by_Addr_reset SCB_CleanDCache_by_Addr_fff_reset

#define SCB_CleanInvalidateDCache_by_Addr SCB_CleanInvalidateDCache_by_Addr_fff
#define SCB_CleanInvalidateDCache_by_Addr_fake SCB_CleanInvalidateDCache_by_Addr_fff_fake
#define SCB_CleanInvalidateDCache_by_Addr_reset SCB_CleanInvalidateDCache_by_Addr_fff_reset

#define SysTick_Config SysTick_Config_fff
#define SysTick_Config_fake SysTick_Config_fff_fake
#define SysTick_Config_reset SysTick_Config_fff_reset

#define ITM_SendChar ITM_SendChar_fff
#define ITM_SendChar_fake ITM_SendChar_fff_fake
#define ITM_SendChar_reset ITM_SendChar_fff_reset

#define ITM_ReceiveChar ITM_ReceiveChar_fff
#define ITM_ReceiveChar_fake ITM_ReceiveChar_fff_fake
#define ITM_ReceiveChar_reset ITM_ReceiveChar_fff_reset

#define ITM_CheckChar ITM_CheckChar_fff
#define ITM_CheckChar_fake ITM_CheckChar_fff_fake
#define ITM_CheckChar_reset ITM_CheckChar_fff_reset

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

FAKE_VALUE_FUNC(uint32_t, SCB_GetFPUType);

FAKE_VOID_FUNC(SCB_EnableICache);

FAKE_VOID_FUNC(SCB_DisableICache);

FAKE_VOID_FUNC(SCB_InvalidateICache);

FAKE_VOID_FUNC(SCB_EnableDCache);

FAKE_VOID_FUNC(SCB_DisableDCache);

FAKE_VOID_FUNC(SCB_InvalidateDCache);

FAKE_VOID_FUNC(SCB_CleanDCache);

FAKE_VOID_FUNC(SCB_CleanInvalidateDCache);

FAKE_VOID_FUNC(SCB_InvalidateDCache_by_Addr, uint32_t*, int32_t);

FAKE_VOID_FUNC(SCB_CleanDCache_by_Addr, uint32_t*, int32_t);

FAKE_VOID_FUNC(SCB_CleanInvalidateDCache_by_Addr, uint32_t*, int32_t);

FAKE_VALUE_FUNC(uint32_t, SysTick_Config, uint32_t);

FAKE_VALUE_FUNC(uint32_t, ITM_SendChar , uint32_t);

FAKE_VALUE_FUNC(int32_t, ITM_ReceiveChar);

FAKE_VALUE_FUNC(int32_t, ITM_CheckChar);

#endif /* CORE_CM7_TH_HH_ */
