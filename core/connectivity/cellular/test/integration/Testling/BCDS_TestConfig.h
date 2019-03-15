/******************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *
 *  Configuration header for the testcases.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_TESTCONFIG_H_
#define BCDS_TESTCONFIG_H_

/* additional interface header files */

/* public interface declaration ********************************************* */
#include "BCDS_Retcode.h"

/* public type and macro definitions */

/**
 * @brief Enumeration of the test suites
 */
enum TestSuiteId_E
{
	CELLULAR_ON_OFF = UINT8_C(1),
	CELLULAR_NETWORK_CONNECTION,
	CELLULAR_SOCKET_OPERATION,
	CELLULAR_NEGATIVE_OPERATION,
	CELLULAR_QUERY,
	CELLULAR_LOAD,
	CELLULAR_STRESS,
	UDP_TEST,
	SMOKE_TEST,
	SMS_TEST,
	MULTITASK_TEST,
	TESTSUITE_MAX
};

typedef enum TestSuiteId_E TestSuiteId_T;

/**
 * @brief List of custom return codes for the Integration test application of Cellular
 */
enum CellularTests_Retcode_E
{
	CELLULAR_TESTS_PACKET_RX_FAIL = RETCODE_FIRST_CUSTOM_CODE, /**< A single packet reception failed */
	CELLULAR_TESTS_PACKET_TX_FAIL, /**< A single packet transmission failed */
	CELLULAR_TESTS_PACKETS_LOST_DURING_TX, /**< One or more packets are not transmitted from device */
	CELLULAR_TESTS_PACKETS_LOST_DURING_RX, /**< One or more packets are not received by the device */
	CELLULAR_TESTS_PACKETS_LOST_DURING_TX_RX, /**< One or more packets are not transmitted and received by the device */
	CELLULAR_TESTS_PACKET_MISMATCH, /**< The received packet does not match the transmitted packet */

};

typedef enum CellularTests_Retcode_E CellularTests_Retcode_T;

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* BCDS_TESTCONFIG_H_ */

/** ************************************************************************* */
