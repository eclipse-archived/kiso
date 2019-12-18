/**********************************************************************************************************************
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
**********************************************************************************************************************/

/**
 * @file AtResponseParser.c
 *
 * @brief Implements AT Response Parser.
 */

/*###################### INCLUDED HEADERS ###########################################################################*/

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_ATPARSER

#include "AtResponseParser.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#include "FreeRTOS.h"
#include "task.h"

/*###################### MACROS DEFINITION ##########################################################################*/

#define ATRP_PARSE_FAILURE_RETVAL -1
#define ATRP_CONSUME_STATUS_FOUND_DELIMITERA 0x01
#define ATRP_CONSUME_STATUS_FOUND_DELIMITERB 0x02
#define NULL_CHAR ('\0') //The C string terminating NULL char
#define AT_RESPONSE_CODE_NAME_OK "OK"
#define AT_RESPONSE_CODE_NAME_CONNECT "CONNECT"
#define AT_RESPONSE_CODE_NAME_RING "RING"
#define AT_RESPONSE_CODE_NAME_NO_CARRIER "NO CARRIER"
#define AT_RESPONSE_CODE_NAME_ERROR "ERROR"
#define AT_RESPONSE_CODE_NAME_NO_DIALTONE "NO DIALTONE"
#define AT_RESPONSE_CODE_NAME_BUSY "BUSY"
#define AT_RESPONSE_CODE_NAME_NO_ANSWER "NO ANSWER"
#define AT_RESPONSE_CODE_NAME_ABORTED "ABORTED"

/*###################### LOCAL_TYPES ################################################################################*/
typedef enum
{
    ATRP_BUFFER_OUT_OF_SPACE,
    ATRP_BUFFER_OK
} AtrpBufferResult_t;

/*###################### LOCAL FUNCTIONS DECLARATION ################################################################*/

static int32_t AtrpStateRoot(const uint8_t *buffer, uint32_t len);
static AtrpBufferResult_t AtrpAppendToBuffer(const uint8_t *buffer, uint32_t len);
static void AtrpRollbackBuffer(uint32_t len);
static const uint8_t *AtrpTrimWhitespace(const uint8_t *buffer, uint32_t bufferLength, uint32_t *newBufferLength);
static void AtrpResetBuffer(void);
static void AtrpSwitchState(AtrpStateCallback_T NewCallback);
static const uint8_t *AtrpStrChrN(const uint8_t *buffer, uint32_t len, uint8_t delimiter);
static int32_t AtrpConsumeUntil(const uint8_t *buffer, uint32_t len, char delimiterA, char delimiterB, uint32_t *status);
static int32_t AtrpStateError(const uint8_t *buffer, uint32_t len);
static int32_t AtrpStateCmdarg(const uint8_t *buffer, uint32_t len);
static int32_t AtrpStateCmdEcho(const uint8_t *buffer, uint32_t len);
static int32_t AtrpStateCmd(const uint8_t *buffer, uint32_t len);
static int32_t AtrpStateCmd(const uint8_t *buffer, uint32_t len);
static int32_t AtrpStateResponseCode(const uint8_t *buffer, uint32_t len);

/*###################### VARIABLES DECLARATION ######################################################################*/

#ifdef GTEST
static AtResponseParserState_T state;
#else
static AtResponseParserState_T state =
    {
        .EventResponseCodeCallback = NULL,
        .EventCmdEchoCallback = NULL,
        .EventCmdCallback = NULL,
        .EventCmdArgCallback = NULL,
        .EventMiscCallback = NULL,
        .StateCallback = AtrpStateRoot};
#endif

/*###################### EXPOSED FUNCTIONS IMPLEMENTATION ###########################################################*/

Retcode_T AtResponseParser_Parse(const uint8_t *buffer, uint32_t len)
{
    if (NULL == state.StateCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, AT_RESPONSE_PARSER_NOT_INITIALIZED);
    }
    else if (len < 1)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, AT_RESPONSE_PARSER_INPUT_TOO_SHORT);
    }

    Retcode_T result = RETCODE_OK;

    while (len > 0)
    {
        // call the parser state and check if a state switch happened
        int32_t ConsumedCharacters = state.StateCallback(buffer, len);

        if (ATRP_PARSE_FAILURE_RETVAL == ConsumedCharacters)
        {
            // last state failed, let's move to error state and let the user recover from this
            AtrpSwitchState(AtrpStateError);
            result = RETCODE(RETCODE_SEVERITY_ERROR, AT_RESPONSE_PARSER_PARSE_ERROR);
            break;
        }
        else
        {
            // maybe we didn't consume all characters. So let's try again
            assert(ConsumedCharacters <= (int32_t)len); // "Consumed more characters than the buffer held. This should not happen."

            buffer = buffer + ConsumedCharacters;
            len = len - ConsumedCharacters;
            result = RETCODE_OK;
        }
    }

    return result;
}

void AtResponseParser_Reset(void)
{
    taskENTER_CRITICAL();
    AtrpResetBuffer();
    state.StateCallback = AtrpStateRoot;
    taskEXIT_CRITICAL();
}

void AtResponseParser_RegisterResponseCodeCallback(AtrpEventWithResponseCodeCallback_T ResponseCodeCallback)
{
    state.EventResponseCodeCallback = ResponseCodeCallback;
}

void AtResponseParser_RegisterErrorCallback(AtrpEventCallback_T ErrorCallback)
{
    state.EventErrorCallback = ErrorCallback;
}

void AtResponseParser_RegisterCmdEchoCallback(AtrpEventWithDataCallback_T CmdEchoCallback)
{
    state.EventCmdEchoCallback = CmdEchoCallback;
}

void AtResponseParser_RegisterCmdCallback(AtrpEventWithDataCallback_T CmdCallback)
{
    state.EventCmdCallback = CmdCallback;
}

void AtResponseParser_RegisterCmdArgCallback(AtrpEventWithDataCallback_T CmdArgCallback)
{
    state.EventCmdArgCallback = CmdArgCallback;
}

void AtResponseParser_RegisterMiscCallback(AtrpEventWithDataCallback_T MiscCallback)
{
    state.EventMiscCallback = MiscCallback;
}

/*###################### LOCAL FUNCTIONS IMPLEMENTATION #############################################################*/

static AtrpBufferResult_t AtrpAppendToBuffer(const uint8_t *buffer, uint32_t len)
{
    if (0 == len)
    {
        return ATRP_BUFFER_OK;
    }

    if (state.BufferPosition + len >= ATRP_INTERNAL_BUFFER_LEN)
    {
        return ATRP_BUFFER_OUT_OF_SPACE;
    }

    memcpy(state.Buffer + state.BufferPosition, buffer, len);
    state.BufferPosition += len;
    state.Buffer[state.BufferPosition] = '\0';

    return ATRP_BUFFER_OK;
}

static void AtrpRollbackBuffer(uint32_t len)
{
    if (len > state.BufferPosition)
    {
        return;
    }

    state.BufferPosition -= len;
}

static void AtrpResetBuffer(void)
{
    state.Buffer[0] = '\0';
    state.BufferPosition = 0;
}

static void AtrpSwitchState(AtrpStateCallback_T NewCallback)
{
    state.StateCallback = NewCallback;
}

/**
 * @brief Trims space, AT_DEFAULT_S3_CHARACTER and AT_DEFAULT_S4_CHARACTER on the beginning and end of a string.
 *
 * @param[in] buffer The buffer to trim. The content of this buffer is not modified by this function.
 * @param[in] BufferLength The current buffer length in bytes
 * @param[out] NewBufferLength The new buffer length after being trimmed of whitespace chars
 *
 * @return A pointer to the trimmed string.
 */
static const uint8_t *AtrpTrimWhitespace(const uint8_t *buffer, uint32_t bufferLength, uint32_t *newBufferLength)
{
    uint32_t ResultBufferLength = bufferLength;
    const uint8_t *ResultBuffer = buffer;

    for (uint8_t c = *ResultBuffer; ResultBufferLength > 0 && (' ' == c || AT_DEFAULT_S3_CHARACTER == c || AT_DEFAULT_S4_CHARACTER == c); c = *ResultBuffer)
    {
        ResultBuffer++;
        ResultBufferLength--;
    }

    for (uint8_t c = *(ResultBuffer + ResultBufferLength - 1); ResultBufferLength > 0 && (' ' == c || AT_DEFAULT_S3_CHARACTER == c || AT_DEFAULT_S4_CHARACTER == c); c = *(ResultBuffer + ResultBufferLength - 1))
    {
        ResultBufferLength--;
    }

    *newBufferLength = ResultBufferLength;
    return ResultBuffer;
}

static const uint8_t *AtrpStrChrN(const uint8_t *buffer, uint32_t len, uint8_t delimiter)
{
    if (NULL_CHAR == delimiter)
    {
        return 0;
    }

    uint32_t i = 0;
    for (i = 0; i < len; i++)
    {
        if (*(buffer + i) == delimiter)
        {
            return buffer + i;
        }
    }

    return NULL;
}

static int32_t AtrpConsumeUntil(const uint8_t *buffer, uint32_t len, char delimiterA, char delimiterB, uint32_t *status)
{
    int32_t result = ATRP_PARSE_FAILURE_RETVAL;
    *status = 0;

    // search for end of argument which is delimited by either delimiter
    const uint8_t *EndOfConsumptionA = AtrpStrChrN(buffer, len, delimiterA);
    const uint8_t *EndOfConsumptionB = AtrpStrChrN(buffer, len, delimiterB);

    // let's see which delimiter was found first
    const uint8_t *EndOfConsumption;
    if (NULL != EndOfConsumptionA && NULL != EndOfConsumptionB)
    {
        if (EndOfConsumptionA < EndOfConsumptionB)
        {
            *status |= ATRP_CONSUME_STATUS_FOUND_DELIMITERA;
            EndOfConsumption = EndOfConsumptionA;
        }
        else
        {
            *status |= ATRP_CONSUME_STATUS_FOUND_DELIMITERB;
            EndOfConsumption = EndOfConsumptionB;
        }
    }
    else if (NULL != EndOfConsumptionA)
    {
        *status |= ATRP_CONSUME_STATUS_FOUND_DELIMITERA;
        EndOfConsumption = EndOfConsumptionA;
    }
    else if (NULL != EndOfConsumptionB)
    {
        *status |= ATRP_CONSUME_STATUS_FOUND_DELIMITERB;
        EndOfConsumption = EndOfConsumptionB;
    }
    else
    {
        EndOfConsumption = NULL;
    }

    // check if we found any delimiter
    if (NULL == EndOfConsumption)
    {
        // nope, no delimiter found
        // the string is not yet complete, append to buffer
        AtrpBufferResult_t success = AtrpAppendToBuffer(buffer, len);
        if (ATRP_BUFFER_OUT_OF_SPACE == success)
        {
            result = ATRP_PARSE_FAILURE_RETVAL;
        }
        else
        {
            result = len;
        }
    }
    else
    {
        // delimiter was found, let's copy the rest to the buffer and finish
        int32_t RestOfCmdLength = EndOfConsumption - buffer;
        assert(RestOfCmdLength >= 0); // "Error in buffer calculation"

        AtrpBufferResult_t success = AtrpAppendToBuffer(buffer, RestOfCmdLength);
        if (ATRP_BUFFER_OUT_OF_SPACE == success)
        {
            result = ATRP_PARSE_FAILURE_RETVAL;
        }
        else
        {
            // + 1 to consume the delimiter
            result = RestOfCmdLength + 1;
        }
    }

    return result;
}

/* *** PARSER STATE HANDLERS ************************************************ */

static int32_t AtrpStateError(const uint8_t *buffer, uint32_t len)
{
    KISO_UNUSED(buffer);
    KISO_UNUSED(len);

    if (NULL != state.EventErrorCallback)
    {
        state.EventErrorCallback();
    }

    return 0;
}

static int32_t AtrpStateCmdarg(const uint8_t *buffer, uint32_t len)
{
    /*
     * This state parses until a ',' or AT_DEFAULT_S4_CHARACTER is found.
     * In case of the latter, a AT_DEFAULT_S3_CHARACTER will be part of the
     * string which is then trimmed by AtrpTrimWhitespace.
     */
    uint32_t status = 0;
    int32_t result = AtrpConsumeUntil(buffer, len, ',', AT_DEFAULT_S4_CHARACTER, &status);
    if (ATRP_PARSE_FAILURE_RETVAL == result)
    {
        return ATRP_PARSE_FAILURE_RETVAL;
    }

    if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERA | ATRP_CONSUME_STATUS_FOUND_DELIMITERB))
    {
        if (NULL != state.EventCmdArgCallback)
        {
            uint32_t NewLength;
            const uint8_t *NewBuffer = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
            if (NewLength)
            {
                state.EventCmdArgCallback(NewBuffer, NewLength);
            }
        }
        AtrpResetBuffer();

        if (status & ATRP_CONSUME_STATUS_FOUND_DELIMITERA)
        {
            // found a comma, preparing for another arg
            AtrpSwitchState(AtrpStateCmdarg);
        }
        else
        {
            // found a newline, that's it
            AtrpSwitchState(AtrpStateRoot);
        }
    }

    return result;
}

static int32_t AtrpStateCmdEcho(const uint8_t *buffer, uint32_t len)
{
    uint32_t status = 0;
    int32_t result = AtrpConsumeUntil(buffer, len, AT_DEFAULT_S4_CHARACTER, AT_DEFAULT_S3_CHARACTER, &status);
    if (ATRP_PARSE_FAILURE_RETVAL == result)
    {
        return ATRP_PARSE_FAILURE_RETVAL;
    }

    if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERA | ATRP_CONSUME_STATUS_FOUND_DELIMITERB))
    {
        if (NULL != state.EventCmdEchoCallback)
        {
            uint32_t NewLength;
            const uint8_t *NewBuffer = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
            state.EventCmdEchoCallback(NewBuffer, NewLength);
        }
        AtrpResetBuffer();
        AtrpSwitchState(AtrpStateRoot);
    }

    return result;
}

static int32_t AtrpStateCmd(const uint8_t *buffer, uint32_t len)
{
    uint32_t status = 0;
    int32_t result = AtrpConsumeUntil(buffer, len, ':', AT_DEFAULT_S4_CHARACTER, &status);
    if (ATRP_PARSE_FAILURE_RETVAL == result)
        return ATRP_PARSE_FAILURE_RETVAL;

    if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERA | ATRP_CONSUME_STATUS_FOUND_DELIMITERB))
    {
        if (NULL != state.EventCmdCallback)
        {
            uint32_t NewLength;
            const uint8_t *NewBuffer = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
            state.EventCmdCallback(NewBuffer, NewLength);
        }
        AtrpResetBuffer();

        AtrpStateCallback_T NextState = (status & ATRP_CONSUME_STATUS_FOUND_DELIMITERA) ? AtrpStateCmdarg : AtrpStateRoot;
        AtrpSwitchState(NextState);
    }

    return result;
}

/**
 * \todo: the current implementation of the OK parsing uses a look-ahead of 1.
 * This clashes with Misc. Content which starts with an upper-case O.
 * Our implementation is according to spec, where OK has a higher priority than
 * misc content, yet this is a source of problems/errors.
 */
static int32_t AtrpStateResponseCode(const uint8_t *buffer, uint32_t len)
{
    uint32_t status = 0;
    int32_t result = AtrpConsumeUntil(buffer, len, AT_DEFAULT_S3_CHARACTER, AT_DEFAULT_S4_CHARACTER, &status);
    if (ATRP_PARSE_FAILURE_RETVAL == result)
    {
        return ATRP_PARSE_FAILURE_RETVAL;
    }
    if (NULL == state.EventResponseCodeCallback)
    {
        AtrpResetBuffer();
        AtrpSwitchState(AtrpStateRoot);
        return result;
    }

    if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERA | ATRP_CONSUME_STATUS_FOUND_DELIMITERB))
    {
        uint32_t NewLength;
        const uint8_t *ResponseCodeName = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
        if (0 == strncmp(AT_RESPONSE_CODE_NAME_ABORTED, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_ABORTED);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_BUSY, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_BUSY);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_CONNECT, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_CONNECT);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_ERROR, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_ERROR);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_NO_ANSWER, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_NO_ANSWER);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_NO_CARRIER, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_NO_CARRIER);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_NO_DIALTONE, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_NO_DIALTONE);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_OK, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_OK);
        }
        else if (0 == strncmp(AT_RESPONSE_CODE_NAME_RING, (const char *)ResponseCodeName, NewLength))
        {
            state.EventResponseCodeCallback(AT_RESPONSE_CODE_RING);
        }
        else
        {
            // did not find valid response code ... fail
            result = ATRP_PARSE_FAILURE_RETVAL;
        }

        AtrpResetBuffer();
        AtrpSwitchState(AtrpStateRoot);
    }

    return result;
}

/**
 * \todo: if the buffer overflows within one MISC message, we go to the error
 * state. Instead, we should trigger the MISC event and reset the buffer.
 */
static int32_t AtrpStateMiscContent(const uint8_t *buffer, uint32_t len)
{
    /*
     * This state parses until AT_DEFAULT_S4_CHARACTER is found.
     * When a delimiter has been found, one stray AT_DEFAULT_S3_CHARACTER
     * will be part of the string which is then trimmed by AtrpTrimWhitespace.
     */

    uint32_t status = 0;
    int32_t result = AtrpConsumeUntil(buffer, len, AT_DEFAULT_S4_CHARACTER, NULL_CHAR, &status);

    if (ATRP_PARSE_FAILURE_RETVAL == result)
    {
        return ATRP_PARSE_FAILURE_RETVAL;
    }

    if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERA))
    {
        //FIXME: Find better way of handling non-AT conform responses!
        //uint8_t* NewBuffer = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
        state.Buffer[state.BufferPosition++] = '\n';
        if (result > 0 && NULL != state.EventMiscCallback)
            state.EventMiscCallback(state.Buffer, state.BufferPosition);
        AtrpResetBuffer();
        AtrpSwitchState(AtrpStateRoot);
    }

    return result;
}

static int32_t AtrpStateRoot(const uint8_t *buffer, uint32_t len)
{
    uint32_t status = 0;
    int32_t result = AtrpConsumeUntil(buffer, len, '+', AT_DEFAULT_S4_CHARACTER, &status);
    if (ATRP_PARSE_FAILURE_RETVAL == result)
    {
        return ATRP_PARSE_FAILURE_RETVAL;
    }

    if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERA))
    {
        // found a command echo or a command response
        uint32_t NewLength;
        const uint8_t *NewBuffer = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
        if (0 == strncmp("AT", (const char *)NewBuffer, 2))
        {
            // found a cmd echo
            /*
             * Rollback (instead of buffer reset) is needed if the next state needs to re-consume
             * the current input (i.e., when using "predictive parsing" to decide said next state).
             *
             * Result is the number of consumed characters which includes the delimiter.
             * However, the delimiter is NOT copied to the buffer, thus the -1.
             */
            AtrpRollbackBuffer(result - 1);
            result = 0;

            AtrpSwitchState(AtrpStateCmdEcho);
        }
        else
        {
            // this is a command
            AtrpSwitchState(AtrpStateCmd);
        }
    }
    else if (status & (ATRP_CONSUME_STATUS_FOUND_DELIMITERB))
    {
        /*
         * This implementation of a "predictive parser" is a bit of a hack.
         * I really don't want to do backtracking, but also can't be bothered to implement
         * this as a table predicitive parser.
         */

        // found end of line - maybe this is a response code
        uint32_t NewLength;
        const uint8_t *ResponseCodeName = AtrpTrimWhitespace(state.Buffer, state.BufferPosition, &NewLength);
        uint8_t isResponseCode = (0 == strncmp(AT_RESPONSE_CODE_NAME_ABORTED, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_BUSY, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_CONNECT, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_ERROR, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_NO_ANSWER, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_NO_CARRIER, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_NO_DIALTONE, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_OK, (const char *)ResponseCodeName, NewLength)) || (0 == strncmp(AT_RESPONSE_CODE_NAME_RING, (const char *)ResponseCodeName, NewLength));

        if (0 == NewLength)
        {
            // empty line will be filed as Misc content

            AtrpRollbackBuffer(result - 1);
            result = 0;
            AtrpSwitchState(AtrpStateMiscContent);
        }
        else if (isResponseCode)
        {
            /*
             * Rollback (instead of buffer reset) is needed if the next state needs to re-consume
             * the current input (i.e., when using "predictive parsing" to decide said next state).
             *
             * Result is the number of consumed characters which includes the delimiter.
             * However, the delimiter is NOT copied to the buffer, thus the -1.
             */
            AtrpRollbackBuffer(result - 1);
            result = 0;

            AtrpSwitchState(AtrpStateResponseCode);
        }
        else
        {
            /*
             * Rollback (instead of buffer reset) is needed if the next state needs to re-consume
             * the current input (i.e., when using "predictive parsing" to decide said next state).
             *
             * Result is the number of consumed characters which includes the delimiter.
             * However, the delimiter is NOT copied to the buffer, thus the -1.
             */
            AtrpRollbackBuffer(result - 1);
            result = 0;

            AtrpSwitchState(AtrpStateMiscContent);
        }
    }

    return result;
}