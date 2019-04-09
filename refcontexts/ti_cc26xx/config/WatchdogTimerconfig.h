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

/**
 * @file
 * description:
 * This file contains the configuration information about the watchdog timer
 * User can use the defined Macro's in this file to configure Watchdog timer
 */

#ifndef WATCHDOGTIMER_CONFIG_H_
#define WATCHDOGTIMER_CONFIG_H_

/* public interface declaration */

/* public type and macro definitions */

/**
 * NOTES:
 * Watchdog module works on the Infrastructure clock  which is 1.5 MHz,
 * since the prescaler is set to divide system clock by 32(fixed refer Technical ref manual
 * (48 / 32 = 1.5 MHz).
 * Watchdog reset occurs on timeout of the watchdog for 2 consecutive times, if
 * Watchdog interrupt is not cleared.
 * At first timeout, only the interrupt is generated (NMI /Standard Interrupt).
 * If this interrupt flag is not cleared, at second timeout, System is reset.
 *
 * In Technical reference manual: http://www.ti.com/lit/ug/swcu117d/swcu117d.pdf (page no 424)
 * Infrastructure clock prescaler always fixed for watchdog  ie:32
 *
 * Count calculation:
 * 1tick = 1/1.5Mhz = 0.00066msec
 * for 500msec - > 500/0.000666 = 0xB71B0
 *
 * counter value        timeout                 system reset time
 * 0xB71B0              0.5000  sec             1.0000 sec
 * 0xFFFFF              0.6990  sec             1.3981 sec
 * 0x16E360             1.0000  sec             2.0000 sec
 * 0x2FFFFF             2.0971  sec             4.1902 sec
 * 0x9FFFFF             6.9905  sec             13.981 sec
 */
enum WatchdogTimer_TimeoutVal_E
{
    WATCHDOG_TIMEOUT_500_MSEC = 0xB71B0UL,
    WATCHDOG_TIMEOUT_699_MSEC = 0xFFFFFUL,
    WATCHDOG_TIMEOUT_1_SEC = 0x16E360UL,
    WATCHDOG_TIMEOUT_2_SEC = 0x2FFFFFUL,
    WATCHDOG_TIMEOUT_6_SEC = 0x9FFFFFUL,
};

typedef enum WatchdogTimer_TimeoutVal_E WatchdogTimer_TimeoutVal_T;


/* public function prototype declarations */


#endif /* WATCHDOGTIMER_CONFIG_H_ */

