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

#ifndef CMSIS_GCC_TH_HH_
#define CMSIS_GCC_TH_HH_

#ifndef __STATIC_INLINE
#define __STATIC_INLINE     inline
#endif /* ifndef __STATIC_INLINE */
#ifndef __ASM
#define __ASM               asm
#endif /* ifndef __ASM */

/* include or ban the real interface header */
#include "cmsis_gcc.h"

/* NOTE: As cmsis_gcc defines the functions INSIDE the .h file, we have to apply
 * some hacks to be able to use fff-fakes instead.
 *
 * What we do here is, first include the original header like it is, then we
 * define all function calls to the original functions to go to a "_fff"
 * version.
 * Below we then define fakes for these "_fff" versions, so all calls to e.g.
 * __enable_irq in our C.U.T will actually go to __enable_irq_fff, which is the
 * faked version.
 * The second set of macros is there to enable us to use the
 * __enable_irq_fake and _reset inside our unit-test file.
 */
#define __enable_irq __enable_irq_fff
#define __enable_irq_fake __enable_irq_fff_fake
#define __enable_irq_reset __enable_irq_fff_reset

#define __disable_irq __disable_irq_fff
#define __disable_irq_fake __disable_irq_fff_fake
#define __disable_irq_reset __disable_irq_fff_reset

#define __get_CONTROL __get_CONTROL_fff
#define __get_CONTROL_fake __get_CONTROL_fff_fake
#define __get_CONTROL_reset __get_CONTROL_fff_reset

#define __set_CONTROL __set_CONTROL_fff
#define __set_CONTROL_fake __set_CONTROL_fff_fake
#define __set_CONTROL_reset __set_CONTROL_fff_reset

#define __get_IPSR __get_IPSR_fff
#define __get_IPSR_fake __get_IPSR_fff_fake
#define __get_IPSR_reset __get_IPSR_fff_reset

#define __get_APSR __get_APSR_fff
#define __get_APSR_fake __get_APSR_fff_fake
#define __get_APSR_reset __get_APSR_fff_reset

#define __get_xPSR __get_xPSR_fff
#define __get_xPSR_fake __get_xPSR_fff_fake
#define __get_xPSR_reset __get_xPSR_fff_reset

#define __get_PSP __get_PSP_fff
#define __get_PSP_fake __get_PSP_fff_fake
#define __get_PSP_reset __get_PSP_fff_reset

#define __set_PSP __set_PSP_fff
#define __set_PSP_fake __set_PSP_fff_fake
#define __set_PSP_reset __set_PSP_fff_reset

#define __get_MSP __get_MSP_fff
#define __get_MSP_fake __get_MSP_fff_fake
#define __get_MSP_reset __get_MSP_fff_reset

#define __set_MSP __set_MSP_fff
#define __set_MSP_fake __set_MSP_fff_fake
#define __set_MSP_reset __set_MSP_fff_reset

#define __get_PRIMASK __get_PRIMASK_fff
#define __get_PRIMASK_fake __get_PRIMASK_fff_fake
#define __get_PRIMASK_reset __get_PRIMASK_fff_reset

#define __set_PRIMASK __set_PRIMASK_fff
#define __set_PRIMASK_fake __set_PRIMASK_fff_fake
#define __set_PRIMASK_reset __set_PRIMASK_fff_reset

#if (__CORTEX_M >= 0x03U)
#define __enable_fault_irq __enable_fault_irq_fff
#define __enable_fault_irq_fake __enable_fault_irq_fff_fake
#define __enable_fault_irq_reset __enable_fault_irq_fff_reset

#define __disable_fault_irq __disable_fault_irq_fff
#define __disable_fault_irq_fake __disable_fault_irq_fff_fake
#define __disable_fault_irq_reset __disable_fault_irq_fff_reset

#define __get_BASEPRI __get_BASEPRI_fff
#define __get_BASEPRI_fake __get_BASEPRI_fff_fake
#define __get_BASEPRI_reset __get_BASEPRI_fff_reset

#define __set_BASEPRI __set_BASEPRI_fff
#define __set_BASEPRI_fake __set_BASEPRI_fff_fake
#define __set_BASEPRI_reset __set_BASEPRI_fff_reset

#define __set_BASEPRI_MAX __set_BASEPRI_MAX_fff
#define __set_BASEPRI_MAX_fake __set_BASEPRI_MAX_fff_fake
#define __set_BASEPRI_MAX_reset __set_BASEPRI_MAX_fff_reset

#define __get_FAULTMASK __get_FAULTMASK_fff
#define __get_FAULTMASK_fake __get_FAULTMASK_fff_fake
#define __get_FAULTMASK_reset __get_FAULTMASK_fff_reset

#define __set_FAULTMASK __set_FAULTMASK_fff
#define __set_FAULTMASK_fake __set_FAULTMASK_fff_fake
#define __set_FAULTMASK_reset __set_FAULTMASK_fff_reset

#endif /* (__CORTEX_M >= 0x03U) */
#if (__CORTEX_M == 0x04U) || (__CORTEX_M == 0x07U)
#define __get_FPSCR __get_FPSCR_fff
#define __get_FPSCR_fake __get_FPSCR_fff_fake
#define __get_FPSCR_reset __get_FPSCR_fff_reset

#define __set_FPSCR __set_FPSCR_fff
#define __set_FPSCR_fake __set_FPSCR_fff_fake
#define __set_FPSCR_reset __set_FPSCR_fff_reset

#endif /* (__CORTEX_M == 0x04U) || (__CORTEX_M == 0x07U) */
#define __NOP __NOP_fff
#define __NOP_fake __NOP_fff_fake
#define __NOP_reset __NOP_fff_reset

#define __WFI __WFI_fff
#define __WFI_fake __WFI_fff_fake
#define __WFI_reset __WFI_fff_reset

#define __WFE __WFE_fff
#define __WFE_fake __WFE_fff_fake
#define __WFE_reset __WFE_fff_reset

#define __SEV __SEV_fff
#define __SEV_fake __SEV_fff_fake
#define __SEV_reset __SEV_fff_reset

#define __ISB __ISB_fff
#define __ISB_fake __ISB_fff_fake
#define __ISB_reset __ISB_fff_reset

#define __DSB __DSB_fff
#define __DSB_fake __DSB_fff_fake
#define __DSB_reset __DSB_fff_reset

#define __DMB __DMB_fff
#define __DMB_fake __DMB_fff_fake
#define __DMB_reset __DMB_fff_reset

#define __REV __REV_fff
#define __REV_fake __REV_fff_fake
#define __REV_reset __REV_fff_reset

#define __REV16 __REV16_fff
#define __REV16_fake __REV16_fff_fake
#define __REV16_reset __REV16_fff_reset

#define __REVSH __REVSH_fff
#define __REVSH_fake __REVSH_fff_fake
#define __REVSH_reset __REVSH_fff_reset

#define __ROR __ROR_fff
#define __ROR_fake __ROR_fff_fake
#define __ROR_reset __ROR_fff_reset

#define __RBIT __RBIT_fff
#define __RBIT_fake __RBIT_fff_fake
#define __RBIT_reset __RBIT_fff_reset

#if (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U)
#define __LDREXB __LDREXB_fff
#define __LDREXB_fake __LDREXB_fff_fake
#define __LDREXB_reset __LDREXB_fff_reset

#define __LDREXH __LDREXH_fff
#define __LDREXH_fake __LDREXH_fff_fake
#define __LDREXH_reset __LDREXH_fff_reset

#define __LDREXW __LDREXW_fff
#define __LDREXW_fake __LDREXW_fff_fake
#define __LDREXW_reset __LDREXW_fff_reset

#define __STREXB __STREXB_fff
#define __STREXB_fake __STREXB_fff_fake
#define __STREXB_reset __STREXB_fff_reset

#define __STREXH __STREXH_fff
#define __STREXH_fake __STREXH_fff_fake
#define __STREXH_reset __STREXH_fff_reset

#define __STREXW __STREXW_fff
#define __STREXW_fake __STREXW_fff_fake
#define __STREXW_reset __STREXW_fff_reset

#define __CLREX __CLREX_fff
#define __CLREX_fake __CLREX_fff_fake
#define __CLREX_reset __CLREX_fff_reset

#define __RRX __RRX_fff
#define __RRX_fake __RRX_fff_fake
#define __RRX_reset __RRX_fff_reset

#define __LDRBT __LDRBT_fff
#define __LDRBT_fake __LDRBT_fff_fake
#define __LDRBT_reset __LDRBT_fff_reset

#define __LDRHT __LDRHT_fff
#define __LDRHT_fake __LDRHT_fff_fake
#define __LDRHT_reset __LDRHT_fff_reset

#define __LDRT __LDRT_fff
#define __LDRT_fake __LDRT_fff_fake
#define __LDRT_reset __LDRT_fff_reset

#define __STRBT __STRBT_fff
#define __STRBT_fake __STRBT_fff_fake
#define __STRBT_reset __STRBT_fff_reset

#define __STRHT __STRHT_fff
#define __STRHT_fake __STRHT_fff_fake
#define __STRHT_reset __STRHT_fff_reset

#define __STRT __STRT_fff
#define __STRT_fake __STRT_fff_fake
#define __STRT_reset __STRT_fff_reset

#endif /* (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U) */
#if (__CORTEX_M >= 0x04U)  /* only for Cortex-M4 and above */
#define __SADD8 __SADD8_fff
#define __SADD8_fake __SADD8_fff_fake
#define __SADD8_reset __SADD8_fff_reset

#define __QADD8 __QADD8_fff
#define __QADD8_fake __QADD8_fff_fake
#define __QADD8_reset __QADD8_fff_reset

#define __SHADD8 __SHADD8_fff
#define __SHADD8_fake __SHADD8_fff_fake
#define __SHADD8_reset __SHADD8_fff_reset

#define __UADD8 __UADD8_fff
#define __UADD8_fake __UADD8_fff_fake
#define __UADD8_reset __UADD8_fff_reset

#define __UQADD8 __UQADD8_fff
#define __UQADD8_fake __UQADD8_fff_fake
#define __UQADD8_reset __UQADD8_fff_reset

#define __UHADD8 __UHADD8_fff
#define __UHADD8_fake __UHADD8_fff_fake
#define __UHADD8_reset __UHADD8_fff_reset

#define __SSUB8 __SSUB8_fff
#define __SSUB8_fake __SSUB8_fff_fake
#define __SSUB8_reset __SSUB8_fff_reset

#define __QSUB8 __QSUB8_fff
#define __QSUB8_fake __QSUB8_fff_fake
#define __QSUB8_reset __QSUB8_fff_reset

#define __SHSUB8 __SHSUB8_fff
#define __SHSUB8_fake __SHSUB8_fff_fake
#define __SHSUB8_reset __SHSUB8_fff_reset

#define __USUB8 __USUB8_fff
#define __USUB8_fake __USUB8_fff_fake
#define __USUB8_reset __USUB8_fff_reset

#define __UQSUB8 __UQSUB8_fff
#define __UQSUB8_fake __UQSUB8_fff_fake
#define __UQSUB8_reset __UQSUB8_fff_reset

#define __UHSUB8 __UHSUB8_fff
#define __UHSUB8_fake __UHSUB8_fff_fake
#define __UHSUB8_reset __UHSUB8_fff_reset

#define __SADD16 __SADD16_fff
#define __SADD16_fake __SADD16_fff_fake
#define __SADD16_reset __SADD16_fff_reset

#define __QADD16 __QADD16_fff
#define __QADD16_fake __QADD16_fff_fake
#define __QADD16_reset __QADD16_fff_reset

#define __SHADD16 __SHADD16_fff
#define __SHADD16_fake __SHADD16_fff_fake
#define __SHADD16_reset __SHADD16_fff_reset

#define __UADD16 __UADD16_fff
#define __UADD16_fake __UADD16_fff_fake
#define __UADD16_reset __UADD16_fff_reset

#define __UQADD16 __UQADD16_fff
#define __UQADD16_fake __UQADD16_fff_fake
#define __UQADD16_reset __UQADD16_fff_reset

#define __UHADD16 __UHADD16_fff
#define __UHADD16_fake __UHADD16_fff_fake
#define __UHADD16_reset __UHADD16_fff_reset

#define __SSUB16 __SSUB16_fff
#define __SSUB16_fake __SSUB16_fff_fake
#define __SSUB16_reset __SSUB16_fff_reset

#define __QSUB16 __QSUB16_fff
#define __QSUB16_fake __QSUB16_fff_fake
#define __QSUB16_reset __QSUB16_fff_reset

#define __SHSUB16 __SHSUB16_fff
#define __SHSUB16_fake __SHSUB16_fff_fake
#define __SHSUB16_reset __SHSUB16_fff_reset

#define __USUB16 __USUB16_fff
#define __USUB16_fake __USUB16_fff_fake
#define __USUB16_reset __USUB16_fff_reset

#define __UQSUB16 __UQSUB16_fff
#define __UQSUB16_fake __UQSUB16_fff_fake
#define __UQSUB16_reset __UQSUB16_fff_reset

#define __UHSUB16 __UHSUB16_fff
#define __UHSUB16_fake __UHSUB16_fff_fake
#define __UHSUB16_reset __UHSUB16_fff_reset

#define __SASX __SASX_fff
#define __SASX_fake __SASX_fff_fake
#define __SASX_reset __SASX_fff_reset

#define __QASX __QASX_fff
#define __QASX_fake __QASX_fff_fake
#define __QASX_reset __QASX_fff_reset

#define __SHASX __SHASX_fff
#define __SHASX_fake __SHASX_fff_fake
#define __SHASX_reset __SHASX_fff_reset

#define __UASX __UASX_fff
#define __UASX_fake __UASX_fff_fake
#define __UASX_reset __UASX_fff_reset

#define __UQASX __UQASX_fff
#define __UQASX_fake __UQASX_fff_fake
#define __UQASX_reset __UQASX_fff_reset

#define __UHASX __UHASX_fff
#define __UHASX_fake __UHASX_fff_fake
#define __UHASX_reset __UHASX_fff_reset

#define __SSAX __SSAX_fff
#define __SSAX_fake __SSAX_fff_fake
#define __SSAX_reset __SSAX_fff_reset

#define __QSAX __QSAX_fff
#define __QSAX_fake __QSAX_fff_fake
#define __QSAX_reset __QSAX_fff_reset

#define __SHSAX __SHSAX_fff
#define __SHSAX_fake __SHSAX_fff_fake
#define __SHSAX_reset __SHSAX_fff_reset

#define __USAX __USAX_fff
#define __USAX_fake __USAX_fff_fake
#define __USAX_reset __USAX_fff_reset

#define __UQSAX __UQSAX_fff
#define __UQSAX_fake __UQSAX_fff_fake
#define __UQSAX_reset __UQSAX_fff_reset

#define __UHSAX __UHSAX_fff
#define __UHSAX_fake __UHSAX_fff_fake
#define __UHSAX_reset __UHSAX_fff_reset

#define __USAD8 __USAD8_fff
#define __USAD8_fake __USAD8_fff_fake
#define __USAD8_reset __USAD8_fff_reset

#define __USADA8 __USADA8_fff
#define __USADA8_fake __USADA8_fff_fake
#define __USADA8_reset __USADA8_fff_reset

#define __UXTB16 __UXTB16_fff
#define __UXTB16_fake __UXTB16_fff_fake
#define __UXTB16_reset __UXTB16_fff_reset

#define __UXTAB16 __UXTAB16_fff
#define __UXTAB16_fake __UXTAB16_fff_fake
#define __UXTAB16_reset __UXTAB16_fff_reset

#define __SXTB16 __SXTB16_fff
#define __SXTB16_fake __SXTB16_fff_fake
#define __SXTB16_reset __SXTB16_fff_reset

#define __SXTAB16 __SXTAB16_fff
#define __SXTAB16_fake __SXTAB16_fff_fake
#define __SXTAB16_reset __SXTAB16_fff_reset

#define __SMUAD __SMUAD_fff
#define __SMUAD_fake __SMUAD_fff_fake
#define __SMUAD_reset __SMUAD_fff_reset

#define __SMUADX __SMUADX_fff
#define __SMUADX_fake __SMUADX_fff_fake
#define __SMUADX_reset __SMUADX_fff_reset

#define __SMLAD __SMLAD_fff
#define __SMLAD_fake __SMLAD_fff_fake
#define __SMLAD_reset __SMLAD_fff_reset

#define __SMLADX __SMLADX_fff
#define __SMLADX_fake __SMLADX_fff_fake
#define __SMLADX_reset __SMLADX_fff_reset

#define __SMLALD __SMLALD_fff
#define __SMLALD_fake __SMLALD_fff_fake
#define __SMLALD_reset __SMLALD_fff_reset

#define __SMLALDX __SMLALDX_fff
#define __SMLALDX_fake __SMLALDX_fff_fake
#define __SMLALDX_reset __SMLALDX_fff_reset

#define __SMUSD __SMUSD_fff
#define __SMUSD_fake __SMUSD_fff_fake
#define __SMUSD_reset __SMUSD_fff_reset

#define __SMUSDX __SMUSDX_fff
#define __SMUSDX_fake __SMUSDX_fff_fake
#define __SMUSDX_reset __SMUSDX_fff_reset

#define __SMLSD __SMLSD_fff
#define __SMLSD_fake __SMLSD_fff_fake
#define __SMLSD_reset __SMLSD_fff_reset

#define __SMLSDX __SMLSDX_fff
#define __SMLSDX_fake __SMLSDX_fff_fake
#define __SMLSDX_reset __SMLSDX_fff_reset

#define __SMLSLD __SMLSLD_fff
#define __SMLSLD_fake __SMLSLD_fff_fake
#define __SMLSLD_reset __SMLSLD_fff_reset

#define __SMLSLDX __SMLSLDX_fff
#define __SMLSLDX_fake __SMLSLDX_fff_fake
#define __SMLSLDX_reset __SMLSLDX_fff_reset

#define __SEL __SEL_fff
#define __SEL_fake __SEL_fff_fake
#define __SEL_reset __SEL_fff_reset

#define __QADD __QADD_fff
#define __QADD_fake __QADD_fff_fake
#define __QADD_reset __QADD_fff_reset

#define __QSUB __QSUB_fff
#define __QSUB_fake __QSUB_fff_fake
#define __QSUB_reset __QSUB_fff_reset

#define __SMMLA __SMMLA_fff
#define __SMMLA_fake __SMMLA_fff_fake
#define __SMMLA_reset __SMMLA_fff_reset

#endif /* (__CORTEX_M >= 0x04) */

/* Include the needed headers */
#include "gtest.h"

/* Faked variables needs to be initialized by the test fixture */
FAKE_VOID_FUNC(__enable_irq);
FAKE_VOID_FUNC(__disable_irq);
FAKE_VALUE_FUNC(uint32_t, __get_CONTROL);
FAKE_VOID_FUNC(__set_CONTROL, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __get_IPSR);
FAKE_VALUE_FUNC(uint32_t, __get_APSR);
FAKE_VALUE_FUNC(uint32_t, __get_xPSR);
FAKE_VALUE_FUNC(uint32_t, __get_PSP);
FAKE_VOID_FUNC(__set_PSP, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __get_MSP);
FAKE_VOID_FUNC(__set_MSP, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __get_PRIMASK);
FAKE_VOID_FUNC(__set_PRIMASK, uint32_t);
#if (__CORTEX_M >= 0x03U)
FAKE_VOID_FUNC(__enable_fault_irq);
FAKE_VOID_FUNC(__disable_fault_irq);
FAKE_VALUE_FUNC(uint32_t, __get_BASEPRI);
FAKE_VOID_FUNC(__set_BASEPRI, uint32_t);
FAKE_VOID_FUNC(__set_BASEPRI_MAX, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __get_FAULTMASK);
FAKE_VOID_FUNC(__set_FAULTMASK, uint32_t);
#endif /* (__CORTEX_M >= 0x03U) */
#if (__CORTEX_M == 0x04U) || (__CORTEX_M == 0x07U)
FAKE_VALUE_FUNC(uint32_t, __get_FPSCR);
FAKE_VOID_FUNC(__set_FPSCR, uint32_t);
#endif /* (__CORTEX_M == 0x04U) || (__CORTEX_M == 0x07U) */
FAKE_VOID_FUNC(__NOP);
FAKE_VOID_FUNC(__WFI);
FAKE_VOID_FUNC(__WFE);
FAKE_VOID_FUNC(__SEV);
FAKE_VOID_FUNC(__ISB);
FAKE_VOID_FUNC(__DSB);
FAKE_VOID_FUNC(__DMB);
FAKE_VALUE_FUNC(uint32_t, __REV, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __REV16, uint32_t);
FAKE_VALUE_FUNC(int32_t, __REVSH, int32_t);
FAKE_VALUE_FUNC(uint32_t, __ROR, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __RBIT, uint32_t);
#if (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U)
FAKE_VALUE_FUNC(uint8_t, __LDREXB, volatile uint8_t*);
FAKE_VALUE_FUNC(uint16_t, __LDREXH, volatile uint16_t*);
FAKE_VALUE_FUNC(uint32_t, __LDREXW, volatile uint32_t*);
FAKE_VALUE_FUNC(uint32_t, __STREXB, uint8_t, volatile uint8_t*);
FAKE_VALUE_FUNC(uint32_t, __STREXH, uint16_t, volatile uint8_t*);
FAKE_VALUE_FUNC(uint32_t, __STREXW, uint32_t, volatile uint32_t*);
FAKE_VOID_FUNC(__CLREX);
#undef __SSAT
FAKE_VALUE_FUNC(uint32_t, __SSAT, int32_t, int32_t);
#undef __USAT
FAKE_VALUE_FUNC(uint32_t, __USAT, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __RRX, uint32_t);
FAKE_VALUE_FUNC(uint8_t, __LDRBT, volatile uint8_t*);
FAKE_VALUE_FUNC(uint16_t, __LDRHT, volatile uint16_t*);
FAKE_VALUE_FUNC(uint32_t, __LDRT, volatile uint32_t*);
FAKE_VOID_FUNC(__STRBT, uint8_t, volatile uint8_t*);
FAKE_VOID_FUNC(__STRHT, uint16_t, volatile uint16_t*);
FAKE_VOID_FUNC(__STRT, uint32_t, volatile uint32_t*);
#endif /* (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U) */
#if (__CORTEX_M >= 0x04U)  /* only for Cortex-M4 and above */
FAKE_VALUE_FUNC(uint32_t, __SADD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __QADD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SHADD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UADD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UQADD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UHADD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SSUB8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __QSUB8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SHSUB8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __USUB8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UQSUB8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UHSUB8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SADD16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __QADD16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SHADD16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UADD16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UQADD16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UHADD16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SSUB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __QSUB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SHSUB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __USUB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UQSUB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UHSUB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SASX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __QASX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SHASX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UASX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UQASX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UHASX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SSAX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __QSAX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SHSAX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __USAX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UQSAX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UHSAX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __USAD8, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __USADA8, uint32_t, uint32_t, uint32_t);
#undef __SSAT16
FAKE_VALUE_FUNC(int32_t, __SSAT16, int32_t, int32_t);
#undef __USAT16
FAKE_VALUE_FUNC(uint32_t, __USAT16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UXTB16, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __UXTAB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SXTB16, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SXTAB16, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMUAD, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMUADX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLAD, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLADX, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLALD, uint32_t, uint32_t, uint64_t);
FAKE_VALUE_FUNC(uint32_t, __SMLALDX, uint32_t, uint32_t, uint64_t);
FAKE_VALUE_FUNC(uint32_t, __SMUSD, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMUSDX, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLSD, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLSDX, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLSLD, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMLSLDX, uint32_t, uint32_t, uint64_t);
FAKE_VALUE_FUNC(uint32_t, __SEL, uint32_t, uint32_t);
FAKE_VALUE_FUNC(int32_t, __QADD, int32_t, int32_t);
FAKE_VALUE_FUNC(int32_t, __QSUB, int32_t, int32_t);
#undef __PKHBT
FAKE_VALUE_FUNC(uint32_t, __PKHBT, uint32_t, uint32_t);
#undef __PKHTB
FAKE_VALUE_FUNC(uint32_t, __PKHTB, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, __SMMLA, int32_t, int32_t, int32_t);
#endif /* (__CORTEX_M >= 0x04) */

/* Mock-ups for the provided interfaces */

#endif /* CMSIS_GCC_TH_HH_ */
