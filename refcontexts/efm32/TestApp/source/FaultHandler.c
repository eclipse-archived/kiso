/********************************************************************************
* Copyright (c) 2010-2019 Robert Bosch GmbH
*
* This program and the accompanying materials are made available under the
* terms of the Eclipse Public License 2.0 which is available at
* http://www.eclipse.org/legal/epl-2.0.
*
* SPDX-License-Identifier: EPL-2.0
*
* Contributors:
*    Robert Bosch GmbH - initial contribution
*
********************************************************************************/



/* module includes ********************************************************** */

/* additional interface header files */
#include "BCDS_Basics.h"

/* own header files */

/* vertor function prototype declarations */
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress);
void UsageFault_Handler(void);
void BusFault_Handler(void);
void MemManage_Handler(void);
void HardFault_Handler(void);

/* global functions ********************************************************* */

/** Debug function which is entered whenever an unrecoverable system fault occurs.
 *
 * @param pulFaultStackAddress Pointer to the saved fault stack.
 */

void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
    /* variables must be volatile to prevent the optimizer removing them */
    volatile uint32_t BCDS_UNUSED_FUNC(r0);
    volatile uint32_t BCDS_UNUSED_FUNC(r1);
    volatile uint32_t BCDS_UNUSED_FUNC(r2);
    volatile uint32_t BCDS_UNUSED_FUNC(r3);
    volatile uint32_t BCDS_UNUSED_FUNC(r12);
    volatile uint32_t BCDS_UNUSED_FUNC(lr); /* link register */
    volatile uint32_t BCDS_UNUSED_FUNC(pc); /* program counter */
    volatile uint32_t BCDS_UNUSED_FUNC(psr); /* program status register */

    r0 = pulFaultStackAddress[0];
    r1 = pulFaultStackAddress[1];
    r2 = pulFaultStackAddress[2];
    r3 = pulFaultStackAddress[3];

    r12 = pulFaultStackAddress[4];
    lr = pulFaultStackAddress[5];
    pc = pulFaultStackAddress[6];
    psr = pulFaultStackAddress[7];

    /* when the following line is hit, the variables contain the register values */
    for (;;)
        ;
}

/** Usage Fault exception handler */
__attribute__((weak, noreturn)) void UsageFault_Handler(void)
{
    for (;;)
        ;
}

/** BUS Fault exception handler */
__attribute__((weak, noreturn)) void BusFault_Handler(void)
{
    for (;;)
        ;
}

/** Memory Manager Fault exception handler */
__attribute__((weak, noreturn)) void MemManage_Handler(void)
{
    for (;;)
        ;
}

/** Hard Fault exception handler */
__attribute__((naked, noreturn)) void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
    for (;;)
        ;
}

/** ************************************************************************* */
