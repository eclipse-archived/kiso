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
 * @file	This file provide the wrapper APIs for UdpTestSuite.
 *
 *  For more information, refer Cellular Integration Test Specification for Cellular Driver
 *  (Test Suite 1.8 for Cellular Driver UDP Data Transmission)
 */

#ifndef UDPTESTSUITE_H
#define UDPTESTSUITE_H

/* Include all headers which are needed by this file. */
#include "BCDS_Retcode.h"

/* Put the type and macro definitions here */

#define NUMBER_OF_UDP_SEND                (UINT8_C(5))
#define NUMBER_OF_UDP_RECEIVE             (UINT8_C(100))
#define FOUR_MIN_DELAY_PERIOD_IN_MS       (UINT32_C(240000)) /**< Four minutes of delay before checking for received data from server */
#define THIRTY_SEC_DELAY_PERIOD_IN_MS     (UINT32_C(30000))  /**< 30 seconds of delay before checking for received data from server */


#define UDP_SCKT_SEND_REPEAT_COUNT        (UINT32_C(100))     /**< configured to count 100 to run in jenkins. Modify to run in local PCs */
#define UDP_SEND_DELAY_50_MILLISECS 	  (UINT32_C(50))      /**< Delay of 50 Milli Sec Sec */
#define UDP_SOCKET_SEND_LENGTH_EXCEED     (UINT32_C(1473))	  /**< UDP data length 1473 representation */
#define UDP_PACKET_FAILURE_COUNT     	  (UDP_SCKT_SEND_REPEAT_COUNT / UINT32_C(10))         /**< UDP send API fail count */

/* Put the function declarations here */
Retcode_T UdpTestSuite_Init(void);

#endif /* UDPTESTSUITE_H */

