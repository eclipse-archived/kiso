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
 *
 * @file
 *
 * @brief Control functions for the modem
 *
 * @details Enables control and verification of the current status of the modem using the following functions:
 *
 * * Cellular_Startup
 * * Cellular_Shutdown
 * * Cellular_Reset
 * * Cellular_GetPowerStatus
 * * Cellular_IsSimCardInserted
 * * Cellular_GetIccid
 * * Cellular_EnableEcho
 */
 
#ifndef QCTEL_CONTROL_H
#define QCTEL_CONTROL_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

#define CELLULAR_BUFFER_SIZE_ICCID (UINT32_C(24))  //-- at least 23 without zero termination (some ICCID are only 20 bytes long)

/**
 * @brief The power status of the Cellular device
 */
enum Cellular_PowerStatus_E
{
    CELLULAR_POWER_STATUS_UNKNOWN,
    CELLULAR_POWER_STATUS_OFF,
    CELLULAR_POWER_STATUS_ON
};
typedef enum Cellular_PowerStatus_E Cellular_PowerStatus_T;

/**
 * @brief The network status of the Cellular cellular modem
 */
enum CellularNetwork_Status_E
{
    CELLULAR_NETWORK_STATUS_UNKNOWN = 0,
    CELLULAR_NETWORK_STATUS_NO_SIM,
    CELLULAR_NETWORK_STATUS_NO_NETWORK,
    CELLULAR_NETWORK_STATUS_REGISTRATION_DENIED,
    CELLULAR_NETWORK_STATUS_REGISTERED
};
typedef enum CellularNetwork_Status_E CellularNetwork_Status_T;

typedef enum {
    CELLULAR_CME_ERROR_NONE = 0,
    CELLULAR_CME_ERROR_MAX = 47
} Cellular_CmeError_T;

/**
 * @brief Callback used for signaling a network status change.
 *
 * @param OldStatus the network status prior to the status change
 * @param NewStatus the network status after to the status change
 */
typedef void (*CellularNetwork_StatusChanged_Callback_T)(CellularNetwork_Status_T OldStatus, CellularNetwork_Status_T NewStatus);

/* Put the function declarations here */

/**
 * @brief Powers up the modem, factory resets the current profile and starts connecting to the network
 *
 * @retval RETCODE_OK The modem was switched on, powered up and is responsive to commands
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond after being switched on
 * @retval RETCODE_FAILURE If the profile factory reset failed (which might cause the modem to be in an undefined state)
 */
Retcode_T Cellular_Startup(void);

/**
 * @brief Shuts down the modem by deregistering from the network and powering off the modem hardware
 *
 * @retval RETCODE_OK The modem was shut down and powered off correctly
 * @retval RETCODE_FAILURE The modem was still responsive/running afer it should have been switched off
 * 		   (should never happen and points towards a bug in the BSP/this implementation)
 */
Retcode_T Cellular_Shutdown(void);

/**
 * @brief Resets the modem through hardware means, causing the modem to shut down and start up again.
 * 	      Unlike the Cellular_Startup() this function does not perform a profile factory reset. Unless the
 * 	      modem profile was reconfigured, this also triggers a network reconnect.
 *
 * @retval RETCODE_OK The modem was reset correctly
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem did not respond after being reset
 * @retval RETCODE_FAILURE The modem was still responsive/running afer it should have been switched off
 * 		   (should never happen and points towards a bug in the BSP/this implementation)
 */
Retcode_T Cellular_Reset(void);

/**
 * @brief Returns the current modem power status
 *
 * @param[out] PowerStatus The current modem power status.
 *
 * @retval RETCODE_OK The network status was retrieved correctly
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE The modem did not respond to the network status query
 */
Retcode_T Cellular_GetPowerStatus(Cellular_PowerStatus_T* PowerStatus);

/**
 * @brief Retrieves the current SIM insertion status from the modem
 *
 * @param[out] SimCardInserted TRUE if the SIM card is inserted and responsive, FALSE otherwise
 *
 * @retval RETCODE_OK The SIM insertion status was retrieved correctly
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE The modem did not respond to the network status query
 */
Retcode_T Cellular_IsSimCardInserted(bool *SimCardInserted);

/**
 * @brief Retrieves the current SIM ICCID from the modem
 * @param[in/out] iccid The ICCID of the SIM card. This buffer must be at least 24 bytes long and is to be allocated by caller.
 * @param[in] MaxLength Max length of buffer iccid.
 * @param[out] length of iccid (without zero termination).
 * @warning The buffer passed as iccid parameter needs to be at least 24 bytes long.
 * @retval RETCODE_OK The ICCID was retrieved correctly
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE The modem did not respond to the network status query
 */
Retcode_T Cellular_GetIccid(uint8_t *iccid, uint32_t MaxLength, uint32_t* length); //-- MaxLength expected to be at least CELLULAR_BUFFER_SIZE_ICCID

/**
 * @brief Enables/disables modem command echo
 * @param[in] enable TRUE if the modem echo should be enabled, FALSE otherwise
 * @return Status of the command execution, RETCODE_OK on success or some error code otherwise
 */
Retcode_T Cellular_EnableEcho(bool enable);

#endif /* QCTEL_CONTROL_H */
