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
 * @brief Cellular Interface
 *
 */

/**
 * @defgroup CELLULAR Cellular
 * @{
 *
 * @brief This module is used to initialize, connect, disconnect, uninitialize and get modem status.
 * 		  \image html Cellular-overview.png
 *
 */

#ifndef BCDS_CELLULAR_H
#define BCDS_CELLULAR_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"
#include "BCDS_CellularConfig.h"


/**
 * @brief   Typedef to represent the cellular status
 */
typedef enum
{
    CELLULAR_DEVICESTATUS_IDLE,
    CELLULAR_DEVICESTATUS_INIT,
    CELLULAR_DEVICESTATUS_CONNECTED,
    CELLULAR_DEVICESTATUS_DISCONNECTED,
} Cellular_DeviceStatus_T;

/**
 * @brief Typedef to represent the cellular event.
 */
typedef enum
{
    CELLULAR_EVENT_INITIALIZING = 1, /** Trigger event while cellular initializing */
    CELLULAR_EVENT_INITIALIZED, /** Trigger event after cellular initialized */
    CELLULAR_EVENT_DATARECEIVED, /** Trigger event after UDP data received. Intimated through socket callback. */
    CELLULAR_EVENT_CONNECTED, /** Trigger event GPRS bearer activated */
    CELLULAR_EVENT_DISCONNECTED, /** Trigger event GPRS bearer Deactivated */
    CELLULAR_EVENT_ERROR, /** Trigger event if any error */
    CELLULAR_EVENT_FTP_COMPLETE, /** Deprecated / Not supported */
    CELLULAR_EVENT_SOC_CLOSE, /** Trigger event socket close. Intimated through socket callback. */
    CELLULAR_EVENT_SMS, /** Trigger event SMS. Intimated through SMS callback*/
    CELLULAR_EVENT_MAX
} Cellular_CallbackEvent_T;

/**
 * @brief Typedef to represent the cellular power modes. --  why power state instead of state? Need clarification ---
 */
typedef enum
{
    CELLULAR_POWER_STATE_OFF,
    CELLULAR_POWER_STATE_OFF_EAT,
    CELLULAR_POWER_STATE_SLEEP,
    CELLULAR_POWER_STATE_WAKEUP,
    CELLULAR_POWER_STATE_ON
} Cellular_PowerState_T;

/**
 * @brief Typedef to Query Information.
 */
typedef enum
{
    CELLULAR_QUERYINFO_NETWORKINFO = 1, /** n/w info like cell ID, lac,etc */
    CELLULAR_QUERYINFO_ERROR, /** n/w cell ID */
    CELLULAR_QUERYINFO_STATUS
} Cellular_QueryInfo_T;

/**
 * @brief  Typedef to represent the cellular controls
 */
typedef enum
{
    CELLULAR_CONFIG,
    CELLULAR_TEST,
    CELLULAR_RESET,
} Cellular_Control_T;

/**
 * @brief   Typedef to represent the network information such as MCC(Mobile Country Code), Mobile Network Code(MNC),
 *          LAC(Location Area Code) and CID(Cell ID)
 */
typedef struct
{
    uint8_t CountryCode[CELLULAR_COUNTRY_CODE_LENGTH]; /**< The size of this string should be 3. It will not vary. */
    uint8_t NetworkCode[CELLULAR_NETWORK_CODE_LENGTH]; /**< The size of this string should be 3, as the network code will vary from 2 to 3 based on the country */
    uint8_t LocationCode[CELLULAR_LOCATION_CODE_LENGTH]; /**< The size of this string should be 4. It will not vary. */
    uint8_t CellId[CELLULAR_CELL_ID_LENGTH]; /**< The size of this string should be 4. It will not vary. */
    uint8_t NetworkCodeLength; /**< The length of the network code will be 2 or 3 depending on the European or the North American standards */
} Cellular_NetworkInfo_T;


/**
 * @brief Typedef to represent the APN configuration setting
 */
typedef struct
{
    uint8_t ApnName[CELLULAR_APN_NAME_SIZE + 1];
    uint8_t ApnUserName[CELLULAR_APN_USER_SIZE + 1];
    uint8_t ApnPassword[CELLULAR_APN_PASSWORD_SIZE + 1];
} Cellular_ConfigurationSettings_T;


/**
 * @brief   This data type represents a function pointer which would be executed
 *          when any event (error/status/...) is to be notified.
 *
 * @param [in]  event : The argument containing relevant event information.
 */
typedef void (*Cellular_CallbackFunction_T)(Cellular_CallbackEvent_T event, void *info);


/**
 * @brief   Pointer to initialize cellular Interface.
 *          This would enable the clock configurations.
 *
 * @param [in]  eventCB     : Callback to be called in case of any event notification.
 *
 * @retval RETCODE_OK If cellular initialization success.
 * @retval RETCODE_INVALID_PARAM If callback is null
 * @retval RETCODE_OUT_OF_RESOURCES If Cellular engine init failed
 * @retval RETCODE_DOPPLE_INITIALIZATION If cellular Bsp initialized and also the return values depends on #MCU_UART_Initialize and #BSP_UMTS_QuectelUC20_Enable
 */
Retcode_T Cellular_Initialize(Cellular_CallbackFunction_T eventCallback);

/**
 * @brief   Pointer to uninitialize cellular Interface.
 *
 * @retval RETCODE_OK If cellular deinitialization success.
 * @retval RETCODE_INVALID_PARAM If queue is null and lso the return values depends on #BSP_UMTS_QuectelUC20_Disable or #MCU_UART_Deinitialize
 * @retval RETCODE_UNINITIALIZED If cellular bsp not in initialized state
 */
Retcode_T Cellular_Uninitialize(void);

/**
 * @brief   Get device status
 *
 * @retval  CELLULAR_DEVICESTATUS_IDLE
 * @retval  CELLULAR_DEVICESTATUS_INIT
 * @retval  CELLULAR_DEVICESTATUS_CONNECTED
 * @retval  CELLULAR_DEVICESTATUS_DISCONNECTED
 */
Cellular_DeviceStatus_T Cellular_GetDeviceStatus(void);

/**
 * @brief   Query the cellular modem
 *
 * @param [in]  query       : command which needs to be queried.
 * @parm [out] value        : result of the command.
 *
 * @retval  RETCODE_OK If cellular Query Modem is success
 * @retval  RETCODE_INVALID_PARAM If network registration status is Null
 * @retval  RETCODE_NULL_POINTER If function pointer is null
 * @retval  RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval  RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock and other return values depends on #MCU_UART_Send
 * @retval  RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval  RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval  RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval  RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval  RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval  RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted
 * @retval  RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error
 * @retval  RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other
 * @retval  RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval  RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 */
Retcode_T Cellular_QueryModem(Cellular_QueryInfo_T info, void* value);

/**
 * @brief   Pointer to control a cellular modem features
 *
 * @note       cellular reset    : cellular modem should be powered ON before reset. In Modem OFF state, reset will fail
 *
 * @param [in]  control     : This would be the Cellular_Control_E members.
 *
 *
 * @param [in]  arg         :  value to be set based on command.
 *
 * @retval RETCODE_OK If cellular control operation is success
 * @retval RETCODE_NULL_POINTER If function pointer is null for cellular reset
 * @retval RETCODE_UNINITIALIZED If cellular Bsp not in initialized state or semaphore handle is null for cellular reset
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE Unable to startup cellular for cellular reset
 * @retval RETCODE_INVALID_PARAM If apn name, user name and password is invalid for cellular configuration
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock for cellular configuration
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry for cellular configuration
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured for cellular configuration
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured for cellular configuration
 * @retval RETCODE_CELLULAR_RESPONDED_ABORTED If modem responses aborted for cellular configuration
 * @retval RETCODE_CELLULAR_RESPONDED_ERROR If modem responses error for cellular configuration
 * @retval RETCODE_CELLULAR_RESPONDED_OTHER If modem responses other for cellular configuration
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive for cellular configuration
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly for cellular configuration
 * @retval RETCODE_NOT_SUPPORTED If the options are other than CELLULAR_RESET, CELLULAR_CONFIG and CELLULAR_TEST
 */
Retcode_T Cellular_Control(Cellular_Control_T control, void* value);

/**
 * @brief   Pointer to initialize the cellular modem to n/w.
 *
 * @retval RETCODE_OK if Network initialization success
 * @retval RETCODE_CELLULAR_SIM_NOT_READY If did not find SIM card
 * @retval RETCODE_CELLULAR_NETWORK_ERROR If network configuration failed or enable Auto Network Selection fails
 */
Retcode_T Cellular_NetworkInit(void);

/**
 * @brief   Pointer to connect the cellular modem to n/w.
 *
 * @retval RETCODE_OK if Network connection success
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock
 * @retval RETCODE_INVALID_PARAM If the parameter is out of range and other return values depends on #MCU_UART_Send
 * @retval RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 * @retval RETCODE_FAILURE If parameter is out of range
 */
Retcode_T Cellular_Connect(void);

/**
 * @brief   Pointer to disconnect the cellular modem to n/w.
 *
 * @retval RETCODE_OK if Network connection success
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock
 * @retval RETCODE_INVALID_PARAM If the parameter is out of range and other return values depends on #MCU_UART_Send
 * @retval RETCODE_SEMAPHORE_ERROR If serial transfer is failed
 * @retval RETCODE_AT_RESPONSE_QUEUE_TIMEOUT If unable to get queue entry
 * @retval RETCODE_AT_RESPONSE_QUEUE_ERROR_EVENT If AT event type error occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_EVENT If wrong event occured
 * @retval RETCODE_AT_RESPONSE_QUEUE_WRONG_RESPONSE If Buffer length mismatches or wrong response received
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 * @retval RETCODE_CELLULAR_RESPONSE_UNEXPECTED If the modem behaves unexpectedly
 * @retval RETCODE_FAILURE If parameter is out of range
 */
Retcode_T Cellular_Disconnect(void);

/**
 * @brief   Pointer to control cellular power modes.
 *
 * @note     CELLULAR_POWER_ON   : When modem is being turned-on (and it is already on) it will lead to a reset
 * @note     CELLULAR_POWER_OFF  : If modem is already turned off, an error will be returned from PortInterface layer.
 *
 * @param [in]  state       : state of Power that the cellular is to be set.
 *
 * @retval RETCODE_OK If the power control operation is success
 * @retval RETCODE_NULL_POINTER If function pointer is null
 * @retval RETCODE_UNINITIALIZED If semaphore handle is null
 * @retval RETCODE_CELLULAR_DRIVER_BUSY If semaphore has timeout and unable to unlock the request lock
 * @retval RETCODE_UNINITIALIZED   If the BspCellular was not initialized correctly yet.
 * @retval RETCODE_CELLULAR_NOT_RESPONSIVE If the modem is not responsive
 */
Retcode_T Cellular_PowerControl(Cellular_PowerState_T state);

#endif /* BCDS_CELLULAR_H */
/**@} */
