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
 * @defgroup ATRESPONSEPARSER AT Response Parser
 * @{
 * @brief Defines the interface for the AT Response Parser.
 *
 * @details This parser consumes the following grammar:
 *
 * * MISC_CONTENT = ^[\n] ;
 * * CMD_NAME     = ^[:]+
 * * ARG_VALUE    = ^[,\n]+ ;
 * * ARG_EOL      = [,\n] ;
 * * WS           = [\n ] ;
 * \n
 * * start := CmdEcho | cmd | ResponseCode | MISC_CONTENT ;
 * * ResponseCode := 'OK' | 'CONNECT' | 'RING' | 'NO CARRIER' | 'ERROR' | 'NO DIALTONE' | 'BUSY' | 'NO ANSWER' | 'ABORTED' ;
 * * CmdEcho := "AT" cmd ;
 * * cmd := '+' CMD_NAME ((':' WS* arg*) | '\n') ;
 * * arg := '\"'? ARG_VALUE '\"'? ARG_EOL ;
 *
 * @file
 */

#ifndef AT_RESPONSE_PARSER_H_
#define AT_RESPONSE_PARSER_H_

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Cellular.h"

/**
 * @brief Should be greater than 1024 bytes to ensure that a full network packet read
 * does not overflow the AT response buffer (which would result in an error state).
 */
#define ATRP_INTERNAL_BUFFER_LEN (UINT32_C(1024))

/**
 * @brief The default S4 character of UBlox modules is Line Feed (LF) = 10
 */
#define AT_DEFAULT_S4_CHARACTER ('\n')

/**
 * @brief The default S3 character of UBlox modules is Carriage Return (CR) = 13
 */
#define AT_DEFAULT_S3_CHARACTER ('\r')

/**
 * @brief The fluke character bounds
 */
#define AT_SPACE_CHARACTER (' ')
#define AT_TILDE_CHARACTER ('~')

/**
 * @brief Enumeration representing the result of an AT response parser call.
 */
typedef enum
{
    AT_RESPONSE_PARSER_NOT_INITIALIZED = RETCODE_CELLULAR_LAST_CUSTOM_CODE,
    AT_RESPONSE_PARSER_INPUT_TOO_SHORT,
    AT_RESPONSE_PARSER_PARSE_ERROR,

    AT_RESPONSE_PARSER_LAST_CUSTOM_CODE
} AtResponseParserRetcode_T;

/**
 * @brief Enumeration of the potential AT response codes as specified in section 1.1.4
 * of the uBlox AT commands manual.
 */
typedef enum
{
    AT_RESPONSE_CODE_OK = 0,
    AT_RESPONSE_CODE_CONNECT = 1,
    AT_RESPONSE_CODE_RING = 2,
    AT_RESPONSE_CODE_NO_CARRIER = 3,
    AT_RESPONSE_CODE_ERROR = 4,
    AT_RESPONSE_CODE_NO_DIALTONE = 6,
    AT_RESPONSE_CODE_BUSY = 7,
    AT_RESPONSE_CODE_NO_ANSWER = 8,
    AT_RESPONSE_CODE_ABORTED = 18
} AtResponseCode_T;

/**
 * @brief Event callback for events without char content
 */
typedef void (*AtrpEventCallback_T)(void);

/**
 * @brief Event callback for events with char content
 */
typedef void (*AtrpEventWithDataCallback_T)(const uint8_t *cmd, uint32_t len);

/**
 * @brief Event callback for events without char content
 */
typedef void (*AtrpEventWithResponseCodeCallback_T)(AtResponseCode_T code);

/**
 * @brief the parser internal state callback type
 *
 * @return The number of consumed characters, or -1 if there was a parser error
 */
typedef int32_t (*AtrpStateCallback_T)(const uint8_t *buffer, uint32_t len);

/**
 * @brief The AT response parser state struct
 */
typedef struct
{

    /**
     * @brief the internal buffer
     */
    uint8_t Buffer[ATRP_INTERNAL_BUFFER_LEN];

    /**
     * @brief the buffer position
     */
    uint32_t BufferPosition;

    /**
     * @brief the OK event callback
     */
    AtrpEventWithResponseCodeCallback_T EventResponseCodeCallback;

    /**
     * @brief the OK event callback
     */
    AtrpEventCallback_T EventErrorCallback;

    /**
     * @brief the command echo event callback
     */
    AtrpEventWithDataCallback_T EventCmdEchoCallback;

    /**
     * @brief the OK event callback
     */
    AtrpEventWithDataCallback_T EventCmdCallback;

    /**
     * @brief the OK event callback
     */
    AtrpEventWithDataCallback_T EventCmdArgCallback;

    /**
     * @brief called when misc data is encountered
     */
    AtrpEventWithDataCallback_T EventMiscCallback;

    /**
     * @brief the internal parser state callback
     */
    AtrpStateCallback_T StateCallback;

} AtResponseParserState_T;

/**
 * @brief Registers the response code event callback which is called when a
 * response code string is parsed. This generally marks the end of an AT
 * command response - meaning that the AT command is finished upon reception of
 * the response code.
 */
void AtResponseParser_RegisterResponseCodeCallback(AtrpEventWithResponseCodeCallback_T ResponseCodeCallback);

/**
 * @brief Registers the ERROR event callback which is called when a parsing
 * error occurs.
 */
void AtResponseParser_RegisterErrorCallback(AtrpEventCallback_T ErrorCallback);

/**
 * @brief Registers the CMD event callback which is called when a command
 * response is parsed.
 */
void AtResponseParser_RegisterCmdEchoCallback(AtrpEventWithDataCallback_T CmdEchoCallback);

/**
 * @brief Registers the CMD event callback which is called when a command
 * response is parsed.
 */
void AtResponseParser_RegisterCmdCallback(AtrpEventWithDataCallback_T CmdCallback);

/**
 * @brief Registers the CMD ARG event callback which is called when a command
 * argument is parsed. Any call to this callback will be preceded by an
 * invocation of the CMD event handler.
 */
void AtResponseParser_RegisterCmdArgCallback(AtrpEventWithDataCallback_T CmdArgCallback);

/**
 * @brief Registers the MISC event callback which is called when miscellaneous
 * content is parsed.
 */
void AtResponseParser_RegisterMiscCallback(AtrpEventWithDataCallback_T MiscCallback);

/**
 * @brief Resets the response parser to its initialized state. Call this from an
 * error callback to restore normal parser operation.
 */
void AtResponseParser_Reset(void);

/**
 * @brief Main interface to the AT response parser.
 */
Retcode_T AtResponseParser_Parse(const uint8_t *buffer, uint32_t len);

#endif /* AT_RESPONSE_PARSER_H_ */

/** @} */
