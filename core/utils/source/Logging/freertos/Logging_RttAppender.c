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
 *
 * @brief Implements log appender over Rtt Jlink for stm32 target.
 *
 */

/* Include utils to have access to the defined module and error IDs */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID  KISO_UTILS_MODULE_ID_LOGGING_APPENDER_RTT

/* Include the Logging header, which include the configuration that enable and define macros for this module */
#include "Kiso_Logging.h"

/* Enable/Disable macro for the feature */
#if KISO_FEATURE_LOGGING && KISO_RTT_APPENDER

/* Include needed headers */
#include "Kiso_Retcode.h"
#include "SEGGER_RTT.h"

/**
 * @brief
 * 		Initialize the RTT module to be able to use it later on.
 * @param [in] void *init
 * 		Not used, fulfillment of the pattern.
 * @return
 * 		RETCODE_OK - If everything was fine.
 */
static Retcode_T RttAppenderInit(void *init)
{
    (void)init;
    SEGGER_RTT_Init();
    return RETCODE_OK;
}

/**
 * @brief
 * 		Write into the RTT block.
 * @param [in] const char *message
 * 		Message to send out.
 * @param [in] uint32_t length
 * 		Message length.
 * @return
 * 		RETCODE_OK - If everything was fine.
 * 		RETCODE_NULL_POINTER - Message was a NULL pointer.
 * 		RETCODE_INVALID_PARAM - The length is not valid.
 */
static Retcode_T RttAppenderWrite(const char *message, uint32_t length)
{
    Retcode_T rc = RETCODE_OK;

    if (NULL == message)
    {
        rc = RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_NULL_POINTER);
    }
    else if (0UL == length)
    {
        rc = RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_INVALID_PARAM);
    }
    else
    {
    	SEGGER_RTT_Write(0, message, length);
    }

    return rc;
}

/**
 * @brief Create singleton
 */
static const LogAppender_T RttAppender =
{
    .Init = RttAppenderInit,
    .Write = RttAppenderWrite
};
const LogAppender_T *Logging_RttAppender = &RttAppender;

#endif /* if KISO_FEATURE_LOGGING && KISO_RTT_APPENDER */
