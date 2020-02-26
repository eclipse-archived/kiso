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
 * @ingroup KISO_CONNECTIVITY
 * @defgroup KISO_CELLULAR Cellular
 * @{
 *
 * @brief High level connectivity driver for AT-communications-based cellular
 * modems.
 *
 * @startuml
 *  package "Cellular" {
 *      () "Cellular Modem Management API" as cellular_core_if
 *      [Modem Management] as core
 *      () "Cellular Sockets API" as cellular_socket_if
 *      [Socket Service] as socket_service
 *      () "Cellular DNS API" as cellular_dns_if
 *      [DNS] as dns_service
 *      () "Cellular HTTP API" as cellular_http_if
 *      [HTTP] as http_service
 *      node "Variants" as variants {
 *          [u-blox SARA R4/N4 variant] as ublox_sara_r4n4_variant
 *          [other variant] as other_variant
 *      }
 *      node "Common" as common {
 *          [Hardware] as hardware
 *          [Engine] as engine
 *          [Misc. Utilities] as utils
 *      }
 *  }
 *  package "Essentials" {
 *      () "MCU API" as mcu_if
 *      [MCU] as mcu
 *      () "BSP API" as bsp_if
 *      [BSP] as bsp
 *  }
 *
 *  cellular_core_if -down- core
 *  cellular_socket_if -down- socket_service
 *  cellular_dns_if -down- dns_service
 *  cellular_http_if -down- http_service
 *
 *  core <|.down. variants
 *  socket_service <|.down. variants
 *  dns_service <|.down. variants
 *  http_service <|.down. variants
 *
 *  variants -down- common
 *
 *  engine -right- hardware
 *  engine -left- utils
 *  hardware -down-> bsp_if
 *  engine -down-> mcu_if
 *
 *  mcu_if -down- mcu
 *  bsp_if -down- bsp
 *
 * @enduml
 *
 * @defgroup KISO_CELLULAR_VARIANTS Variants
 * @{
 * @brief Supported variants implementing the Cellular API
 *
 * @defgroup KISO_CELLULAR_VARIANT_UBLOX u-blox
 * @{
 * @brief Variant implementation for u-blox
 * @}
 * @}
 * @defgroup KISO_CELLULAR_COMMON Common
 * @{
 * @brief Common modules shared accross variants and acting as the drivers
 * backbone.
 * @}
 *
 * @defgroup KISO_CELLULAR_CORE Modem Management
 * @{
 *
 * @brief Management interface to change power state and radio mode of cellular
 * modems.
 *
 * @details Offers APIs to control power and networking functions of a cellular
 * modem via AT-commands. The primary purpose of this function-set is to
 * power-on/-off the modem, initiate a network registration to some base-station
 * and create a packet-based data connection (PSD-context) over which IP
 * packets can be sent.
 *
 * After the modem is powered on, registered and has obtained a PSD-context, the
 * user may proceed to utilize the data-service-oriented APIs found in the
 * Cellular public API.
 *
 * @section KISO_CELLULAR_STATEMACHINE State Machine
 *
 * Regarding the driver state-machine please refer to the state-diagram below.
 * Please also mind, that certain API functions have blocking behavior, in which
 * cases it may be optional to handle the @b on-state-changed callback.
 * @startuml
 *
 *  [*] --> Power_Off : Initialize
 *
 *  Power_Off -> Power_On : Power on
 *
 *  Power_On --> Registering : Call for registration network
 *  Registering -> Registered : Registration complete
 *  Registered -> Registering : Registration lost
 *  Registered --> Power_On : Call for deregistration from network
 *  Registering --> Power_On : Call for deregistration from network
 *
 *  Registered -> DataActive : Call for data-context activation
 *  DataActive -> Registered : Call for data-context deactivation
 *  DataActive -> Registering : Registration lost
 *  DataActive -> Registered : Data-Context closed by network
 *
 *  Power_On -> Power_Off : Power down
 *
 *  Power_Off --> [*] : Deinitialize
 *
 *  note top of DataActive
 *  TCP/UDP, DNS, etc. data-services may be used now. Continue to monitor the state
 *  callbacks to get notified about network changes.
 *  end note
 *
 *  note bottom of DataActive
 *  If the modem for whatever reason loses the data-context (<u>DataActive</u> state), a
 *  client is expected to call <i>ActivateDataContext()</i> again, to initiate the process
 *  of obtaining a new data-context. The modem may buffer short desyncs on-chip as
 *  to not having to fall back into the <u>Registered</u> state.
 *  end note
 *
 *  note top of Power_Off
 *  Calling <i>PowerOff()</i> from any state after <u>Power_On</u> will invoke a graceful
 *  device power down, including closing any data-contexts and deregistering from the
 *  network.
 *  end note
 *
 *  footer Copyright (C) 2019 Robert Bosch GmbH.
 *
 * @enduml
 *
 * @section KISO_CELLULAR_USAGE Usage
 *
 * Start by initializing Cellular and register a state-handler:
 *
 * @code{.c}
 *  Retcode_T rc = Cellular_Initialize(HandleStateChanged);
 *  if (RETCODE_OK != rc)
 *  {
 *      LOG_FATAL("Error during cellular initialization: %x", rc);
 *      return rc;
 *  }
 *  // Implement a state change handler, e.g. store the state signal some semaphore.
 *  static void HandleStateChanged(Cellular_State_T oldState,
 *                                 Cellular_State_T newState,
 *                                 void *param, uint32_t len)
 *  {
 *      CurrentCellularState = newState;
 *      (void)xSemaphoreGive(CellularStateChangedSignal);
 *  }
 * @endcode
 *
 * Next, power on the hardware to start talking with the modem:
 *
 * @code{.c}
 *  assert(CurrentCellularState == CELLULAR_STATE_POWEROFF); // In this snippet we assume modem is powered off.
 *
 *  Cellular_PowerUpParameters_T powerUpParams;
 *  powerUpParams.SimPin = "1234"; // If your SIM card requires a PIN, set it here.
 *  Retcode_T rc = Cellular_PowerOn(&powerUpParams);
 *  if (RETCODE_OK != rc)
 *  {
 *      LOG_FATAL("Error during cellular power-on: %x", rc);
 *      return rc;
 *  }
 * @endcode
 *
 * The power-up sequence is blocking, still you'll additionally receive a
 * state-changed-callback which you may choose ignore. Once powered up, the
 * modem will be kept in a non-registered state. This allows us to configure a
 * so called PSD (Packet-Switched-Data) context. For certain AcT (Access
 * Technology) we @b must do this @b before registering to the network (namely
 * LTE and NB-IoT). These AcT are inherently packet-based and therefore activate
 * make use of a "default-context" to establish network communication. Older
 * AcTs allow you to register to the network without a preconfigured
 * data-context.
 *
 * Configuring a data-context for NB-IoT and initiating network registration may
 * look like this:
 *
 * @code{.c}
 *  assert(CurrentCellularState == CELLULAR_STATE_POWERON); // In this snippet we assume modem is powered on.
 *
 *  Cellular_DataContextParameters_T ctxParam;
 *  ctxParam.Type = CELLULAR_DATACONTEXTTYPE_INTERNAL;
 *  ctxParam.ApnSettings.ApnName = APP_APN_NAME; // These settings depend on your network operator and SIM card
 *  ctxParam.ApnSettings.AuthMethod = APP_APN_AUTHMETHOD;
 *  ctxParam.ApnSettings.Username = APP_APN_USER;
 *  ctxParam.ApnSettings.Password = APP_APN_PASSWORD;
 *  Retcode_T rc = Cellular_ConfigureDataContext(0, &ctxParam); // cid=0 being the default-context in this case.
 *  if (RETCODE_OK != rc)
 *  {
 *      LOG_FATAL("Error while configuring data-context: %x", rc);
 *      return rc;
 *  }
 *
 *  Cellular_NetworkParameters_T networkParams;
 *  networkParams.AcT = CELLULAR_RAT_DEFAULT; // This obviously depends on your use-case.
 *  networkParams.FallbackAcT = CELLULAR_RAT_DEFAULT;
 *  Retcode_T rc = Cellular_RegisterOnNetwork(&networkParams);
 *  if (RETCODE_OK != rc)
 *  {
 *      LOG_FATAL("Error during cellular registration initiation: %x", rc);
 *      return rc;
 *  }
 * @endcode
 *
 * After that, the modem is instructed to automatically connect with an
 * available network. This may take some seconds to minutes depending on signal
 * strength and selected AcT. Because of that, this process happens
 * asynchronously in the background and you'll receive a state-changed-callback
 * once the modem was able to register to a network.
 *
 * If you were using LTE or NB-IoT as your AcT, the default-context will be
 * implicitly activated and may now be used for TCP/IP communication. On older
 * AcTs you will have to configure and activate the data-context after
 * registration. It's safe to call #Cellular_ActivateDataContext() on an already
 * active context (like the default-context in LTE/NB-IoT). The action will
 * simply be ignored by the modem, but it allows us to implement the same
 * control flow for newer AcTs as for older ones.
 *
 * @note Currently #CELLULAR_STATE_DATAACTIVE is only set when explicitly calling
 * #Cellular_ActivateDataContext(), even if context was implicitly activated.
 *
 * @code
 *  assert(CurrentCellularState == CELLULAR_STATE_REGISTERED); // In this snippet we assume modem is registered.
 *  const Cellular_DataContext_T *dataContext;
 *  Retcode_T rc = Cellular_ActivateDataContext(0, &DataContext);
 *  if (RETCODE_OK != rc)
 *  {
 *      LOG_FATAL("Error while activating data-context: %x", rc);
 *      return rc;
 *  }
 * @endcode
 *
 * @file
 */
#ifndef KISO_CELLULAR_H_
#define KISO_CELLULAR_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#include "Kiso_CellularConfig.h"
#include "Kiso_CellularModules.h"

#define CELLULAR_ICCID_MAX_LENGTH (UINT32_C(20))

/**
 * @brief Cellular package specific retcode-codes.
 */
enum
{
    RETCODE_CELLULAR_NOT_RESPONSIVE = RETCODE_FIRST_CUSTOM_CODE,
    RETCODE_CELLULAR_RESPONDED_ERROR,
    RETCODE_CELLULAR_RESPONDED_ABORTED,
    RETCODE_CELLULAR_RESPONDED_OTHER,
    RETCODE_CELLULAR_RESPONSE_UNEXPECTED,
    RETCODE_CELLULAR_IN_UNKNOWN_STATE,
    RETCODE_CELLULAR_URC_NOT_PRESENT,
    RETCODE_CELLULAR_NETWORK_ERROR,
    RETCODE_CELLULAR_SHUTDOWN_FAILURE,
    RETCODE_CELLULAR_DRIVER_BUSY,
    RETCODE_CELLULAR_SOCKET_INVALID_ID,
    RETCODE_CELLULAR_SOCKET_BIND_FAILED,
    RETCODE_CELLULAR_DRIVER_ERROR_CMD_ARG,
    RETCODE_CELLULAR_SIM_NOT_READY,

    RETCODE_CELLULAR_LAST_CUSTOM_CODE
};

/**
 * @brief Cellular APN authentication methods.
 */
enum Cellular_ApnAuthMethod_E
{
    CELLULAR_APNAUTHMETHOD_NONE,      //!<       No authentication (user+pw ignored)
    CELLULAR_APNAUTHMETHOD_PAP,       //!<        Personal Authentication Protocol
    CELLULAR_APNAUTHMETHOD_CHAP,      //!<       Challenge Handshake Authentication Protocol
    CELLULAR_APNAUTHMETHOD_AUTOMATIC, //!<  Automatically detect the auth-method (may not be supported by every modem)

    CELLULAR_APNAUTHMETHOD_MAX //!<         Placeholder for iterating
};
typedef enum Cellular_ApnAuthMethod_E Cellular_ApnAuthMethod_T;

struct Cellular_PowerUpParameters_S
{
    /**
     * @brief PIN to unlock the SIM card. Can be NULL.
     */
    const char *SimPin;
};
typedef struct Cellular_PowerUpParameters_S Cellular_PowerUpParameters_T;

/**
 * @brief Cellular radio access technology.
 *
 * May be bit-OR'ed together for certain API functions.
 */
enum Cellular_RadioAccessTechnology_E
{
    CELLULAR_RAT_DEFAULT = 0x00,    //!< Use what's available/supported by SIM
    CELLULAR_RAT_GSM = 0x01,        //!< GSM/GPRS/eGPRS
    CELLULAR_RAT_UMTS = 0x02,       //!< UMTS
    CELLULAR_RAT_LTE = 0x04,        //!< LTE
    CELLULAR_RAT_LTE_CAT_M1 = 0x08, //!< LTE Cat. M1
    CELLULAR_RAT_LTE_CAT_NB1 = 0x10 //!< LTE Cat. Narrow-Band IoT
};
typedef enum Cellular_RadioAccessTechnology_E Cellular_RadioAccessTechnology_T;

/**
 * @brief Cellular data context parameters, used to establish a PDP (or EPS)
 * context with the network.
 */
struct Cellular_NetworkParameters_S
{
    /**
     * @brief The Radio Access Technology (RAT) to be used. May be a bit-OR'ed
     * value of multiple RATs. If the modem does not support one of the
     * specified RATs and error will be returned.
     */
    Cellular_RadioAccessTechnology_T AcT;

    /**
     * @brief The fall-back RAT selection to be used if @e AcT can not be met.
     */
    Cellular_RadioAccessTechnology_T FallbackAcT;
};
typedef struct Cellular_NetworkParameters_S Cellular_NetworkParameters_T;

struct Cellular_NetworkInfo_S
{
    /**
     * @brief Country Code associated with the current network.
     * Default length should be 4 bytes.
     */
    uint8_t CountryCode[CELLULAR_COUNTRY_CODE_LENGTH];

    /**
     * @brief Network Code associated with the current network.
     * Default length should be 3. However number of used bytes may vary from
     * 2 to 3 based on the country.
     */
    uint8_t NetworkCode[CELLULAR_NETWORK_CODE_LENGTH];

    /**
     * @brief Length associated with the current Network Code buffer.
     */
    uint8_t NetworkCodeLength;

    /**
     * @brief Location Area Code associated with the current network.
     * Default length should be 4.
     */
    uint8_t LocationCode[CELLULAR_LOCATION_CODE_LENGTH];

    /**
     * @brief Cell Id associated with the current network.
     * Default length should be 4.
     */
    uint8_t CellId[CELLULAR_CELL_ID_LENGTH];
};
typedef struct Cellular_NetworkInfo_S Cellular_NetworkInfo_T;

/**
 * @brief Cellular data-context types.
 */
enum Cellular_DataContextType_E
{
    CELLULAR_DATACONTEXTTYPE_INTERNAL, //!< Data-context is utilizing the internal (proprietary TCP/IP stack)
    CELLULAR_DATACONTEXTTYPE_EXTERNAL, //!< Data-context requires the client to do provide a basic TCP/IP stack (currently no supported)

    CELLULAR_DATACONTEXTTYPE_MAX //!<       Placeholder for iterating
};
typedef enum Cellular_DataContextType_E Cellular_DataContextType_T;

/**
 * @brief Cellular APN settings.
 */
struct Cellular_ApnSettings_S
{
    /**
     * @brief C-string containing the name of the APN (e.g. internet.t-d1.de).
     * This info is usually provided with the SIM card or operator.
     *
     * Set this to NULL if you want the default APN as defined by the
     * MNO-profile to be used.
     */
    const char *ApnName;
    /**
     * @brief Authentication method to use.
     */
    Cellular_ApnAuthMethod_T AuthMethod;
    /**
     * @brief C-string containing the APN username. May be NULL.
     */
    const char *Username;
    /**
     * @brief C-string containing the APN password. May be NULL.
     */
    const char *Password;
};
typedef struct Cellular_ApnSettings_S Cellular_ApnSettings_T;

/**
 * @brief Cellular data-context configuration parameters.
 */
struct Cellular_DataContextParameters_S
{
    /**
     * @brief Configures the type of data-context.
     */
    Cellular_DataContextType_T Type;

    /**
     * @brief Configures the APN settings of the data-context.
     */
    Cellular_ApnSettings_T ApnSettings;
};
typedef struct Cellular_DataContextParameters_S Cellular_DataContextParameters_T;

/**
 * @brief Cellular IP address type.
 */
enum Cellular_IpAddressType_E
{
    CELLULAR_IPADDRESSTYPE_IPV4, //!<   IPv4 address
    CELLULAR_IPADDRESSTYPE_IPV6, //!<   IPv6 address

    CELLULAR_IPADDRESSTYPE_MAX //!<     Placeholder for iterating
};
typedef enum Cellular_IpAddressType_E Cellular_IpAddressType_T;

/**
 * @brief Cellular IP (v4 or v6) address data-structure.
 *
 * Depending on the value of @e x.Type, @e x.Address.IPv4 or @e x.Address.IPv6
 * should be used.
 */
struct Cellular_IpAddress_S
{
    Cellular_IpAddressType_T Type;
    union {
        uint8_t IPv4[4];
        uint8_t IPv6[16];
    } Address;
};
typedef struct Cellular_IpAddress_S Cellular_IpAddress_T;

struct Cellular_DataContext_S
{
    /**
     * @brief The id/slot of the data-context (alias cid).
     */
    uint32_t Id;

    /**
     * @brief Type of data-context.
     */
    Cellular_DataContextType_T Type;

    /**
     * @brief IP address associated with this data-context.
     */
    Cellular_IpAddress_T IpAddress;
};
typedef struct Cellular_DataContext_S Cellular_DataContext_T;

/**
 * @brief Cellular driver states.
 */
enum Cellular_State_E
{
    CELLULAR_STATE_POWEROFF,    //!< Modem powered off
    CELLULAR_STATE_POWERON,     //!< Modem powered on and idling (no radio communication)
    CELLULAR_STATE_REGISTERING, //!< Modem in the process of registering on a network
    CELLULAR_STATE_REGISTERED,  //!< Modem registered on network
    CELLULAR_STATE_DATAACTIVE,  //!< Modem obtained data-context and may use data-services like TCP/IP

    CELLULAR_STATE_MAX //!< Placeholder for iterating
};
typedef enum Cellular_State_E Cellular_State_T;

/**
 * @brief Used to communicate state changes between driver and user. The
 * callback is always called after the actual state-change occurred.
 * Some API functions may be blocking in behavior, which may make further
 * handling in the callback unnecessary.
 *
 * @note This callback may be invoked by a driver-internal task and should not
 * be blocked for a sustained period of time.
 *
 * @param[in] oldState
 * Old driver state.
 *
 * @param[in] newState
 * New driver state.
 *
 * @param[in] param
 * A pointer to an arbitrary data-structure or array, depending on the
 * @e newState parameter.
 *
 * @param[in] len
 * Length of the data-structure or array @e param is pointing to.
 */
typedef void (*Cellular_StateChanged_T)(Cellular_State_T oldState, Cellular_State_T newState, void *param, uint32_t len);

/**
 * @brief Initialize the driver resources. Only driver-side software resources
 * are initialized, the modem is not yet considered @b powered-on.
 *
 * @param[in] onStateChanged
 * Invoked whenever the local driver state changes.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_Initialize(Cellular_StateChanged_T onStateChanged);

/**
 * @brief Power up the modem hardware.
 *
 * This function is @b blocking. After completion the driver will be in
 * #CELLULAR_STATE_POWERON state.
 *
 * @param[in] parameters
 * Valid structure pointer containing startup parameters, like SIM-PIN, etc.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_PowerOn(const Cellular_PowerUpParameters_T *parameters);

/**
 * @brief Reset the modem hardware.
 *
 * May not perform a graceful network deregister. To be used conservatively.
 *
 * @param[in] parameters
 * Valid structure pointer containing startup parameters, like SIM-PIN, etc.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_Reset(const Cellular_PowerUpParameters_T *parameters);

/**
 * @brief Initiate the automatic network registration process.
 *
 * Because this process can take a very long time, it is @b non-blocking. Please
 * subscribe to the #CELLULAR_STATE_REGISTERED state once a network sync has
 * been accomplished.
 *
 * @param[in] networkParameters
 * Parameters needed to establish the network-register and later data-context
 * activation.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_RegisterOnNetwork(const Cellular_NetworkParameters_T *networkParameters);

/**
 * @brief Deregister from the current network and remain in
 * #CELLULAR_STATE_POWERON state. This is no substitute for air-plane mode.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_DisconnectFromNetwork(void);

/**
 * @brief Switch the radio transceiver into air-plane mode. No further radio
 * communication will commence. Once air-plane mode is switched off
 * (@e airPlanModeOn=false) the driver will attempt to resume the
 * previous connectivity state.
 *
 * This function is @b blocking.
 *
 * @param[in] airPlanModeOn
 * Set @e true to enable air-plane mode, @e false to exit it.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_SetAirPlaneMode(bool airPlanModeOn);

/**
 * @brief Query the Integrated Circuit Card Identifier (ICCID).
 *
 * @note Refer to #CELLULAR_ICCID_MAX_LENGTH for the max. ICCID length.
 *
 * @param[in] iccid
 * A caller-allocated char-buffer with a length of 19 or 20 bytes. The ICCID
 * will be copied into this buffer. This function does not guarantee
 * zero-termination.
 *
 * @param[in,out] iccidLen
 * On call: must contain the number of writable bytes in @e iccid buffer.
 * If smaller than needed by the internals, a warning will be associated with
 * the #Retcode_T return value and the ICCID may be cropped.
 * On return: will contain the number of written bytes into the @e iccid buffer.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_QueryIccid(char *iccid, uint32_t *iccidLen);

/**
 * @brief Query the current cell and network information.
 *
 * @param[in] networkInfo
 * Will be filled with the cell and network info associated with the current
 * cell.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_QueryNetworkInfo(Cellular_NetworkInfo_T *networkInfo);

/**
 * @brief Configure the specified data-context identified by the Context-Id.
 *
 * @note Depending on the Radio Access Technology (RAT) this may need to be
 * configured @e before #Cellular_RegisterOnNetwork is initiated. E.g. LTE will
 * implicitly activate a data-context during the registration procedure.
 * Depending on your SIM-card or operator it may be necessary to configure a
 * private APN to even join the network.
 *
 * @param[in] cid
 * Context-Id of the data-context to configure.
 *
 * @param[in] parameters
 * Configuration parameters to apply to the specified data-context.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_ConfigureDataContext(uint32_t cid, const Cellular_DataContextParameters_T *parameters);

/**
 * @brief Activate the PDP or EPS data context.
 *
 * @note On some Radio-Access-Technologies (RAT) a data-context may inherently
 * be created by the network registration itself, in which case this function
 * does nothing except return the already activated context.
 *
 * @param[in] cid
 * The Context-Id of the data-context to activate.
 * (usually 0 .. #CELLULAR_DATACTX_COUNT)
 *
 * @param[out] ctx
 * Will be pointed to the activated data-context.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_ActivateDataContext(uint32_t cid, const Cellular_DataContext_T **ctx);

/**
 * @brief Attempt to deactivate any PDP context created by the driver.
 *
 * @note On some Radio-Access-Technologies (RAT) a data-context may inherently
 * be created by the network registration itself, in which case this function
 * does nothing.
 *
 * @param[in] cid
 * The Context-Id of the data-context to activate.
 * (usually 0 .. #CELLULAR_DATACTX_COUNT)
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_DeactivateDataContext(uint32_t cid);

/**
 * @brief Power down the modem hardware. The procedure first attempts a graceful
 * power-down, i.e. taking the necessary steps to de-register from the network
 * properly. If this fails, a hard shut-down is initiated instead, noted by a
 * warning in the #Retcode_T return value.
 *
 * This function is @b blocking. After completion the driver will be in
 * #CELLULAR_STATE_POWEROFF state.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_PowerOff(void);

/**
 * @brief Check the modem for power setting and responsiveness.
 *
 * This function is @b blocking.
 *
 * @param[out] isPoweredOn
 * Will contain @e true if the modem is powered, @e false otherwise.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_IsPoweredOn(bool *isPoweredOn);

/**
 * @brief Deinitialize the driver and release hardware-resources.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Retcode_T Cellular_Deinitialize(void);

/**
 * @brief Return the current state of the driver state-machine as published by
 * the state-callback.
 *
 * @return A #Retcode_T indicating the result of the procedure.
 */
Cellular_State_T Cellular_GetState(void);

#endif /* KISO_CELLULAR_H_ */

/** @}*/
/** @}*/
