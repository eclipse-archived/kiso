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
 * This unit test header substitutes "em_leuart.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */
#ifndef EM_LEUART_TH_HH_
#define EM_LEUART_TH_HH_


#include "em_device_th.hh"

/**
 *  Settings taken with
 *  Giant Gecko EFM32GG390F1024 as reference
 *  Should be taken from "em_device_th.hh"
 *
 */
#define LEUART_PRESENT
#define LEUART_COUNT        2

#ifdef LEUART_PRESENT
#include "efm32gg_leuart.h"
#endif

/* ************************************************************************** */
/* * API WRAP *************************************************************** */
/* ************************************************************************** */
#include "em_leuart.h"


/* ************************************************************************** */
/* * TEST VARIABLES ********************************************************* */
/* ************************************************************************** */

#define LEUART_TEST_BASE        (0x12345) /**< UART0 base address  */

#define LEUART_TEST        ((LEUART_TypeDef *) LEUART_TEST_BASE)         /**< UART0 base pointer */

/* faked variables needs to be initialized by the test fixture */
#if (LEUART_COUNT == 1)
#define LEUART_REF_VALID(ref)    ((ref) == LEUART_TEST)
#elif (LEUART_COUNT == 2)
#define LEUART_REF_VALID(ref)    (((ref) == LEUART_TEST) || ((ref) == LEUART_TEST))
#else
#error Undefined number of low energy UARTs (LEUART).
#endif



/* ************************************************************************** */
/* * API FAKES ************************************************************** */
/* ************************************************************************** */

FAKE_VALUE_FUNC(uint32_t, LEUART_BaudrateCalc, uint32_t , uint32_t );
FAKE_VALUE_FUNC(uint32_t, LEUART_BaudrateGet, LEUART_TypeDef *);
FAKE_VOID_FUNC(LEUART_BaudrateSet, LEUART_TypeDef *, uint32_t , uint32_t );
FAKE_VOID_FUNC(LEUART_Enable, LEUART_TypeDef *, LEUART_Enable_TypeDef);
FAKE_VOID_FUNC(LEUART_FreezeEnable, LEUART_TypeDef *, bool);
FAKE_VOID_FUNC(LEUART_Init, LEUART_TypeDef* , LEUART_Init_TypeDef const *);
FAKE_VOID_FUNC(LEUART_TxDmaInEM2Enable, LEUART_TypeDef *, bool);
FAKE_VOID_FUNC(LEUART_RxDmaInEM2Enable, LEUART_TypeDef *, bool);

FAKE_VOID_FUNC(LEUART_Reset, LEUART_TypeDef *);
FAKE_VALUE_FUNC(uint8_t, LEUART_Rx, LEUART_TypeDef *);
FAKE_VALUE_FUNC(uint16_t, LEUART_RxExt, LEUART_TypeDef *);
FAKE_VOID_FUNC(LEUART_Tx, LEUART_TypeDef *, uint8_t );
FAKE_VOID_FUNC(LEUART_TxExt, LEUART_TypeDef *, uint16_t);

#endif /* EM_LEUART_TH_HH_ */

/** ************************************************************************* */
