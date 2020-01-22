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
 * @brief AT commands specific to u-blox.
 */
#ifndef AT_UBLOX_H_
#define AT_UBLOX_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#define UBLOX_MAX_DNS_DOMAIN_NAME_LENGTH (UINT32_C(253))
#define UBLOX_MAX_FILE_NAME_LENGTH (UINT32_C(248))

/**
 * @brief u-blox specific <ProfileId> parameter for the AT+HTTP command.
 */
enum AT_UHTTP_ProfileId_E
{
    AT_UHTTP_PROFILE_ID_0 = 0,         //!<    HTTP profile id 0
    AT_UHTTP_PROFILE_ID_1 = 1,         //!<    HTTP profile id 1
    AT_UHTTP_PROFILE_ID_2 = 2,         //!<    HTTP profile id 2
    AT_UHTTP_PROFILE_ID_3 = 3,         //!<    HTTP profile id 3
    AT_UHTTP_PROFILE_ID_INVALID = 255, //!<invalid value
};
typedef enum AT_UHTTP_ProfileId_E AT_UHTTP_ProfileId_T;

/**
 * @brief u-blox specific <Opcode> parameter for the AT+HTTP command.
 */
enum AT_UHTTP_Opcode_E
{
    AT_UHTTP_OPCODE_SERVER_IP = 0,     //!<    HTTP server IP address
    AT_UHTTP_OPCODE_SERVER_NAME = 1,   //!<    HTTP server name
    AT_UHTTP_OPCODE_USERNAME = 2,      //!<    username
    AT_UHTTP_OPCODE_PASSWORD = 3,      //!<    password
    AT_UHTTP_OPCODE_AUTH_TYPE = 4,     //!<    authentication type
    AT_UHTTP_OPCODE_SERVER_PORT = 5,   //!<    HTTP server port
    AT_UHTTP_OPCODE_SECURE_OPTION = 6, //!<    HTTP Secure option (SSL encryption)
    AT_UHTTP_OPCODE_RESERVED0 = 7,     //!<    reserved for internal use only
    AT_UHTTP_OPCODE_RESERVED1 = 8,     //!<    reserved for internal use only
    AT_UHTTP_OPCODE_CUSTOM_HEADER = 9, //!<    HTTP add custom request headers
    AT_UHTTP_OPCODE_INVALID = 255,     //!<invalid value
};
typedef enum AT_UHTTP_Opcode_E AT_UHTTP_Opcode_T;

/**
 * @brief u-blox specific UHTTP parameters
 */
struct AT_UHTTP_Param_S
{
    /**
     * @brief Indicates http profile to use
     */
    AT_UHTTP_ProfileId_T ProfileId;

    /**
     * @brief Indicates Opcode (config item) to set
     */
    AT_UHTTP_Opcode_T Opcode;

    /**
     * @brief Value to be set. Either c-string or numeric value depending on
     * Opcode.
     */
    union {
        const char *String;
        uint32_t Numeric;
    } Value;
};
typedef struct AT_UHTTP_Param_S AT_UHTTP_Param_T;

/**
 * @brief u-blox specific <Command> parameter for the AT+HTTPC command.
 */
enum AT_UHTTPC_Command_E
{
    AT_UHTTPC_COMMAND_HEAD = 0,       //!<    HTTP HEAD
    AT_UHTTPC_COMMAND_GET = 1,        //!<    HTTP GET
    AT_UHTTPC_COMMAND_DELETE = 2,     //!<    HTTP DELETE
    AT_UHTTPC_COMMAND_PUT = 3,        //!<    HTTP PUT
    AT_UHTTPC_COMMAND_POST_FILE = 4,  //!<    HTTP POST FILE
    AT_UHTTPC_COMMAND_POST_DATA = 5,  //!<    HTTP POST DATA
    AT_UHTTPC_COMMAND_GET_FOTA = 100, //!<    HTTP GET FOTA (ublox proprietary)
    AT_UHTTPC_COMMAND_INVALID = 255,  //!<invalid value
};
typedef enum AT_UHTTPC_Command_E AT_UHTTPC_Command_T;

/**
 * @brief u-blox specific <Command> parameter for the AT+HTTPC command.
 */
enum AT_UHTTPC_Result_E
{
    AT_UHTTPC_RESULT_FAILURE = 0, //!<    Failure
    AT_UHTTPC_RESULT_SUCCESS = 1  //!<    Success
};
typedef enum AT_UHTTPC_Result_E AT_UHTTPC_Result_T;

/**
 * @brief u-blox specific <content> parameter for the AT+HTTPC command.
 */
enum AT_UHTTPC_Content_E
{
    AT_UHTTPC_CONTENT_APP_X_WWW_URL = 0, //!<    application/x-www-form-urlencoded
    AT_UHTTPC_CONTENT_TEXT_PLAIN = 1,    //!<    text/plain
    AT_UHTTPC_CONTENT_APP_OCTET = 2,     //!<    application/octet-stream
    AT_UHTTPC_CONTENT_MULTIPART = 3,     //!<    multipart/form-data
    AT_UHTTPC_CONTENT_APP_JSON = 4,      //!<    application/json
    AT_UHTTPC_CONTENT_APP_XML = 5,       //!<    application/xml
    AT_UHTTPC_CONTENT_USER_DEFINED = 6,  //!<    user defined
    AT_UHTTPC_CONTENT_INVALID = 255,     //!<invalid value
};
typedef enum AT_UHTTPC_Content_E AT_UHTTPC_Content_T;

/**
 * @brief u-blox specific UHTTPC parameters
 */
struct AT_UHTTPC_Param_S
{
    /**
     * @brief Sets which HTTP profile should use.
     */
    AT_UHTTP_ProfileId_T ProfileId;

    /**
     * @brief Selects the HTTP request type.
     */
    AT_UHTTPC_Command_T Command;

    /**
     * @brief Path of the resource on the HTTP server. Path is relative to the
     * server root.
     *
     * E.g. "/index.html" points to http://<server-ip>:<server-port>/index.html
     */
    const char *PathOnServer;

    /**
     * @brief Path to the file which shall contain the HTTP response received
     * received from the HTTP server. Path is relative to the modems internal
     * flash.
     */
    const char *ResponseFilename;

    /**
     * @brief In case of PUT and POST (file-mode) points to a file on the modem
     * flash. The file must exists before invoking this command. If POST
     * (data-mode) is selected, points to a string that shall be out as the
     * request payload. Care must be taken that the payload does not break the
     * AT command structure.
     */
    const char *Payload;

    /**
     * @brief HTTP content-type associated with the transferred payload. Only
     * applicable for request type POST or PUT.
     */
    AT_UHTTPC_Content_T ContentType;

    /**
     * @brief Custom HTTP content-type outside the range known types. Only
     * applicable if ContentType is set to AT_UHTTPC_CONTENT_USER_DEFINED.
     *
     * @note: Currently not supported.
     */
    const char *UserDefinedContentType;
};
typedef struct AT_UHTTPC_Param_S AT_UHTTPC_Param_T;

/**
 * @brief u-blox specific <SelectedAct> parameter for the AT+URAT command.
 */
enum AT_URAT_SelectedAcT_E
{
    AT_URAT_SELECTEDACT_GSM_GPRS_EGPRS = 0, //!<    GSM/GPRS/eGPRS (single mode)
    AT_URAT_SELECTEDACT_GSM_UMTS = 1,       //!<          GSM/UMTS (dual mode)
    AT_URAT_SELECTEDACT_UMTS = 2,           //!<              UMTS (single mode)
    AT_URAT_SELECTEDACT_LTE = 3,            //!<               LTE (single mode)
    AT_URAT_SELECTEDACT_GSM_UMTS_LTE = 4,   //!<      GSM/UMTS/LTE (tri mode)
    AT_URAT_SELECTEDACT_GSM_LTE = 5,        //!<           GSM/LTE (dual mode)
    AT_URAT_SELECTEDACT_UMTS_LTE = 6,       //!<          UMTS/LTE (dual mode)
    AT_URAT_SELECTEDACT_LTEM1 = 7,          //!<             LTE Cat.M1
    AT_URAT_SELECTEDACT_LTENB1 = 8,         //!<            LTE Cat.NB1
    AT_URAT_SELECTEDACT_GPRS_EGPRS = 9,     //!<        GPRS/eGPRS
    AT_URAT_SELECTEDACT_INVALID = 255,      //!<         Invalid <SelectedAct> value, used to signal that parameter not present
};
typedef enum AT_URAT_SelectedAcT_E AT_URAT_SelectedAcT_T;

/**
 * @brief u-blox specific <PreferredAcT> parameter for the AT+URAT command.
 */
enum AT_URAT_PreferredAcT_E
{
    AT_URAT_PREFERREDACT_GSM_GPRS_EGPRS = 0, //!<   GSM/GPRS/eGPRS
    /* no #1 */
    AT_URAT_PREFERREDACT_UTRAN = 2, //!<            UTRAN
    AT_URAT_PREFERREDACT_LTE = 3,   //!<              LTE
    /* no #4, #5, #6*/
    AT_URAT_PREFERREDACT_LTEM1 = 7,      //!<            LTE Cat.M1
    AT_URAT_PREFERREDACT_LTENB1 = 8,     //!<           LTE Cat.NB1
    AT_URAT_PREFERREDACT_GPRS_EGPRS = 9, //!<       GPRS/eGPRS
    AT_URAT_PREFERREDACT_INVALID = 255,  //!<        Invalid <PreferredAct> value, used to signal that parameter not present
};
typedef enum AT_URAT_PreferredAcT_E AT_URAT_PreferredAcT_T;

/**
 * @brief u-blox specific <2ndPreferredAcT> parameter for the AT+URAT command.
 */
enum AT_URAT_SecondPreferredAcT_E
{
    AT_URAT_SECONDPREFERREDACT_GSM_GPRS_EGPRS = 0, //!< GSM/GPRS/eGPRS
    /* no #1 */
    AT_URAT_SECONDPREFERREDACT_UTRAN = 2, //!<          UTRAN
    AT_URAT_SECONDPREFERREDACT_LTE = 3,   //!<            LTE
    /* no #4, #5, #6*/
    AT_URAT_SECONDPREFERREDACT_LTEM1 = 7,      //!<          LTE Cat.M1
    AT_URAT_SECONDPREFERREDACT_LTENB1 = 8,     //!<         LTE Cat.NB1
    AT_URAT_SECONDPREFERREDACT_GPRS_EGPRS = 9, //!<     GPRS/eGPRS
    AT_URAT_SECONDPREFERREDACT_INVALID = 255,  //!<      Invalid <2ndPreferredAct> value, used to signal that parameter not present
};
typedef enum AT_URAT_SecondPreferredAcT_E AT_URAT_SecondPreferredAcT_T;

/**
 * @brief u-blox specific URAT parameters
 */
struct AT_URAT_Param_S
{
    /**
     * @brief Indicates the RAT.
     */
    AT_URAT_SelectedAcT_T SelectedAcT;

    /**
     * @brief Indicates the preferred RAT. Ignored if no dual or tri mode RAT
     * is selected.
     */
    AT_URAT_PreferredAcT_T PreferredAcT;

    /**
     * @brief Indicates the 2nd preferred RAT. Ignored if no tri mode RAT is
     * selected.
     */
    AT_URAT_SecondPreferredAcT_T SecondPreferredAcT;
};
typedef struct AT_URAT_Param_S AT_URAT_Param_T;

/**
 * @brief u-blox specific <MNO> parameter settings for the AT+UMNOPROF command.
 */
enum AT_UMNOPROF_Mno_E
{
    AT_UMNOPROF_MNO_DEFAULT = 0,     //!<   SW default (MNO not set, may hinder registration)
    AT_UMNOPROF_MNO_SIM = 1,         //!<   Select SIM ICCID
    AT_UMNOPROF_MNO_ATT = 2,         //!<   Select AT&T
    AT_UMNOPROF_MNO_VERIZON = 3,     //!<   Select Verizon
    AT_UMNOPROF_MNO_TELSTRA = 4,     //!<   Select Telstra
    AT_UMNOPROF_MNO_TMO = 5,         //!<   Select T-Mobile (US)
    AT_UMNOPROF_MNO_CT = 6,          //!<   Select China Telecom
    AT_UMNOPROF_MNO_SPRINT = 8,      //!<   Select Sprint
    AT_UMNOPROF_MNO_VODSFONE = 19,   //!<   Select Vodafone
    AT_UMNOPROF_MNO_NTTDOCOMO = 20,  //!<   Select NTT DoCoMo
    AT_UMNOPROF_MNO_TELUS = 21,      //!<   Select Telus
    AT_UMNOPROF_MNO_SOFTBANK = 28,   //!<   Select SoftBank
    AT_UMNOPROF_MNO_TELEKOM = 31,    //!<   Select Deutsche Telekom
    AT_UMNOPROF_MNO_USCELLULAR = 32, //!<   Select US Cellular
    AT_UMNOPROF_MNO_SKT = 39,        //!<   Select <SKT
    AT_UMNOPROF_MNO_EUROPE = 100,    //!<   Select Standard Europe
    AT_UMNOPROF_MNO_INVALID = 255    //!<   Invalid <MNO> value, used to signal that parameter not present
};
typedef enum AT_UMNOPROF_Mno_E AT_UMNOPROF_Mno_T;

/**
 * @brief u-blox specific <MNO> parameter settings for the AT+UMNOPROF command.
 */
enum AT_ULSTFILE_Opcode_E
{
    AT_ULSTFILE_OPCODE_LIST = 0,     //!< list files for a given tag
    AT_ULSTFILE_OPCODE_FREE = 1,     //!< get free space for a given tag
    AT_ULSTFILE_OPCODE_SIZE = 2,     //!< get size of a given tag
    AT_ULSTFILE_OPCODE_INVALID = 255 //!< Invalid <op_code> value, used to signal that parameter not present
};
typedef enum AT_ULSTFILE_Opcode_E AT_ULSTFILE_Opcode_T;

struct AT_ULSTFILE_Param_S
{
    AT_ULSTFILE_Opcode_T Opcode;
    const char *Filename;
    uint32_t Filesize;
};
typedef struct AT_ULSTFILE_Param_S AT_ULSTFILE_Param_T;

struct AT_URDBLOCK_Param_S
{
    const char *Filename;
    uint32_t Offset;
    uint32_t Size;
};
typedef struct AT_URDBLOCK_Param_S AT_URDBLOCK_Param_T;

struct AT_URDBLOCK_Resp_S
{
    char Filename[UBLOX_MAX_FILE_NAME_LENGTH + 1];
    uint32_t Size;
    uint8_t *Data;
};
typedef struct AT_URDBLOCK_Resp_S AT_URDBLOCK_Resp_T;

struct AT_URDFILE_Param_S
{
    const char *Filename;
    uint8_t *Data;
    uint32_t DataSize;
};
typedef struct AT_URDFILE_Param_S AT_URDFILE_Param_T;

struct AT_UDWNFILE_Param_S
{
    const char *Filename;
    const uint8_t *Data;
    uint32_t DataSize;
};
typedef struct AT_UDWNFILE_Param_S AT_UDWNFILE_Param_T;

struct AT_UDELFILE_Param_S
{
    const char *Filename;
};
typedef struct AT_UDELFILE_Param_S AT_UDELFILE_Param_T;

/**
 * @brief u-blox specific <protocol> parameter for the AT+USOCR command.
 */
enum AT_USOCR_Protocol_E
{
    AT_USOCR_PROTOCOL_TCP = 6,      //!<         TCP protocol
    AT_USOCR_PROTOCOL_UDP = 17,     //!<        UDP protocol
    AT_USOCR_PROTOCOL_INVALID = 255 //!<    Invalid <protocol> value, used to signal that parameter not present
};
typedef enum AT_USOCR_Protocol_E AT_USOCR_Protocol_T;

/**
 * @brief u-blox specific parameters for the AT+USOCR command.
 */
struct AT_USOCR_Param_S
{
    /**
     * @brief Protocol to associate with the socket.
     */
    AT_USOCR_Protocol_T Protocol;

    /**
     * @brief Local port to be used. If set to 0 a random port will be chosen.
     */
    uint16_t LocalPort;
};
typedef struct AT_USOCR_Param_S AT_USOCR_Param_T;

enum AT_UBlox_AddressType_E
{
    AT_UBLOX_ADDRESSTYPE_IPV4,
    AT_UBLOX_ADDRESSTYPE_IPV6,
    AT_UBLOX_ADDRESSTYPE_INVALID
};
typedef enum AT_UBlox_AddressType_E AT_UBlox_AddressType_T;

struct AT_UBlox_Address_S
{
    AT_UBlox_AddressType_T Type;
    union {
        uint8_t IPv4[4];
        uint16_t IPv6[8];
    } Address;
};
typedef struct AT_UBlox_Address_S AT_UBlox_Address_T;

/**
 * @brief u-blox specific parameters for the AT+USOCO command.
 */
struct AT_USOCO_Param_S
{
    /**
     * @brief Socket-Id of the socket that shall be used to initiate the
     * connection.
     */
    uint32_t Socket;

    /**
     * @brief IP-Address of the remote host.
     */
    AT_UBlox_Address_T RemoteAddr;

    /**
     * @brief Port of the remote host.
     */
    uint16_t RemotePort;
};
typedef struct AT_USOCO_Param_S AT_USOCO_Param_T;

/**
 * @brief Payload encoding schemes used for encoding raw payload bytes over the
 * AT interface.
 */
enum AT_UBlox_PayloadEncoding_E
{
    AT_UBLOX_PAYLOADENCODING_BASE,   //!<     Normal ASCII strings only. Some characters are forbidden.
    AT_UBLOX_PAYLOADENCODING_HEX,    //!<      Encode the payload in ASCII HEX. So { 0x12, 0x34, 0x56 } becomes the C-string "123456".
    AT_UBLOX_PAYLOADENCODING_BINARY, //!<   Send the payload in raw binary using special out-of-AT mode.
    AT_UBLOX_PAYLOADENCODING_INVALID //!<   Invalid payload encoding scheme.
};
typedef enum AT_UBlox_PayloadEncoding_E AT_UBlox_PayloadEncoding_T;

/**
 * @brief u-blox specific parameters for the AT+USOWR command.
 */
struct AT_USOWR_Param_S
{
    /**
     * @brief Socket-Id on which to send data on.
     */
    uint32_t Socket;

    /**
     * @brief Declares in what format Data has to be encoded before sending it
     * to the modem.
     *
     * @note The encoding is done internally. Please provide the raw bytes to
     * send in the Data field.
     */
    AT_UBlox_PayloadEncoding_T Encoding;

    /**
     * @brief Length of data to sent in bytes.
     */
    uint32_t Length;

    /**
     * @brief Raw data payload to sent (no encoding).
     *
     * In case #AT_UBLOX_PAYLOADENCODING_BASE is used, special care needs to be
     * taken by the caller that the Data buffer does not contain any illegal
     * characters.
     */
    const uint8_t *Data;
};
typedef struct AT_USOWR_Param_S AT_USOWR_Param_T;

/**
 * @brief u-blox specific response for the AT+USOWR command.
 */
struct AT_USOWR_Resp_S
{
    /**
     * @brief Socket-Id on which the data was sent on.
     */
    uint32_t Socket;

    /**
     * @brief Number of bytes sent.
     */
    uint32_t Length;
};
typedef struct AT_USOWR_Resp_S AT_USOWR_Resp_T;

/**
 * @brief u-blox specific parameters for the AT+USOST command.
 */
struct AT_USOST_Param_S
{
    /**
     * @brief Socket-Id on which to send data on.
     */
    uint32_t Socket;

    /**
     * @brief IP of the remote host to send to.
     */
    AT_UBlox_Address_T RemoteIp;

    /**
     * @brief Port of the remote host to send to.
     */
    uint16_t RemotePort;

    /**
     * @brief Declares in what format Data has to be encoded before sending it
     * to the modem.
     *
     * @note The encoding is done internally. Please provide the raw bytes to
     * send in the Data field.
     */
    AT_UBlox_PayloadEncoding_T Encoding;

    /**
     * @brief Length of data to sent in bytes.
     */
    uint32_t Length;

    /**
     * @brief Raw data payload to sent (no encoding).
     *
     * In case #AT_UBLOX_PAYLOADENCODING_BASE is used, special care needs to be
     * taken by the caller that the Data buffer does not contain any illegal
     * characters.
     */
    const uint8_t *Data;
};
typedef struct AT_USOST_Param_S AT_USOST_Param_T;

/**
 * @brief u-blox specific response for the AT+USOST command.
 */
struct AT_USOST_Resp_S
{
    /**
     * @brief Socket-Id on which the data was sent on.
     */
    uint32_t Socket;

    /**
     * @brief Number of bytes sent.
     */
    uint32_t Length;
};
typedef struct AT_USOST_Resp_S AT_USOST_Resp_T;

/**
 * @brief u-blox specific parameters for the AT+USORD command.
 */
struct AT_USORD_Param_S
{
    /**
     * @brief Socket-Id to read from.
     */
    uint32_t Socket;

    /**
     * @brief Number of bytes to read (after decoding). Can be zero, in which
     * case the command will return the amount of bytes buffered on the modem.
     */
    uint32_t Length;

    /**
     * @brief Declares in what format the response data will be encoded in.
     *
     * @note The decoding is done internally. In HEX mode please provide a Data
     * buffer that's at least twice the size of Length!
     */
    AT_UBlox_PayloadEncoding_T Encoding;
};
typedef struct AT_USORD_Param_S AT_USORD_Param_T;

/**
 * @brief u-blox specific response for the AT+USORD command.
 */
struct AT_USORD_Resp_S
{
    /**
     * @brief Socket-Id from which data was read.
     */
    uint32_t Socket;

    /**
     * @brief Number of bytes that were actually read. Might be less than
     * requested.
     */
    uint32_t Length;

    /**
     * @brief Will hold the data that was read. May be NULL if requested length
     * is 0.
     *
     * @note The caller has to provide a buffer to write into! In HEX-mode, this
     * buffer must be at least twice the length given in the
     * #AT_USORD_Param_T::Length field.
     */
    uint8_t *Data;
};
typedef struct AT_USORD_Resp_S AT_USORD_Resp_T;

/**
 * @brief u-blox specific parameters for the AT+USORF command.
 */
struct AT_USORF_Param_S
{
    /**
     * @brief Socket-Id to read from.
     */
    uint32_t Socket;

    /**
     * @brief Number of bytes to read (after decoding). Can be zero, in which
     * case the command will return the amount of bytes buffered on the modem.
     */
    uint32_t Length;

    /**
     * @brief Declares in what format the response data will be encoded in.
     *
     * @note The decoding is done internally. In HEX mode please provide a Data
     * buffer that's at least twice the size of Length!
     */
    AT_UBlox_PayloadEncoding_T Encoding;
};
typedef struct AT_USORF_Param_S AT_USORF_Param_T;

/**
 * @brief u-blox specific response for the AT+USORF command.
 */
struct AT_USORF_Resp_S
{
    /**
     * @brief Socket-Id from which data was read.
     */
    uint32_t Socket;

    /**
     * @brief Remote IP where the packet came from.
     */
    AT_UBlox_Address_T RemoteIp;

    /**
     * @brief Remote port where the packet came from.
     */
    uint16_t RemotePort;

    /**
     * @brief Number of bytes that were actually read. Might be less than
     * requested.
     */
    uint32_t Length;

    /**
     * @brief Will hold the data that was read. May be NULL if requested length
     * is 0.
     *
     * @note The caller has to provide a buffer to write into! In HEX-mode, this
     * buffer must be at least twice the length given in the
     * #AT_USORF_Param_T::Length field.
     */
    uint8_t *Data;
};
typedef struct AT_USORF_Resp_S AT_USORF_Resp_T;

/**
 * @brief u-blox specific response for the AT+USOLI command.
 */
struct AT_USOLI_Param_S
{
    /**
     * @brief Socket-Id of the socket to use for listening.
     */
    uint32_t Socket;

    /**
     * @brief Local port to listen on.
     */
    uint16_t Port;
};
typedef struct AT_USOLI_Param_S AT_USOLI_Param_T;

/**
 * @brief u-blox specific parameters for the AT+USOCL command.
 */
struct AT_USOCL_Param_S
{
    /**
     * @brief Socket-Id of the socket to close.
     */
    uint32_t Socket;
};
typedef struct AT_USOCL_Param_S AT_USOCL_Param_T;

/**
 * @brief u-blox configuration options for AT+UDCONF.
 */
enum AT_UDCONF_Config_E
{
    AT_UDCONF_CONFIG_HEXMODE = 1,   //!<   Enable/disable HEX-mode for +USOWR, +USOST, +USORD, +USORF.
    AT_UDCONF_CONFIG_INVALID = 255, //!< Invalid <config> parameter.
};
typedef enum AT_UDCONF_Config_E AT_UDCONF_Config_T;

/**
 * @brief u-blox specific parameters for the AT+UDCONF command.
 */
struct AT_UDCONF_Param_S
{
    /**
     * @brief Config option to set.
     */
    AT_UDCONF_Config_T Config;

    /**
     * @brief Value to set. Type may differ depending on what Config value is
     * chosen.
     *
     * @note When reading a value from the modem, this parameter must point to
     * a valid data structure to hold the result.
     */
    void *Value;
};
typedef struct AT_UDCONF_Param_S AT_UDCONF_Param_T;

/**
 * @brief u-blox specific ICCID length type declaration.
 */
enum AT_CCID_Type_E
{
    AT_CCID_TYPE_19CHAR = 19,  //!<  Used for ICCIDs with length of 19 chars
    AT_CCID_TYPE_20CHAR = 20,  //!<  Used for ICCIDs with length of 20 chars
    AT_CCID_TYPE_INVALID = 255 //!< Invalid ICCID type.
};
typedef enum AT_CCID_Type_E AT_CCID_Type_T;

/**
 * @brief u-blox specific response to the AT+ICCID command.
 */
struct AT_CCID_Resp_S
{
    AT_CCID_Type_T Type;
    char Iccid[AT_CCID_TYPE_20CHAR + 1]; /* add zero-termination for safety */
};
typedef struct AT_CCID_Resp_S AT_CCID_Resp_T;

/**
 * @brief u-blox specific <resolution_type> parameter for the AT+UDNSRN command.
 */
enum AT_UDNSRN_ResolutionType_E
{
    AT_UDNSRN_RESOLUTIONTYPE_DOMAINTOIP = 0, //!<   Domain to IP address
    AT_UDNSRN_RESOLUTIONTYPE_IPTODOMAIN = 1, //!<   IP address to domain name (host by name)
    AT_UDNSRN_RESOLUTIONTYPE_INVALID = 255   //!<     Invalid <resolution_type> parameter.
};
typedef enum AT_UDNSRN_ResolutionType_E AT_UDNSRN_ResolutionType_T;

/**
 * @brief u-blox specifc parameters for the AT+UDNSRN command.
 */
struct AT_UDNSRN_Param_S
{
    AT_UDNSRN_ResolutionType_T ResolutionType;
    union {
        const char *Domain;
        const AT_UBlox_Address_T *Ip;
    } DomainIpString;
};
typedef struct AT_UDNSRN_Param_S AT_UDNSRN_Param_T;

/**
 * @brief u-blox specific response to the AT+UDNSRN command.
 */
struct AT_UDNSRN_Resp_S
{
    union {
        char Domain[UBLOX_MAX_DNS_DOMAIN_NAME_LENGTH + 1]; /* zero-terminated */
        AT_UBlox_Address_T Ip;
    } DomainIpString;
};
typedef struct AT_UDNSRN_Resp_S AT_UDNSRN_Resp_T;

/**
 * @brief u-blox specifc <RAT> parameter for the AT+UBANDMASK command.
 */
enum AT_UBANDMASK_Rat_E
{
    AT_UBANDMASK_RAT_LTECATM1 = 0,  //!< Control the Radio Access Technology LTE Cat M1
    AT_UBANDMASK_RAT_LTECATNB1 = 1, //!< Control the Radio Access Technology LTE Cat NB1
    AT_UBANDMASK_RAT_INVALID = 255  //!< Invalid <RAT> value.
};
typedef enum AT_UBANDMASK_Rat_E AT_UBANDMASK_Rat_T;

/**
 * @brief u-blox specific parameters for the AT+UBANDMASK command.
 */
struct AT_UBANDMASK_Param_S
{
    AT_UBANDMASK_Rat_T Rat;
    /**
     * @brief Bits control bands 1 to 64.
     * Bit 0: band 1
     * Bit 1: band 2
     * Bit 2: band 3
     * ...
     */
    uint64_t Bitmask1;
    /**
     * @brief Bits control bands 65 to 128.
     * Bit 0: band 65
     * Bit 1: band 66
     * Bit 2: band 67
     * ...
     */
    uint64_t Bitmask2;
};
typedef struct AT_UBANDMASK_Param_S AT_UBANDMASK_Param_T;

/* *** RADIO CONFIGURATION ************************************************** */

Retcode_T At_Set_UHTTP(const AT_UHTTP_Param_T *param);

Retcode_T At_Set_UHTTPC(const AT_UHTTPC_Param_T *param);

Retcode_T At_Set_UDWNFILE(const AT_UDWNFILE_Param_T *param);

Retcode_T At_Set_UDELFILE(const AT_UDELFILE_Param_T *param);

Retcode_T At_Set_ULSTFILE(AT_ULSTFILE_Param_T *param);

/**
 * @brief Read a block of data from a file on the modem internal flash.
 *
 * @param[in] param
 * Block-read parameters to be used.
 *
 * @param[in,out] resp
 * Will contain read response. Please provide a valid Data buffer within the
 * response structure into which data can be written. The length of that buffer
 * has to match the length requested in the #AT_URDBLOCK_Param_T structure.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_URDBLOCK(const AT_URDBLOCK_Param_T *param, AT_URDBLOCK_Resp_T *resp);

Retcode_T At_Set_URDFILE(AT_URDFILE_Param_T *param);

/**
 * @brief Set the forced selection of which RATs to use in the modem
 * protocol-stack. These settings should not be changed while the modem is
 * registered to a network and may only apply on subsequent registrations.
 *
 * @param[in] param
 * A valid structure pointer containing the RAT settings to apply.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_URAT(const AT_URAT_Param_T *param);

/**
 * @brief Set the MNO (Mobile Network Operator) profile. This will automatically
 * apply various radio settings to be compliant with the selected network
 * operator.
 *
 * @note It may be required to set a correct MNO profile before trying to
 * establish a network registration.
 *
 * @param[in] mno
 * MNO profile to apply.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_UMNOPROF(AT_UMNOPROF_Mno_T mno);

/**
 * @brief Get the MNO (Mobile Network Operator) profile currently configured on
 * the modem.
 *
 * @param[out] mno
 * Will contain the current MNO setting after return.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_UMNOPROF(AT_UMNOPROF_Mno_T *mno);

/**
 * @brief Set the 4G LTE bands for different RATs. Each bit in the bitmask
 * represents a 4G LTE band.
 *
 * @param[in] param
 * RAT and bitmask to set.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_UBANDMASK(const AT_UBANDMASK_Param_T *param);

/* *** SOCKET SERVICE ******************************************************* */

/**
 * @brief Create a socket using the u-blox internal TCP/IP stack.
 *
 * @param[in] param
 * Socket parameters to be use for creating the socket.
 *
 * @param[out] socketId
 * Will contain the socket id of the created socket.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USOCR(const AT_USOCR_Param_T *param, uint32_t *socketId);

/**
 * @brief Connect a socket using the u-blox internal TCP/IP stack.
 *
 * @param[in] param
 * Connection parameters to be used.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USOCO(const AT_USOCO_Param_T *param);

/**
 * @brief Send data over an already connected socket. This command also works
 * for UDP sockets provided that AT+USOCO has been called before.
 *
 * @param[in] param
 * Send parameters to be used.
 *
 * @param[out] resp
 * Will contain information about the send result.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USOWR(const AT_USOWR_Param_T *param, AT_USOWR_Resp_T *resp);

/**
 * @brief Send data over a UDP socket. This command only works
 * for UDP sockets, no need to call AT+USOCO before.
 *
 * @param[in] param
 * Send parameters to be used.
 *
 * @param[out] resp
 * Will contain information about the send result.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USOST(const AT_USOST_Param_T *param, AT_USOST_Resp_T *resp);

/**
 * @brief Read data from an already connected socket. This command also works
 * for UDP sockets provided that AT+USOCO has been called before.
 *
 * @note Regarding UDP, note that the response does not contain any information
 * about where the data came from. U-blox therefore recommends AT+USORF for UDP
 * sockets.
 *
 * @param[in] param
 * Read parameters to be used.
 *
 * @param[in,out] resp
 * Will contain read response. Please provide a valid Data buffer within the
 * response structure into which data can be written. The length of that buffer
 * has to match the length requested in the #AT_USORD_Param_T structure.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USORD(const AT_USORD_Param_T *param, AT_USORD_Resp_T *resp);

/**
 * @brief Read data from a UDP socket. Similar to #At_Set_USORD but including
 * information about the remote host.
 *
 * @param[in] param
 * Read parameters to be used.
 *
 * @param[in,out] resp
 * Will contain read response. Please provide a valid Data buffer within the
 * response structure into which data can be written. The length of that buffer
 * has to match the length requested in the #AT_USORF_Param_T structure.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USORF(const AT_USORF_Param_T *param, AT_USORF_Resp_T *resp);

/**
 * @brief Set an already created socket in listening mode and wait for incoming
 * traffic on a specified port.
 *
 * For TCP sockets, incoming connections will be automatically accepted and
 * notified via the +UUSOLI URC.
 * For UDP sockets, incoming data will be notified via +UUSORF URC.
 *
 * @param[in] param
 * Parameters to perform the socket listen.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USOLI(const AT_USOLI_Param_T *param);

/**
 * @brief Close an open socket inside the u-blox internal TCP/IP stack..
 *
 * @param[in] param
 * Socket parameters to perform the socket close.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_USOCL(const AT_USOCL_Param_T *param);

/**
 * @brief Set general purpose config parameters.
 *
 * @param[in] param
 * Config parameters to apply.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_UDCONF(const AT_UDCONF_Param_T *param);

/**
 * @brief Get general purpose config parameters.
 *
 * @param[in,out] param
 * Config parameters to apply.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_UDCONF(AT_UDCONF_Param_T *param);

/* *** SIM SERVICE ********************************************************** */

/**
 * @brief Read out the ICCID (Integrated Circuit Card ID) of the SIM-card.
 *
 * The SIM card needs to work correctly for this command to succed.
 *
 * @param[out] resp
 * Will be filled with the result on completion.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Get_CCID(AT_CCID_Resp_T *resp);

/* *** DNS SERVICE ********************************************************** */

/**
 * @brief Translates a domain name to and IP address or and IP address to a
 * domain name using the network provided primary or secondary DNS.
 *
 * @param[in] param
 * Parameters to perform the domain name lookup.
 *
 * @param[out] resp
 * The response to the domain name lookup.
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_UDNSRN(const AT_UDNSRN_Param_T *param, AT_UDNSRN_Resp_T *resp);

/**
 * @brief Configures the test mode of the ublox module
 *
 * @param[in] param device specific test parameter
 *
 * @return A #Retcode_T indicating the result of the requested action.
 */
Retcode_T At_Set_UTEST(uint32_t param);

/* *** URC HANDLERS ********************************************************* */

Retcode_T At_HandleUrc_UUSORD(void);

Retcode_T At_HandleUrc_UUSORF(void);

Retcode_T At_HandleUrc_UUSOLI(void);

Retcode_T At_HandleUrc_UUSOCL(void);

Retcode_T At_HandleUrc_UUHTTPCR(void);

#endif /* AT_UBLOX_H_ */
