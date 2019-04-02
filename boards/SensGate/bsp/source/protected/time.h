/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch GmbH. 
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
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Provides microsecond time stamping based on internal timer.
 *
 * @details 
 */
#ifndef PROTECTED_TIME_H_
#define PROTECTED_TIME_H_

#include "BCDS_HAL.h"

/**
 * Returns the microseconds elapsed since last system reset.
 */
uint64_t Time_GetTimeStamp(void);

/**
 * This function initializes the microseconds timer used to run the system clock.
 * LPTIM1 is used as microseconds timer and is clocked at 1MHz rate using HSI16(16MHz) prescaled
 * by 16.
 */
Retcode_T Time_StartTime(uint64_t initialValue);

/**
 * This function adds an offset to the timeUs value.,
 */
void Time_AdjustTime(uint64_t timeOffset);

#endif /* PROTECTED_TIME_H_ */
