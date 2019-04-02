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
 *  @file
 *
 *  The fault handler module can aid software developers to analyze exceptions
 *  thrown by the MCU.
 *
 *  The module defines a exception's handler function for each CMSIS defined
 *  fault handlers.
 *
 *  In case of exceptions, the hard fault exception handler will be called
 *  which will save the context into C variables which can be checked in the
 *  debugger after the exception event as the event handler blocks the
 *  application.
 *
 *  @note The SCB structure from CMSIS can be monitored in the debugger to get
 *        additional information about the hard fault failure.
 *        For example, SCB->CFSR can tell what kind of exception occurred.
 *
 *  @note The module is only active in debug builds!
 *
 *  Release builds must be built with the NDEBUG symbol defined in the
 *  makefile.
 *
 *  @note The library is specifically designed for ARM based MCUs implementing
 *  the ARM Embedded ABI.
 */

#include "BCDS_HAL.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress);
void UsageFault_Handler(void);
void BusFault_Handler(void);
void MemManage_Handler(void);
void HardFault_Handler(void);

/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/

/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/

/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

/** Debug function which is entered whenever an unrecoverable system fault occurs.
 *
 * @param pulFaultStackAddress Pointer to the saved fault stack.
 */
/*lint -e550*/
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
/*lint +e550*/

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
#ifndef UNIT_TESTING
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
#else
void HardFault_Handler(void)
{};
#endif

/** ************************************************************************* */
