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
 * @brief GNSS-Ublox7 driver config header.
 */

#ifndef BCDS_GNSSUBLOX7CONFIG_H_
#define BCDS_GNSSUBLOX7CONFIG_H_

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */
#define GNSSUBLOX7_RECEIVE_BUFFER_SIZE  (UINT32_C(256))
#define GNSSUBLOX7_RECEIVE_BUFFER_DELAY (pdMS_TO_TICKS(10))
#define GNSSUBLOX7_SIGNIFICANT_RECEIVE_BUFFER_SIZE \
    (GNSSUBLOX7_RECEIVE_BUFFER_SIZE / 2)

#define GNSSUBLOX7_TASK_NAME            ("UBLOX7-DRIVER")
#define GNSSUBLOX7_TASK_STACK_SIZE      (UINT32_C(350))
#define GNSSUBLOX7_TASK_PRIORITY        (UINT32_C(3))

#define GNSSUBLOX7_WAIT_ACK_TIMEOUT     (pdMS_TO_TICKS(3000))
#define GNSSUBLOX7_POWER_ON_TIMEOUT     (pdMS_TO_TICKS(3000))
#define GNSSUBLOX7_WAIT_NAV_TIMEOUT     (pdMS_TO_TICKS(3000))

#define GNSSUBLOX7_SET_EGNOS_SBAS_PRNS(SCANMODE1, SCANMODE2) \
    do \
    { \
        SCANMODE1.Prn120 = 1; \
        SCANMODE1.Prn136 = 1; \
    } \
    while(false)

/* Put the function declarations here */

#endif /* BCDS_GNSSUBLOX7CONFIG_H_ */

