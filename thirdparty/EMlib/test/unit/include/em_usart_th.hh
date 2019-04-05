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
 * This unit test header substitutes "em_usart.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_USART_TH_HH_
#define EM_USART_TH_HH_


#include "em_device_th.hh"

/**
 *  Settings taken with
 *  Giant Gecko EFM32GG390F1024 as reference
 *  Should be taken from "em_device_th.hh"
 *
 */
#define USART_PRESENT
#define USART_COUNT         3
#define UART_PRESENT
#define UART_COUNT          2

#ifdef USART_PRESENT
#include "efm32gg_usart.h"
#endif

#ifdef UART_PRESENT
#include "efm32gg_uart.h"
#endif

/* ************************************************************************** */
/* * API WRAP *************************************************************** */
/* ************************************************************************** */
#include "em_usart.h"

/* ************************************************************************** */
/* * TEST VARIABLES ********************************************************* */
/* ************************************************************************** */

/* faked variables needs to be initialized by the test fixture */
#define UART_TEST_BASE        (0x12345) /**< UART0 base address  */
#define UART_TEST        ((USART_TypeDef *) UART_TEST_BASE)         /**< UART0 base pointer */


/* faked variables needs to be initialized by the test fixture */
/** Validation of USART register block pointer reference for assert statements. */
#if (USART_COUNT == 1) && defined(USART0)
#define USART_REF_VALID(ref)    ((ref) == UART_TEST)

#elif (USART_COUNT == 1) && defined(USART1)
#define USART_REF_VALID(ref)    ((ref) == UART_TEST)

#elif (USART_COUNT == 2)
#define USART_REF_VALID(ref)    (((ref) == UART_TEST) || ((ref) == UART_TEST))

#elif (USART_COUNT == 3)
#define USART_REF_VALID(ref)    (((ref) == UART_TEST) || ((ref) == UART_TEST) || \
                                 ((ref) == UART_TEST))
#elif (USART_COUNT == 4)
#define USART_REF_VALID(ref)    (((ref) == UART_TEST) || ((ref) == UART_TEST) || \
                                 ((ref) == UART_TEST) || ((ref) == UART_TEST))
#else
#error Undefined number of USARTs.
#endif

#if defined(USART0)
#define USART_IRDA_VALID(ref)    ((ref) == UART_TEST)
#elif (USART_COUNT == 1) && defined(USART1)
#define USART_IRDA_VALID(ref)    ((ref) == UART_TEST)
#else
#define USART_IRDA_VALID(ref)    (0)
#endif

#if defined(_EFM32_TINY_FAMILY) || defined(_EFM32_ZERO_FAMILY)
#define USART_I2S_VALID(ref)    ((ref) == UART_TEST)

#elif defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY)
#define USART_I2S_VALID(ref)    (((ref) == UART_TEST) || ((ref) == UART_TEST))
#endif

#if (UART_COUNT == 1)
#define UART_REF_VALID(ref)    ((ref) == UART_TEST)
#elif (UART_COUNT == 2)
#define UART_REF_VALID(ref)    (((ref) == UART_TEST) || ((ref) == UART_TEST))
#else
#define UART_REF_VALID(ref)    (0)
#endif



/* ************************************************************************** */
/* * API FAKES ************************************************************** */
/* ************************************************************************** */

FAKE_VOID_FUNC(USART_BaudrateAsyncSet, USART_TypeDef *,uint32_t , uint32_t, USART_OVS_TypeDef);
FAKE_VALUE_FUNC(uint32_t, USART_BaudrateCalc, uint32_t , uint32_t , bool , USART_OVS_TypeDef);
FAKE_VALUE_FUNC(uint32_t, USART_BaudrateGet, USART_TypeDef *);
FAKE_VOID_FUNC(USART_BaudrateSyncSet, USART_TypeDef *, uint32_t , uint32_t );
FAKE_VOID_FUNC(USART_Enable, USART_TypeDef *, USART_Enable_TypeDef );
FAKE_VOID_FUNC(USART_InitAsync, USART_TypeDef *, const USART_InitAsync_TypeDef *);
FAKE_VOID_FUNC(USART_InitSync, USART_TypeDef *, const USART_InitSync_TypeDef *);

FAKE_VOID_FUNC(USART_InitIrDA, const USART_InitIrDA_TypeDef *);
FAKE_VOID_FUNC(USART_InitI2s, USART_TypeDef *, USART_InitI2s_TypeDef *);
FAKE_VOID_FUNC(USART_InitPrsTrigger, USART_TypeDef *, const USART_PrsTriggerInit_TypeDef *);

FAKE_VOID_FUNC(USART_Reset, USART_TypeDef *);
FAKE_VALUE_FUNC(uint8_t, USART_Rx, USART_TypeDef *);
FAKE_VALUE_FUNC(uint16_t, USART_RxDouble, USART_TypeDef *);
FAKE_VALUE_FUNC(uint32_t, USART_RxDoubleExt, USART_TypeDef *);
FAKE_VALUE_FUNC(uint16_t, USART_RxExt, USART_TypeDef *);

FAKE_VALUE_FUNC(uint8_t, USART_SpiTransfer, USART_TypeDef *, uint8_t);
FAKE_VOID_FUNC(USART_Tx, USART_TypeDef *, uint8_t );
FAKE_VOID_FUNC(USART_TxDouble, USART_TypeDef *, uint16_t );
FAKE_VOID_FUNC(USART_TxDoubleExt, USART_TypeDef *, uint32_t );
FAKE_VOID_FUNC(USART_TxExt, USART_TypeDef *, uint16_t );


#endif /* EM_USART_TH_HH_ */

/** ************************************************************************* */
