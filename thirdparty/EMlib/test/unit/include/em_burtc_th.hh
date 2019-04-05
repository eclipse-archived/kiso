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
 * This unit test header substitutes "em_burtc.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_BURTC_TH_HH_
#define EM_BURTC_TH_HH_


/* Real Header Ban */
#define __EM_BURTC_H

#define BURTC_PRESENT
#define BURTC_COUNT 1

/* Not reinventing the wheel. */
#include "efm32gg_burtc_ret.h"
#include "efm32gg_burtc.h"

/* #include "em_burtc.h" */


/* ************************************************************************** */
/* * DEFINITIONS ************************************************************ */
/* ************************************************************************** */
#define burtcClkDiv_1      1
#define burtcClkDiv_2      2
#define burtcClkDiv_4      4
#define burtcClkDiv_8      8
#define burtcClkDiv_16     16
#define burtcClkDiv_32     32
#define burtcClkDiv_64     64
#define burtcClkDiv_128    128

/* ************************************************************************** */
/* * ENUMS ****************************************************************** */
/* ************************************************************************** */

typedef enum
{
  burtcClkSelULFRCO = BURTC_CTRL_CLKSEL_ULFRCO,
  burtcClkSelLFRCO  = BURTC_CTRL_CLKSEL_LFRCO,
  burtcClkSelLFXO   = BURTC_CTRL_CLKSEL_LFXO
} BURTC_ClkSel_TypeDef;


/** BURTC mode of operation */
typedef enum
{
  burtcModeDisable = BURTC_CTRL_MODE_DISABLE,
  burtcModeEM2     = BURTC_CTRL_MODE_EM2EN,
  burtcModeEM3     = BURTC_CTRL_MODE_EM3EN,
  burtcModeEM4     = BURTC_CTRL_MODE_EM4EN,
} BURTC_Mode_TypeDef;

/** BURTC low power mode */
typedef enum
{
  burtcLPDisable = BURTC_LPMODE_LPMODE_DISABLE,
  burtcLPEnable  = BURTC_LPMODE_LPMODE_ENABLE,
  burtcLPBU      = BURTC_LPMODE_LPMODE_BUEN
} BURTC_LP_TypeDef;


/* ************************************************************************** */
/* * STRUCTS **************************************************************** */
/* ************************************************************************** */

/** BURTC initialization structure. */
typedef struct
{
  bool                 enable;
  BURTC_Mode_TypeDef   mode;
  bool                 debugRun;
  BURTC_ClkSel_TypeDef clkSel;
  uint32_t             clkDiv;
  uint32_t             lowPowerComp;
  bool                 timeStamp;
  bool                 compare0Top;
  BURTC_LP_TypeDef     lowPowerMode;
} BURTC_Init_TypeDef;


/** Default configuration for BURTC init structure */
#define BURTC_INIT_DEFAULT \
  { true,                  \
    burtcModeEM2,          \
    false,                 \
    burtcClkSelULFRCO,     \
    burtcClkDiv_1,         \
    0,                     \
    true,                  \
    false,                 \
    burtcLPDisable,        \
  }


/* ************************************************************************** */
/* * INLINE FAKES *********************************************************** */
/* ************************************************************************** */

FAKE_VOID_FUNC(BURTC_IntClear, uint32_t);
FAKE_VOID_FUNC(BURTC_IntDisable, uint32_t);
FAKE_VOID_FUNC(BURTC_IntEnable, uint32_t);
FAKE_VALUE_FUNC(uint32_t, BURTC_IntGet);
FAKE_VALUE_FUNC(uint32_t, BURTC_IntGetEnabled);
FAKE_VOID_FUNC(BURTC_IntSet, uint32_t);
FAKE_VALUE_FUNC(uint32_t, BURTC_Status);
FAKE_VOID_FUNC(BURTC_StatusClear);
FAKE_VOID_FUNC(BURTC_Enable, bool);
FAKE_VALUE_FUNC(uint32_t, BURTC_CounterGet);
FAKE_VALUE_FUNC(uint32_t, BURTC_TimestampGet);
FAKE_VOID_FUNC(BURTC_FreezeEnable, bool);
FAKE_VOID_FUNC(BURTC_Powerdown, bool);
FAKE_VOID_FUNC(BURTC_RetRegSet, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, BURTC_RetRegGet, uint32_t);
FAKE_VOID_FUNC(BURTC_Lock);
FAKE_VOID_FUNC(BURTC_Unlock);


/* ************************************************************************** */
/* * API FAKES ************************************************************** */
/* ************************************************************************** */

FAKE_VOID_FUNC(BURTC_Reset);
FAKE_VOID_FUNC(BURTC_Init, const BURTC_Init_TypeDef *);
FAKE_VOID_FUNC(BURTC_CounterReset);
FAKE_VOID_FUNC(BURTC_CompareSet, unsigned int, uint32_t);
FAKE_VALUE_FUNC(uint32_t, BURTC_CompareGet,unsigned int);
FAKE_VALUE_FUNC(uint32_t, BURTC_ClockFreqGet);


#endif /* EM_BURTC_TH_HH_ */

