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
 * @ingroup KISO_CELLULAR_COMMON
 * @defgroup AT3GPP27007 3GPP TS 27.007 AT Commands
 * @{
 * @brief AT commands as specified by 3GPP TS 27.007 V15.4.0 (2018-12).
 * @file
 */
#ifndef AT3GPP27007_H_
#define AT3GPP27007_H_

#include "Kiso_Retcode.h"
#include <limits.h>

#define AT_COPS_OPER_FORMAT_LONG_ALPHANUMERIC_MAX_LEN (UINT32_C(16))
#define AT_COPS_OPER_FORMAT_SHORT_ALPHANUMERIC_MAX_LEN (UINT32_C(8))

#define AT_INVALID_LAC (UINT16_MAX)
#define AT_INVALID_CI (UINT32_MAX)
#define AT_INVALID_RAC (UINT8_MAX)
#define AT_INVALID_TAC (UINT16_MAX)

/**
 * @brief 3GPP TS 27.007 CxREG @<n@> parameter controlling the degree of detail of
 * the CxREG URC.
 *
 * Applicable for CREG, CGREG and CEREG.
 */
enum AT_CXREG_N_E
{
    AT_CXREG_N_DISABLED = 0,          //!< URC disabled
    AT_CXREG_N_URC = 1,               //!< Basic URC containing registration info enabled
    AT_CXREG_N_URC_LOC = 2,           //!< Verbose URC containing registration and location info enabled
    AT_CXREG_N_URC_LOC_CAUSE = 3,     //!< (not supported) Verbose URC containing registration, location and cause info enabled
    AT_CXREG_N_URC_LOC_PSM = 4,       //!< (not supported, CGREG and CEREG only) Verbose URC containing registration, location and PSM info enabled
    AT_CXREG_N_URC_LOC_PSM_CAUSE = 5, //!< (not supported, CGREG and CEREG only) Verbose URC containing registration, location, PSM and cause info enabled
    AT_CXREG_N_INVALID = 255          //!< Invalid @<n@> value, used to signal that parameter not present
};
typedef enum AT_CXREG_N_E AT_CXREG_N_T;

/**
 * @brief 3GPP TS 27.007 CxREG @<stat@> parameter.
 *
 * Applicable for CREG, CGREG and CEREG.
 */
enum AT_CXREG_Stat_E
{
    AT_CXREG_STAT_NOT = 0,             //!< Not connected and also not trying to connect to any network
    AT_CXREG_STAT_HOME = 1,            //!< Registered in home network
    AT_CXREG_STAT_NOT_AND_SEARCH = 2,  //!< Not connected but trying to find a network
    AT_CXREG_STAT_DENIED = 3,          //!< Registration denied
    AT_CXREG_STAT_UNKNOWN = 4,         //!< Unknown (e.g out of GERAN/UTRAN&E-UTRAN coverage)
    AT_CXREG_STAT_ROAMING = 5,         //!< Registered to roaming
    AT_CXREG_STAT_SMSONLY_HOME = 6,    //!< Registered to home network for "SMS only" (E-UTRAN only)
    AT_CXREG_STAT_SMSONLY_ROAMING = 7, //!< Registered to roaming network for "SMS only" (E-UTRAN only)
    /* No nb. 8 (attached for emergency bearer services only), note:
     * 3GPP TS 24.008 [8] and 3GPP TS 24.301 [83] specify the condition when
     * the MS is considered as attached for emergency bearer services. */
    AT_CXREG_STAT_CSFB_NOT_PREF_HOME = 9,     //!< Registered to home network for "CSFB not preferred" (E-UTRAN only)
    AT_CXREG_STAT_CSFB_NOT_PREF_ROAMING = 10, //!< Registered to roaming network for "CSFB not preferred" (E-UTRAN only)
    AT_CXREG_STAT_INVALID = 255               //!< Invalid @<stat@> value, used to signal that parameter not present
};
typedef enum AT_CXREG_Stat_E AT_CXREG_Stat_T;

/**
 * @brief 3GPP TS 27.007 CxREG @<AcT@> (Access Technology) parameter of the CxREG
 * command family
 *
 * Applicable for CREG, CGREG and CEREG.
 */
enum AT_CXREG_AcT_E
{
    AT_CXREG_ACT_GSM = 0,               //!< GSM
    AT_CXREG_ACT_GSM_COMPACT = 1,       //!< GSM Compact
    AT_CXREG_ACT_UTRAN = 2,             //!< UTRAN
    AT_CXREG_ACT_GSM_EGPRS = 3,         //!< GSM + EGPRS
    AT_CXREG_ACT_UTRAN_HSDPA = 4,       //!< UTRAN + HSDPA
    AT_CXREG_ACT_UTRAN_HSUPA = 5,       //!< UTRAN + HSUPA
    AT_CXREG_ACT_UTRAN_HSDPA_HSUPA = 6, //!< UTRAN + HSDPA + HSUPA
    AT_CXREG_ACT_EUTRAN = 7,            //!< E-UTRAN
    AT_CXREG_ACT_ECGSMIOT = 8,          //!< EC-GSM-IoT (A/Gb mode)
    AT_CXREG_ACT_EUTRAN_NBS1 = 9,       //!< E-UTRAN (NB-S1 mode)
    AT_CXREG_ACT_EUTRA_5GCN = 10,       //!< E-UTRA connected to a 5GCN
    AT_CXREG_ACT_NR_5GCN = 11,          //!< NR connected to a 5GCN
    AT_CXREG_ACT_NGRAN = 12,            //!< NG-RAN
    AT_CXREG_ACT_EUTRA_NR = 13,         //!< E-UTRAN-NR dual connectivity
    AT_CXREG_ACT_INVALID = 255          //!< Invalid @<AcT@> value, used to signal that parameter not present
};
typedef enum AT_CXREG_AcT_E AT_CXREG_AcT_T;

/**
 * @brief 3GPP TS 27.007 CREG parameters
 */
struct AT_CREG_Param_S
{
    AT_CXREG_N_T N;
    AT_CXREG_Stat_T Stat;
    uint16_t Lac; /**< Location Area Code, range 0h-FFFFh (2 octets) */
    uint32_t Ci;  /**< Cell Identity, range 0h-FFFFFFFFh (4 octets) */
    AT_CXREG_AcT_T AcT;
};
typedef struct AT_CREG_Param_S AT_CREG_Param_T;

/**
 * @brief 3GPP TS 27.007 CGREG parameters
 */
struct AT_CGREG_Param_S
{
    AT_CXREG_N_T N;
    AT_CXREG_Stat_T Stat;
    uint16_t Lac; /**< Location Area Code, range 0h-FFFFh (2 octets) */
    uint32_t Ci;  /**< Cell Identity, range 0h-FFFFFFFFh (4 octets) */
    AT_CXREG_AcT_T AcT;
    uint8_t Rac; /**< Routing Area Code, range 0h-FFh (1 octet) */
};
typedef struct AT_CGREG_Param_S AT_CGREG_Param_T;

/**
 * @brief 3GPP TS 27.007 CEREG parameters
 */
struct AT_CEREG_Param_S
{
    AT_CXREG_N_T N;
    AT_CXREG_Stat_T Stat;
    uint16_t Tac; /**< Tracking Area Code, range 0h-FFFFh (2 octets) */
    uint32_t Ci;  /**< Cell Identity, range 0h-FFFFFFFFh (4 octets) */
    AT_CXREG_AcT_T AcT;
};
typedef struct AT_CEREG_Param_S AT_CEREG_Param_T;

/**
 * @brief 3GPP TS 27.007 COPS @<mode@> used to control PLMN (base-station)
 * selection.
 */
enum AT_COPS_Mode_E
{
    AT_COPS_MODE_AUTOMATIC = 0,             //!< Automatic network selection (take what's there)
    AT_COPS_MODE_MANUAL = 1,                //!< Manual network selection (<oper@> required, @<AcT@> optional)
    AT_COPS_MODE_DEREGISTER = 2,            //!< Deregister from network
    AT_COPS_MODE_SET_FORMAT_ONLY = 3,       //!< Set the @<format@> field (format of @<oper@> parameter) only, no registration initiated
    AT_COPS_MODE_MANUAL_THEN_AUTOMATIC = 4, //!< Try manual network registration (<oper@> field required), fall back to automatic on failed attempt
    AT_COPS_MODE_INVALID = 255              //!< Invalid @<mode@> value, used to signal that parameter not present
};
typedef enum AT_COPS_Mode_E AT_COPS_Mode_T;

/**
 * @brief 3GPP TS 27.007 COPS @<format@> used to control the interpretation of the
 * @<oper@> parameter.
 */
enum AT_COPS_Format_E
{
    AT_COPS_FORMAT_LONG_ALPHANUMERIC = 0,  //!< Long alphanumeric format
    AT_COPS_FORMAT_SHORT_ALPHANUMERIC = 1, //!< Short alphanumeric format
    AT_COPS_FORMAT_NUMERIC = 2,            //!< Numeric format
    AT_COPS_FORMAT_INVALID = 255           //!< Invalid @<format@> value, used to signal that parameter not present
};
typedef enum AT_COPS_Format_E AT_COPS_Format_T;

/**
 * @brief 3GPP TS 27.007 COPS @<oper@> used to specify a PLMN to connect to.
 *
 * @see #AT_COPS_FORMAT_T
 */
union AT_COPS_Oper_U {
    char LongAlphanumeric[AT_COPS_OPER_FORMAT_LONG_ALPHANUMERIC_MAX_LEN + 1];
    char ShortAlphanumeric[AT_COPS_OPER_FORMAT_SHORT_ALPHANUMERIC_MAX_LEN + 1];
    uint16_t Numeric;
};
typedef union AT_COPS_Oper_U AT_COPS_Oper_T;

/**
 * @brief 3GPP TS 27.007 COPS @<stat@>, currently unused.
 */
enum AT_COPS_Stat_E
{
    AT_COPS_STAT_UNKNOWN = 0,   //!< Status unknown
    AT_COPS_STAT_AVAILABLE = 1, //!< Available
    AT_COPS_STAT_CURRENT = 2,   //!< Currently connected
    AT_COPS_STAT_FORBIDDEN = 3, //!< Forbidden (registration denied)
};
typedef enum AT_COPS_Stat_E AT_COPS_Stat_T;

/**
 * @brief 3GPP TS 27.007 COPS @<AcT@> (Access Technology) parameter of the COPS
 * command.
 */
enum AT_COPS_AcT_E
{
    AT_COPS_ACT_GSM = 0,               //!< GSM
    AT_COPS_ACT_GSM_COMPACT = 1,       //!< GSM Compact
    AT_COPS_ACT_UTRAN = 2,             //!< UTRAN
    AT_COPS_ACT_GSM_EGPRS = 3,         //!< GSM + EGPRS
    AT_COPS_ACT_UTRAN_HSDPA = 4,       //!< UTRAN + HSDPA
    AT_COPS_ACT_UTRAN_HSUPA = 5,       //!< UTRAN + HSUPA
    AT_COPS_ACT_UTRAN_HSDPA_HSUPA = 6, //!< UTRAN + HSDPA + HSUPA
    AT_COPS_ACT_EUTRAN = 7,            //!< E-UTRAN
    AT_COPS_ACT_ECGSMIOT = 8,          //!< EC-GSM-IoT (A/Gb mode)
    AT_COPS_ACT_EUTRAN_NBS1 = 9,       //!< E-UTRAN (NB-S1 mode)
    AT_COPS_ACT_EUTRA_5GCN = 10,       //!< E-UTRA connected to a 5GCN
    AT_COPS_ACT_NR_5GCN = 11,          //!< NR connected to a 5GCN
    AT_COPS_ACT_NGRAN = 12,            //!< NG-RAN
    AT_COPS_ACT_EUTRA_NR = 13,         //!< E-UTRAN-NR dual connectivity
    AT_COPS_ACT_INVALID = 255          //!< Invalid @<AcT@> value, used to signal that parameter not present
};
typedef enum AT_COPS_AcT_E AT_COPS_AcT_T;

/**
 * @brief 3GPP TS 27.007 COPS parameters
 */
struct AT_COPS_Param_S
{
    AT_COPS_Mode_T Mode;
    AT_COPS_Format_T Format;
    AT_COPS_Oper_T Oper;
    AT_COPS_AcT_T AcT;
};
typedef struct AT_COPS_Param_S AT_COPS_Param_T;

/**
 * @brief 3GPP TS 27.007 CGDCONT @<PDP_Type@> (Packet Data Protocol) parameter
 * used to specify the type of data protocol.
 */
enum AT_CGDCONT_PdpType_E
{
    AT_CGDCONT_PDPTYPE_X25 = 0,      //!< ITU-T/CCITT X.25 layer 3 (obsolete, not supported)
    AT_CGDCONT_PDPTYPE_IP,           //!< Internet protocol
    AT_CGDCONT_PDPTYPE_IPV6,         //!< Internet protocol version 6
    AT_CGDCONT_PDPTYPE_IPV4V6,       //!< Virtual type used for dual stack IP
    AT_CGDCONT_PDPTYPE_OSPIH,        //!< Internet Hosted Octect Stream Protocol (obsolete, not supported)
    AT_CGDCONT_PDPTYPE_PPP,          //!< Point to Point Protocol (not supported)
    AT_CGDCONT_PDPTYPE_NONIP,        //!< Transfer of Non-IP data to external packet data network (not supported)
    AT_CGDCONT_PDPTYPE_ETHERNET,     //!< Ethernet protocol (not supported)
    AT_CGDCONT_PDPTYPE_UNSTRUCTURED, //!< Transfer of Unstructured data to the Data Network via N6 (not supported)
    AT_CGDCONT_PDPTYPE_INVALID       //!< Invalid @<PDP_Type@> value, used to signal that parameter not present
};
typedef enum AT_CGDCONT_PdpType_E AT_CGDCONT_PdpType_T;

/**
 * @brief 3GPP TS 27.007 CGDCONT parameters
 *
 * @note This is only a small subset of the available parameters. Implement more
 * as needed.
 */
struct AT_CGDCONT_Param_S
{
    /**
     * @brief Context-Id to identify the context to apply the settings to.
     * Must be valid Context-Id.
     */
    uint8_t Cid;
    /**
     * @brief Type of PDP context.
     * If set to #AT_CGDCONT_PDPTYPE_INVALID all context settings will be reset.
     */
    AT_CGDCONT_PdpType_T PdpType;
    /**
     * @brief APN of this context.
     * Can be set to NULL to skip this parameter.
     * Can be set to an empty string to request the APN during network
     * registration.
     */
    const char *Apn;
};
typedef struct AT_CGDCONT_Param_S AT_CGDCONT_Param_T;

/**
 * @brief 3GPP TS 27.007 CGACT @<state@> parameter used to control the state of a
 * data-context.
 */
enum AT_CGACT_State_E
{
    AT_CGACT_STATE_DEACTIVATED = 0, //!< Context will be (set) or is currently (get) deactivated
    AT_CGACT_STATE_ACTIVATED = 1,   //!< Context will be (set) or is currently (get) activated
    AT_CGACT_STATE_INVALID = 255    //!< Invalid @<state@> value, used to signal that parameter not present
};
typedef enum AT_CGACT_State_E AT_CGACT_State_T;

/**
 * @brief 3GPP TS 27.007 CGACT parameters
 *
 * @note Even though the CGACT setter command allows to set the state for
 * multiple contexts at a time, we only support changing the state one context
 * at a time. You may of course call the setter multiple times if you want to
 * activate multiple contexts.
 */
struct AT_CGACT_Param_S
{
    AT_CGACT_State_T State;
    uint8_t Cid;
};
typedef struct AT_CGACT_Param_S AT_CGACT_Param_T;

enum AT_CGPADDR_AddressType_E
{
    AT_CGPADDR_ADDRESSTYPE_IPV4,
    AT_CGPADDR_ADDRESSTYPE_IPV6,
    AT_CGPADDR_ADDRESSTYPE_INVALID
};
typedef enum AT_CGPADDR_AddressType_E AT_CGPADDR_AddressType_T;

struct AT_CGPADDR_Address_S
{
    AT_CGPADDR_AddressType_T Type;
    union {
        uint8_t IPv4[4];
        uint8_t IPv6[16];
    } Address;
};
typedef struct AT_CGPADDR_Address_S AT_CGPADDR_Address_T;

struct AT_CGPADDR_Param_S
{
    /**
     * @brief Context-Id to query.
     */
    uint8_t Cid;
};
typedef struct AT_CGPADDR_Param_S AT_CGPADDR_Param_T;

struct AT_CGPADDR_Resp_S
{
    /**
     * @brief First address of the PDP context.
     *
     * @note An IP address of all zeroes (IPv4: 0.0.0.0) and type INVALID means
     * is currently associated with this context.
     */
    AT_CGPADDR_Address_T Address1;
    /**
     * @brief Optional second address of the PDP context.
     *
     * @note An IP address of all zeroes (IPv4: 0.0.0.0) and type INVALID means
     * is currently associated with this context.
     */
    AT_CGPADDR_Address_T Address2;
};
typedef struct AT_CGPADDR_Resp_S AT_CGPADDR_Resp_T;

/**
 * @brief 3GPP TS 27.007 CFUN @<fun@> parameter used to select the MT
 * functionality level.
 */
enum AT_CFUN_Fun_E
{
    AT_CFUN_FUN_MINIMUM = 0,       //!< Set the MT to minimum functionality (disable rx/tx RF circuits).
    AT_CFUN_FUN_FULL = 1,          //!< Sets the MT to full functionality.
    AT_CFUN_FUN_DISABLETX = 2,     //!< Disables tx RF circuits.
    AT_CFUN_FUN_DISABLERX = 3,     //!< Disables rx RF circuits.
    AT_CFUN_FUN_DISABLERXTX = 4,   //!< Disables rx/tx RF circuits to establish air-plane-mode.
    AT_CFUN_FUN_RESERVEDSTART = 5, //!< Start of the manufacture specific values. Check with your device manufacturer for available modes.
    /* gap */
    AT_CFUN_FUN_RESERVEDEND = 127,     //!< End of the manufacture specific values.
    AT_CFUN_FUN_FULLSRA = 129,         //!< Sets the MT to full functionality with radio access support according to the setting of +CSRA.
    AT_CFUN_FUN_PREPARESHUTDOWN = 128, //!< Prepare for shutdown.
    AT_CFUN_FUN_INVALID = 255          //!< Invalid @<fun@> value.
};
typedef enum AT_CFUN_Fun_E AT_CFUN_Fun_T;

/**
 * @brief 3GPP TS 27.007 CFUN @<rst@> parameter used to control if the MT should
 * perform a reset before switching to the desired @<fun@> mode.
 */
enum AT_CFUN_Rst_E
{
    AT_CFUN_RST_NORESET = 0,  //!< Do not reset MT before switching to the selected @<fun>.
    AT_CFUN_RST_RESET = 1,    //!< Perform a silent MT reset (with graceful network detach) before switching to @<fun>.
    AT_CFUN_RST_INVALID = 255 //!< Invalid @<rst@> value.
};
typedef enum AT_CFUN_Rst_E AT_CFUN_Rst_T;

/**
 * @brief 3GPP TS 27.007 CFUN parameters.
 */
struct AT_CFUN_Param_S
{
    AT_CFUN_Fun_T Fun;
    AT_CFUN_Rst_T Rst;
};
typedef struct AT_CFUN_Param_S AT_CFUN_Param_T;

/**
 * @brief 3GPP TS 27.007 CFUN response.
 */
struct AT_CFUN_Resp_S
{
    AT_CFUN_Fun_T Fun;
};
typedef struct AT_CFUN_Resp_S AT_CFUN_Resp_T;

/* *** NETWORK COMMANDS ***************************************************** */

/**
 * @brief Set the mode and information content of the CREG (network
 * registration) URC.
 *
 * @param[in] n
 * Mode of the CREG URC (supported range: #AT_CXREG_N_DISABLED to
 * #AT_CXREG_N_URC_LOC_CAUSE)
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CREG(AT_CXREG_N_T n);

/**
 * @brief Set the mode and information content of the CGREG (GPRS network
 * registration) URC.
 *
 * @param[in] n
 * Mode of the CGREG URC (supported range: #AT_CXREG_N_DISABLED to
 * #AT_CXREG_N_URC_LOC_PSM_CAUSE)
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CGREG(AT_CXREG_N_T n);

/**
 * @brief Set the mode and information content of the CEREG (EPS network
 * registration) URC.
 *
 * @param[in] n
 * Mode of the CEREG URC (supported range: #AT_CXREG_N_DISABLED to
 * #AT_CXREG_N_URC_LOC_PSM_CAUSE)
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CEREG(AT_CXREG_N_T n);

/**
 * @brief Get the mode of the CREG (network registration) URC.
 *
 * @param[out] n
 * Will hold the value of the 'n' argument returned from AT+CREG?
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_CREG(AT_CREG_Param_T *n);

/**
 * @brief Get the mode of the CGREG (GPRS network registration) URC.
 *
 * @param[out] n
 * Will hold the value of the 'n' argument returned from AT+CGREG?
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_CGREG(AT_CGREG_Param_T *n);

/**
 * @brief Get the mode of the CEREG (EPS network registration) URC.
 *
 * @param[out] n
 * Will hold the value of the 'n' argument returned from AT+CEREG?
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_CEREG(AT_CEREG_Param_T *n);

/**
 * @brief Set the mode of the CMEE (mobile termination error) .
 *
 * @param[in] mode
 * Will hold the used mode
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CMEE(uint32_t mode);

/**
 * @brief Set the network operator selection criteria for GSM/UMTS/EPS/etc.
 * The modem will rely on these to choose an operator to register to.
 *
 * @param[in] param
 * Valid structure pointer containing the network operator criteria details.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_COPS(const AT_COPS_Param_T *param);

/**
 * @brief Set the connection parameters for a data-context.
 *
 * @param[in] param
 * Valid structure pointer containing the context definition parameters.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CGDCONT(const AT_CGDCONT_Param_T *param);

/**
 * @brief Activate or deactivate a specified data-context.
 *
 * @note If the specified data-context is already activated or deactivated, no
 * action is taken.
 *
 * @param[in] param
 * Valid structure pointer containing the details of which context to
 * activate/deactivate.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CGACT(const AT_CGACT_Param_T *param);

/**
 * @brief Show the PDP address for the specified Context-Id.
 *
 * @param[in] param
 * Valid structure pointer to initiate the 'Show Address' procedure. The
 * addresses will be written into the response structure.
 *
 * @param[out] resp
 * Valid structure to hold the response data of the CGPADDR command.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CGPADDR(const AT_CGPADDR_Param_T *param, AT_CGPADDR_Resp_T *resp);

/* *** SIM COMMANDS ********************************************************* */

/**
 * @brief Enter the PIN to unlock the SIM card.
 *
 * @param[in] pin
 * C-string containing the PIN.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CPIN(const char *pin);

/* *** TE-TA INTERFACE COMMANDS ********************************************* */

/**
 * @brief Send out the AT no-operation command (`AT<S3><S4>`) and wait for
 * response.
 * Can be used to probe the AT device for responsiveness.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Test_AT(void);

/**
 * @brief Enable or disable command echoing of the DCE (modem).
 *
 * Refer to ITU-T Recommendation V.250 (07/2003).
 *
 * @param[in] enableEcho
 * True if echoing should be enabled, false otherwise.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_ATE(bool enableEcho);

/* *** POWER CONTROL COMMANDS *********************************************** */

/**
 * @brief Set the MT to a desired functionality level or perform a MT reset.
 *
 * @param[in] param
 * Valid structure pointer that contains the desired functionality level and
 * optional reset behavior.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_CFUN(const AT_CFUN_Param_T *param);

/**
 * @brief Query the MT functionality state.
 *
 * @param[out] resp
 * Will contain the response to the +CFUN getter command.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_CFUN(AT_CFUN_Resp_T *resp);

/* *** URC HANDLERS ********************************************************* */

Retcode_T At_HandleUrc_CREG(void);

Retcode_T At_HandleUrc_CGREG(void);

Retcode_T At_HandleUrc_CEREG(void);

#endif /* AT_3GPP_27007_H_ */

/** @} */
