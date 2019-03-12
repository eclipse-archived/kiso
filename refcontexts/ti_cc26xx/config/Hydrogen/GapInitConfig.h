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
 *  @file       GapInitConfig.h
 *
 *
 * Configuration header for the GapInitConfig.c Module.
 *
 *
 * ****************************************************************************/

#ifndef BCDS_GAPINITCONFIG_H_
#define BCDS_GAPINITCONFIG_H_

/* public interface declaration */
#include "gapbondmgr.h"
/* public type and macro definitions */

 /** For all hardware platforms, device starts advertising upon initialization*/
#define DEFAULT_INITIAL_ADVERT_ENABLE       TRUE

/** By setting this to zero, the device will go into the waiting state after
 being discoverable for 30.72 second, and will not being advertising again
 until the enabler is set back to TRUE*/
#define DEFAULT_ADVERT_OFF_TIME     UINT16_C(0)

/** Advertising interval when device is discoverable (units of 625us, 160=100ms)*/
#define DEFAULT_ADVERTISING_INTERVAL          UINT16_C(160)

/** Limited discoverable mode advertises for 30.72s, and then stops
 General discoverable mode advertises indefinitely*/
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_GENERAL

/** Minimum connection interval (units of 1.25ms, 6=7.5ms)
 if automatic parameter update request is enabled*/
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     UINT16_C(6)

/** Maximum connection interval (units of 1.25ms, 8=10ms)
 if automatic parameter update request is enabled*/
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     UINT16_C(8)

/** Slave latency to use if automatic parameter update request is enabled*/
#define DEFAULT_DESIRED_SLAVE_LATENCY         UINT16_C(0)

/** Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter
 update request is enabled*/
#define DEFAULT_DESIRED_CONN_TIMEOUT          UINT16_C(1000)

/** Whether to enable automatic parameter update request when a connection is
 formed*/
#define DEFAULT_ENABLE_UPDATE_REQUEST         GAPROLE_LINK_PARAM_UPDATE_INITIATE_BOTH_PARAMS

/** Connection Pause Peripheral time value (in seconds)*/
#define DEFAULT_CONN_PAUSE_PERIPHERAL         UINT16_C(6)

/** Passcode for bonding */
#define DEFAULT_PASSCODE    UINT32_C(0)

/** Pair Mode */
#define DEFAULT_PAIR_MODE    GAPBOND_PAIRING_MODE_INITIATE

/** Man-In-The-Middle (MITM), if enabled, basically turns on Passkey protection in the pairing algorithm. */
#define DEFAULT_MITM_PROTECTION 	FALSE

/** I/O capability of GAP bond manager */
#define DEFAULT_IO_CAPABILITY  GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT

/** Request Bonding during the pairing process */
#define DEFAULT_BONDING_ENABLED 	TRUE

#endif /* BCDS_GAPINITCONFIG_H_ */

