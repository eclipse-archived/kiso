/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
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
 * @defgroup    HAL_CRITICAL_SECTION CriticalSection
 * @ingroup     BCDS_ESSENTIALS
 * @{
 *
 * @brief       Provides exclusive execution of code, other tasks and ISR's are blocked out.
 *
 * @details
 * By default the critical section is implemented in HAL by masking all interrupts. Using the function
 * HAL_CriticalSection_SetHooks(), an RTOS critical section implementation can be hooked in if needed to replace the
 * HAL implementation.
 */

#ifndef BCDS_HAL_CRITICALSECTION_H_
#define BCDS_HAL_CRITICALSECTION_H_

#include "BCDS_HAL.h"

/* Function data type for to handle different os Functions by setting a new Function*/
typedef Retcode_T (*HAL_CriticalSection_Hook_T)(uint32_t * count);

/**
 * @brief       Set CriticalSection Mode for use in NoOs or Rtos Context
 *
 * @param[in]   enterHook : Pointer to new CriticalSection Enter Function
 *
 * @param[in]   leaveHook : Pointer to new CriticalSection Leave Function
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when any one of the param has been NULL
 */
Retcode_T HAL_CriticalSection_SetHooks(HAL_CriticalSection_Hook_T enterHook, HAL_CriticalSection_Hook_T leaveHook);

/**
 * @brief       CriticalSection Enter
 *
 * @param[out]  count : Call by ref. Pointer count of nested CriticalSections
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when any one of the param has been NULL
 */
Retcode_T HAL_CriticalSection_Enter(uint32_t * count);

/**
 * @brief       CriticalSection Leave
 *
 * @param[out]  count : Call by ref. Pointer count of nested CriticalSections
 *
 * @retval      #RETCODE_OK when successful
 *
 * @retval      #RETCODE_NULL_POINTER when any one of the param has been NULL
 */
Retcode_T HAL_CriticalSection_Leave(uint32_t * count);

#endif /* BCDS_HAL_CRITICALSECTION_H_ */
/** @} */

