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

