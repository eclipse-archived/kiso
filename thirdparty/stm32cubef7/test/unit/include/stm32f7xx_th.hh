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
 *
 * @file
 *
 * Mock-up implementation for the stm32f7xx module.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef STM32F7XX_TH_HH
#define STM32F7XX_TH_HH

/* Include or ban the real interface header */
#define __STM32F7xx_H

/* Include the needed headers */
#include "gtest.h"

#if defined(STM32F722xx)
  #include "stm32f722xx_th.hh"
#elif defined(STM32F723xx)
  #include "stm32f723xx_th.hh"
#elif defined(STM32F732xx)
  #include "stm32f732xx_th.hh"
#elif defined(STM32F733xx)
  #include "stm32f733xx_th.hh"
#elif defined(STM32F756xx)
  #include "stm32f756xx_th.hh"
#elif defined(STM32F746xx)
  #include "stm32f746xx_th.hh"
#elif defined(STM32F745xx)
  #include "stm32f745xx_th.hh"
#elif defined(STM32F765xx)
  #include "stm32f765xx_th.hh"
#elif defined(STM32F767xx)
  #include "stm32f767xx_th.hh"
#elif defined(STM32F769xx)
  #include "stm32f769xx_th.hh"
#elif defined(STM32F777xx)
  #include "stm32f777xx_th.hh"
#elif defined(STM32F779xx)
  #include "stm32f779xx_th.hh"
#else
 #error "Please select first the target STM32F7xx device used in your application (in stm32f7xx.h file)"
#endif

/* Exported_types */

typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;

typedef enum
{
  ERROR = 0U,
  SUCCESS = !ERROR
} ErrorStatus;

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */

#undef IS_FUNCTIONAL_STATE
FAKE_VOID_FUNC(IS_FUNCTIONAL_STATE, FunctionalState);

#undef SET_BIT
FAKE_VOID_FUNC(SET_BIT, uint32_t, uint32_t);

#undef CLEAR_BIT
FAKE_VOID_FUNC(CLEAR_BIT, uint32_t, uint32_t);

#undef READ_BIT
FAKE_VOID_FUNC(READ_BIT, uint32_t, uint32_t);

#undef CLEAR_REG
FAKE_VOID_FUNC(CLEAR_REG, uint32_t);

#undef WRITE_REG
FAKE_VOID_FUNC(WRITE_REG, uint32_t, uint32_t);

#undef READ_REG
FAKE_VOID_FUNC(READ_REG, uint32_t);

#undef MODIFY_REG
FAKE_VOID_FUNC(MODIFY_REG, uint32_t, uint32_t, uint32_t);

#undef POSITION_VAL
FAKE_VOID_FUNC(POSITION_VAL, uint32_t);

# endif /* STM32F7XX_TH_HH */
