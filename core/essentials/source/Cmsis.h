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

#ifndef CMSIS_H_
#define CMSIS_H_

#if defined (stm32)

    #if defined (stm32l4)
        #include "stm32l4xx_hal.h"
        #include "stm32l4xx.h"
        #include "core_cm4.h"
    #elif defined (stm32f7)
        #include "stm32f7xx_hal.h"
        #include "stm32f7xx.h"
        #include "core_cm7.h"
    #else
        #error "The STM32 family is not defined or not supported."
    #endif

    #include "core_cmInstr.h"
    #include "core_cmFunc.h"

#elif defined (efm32)
#include "em_device.h"
#include "core_cm3.h"
#include "core_cmInstr.h"
#include "core_cmFunc.h"


#elif defined (ti_cc26xx)

#else
#error "invalid target"
#endif

#endif /* CMSIS_H_ */

