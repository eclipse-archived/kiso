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
 * @ingroup KISO_CELLULAR_VARIANT_UBLOX
 * @file
 *
 * @brief Provides functionality for the modem interaction with the network
 *
 * @details Provides functionality for the modem interaction with the network by providing functions that register, configure, queries,
 * deregister the modem form the network and so on
 *
 * Reference:
 *  - Ref 2: u-blox-ATCommands_Manual_(UBX-13002752).pdf
 */

#ifndef UBLOX_NETWORK_H
#define UBLOX_NETWORK_H

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#define CELLULAR_NETWORK_SMALL_BUFFER_SIZE (UINT32_C(32))
#define CELLULAR_NETWORK_BUFFER_SIZE (UINT32_C(256))

#define CELLULAR_NETWORK_PROFILE_ID_LOW (UINT8_C(0))
#define CELLULAR_NETWORK_PROFILE_ID_HIGH (UINT8_C(6))
#define CELLULAR_NETWORK_CELLINFO_COUNT (UINT8_C(4))

#define CELLULAR_NETWORK_APN_LENGTH_MAX (UINT32_C(99))
#define CELLULAR_NETWORK_USERNAME_LENGTH_MAX (UINT32_C(30))
#define CELLULAR_NETWORK_PASSWORD_LENGTH_MAX (UINT32_C(30))

/**
 * @brief Enumeration for selected radio access technology (ref1: SelectedAcT)
 *
 * @details  `<SelectedAcT>` Number Indicates the radio access technology and may be:
 *  * 0: GSM (single mode)
 *  * 1: (factory-programmed value): GSM / UMTS (dual mode)
 *  * 2: UMTS (single mode)
 *  * 3: LTE (single mode)
 *  * 4: GSM / UMTS / LTE (tri mode)
 *  * 5: GSM / LTE (dual mode)
 *  * 6: UMTS / LTE (dual mode)
 *
 * @see  Ref 2, p 88, 7.5 Radio Access Technology (RAT) selection +URAT
 */
enum CellularNetwork_SelectedRadioAccessTechnology_E
{
    CELLULAR_NETWORK_SEL_RAT_GSM_SINGLEMODE = 0,
    CELLULAR_NETWORK_SEL_RAT_GSM_UMTS_DUALMODE = 1,
    CELLULAR_NETWORK_SEL_RAT_UMTS_SINGLEMODE = 2,
    CELLULAR_NETWORK_SEL_RAT_LTE_SINGLEMODE = 3,       //-- NOT for LISA-U200
    CELLULAR_NETWORK_SEL_RAT_GSM_UMTS_LTE_TRIMODE = 4, //-- NOT for LISA-U200
    CELLULAR_NETWORK_SEL_RAT_GSM_LTE_DUALMODE = 5,     //-- NOT for LISA-U200
    CELLULAR_NETWORK_SEL_RAT_UMTS_LTE_DUALMODE = 6     //-- NOT for LISA-U200
};
typedef enum CellularNetwork_SelectedRadioAccessTechnology_E CellularNetwork_SelectedRadioAccessTechnology_T;

/**
 * @brief Enumeration for preferred radio access technology (Ref 1: PreferredAct)
 * @details  `<PreferredAct>` Number Indicates the preferred access technology; it is ignored if dual mode or tri mode are not selected.
 *  * 0: GSM
 *  * 2 (default value and factory-programmed value): UTRAN
 *  * 3: LTE
 *
 * @see  Ref 2, p 88, 7.5 Radio Access Technology (RAT) selection +URAT
 */
enum CellularNetwork_PreferredRadioAccessTechnology_E
{
    CELLULAR_NETWORK_PREF_RAT_GSM = 0,
    CELLULAR_NETWORK_PREF_RAT_UMTS = 2, //-- sic 2 and not 1
    CELLULAR_NETWORK_PREF_RAT_LTE = 3   //-- NOT for module LISA-U200
};
typedef enum CellularNetwork_PreferredRadioAccessTechnology_E CellularNetwork_PreferredRadioAccessTechnology_T;

/**
 * @brief Radio Access Techonology (RAT)
 * @details Applicable modes for LISA-U200
 * * only GSM (2G), "0,0":                     CELLULAR_RAT_SELECT_GSM_SINGLEMODE, CELLULAR_RAT_PREFER_GSM
 * * both GSM and UMTS but prefer GSM, "1,0"   CELLULAR_RAT_SELECT_GSM_UMTS_DUALMODE, CELLULAR_RAT_PREFER_GSM
 * * both GSM and UMTS but prefer UMTS, "1,2"  CELLULAR_RAT_SELECT_GSM_UMTS_DUALMODE, CELLULAR_RAT_PREFER_UMTS
 * * only UMTS (3G), "2,2"                     CELLULAR_RAT_SELECT_UMTS_SINGLEMODE, CELLULAR_RAT_PREFER_UMTS
 *
 * @see CellularNetworkRatSelectedAcT_T and CellularNetworkRatPreferredAct_T
 */
struct CellularNetwork_RadioAccessTechnology_S
{
    CellularNetwork_SelectedRadioAccessTechnology_T Selection;
    CellularNetwork_PreferredRadioAccessTechnology_T Preference;
    bool IsValidSelection;
    bool IsValidPreference;
};
typedef struct CellularNetwork_RadioAccessTechnology_S CellularNetwork_RadioAccessTechnology_T;

/**
 * @brief Enumeration for current GPRS registration status of the MT
 * @details `<status>` Number Current GPRS registration status and PLMN selection mode of the MT
 * * 0: attached, automatic PLMN selection mode
 * * 1: attached, manual PLMN selection mode
 * * 2: detached
 *
 * @see  Ref 2, p 92, 7.9 PS operator selection +UCGOPS
 */
enum CellularNetwork_PsStatus_E
{
    CELLULAR_NETWORK_PS_STATUS_AUTOMATIC = 0,
    CELLULAR_NETWORK_PS_STATUS_MANUAL = 1,
    CELLULAR_NETWORK_PS_STATUS_DETACHED = 2,
    CELLULAR_NETWORK_PS_STATUS_UNKNOWN = 255 //-- invalid  (NEVER part of modem answer)
};
typedef enum CellularNetwork_PsStatus_E CellularNetwork_PsStatus_T;

/**
 * @brief Enumeration for radio access technology
 * @details `<AcT>` Number Indicates the radio access technology
 * * 0: GSM
 * * 1: GSM COMPACT
 * * 2: UTRAN (UMTS)
 * * 3: GSM with EDGE availability
 * * 4: UTRAN with HSDPA availability
 * * 5: UTRAN with HSUPA availability
 * * 6: UTRAN with HSDPA and HSUPA availability
 * * 7: LTE
 * * 255: invalid --> CREG?
 *
 * @see  Ref 2, p 92, 7.9 PS operator selection +UCGOPS
 *                     7.10 Network registration status +CREG
 */
enum CellularNetwork_Rat_E
{
    CELLULAR_NETWORK_RAT_GSM = 0,
    CELLULAR_NETWORK_RAT_GSM_COMPACT = 1,
    CELLULAR_NETWORK_RAT_UTRAN_UMTS = 2,
    CELLULAR_NETWORK_RAT_GSM_EDGE = 3,
    CELLULAR_NETWORK_RAT_UTRAN_HSDPA = 4,
    CELLULAR_NETWORK_RAT_UTRAN_HSUPA = 5,
    CELLULAR_NETWORK_RAT_UTRAN_HSDPA_HSUPA = 6,
    CELLULAR_NETWORK_RAT_LTE = 7,      //-- not for LISA_U200
    CELLULAR_NETWORK_RAT_INVALID = 255 //-- VALID value from modem if unkown!
};
typedef enum CellularNetwork_Rat_E CellularNetwork_Rat_T;

/**
 * @brief Enumeration for network operator name format
 * @details `<format>` Number Mandatory parameter if `<mode>` value is 1, it specifies the format of the network operator name
 * * 0 (default value): long alphanumeric `<oper>`
 * * 1: short format alphanumeric `<oper>`
 * * 2: numeric `<oper>`
 *
 * @see  Ref 2, p 92, 7.9 PS operator selection +UCGOPS
 */
enum CellularNetwork_FormatOperatorName_E
{
    CELLULAR_NETWORK_FORMAT_NAME_LONG_A = 0,
    CELLULAR_NETWORK_FORMAT_NAME_SHORT_A = 1,
    CELLULAR_NETWORK_FORMAT_NAME_NUMERIC = 2,
    CELLULAR_NETWORK_FORMAT_NAME_INVALID = 255 //-- invalid
};
typedef enum CellularNetwork_FormatOperatorName_E CellularNetwork_FormatOperatorName_T;

/**
 * @brief Enumeration for current network operator mode
 * @details `<mode>` Number Is used to chose whether the network selection is automatically done by the MT or is forced by
 * this command to the operator `<oper>` given in the format `<format>`:
 *  * 0 (default value and factory-programmed value): automatic (`<oper>` field is ignored)
 *  * 1: manual
 *  * 2: deregister from network
 *  * 3: set only `<format>`
 *  * 4: manual/automatic
 *  * 5: extended network search
 *  * 6: extended network search without the tags (e.g. MCC, RxLev will not be printed, see the syntax and the command example)
 *  * 8: network timing advance search
 *
 * @see  Ref 2, p83, ch 7.4 Operator selection +COPS
 */
enum CellularNetwork_SelectionMode_E
{
    CELLULAR_NETWORK_SEL_MODE_DEFAULT = 0,
    CELLULAR_NETWORK_SEL_MODE_MANUAL = 1,
    CELLULAR_NETWORK_SEL_MODE_DEREGISTER = 2,
    CELLULAR_NETWORK_SEL_MODE_SET_ONLY = 3,
    CELLULAR_NETWORK_SEL_MODE_MANUAL_ATUO = 4,
    CELLULAR_NETWORK_SEL_MODE_EXTENDED = 5,
    CELLULAR_NETWORK_SEL_MODE_EXTENDED_TAGS = 6,
    CELLULAR_NETWORK_SEL_MODE_RESERVED = 7,
    CELLULAR_NETWORK_SEL_MODE_NETWORK_TA = 8,
    CELLULAR_NETWORK_SEL_MODE_INVALID = 255 //-- invalid (NEVER part of modem answer)
};
typedef enum CellularNetwork_SelectionMode_E CellularNetwork_SelectionMode_T;

/**
 * @brief struct used for operator selection
 * @details `+COPS: <mode>[,<format>,<oper>[,<AcT>]]`
 *
 * @see Ref 2, p83, ch 7.4 Operator selection +COPS
 */
struct CellularNetwork_CurrentOperator_S
{
    CellularNetwork_SelectionMode_T Mode;
    CellularNetwork_FormatOperatorName_T Format;
    CellularNetwork_Rat_T Rat;
    bool IsValidMode;
    bool IsValidFormat;
    bool IsValidRat;
    bool IsValidOperator;
    char Operator[CELLULAR_NETWORK_SMALL_BUFFER_SIZE];
};
typedef struct CellularNetwork_CurrentOperator_S CellularNetwork_CurrentOperator_T;

/**
 * @brief Enumeration for network registration status param 1
 * @details `<n>` Number
 *  * 0 (default value and factory-programmed value): network registration URC disabled
 *  * 1: network registration URC `+CREG: <stat>` enabled
 *  * 2: network registration and location information URC `+CREG: <stat>[,<lac>,<ci>[,<AcTStatus>]]` enabled
 *
 * @see  Ref 2, p 94, 7.10 Network registration status +CREG
 */
enum CellularNetwork_RegistrationInfo_E //-- this is the n in AT+CREG=[<n>]
{
    CELLULAR_NETWORK_REG_INFO_DEFAULT = 0, //-- URC disabled
    CELLULAR_NETWORK_REG_INFO_URC = 1,
    CELLULAR_NETWORK_REG_INFO_URC_LOC = 2,  //-- verbose
    CELLULAR_NETWORK_REG_INFO_INVALID = 255 //-- invalid (NEVER part of modem answer)
};
typedef enum CellularNetwork_RegistrationInfo_E CellularNetwork_RegistrationInfo_T;

/**
 * @brief Enumeration for network registration status param 1
 * @details `<stat>` Number
 * * 0: not registered, the MT is not currently searching a new operator to register to
 * * 1: registered, home network
 * * 2: not registered, but the MT is currently searching a new operator to register to
 * * 3: registration denied
 * * 4: unknown (e.g. out of GERAN/UTRAN/E-UTRAN coverage)
 * * 5: registered, roaming
 * * 6: registered for "SMS only", home network (applicable only when `<AcTStatus>` indicates E-UTRAN)
 * * 7: registered for "SMS only", roaming (applicable only when `<AcTStatus>` indicates E-UTRAN)
 * * 9: registered for "CSFB not preferred", home network (applicable only when `<AcTStatus>` indicates E-UTRAN)
 * * 10: registered for "CSFB not preferred", roaming (applicable only when `<AcTStatus>` indicates E-UTRAN)
 *
 * @see  Ref 2, p 94, 7.10 Network registration status +CREG
 */
enum CellularNetwork_RegistrationStatusStat_E
{
    CELLULAR_NETWORK_REG_STAT_NOT = 0,
    CELLULAR_NETWORK_REG_STAT_HOME = 1,
    CELLULAR_NETWORK_REG_STAT_NOT_AND_SEARCH = 2,
    CELLULAR_NETWORK_REG_STAT_DENIED = 3,
    CELLULAR_NETWORK_REG_STAT_UNKOWN = 4, //-- valid value e.g. out of GERAN/UTRAN/E-UTRAN coverage
    CELLULAR_NETWORK_REG_STAT_ROAMING = 5,
    CELLULAR_NETWORK_REG_STAT_SMSONLY_HOME = 6,
    CELLULAR_NETWORK_REG_STAT_SMSONLY_ROAMING = 7,
    //-- no value 8
    CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_HOME = 9,
    CELLULAR_NETWORK_REG_STAT_CSFB_NOT_PREF_ROAMING = 10,
    CELLULAR_NETWORK_REG_STAT_INVALID = 255 //-- invalid (NEVER part of modem answer)
};
typedef enum CellularNetwork_RegistrationStatusStat_E CellularNetwork_RegistrationStatusStat_T;

/**
 * @brief Enumeration for network operator name format
 *
 * @see  Ref 2, p 94, 7.10 Network registration status +CREG
 */
struct CellularNetwork_RegistrationStatus_S
{
    CellularNetwork_RegistrationInfo_T Info;
    CellularNetwork_RegistrationStatusStat_T Stat;
    uint32_t Lac; //-- Location Area Code, range 0h-FFFFh (2 octets)
    uint32_t Ci;  //-- Cell Identity, range 0h-FFFFFFFFh (4 octets)
    CellularNetwork_Rat_T Rat;
    bool IsValidInfo;
    bool IsValidStat;
    bool IsValidLac;
    bool IsValidCi;
    bool IsValidRat;
};
typedef struct CellularNetwork_RegistrationStatus_S CellularNetwork_RegistrationStatus_T;

/**
 * @brief Enumeration of network cell environment description mode
 * @details `<mode>` Number
 * * 0 (default value): one shot dump
 * * 1: periodic refreshed dump; the information for up to 32 neighbour cells is available
 * * 2: stop periodic dump
 * * 3: one shot serving cell dump
 * * 4: periodic serving cell refreshed dump
 * * 5: one shot serving cell and neighbour cells dump
 * * 6: periodic serving cell and neighbour cells refreshed dump
 * * 128: one shot dump without the tags
 * * 129: periodic refreshed dump without the tags
 * * 130: stop periodic dump (same as 2)
 * * 131: one shot serving cell without the tags
 * * 132: periodic serving cell refreshed without the tags
 * * 133: one shot serving cell and neighbour cells without the tags
 * * 134: periodic serving cell and neighbour cells refreshed without the tags
 *
 * @see  Ref 2, p 106, 7.21 Cell environment description +CGED
 */
enum CellularNetwork_CellEnvironmentDescription_E
{
    CELLULAR_NETWORK_CELL_ENV_MODE_ONESHOT = 0, //-- AT+CGED=0
    CELLULAR_NETWORK_CELL_ENV_MODE_PERIODIC = 1,
    CELLULAR_NETWORK_CELL_ENV_MODE_STOPPERIODIC = 2,
    CELLULAR_NETWORK_CELL_ENV_MODE_ONESHOT_SERVCELL = 3 //-- Note: data contains MCC, LAC, CI (dualmode UMTS preferred)
};
typedef enum CellularNetwork_CellEnvironmentDescription_E CellularNetwork_CellEnvironmentDescription_T;

/**
 * @brief struct for returning network cell information for caller
 * @details The data is transferred by modem among other values by CellularNetwork_CellEnvironmentDescription_T
 * mode CELLULAR_NETWORK_CELL_ENV_MODE_ONESHOT_SERVCELL
 * MCC and MNC are given by modem as decimal number string, LAC and CI are given as hexadecimal number string
 *
 * @see  MMC, MNC, LAC, CI: see Ref 2, p 111 ch 7.21 Cell environment description +CGED
 */
struct Cellular_CellEnvironmentData_S
{
    uint32_t Mcc; //-- Mobile Country Code, range 0 - 999 (3 digits). Other values are to be considered invalid
    uint32_t Mnc; //-- Mobile Network Code, range 0 - 999 (1 to 3 digits). Other values are to be considered invalid not available
    uint32_t Lac; //-- Location Area Code, range 1h-FFFFh without FFFE (2 octets) (Error in doc Ref 1 regarding value range)
    uint32_t Ci;  //-- Cell Identity, range 0h-FFFFFFFFh (4 octets)
};
typedef struct Cellular_CellEnvironmentData_S Cellular_CellEnvironmentData_T;

/**
 * @brief Enumeration for network cell information parser
 */
enum CellularNetwork_CellEnvironmentFieldIndex_E
{
    CELLULAR_NETWORK_CELL_ENV_FIELD_MCC = 0,
    CELLULAR_NETWORK_CELL_ENV_FIELD_MNC = 1,
    CELLULAR_NETWORK_CELL_ENV_FIELD_LAC = 2,
    CELLULAR_NETWORK_CELL_ENV_FIELD_CI = 3,
    CELLULAR_NETWORK_CELL_ENV_FIELD_UNKONW = 255
};
typedef enum CellularNetwork_CellEnvironmentFieldIndex_E CellularNetwork_CellEnvironmentFieldIndex_T;

/**
 * @brief Callback type def for CellularNetwork URC data
 * @warning This callback MUST NOT call driver functions
 */
typedef void (*CellularNetwork_UrcCregCallback_T)(const CellularNetwork_RegistrationStatus_T data);

/**
 * @brief Enumeration for network rssi
 * @details Received Signal Strength Indication (RSSI)
 * * 0: -113 dBm or less
 * * 1: -111 dBm
 * * 2..30: from -109 to -53 dBm with 2 dBm steps
 * * 31: -51 dBm or greater
 * * 99: not known or not detectable or currently not available
 *
 * @see Ref 2, p80, ch 7.2 Signal quality +CSQ
 */
enum CellularNetwork_Rssi_E
{
    CELLULAR_NETWORK_RSSI_LESS_113_DBM = 0,
    CELLULAR_NETWORK_RSSI_111_DBM = 1,
    CELLULAR_NETWORK_RSSI_109_DBM = 2, //-- 2..30: from -109 to -53 dBm with 2 dBm steps
    CELLULAR_NETWORK_RSSI_107_DBM = 3,
    CELLULAR_NETWORK_RSSI_105_DBM = 4,
    CELLULAR_NETWORK_RSSI_103_DBM = 5,
    CELLULAR_NETWORK_RSSI_101_DBM = 6,
    CELLULAR_NETWORK_RSSI_99_DBM = 7,
    CELLULAR_NETWORK_RSSI_97_DBM = 8,
    CELLULAR_NETWORK_RSSI_95_DBM = 9,
    CELLULAR_NETWORK_RSSI_93_DBM = 10,
    CELLULAR_NETWORK_RSSI_91_DBM = 11,
    CELLULAR_NETWORK_RSSI_88_DBM = 12,
    CELLULAR_NETWORK_RSSI_87_DBM = 13,
    CELLULAR_NETWORK_RSSI_85_DBM = 14,
    CELLULAR_NETWORK_RSSI_83_DBM = 15,
    CELLULAR_NETWORK_RSSI_81_DBM = 16,
    CELLULAR_NETWORK_RSSI_79_DBM = 17,
    CELLULAR_NETWORK_RSSI_77_DBM = 18,
    CELLULAR_NETWORK_RSSI_75_DBM = 19,
    CELLULAR_NETWORK_RSSI_73_DBM = 20,
    CELLULAR_NETWORK_RSSI_71_DBM = 21,
    CELLULAR_NETWORK_RSSI_69_DBM = 22,
    CELLULAR_NETWORK_RSSI_67_DBM = 23,
    CELLULAR_NETWORK_RSSI_65_DBM = 24,
    CELLULAR_NETWORK_RSSI_63_DBM = 25,
    CELLULAR_NETWORK_RSSI_61_DBM = 26,
    CELLULAR_NETWORK_RSSI_59_DBM = 27,
    CELLULAR_NETWORK_RSSI_57_DBM = 28,
    CELLULAR_NETWORK_RSSI_55_DBM = 29,
    CELLULAR_NETWORK_RSSI_53_DBM = 30, //-- -53 dBm
    CELLULAR_NETWORK_RSSI_51_DBM = 31, //-- 31: -51 dBm or greater
    CELLULAR_NETWORK_RSSI_UNKNOWN = 99 //-- not known or not detectable or currently not available. Note: valid modem answer
};
typedef enum CellularNetwork_Rssi_E CellularNetwork_Rssi_T;

/**
 * @brief Enumeration for network qual
 * @note Lisa-U200: The `<qual>` parameter is not updated in GPRS and EGPRS packet transfer mode.
 *
 * @details
 * `<qual>` |    2G RAT CS and GPRS     |   2G RAT EGPRS           |   UMTS RAT              |   LTE RAT
 * ---------|---------------------------|--------------------------|-------------------------|-----------------------
 * 0        |    BER < 0.2%             |   28 <= MEAN_BEP <= 31   |   ECN0_LEV >= 44        |   RSRQ_LEV < 5
 * 1        |    0.2% < BER < 0.4%      |   24 <= MEAN_BEP <= 27   |   38 <= ECNO_LEV < 44   |   5 <= RSRQ_LEV < 10
 * 2        |    0.4% < BER < 0.8%      |   20 <= MEAN_BEP <= 23   |   32 <= ECNO_LEV < 38   |   10 <= RSRQ_LEV < 14
 * 3        |    0.8% < BER < 1.6%      |   16 <= MEAN_BEP <= 19   |   26 <= ECNO_LEV < 32   |   14 <= RSRQ_LEV < 18
 * 4        |    1.6% < BER < 3.2%      |   12 <= MEAN_BEP <= 15   |   20 <= ECNO_LEV < 26   |   18 <= RSRQ_LEV < 22
 * 5        |    3.2% < BER < 6.4%      |   8 <= MEAN_BEP <= 11    |   14 <= ECNO_LEV < 20   |   22 <= RSRQ_LEV < 26
 * 6        |    6.4% < BER < 12.8%     |   4 <= MEAN_BEP <= 7     |   8 <= ECNO_LEV < 14    |   26 <= RSRQ_LEV < 30
 * 7        |    BER > 12.8%            |   0 <= MEAN_BEP <= 3     |   ECNO_LEV < 8          |   RSRQ _LEV >= 30
 * 99       |Not known or not detectable| <-- see                  | <-- see                 | <-- see
 *
 * @see  Ref 2, p80, ch 7.2 Signal quality +CSQ
 */
enum CellularNetwork_Quality_E
{
    CELLULAR_NETWORK_QUALITY_0 = 0,
    CELLULAR_NETWORK_QUALITY_1 = 1,
    CELLULAR_NETWORK_QUALITY_2 = 2,
    CELLULAR_NETWORK_QUALITY_3 = 3,
    CELLULAR_NETWORK_QUALITY_4 = 4,
    CELLULAR_NETWORK_QUALITY_5 = 5,
    CELLULAR_NETWORK_QUALITY_6 = 6,
    CELLULAR_NETWORK_QUALITY_7 = 7,
    CELLULAR_NETWORK_QUALITY_UNKOWN = 99 //-- Not known or not detectable. Note: valid modem answer
};
typedef enum CellularNetwork_Quality_E CellularNetwork_Quality_T;

/**
 * @brief struct used for getting signal quality of 2G/3G
 * @details `+CSQ: <rssi>,<qual>`
 * @see Ref 2, p80, ch 7.2 Signal quality +CSQ
 */
struct CellularNetwork_SignalQuality_S
{
    CellularNetwork_Rssi_T Rssi;
    CellularNetwork_Quality_T Quality;
    bool IsValidRssi;
    bool IsValidQuality;
};
typedef struct CellularNetwork_SignalQuality_S CellularNetwork_SignalQuality_T;

/**
 * @brief Enumeration for Packet switched data (PSD) configuration
 * @see Ref 2, p319, ch 18.5 Packet switched data configuration +UPSD
 */
typedef enum
{
    CELLULAR_PSD_PARAM_PROTOCOL = 0,
    CELLULAR_PSD_PARAM_APN = 1,
    CELLULAR_PSD_PARAM_USERNAME = 2,
    CELLULAR_PSD_PARAM_PASSWORD = 3,
    CELLULAR_PSD_PARAM_DNS1 = 4,
    CELLULAR_PSD_PARAM_DNS2 = 5,
    CELLULAR_PSD_PARAM_AUTH = 6
    /* Add more when required */
} Cellular_PsdParam_T;

typedef enum
{
    CELLULAR_PSD_PROTOCOL_IPV4 = 0,
    CELLULAR_PSD_PROTOCOL_IPV6 = 1,
    CELLULAR_PSD_PROTOCOL_IPV4V6 = 2,
    CELLULAR_PSD_PROTOCOL_IPV6V4 = 3
} Cellular_PsdProtocol_T;

typedef enum
{
    CELLULAR_PSD_AUTH_NONE = 0,
    CELLULAR_PSD_AUTH_PAP = 1,
    CELLULAR_PSD_AUTH_CHAP = 2,
    CELLULAR_PSD_AUTH_AUTO = 3,
} Cellular_PsdAuthentication_T;

typedef enum
{
    CELLULAR_PSD_DATA_COMPERSSION_OFF = 0,
    CELLULAR_PSD_DATA_COMPERSSION_PREDEFINED = 1,
    CELLULAR_PSD_DATA_COMPERSSION_V42BIS = 2
} Cellular_PsdDataCompression_T;

typedef enum
{
    CELLULAR_PSD_HEADER_COMPERSSION_OFF = 0,
    CELLULAR_PSD_HEADER_COMPERSSION_PREDEFINED = 1,
    CELLULAR_PSD_HEADER_COMPERSSION_RFC1144 = 2,
    CELLULAR_PSD_HEADER_COMPERSSION_RFC2507 = 3,
    CELLULAR_PSD_HEADER_COMPERSSION_RFC3095 = 4
} Cellular_PsdHeaderCompression_T;

typedef enum
{
    CELLULAR_PSD_QOS_PRECEDENCE_DEFAULT = 0,
    CELLULAR_PSD_QOS_PRECEDENCE_HIGH = 1,
    CELLULAR_PSD_QOS_PRECEDENCE_NORMAL = 2,
    CELLULAR_PSD_QOS_PRECEDENCE_LOW = 3
} Cellular_PsdQosPrecedence_T;

typedef enum
{
    CELLULAR_PSD_PARAM_TYPE_U32,
    CELLULAR_PSD_PARAM_TYPE_STRING
} Cellular_PsdParamType_T;

typedef struct
{
    Cellular_PsdParam_T Tag;
    Cellular_PsdParamType_T Type;
    uint32_t Offset;
} Cellular_PsdProfileItem_T;

typedef struct
{
    uint32_t Id;
    Cellular_PsdProtocol_T Protocol;
    const char *Apn;
    const char *Username;
    const char *Password;
    const char *Dns1;
    const char *Dns2;
    Cellular_PsdAuthentication_T Authentication;
    const char *IpAddress;
    Cellular_PsdDataCompression_T DataCompression;
    Cellular_PsdHeaderCompression_T HeaderCompression;
    Cellular_PsdQosPrecedence_T QosPrecedence;
    /* Add more when required */
} Cellular_PsdProfile_T;

/**
 * @brief Query network signal quality (rssi + qual -> BER)
 * @param[out] NetworkSignalQuality
 * @return RETCODE_OK only if data has been received successfully
 */
Retcode_T CellularNetwork_QueryCurrentSignalQuality(CellularNetwork_SignalQuality_T *NetworkSignalQuality); //-- AT+CSQ

/**
 * @brief Registers callback for data of URC from CREG
 */
void CellularNetwork_RegisterUrcCregCallback(CellularNetwork_UrcCregCallback_T CallbackUrcCreg);

/**
 * @brief Query cell environment information from modem
 * @details `AT+CGED=<n>` (Cell environment description)
 *
 * * `<MCC>` Number Mobile Country Code, range 0 - 999 (3 digits). Other values are to be considered invalid / notavailable
 * * `<MNC>` Number Mobile Network Code, range 0 - 999 (1 to 3 digits). Other values are to be considered invalid / not available
 * * `<LAC>` Number Location Area Code, range 0h-FFFFh (2 octets)
 * * `<CI>` Number Cell Identity, range 0h-FFFFh (2 octets)
 * * `<BSIC>` Number Base Station Identify Code, range 0h-3Fh (6 bits)
 * * `<arfcn>` Number Absolute Radio Frequency Channel Number, range 0 - 1023
 *
 *          The parameter value also decodes the band indicator bit (DCS or PCS) by means of the
 *          most significant byte (8 means 1900 band) (i.e. if the parameter reports the value 33485, it
 *          corresponds to 0x82CD, in the most significant byte there is the band indicator bit, so the
 *          `<arfcn>` is 0x2CD (717) and belongs to 1900 band)
 *
 * @param[out] data cell information
 * @return RETCODE_OK The network status was retrieved correctly
 * @return RETCODE_CELLULAR_NOT_RESPONSIVE The modem did not respond to the network status query
 * @see Ref 2, p 106, ch 7.21 Cell environment description +CGED
 */
Retcode_T CellularNetwork_QueryCellEnvironment(Cellular_CellEnvironmentData_T *data); //-- AT+CGED=3

/**
 * @brief Get the current network registration status from the modem
 * @param[out] status Current registration status
 * @return RETCODE_OK The network status was retrieved correctly
 * @return RETCODE_CELLULAR_NOT_RESPONSIVE The modem did not respond to the network status query
 * @return RETCODE_CELLULAR_RESPONSE_UNEXPECTED Command arguments not expected
 */
Retcode_T CellularNetwork_QueryRegistrationStatus(CellularNetwork_RegistrationStatus_T *status); //-- AT+CREG?

/**
 * @brief Configures current network registration method. Affects sending of URCs.
 * @details
 * * CELLULAR_NETWORK_REG_N_DEFAULT: 0 (default value and factory-programmed value): network registration URC disabled
 * * CELLULAR_NETWORK_REG_N_URC:     1: network registration URC `+CREG: <stat>` enabled
 * * CELLULAR_NETWORK_REG_N_URC_LOC  2: network registration and location information URC `+CREG: <stat>[,<lac>,<ci>[,<AcTStatus>]]` enabled
 * @param[in] setting Network registration method (like statsu)
 * @return RETCODE_OK Only if successful
 * @return RETCODE_CELLULAR_NOT_RESPONSIVE The modem did not respond to the network status query
 * @return RETCODE_CELLULAR_RESPONSE_UNEXPECTED Command arguments not expected
 * @see Ref 2, p 94, ch 7.10 Network registration status +CREG
 */
Retcode_T CellularNetwork_ConfigureRegistrationInformation(CellularNetwork_RegistrationInfo_T setting); //-- AT+CREG=[<n>]

/**
 * @brief Deregister the module from the network
 * @details `AT+COPS=2` (Deregister the module from the network)
 * @return RETCODE_OK only if the network was deregisterred successfully
 */
Retcode_T CellularNetwork_Deregister(void); //-- AT+COPS=2

/**
 * @brief Enable automatic network selection
 * @details `AT+COPS=0` (enable automatic network selection by MT)
 * @return RETCODE_OK only if mode automatic network selection was set successfully
 */
Retcode_T CellularNetwork_EnableAutomaticSelection(void); //-- AT+COPS=0

/**
 * @brief Query current network operator
 * @return RETCODE_OK only if mode automatic network selection was set successfully
 */
Retcode_T CellularNetwork_QueryCurrentOperator(CellularNetwork_CurrentOperator_T *NetworkOperator); //-- AT+COPS?

/**
 * @brief set APN for PDP Context (hardcoded intern)
 * @details `AT+UPSD=` (set networks APN (depending on SIM card, may require also username and password)
 * @param[in] APN
 * @param[in] length of APN buffer
 * @return RETCODE_OK only if successful
 */
Retcode_T CellularNetwork_SetAPN(uint8_t *APN, uint32_t length); //-- AT+UPSD=0,1,"xxx"

/**
 * @brief set PDP Context configuration
 * @param[in] profile Packet switched data configuration profile
 * @return RETCODE_OK only if successful
 */
Retcode_T CellularNetwork_SetPsdProfile(Cellular_PsdProfile_T *profile);

/**
 * @brief Activates prepared PDP Context
 * @return RETCODE_OK only if successful
 */
Retcode_T CellularNetwork_ActivatePdpContext(void); //-- AT+UPSDA=0,3

/**
 * @brief deactivates prepared PDP Context
 * @return RETCODE_OK only if successful
 */
Retcode_T CellularNetwork_DeactivatePdpContext(void); //-- AT+UPSDA=0,4

/**
 * @brief verifies PDP Context
 * @return RETCODE_OK only if successful
 */
Retcode_T Cellular_VerifyPdpContext(bool *success);

/**
 * @brief Verifies PDP Context
 * @param[out] success flag
 * @return RETCODE_OK only if successful
 */
Retcode_T CellularNetwork_VerifyPdpContext(bool *success); //-- AT+UPSND=0,8

/**
 * @brief Sets modem RAT (Radio Access Technology)
 * @details `AT+URAT=` (select network radio access technology)
 * @note: PRECONDITION: detached from network
 *  Lisa-U200: Issue the `AT+COPS=2` AT command to detach the module from the network (Cellular_DeregisterFromNetwork)
 * @param[in] rat Radio Access Technology (select and prefer)
 * @return RETCODE_OK The network RAT selection was successful
 * @see Ref 2, p 88, ch 7.5 Radio Access Technology (RAT) selection +URAT
 */
Retcode_T CellularNetwork_SelectRadioAccessTechnology(CellularNetwork_RadioAccessTechnology_T *rat); //-- e.g. AT+URAT=1,2

/**
 * @brief Gets modem RAT (Radio Access Technology)
 * @param[out] rat Radio Access Technology (select and prefer)
 * @return RETCODE_OK The network RAT query was successful
 */
Retcode_T CellularNetwork_QueryRadioAccessTechnology(CellularNetwork_RadioAccessTechnology_T *rat); //-- AT+URAT?

/**
 * @brief Implements URC handling for CREG events.
 *        This function never blocks longer than required to handle the URC.
 *        If no CREG URC is available, this function returns immediately.
 *        CREG events must be enabled. By default they are not sent by network.
 *        CREG events might be enabled by AT+CREG=1 or AT+CREG=2, see CellularNetwork_ConfigureRegistration.
 *
 * @return RETCODE_OK URC handling was finished successfully.
 * @return RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularNetworkUrc_CREG(void);

/**
 * @brief Implements URC handling for UUPSDA events.
 *        It indicates status of Packet switch data action (AT+UPSDA).
 *        This function never blocks longer than required to handle the URC.
 *        If no UUPSDA URC is available, this function returns immediately.
 *
 * @return RETCODE_OK URC handling was finished successfully.
 * @return RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularNetworkUrc_UUPSDA(void);

/**
 * @brief Implements URC handling for UUPSDD events.
 *        Indicates that the data connection related to the provided PSD profile is deactivated.
 *        This function never blocks longer than required to handle the URC.
 *        If no UUPSDD URC is available, this function returns immediately.
 *
 * @return RETCODE_OK URC handling was finished successfully.
 * @return RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularNetworkUrc_UUPSDD(void);

/**
 * @brief   Initialize the modem to register to n/w.
 *
 * @return  This would return the status of modem initialize operation command
 *          Call back routine will be called to provide the n/w init status
 */
Retcode_T CellularNetwork_Init(void);

#endif /* UBLOX_NETWORK_H */
