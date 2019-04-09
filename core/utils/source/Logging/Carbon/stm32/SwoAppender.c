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
 * @brief Implements log appender over SWO Jlink for stm32 target.
 *
 */

#include "BCDS_Utils.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID  BCDS_UTILS_MODULE_ID_LOGGING_APPENDER_SWO

#include "BCDS_Logging.h"

#if BCDS_FEATURE_LOGGING

#include "LogConfig.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

/*lint -e101 -e451 */
#include "stm32l4xx.h"
#include "core_cm4.h"
/*lint +e101 +e451 */
#include "BCDS_Retcode.h"

static Retcode_T SwoAppenderInit(void *init)
{
    BCDS_UNUSED(init);
    return RETCODE_OK;
}

static Retcode_T SwoAppenderWrite(const char *message, uint32_t length)
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
        for (uint32_t i = 0; i < length; i++)
        {
            /* returns the character transmitted.
             * Since it is of no benefit, making it void. */
            (void) ITM_SendChar(message[i]);
        }
    }

    return rc;
}

static const LogAppender_T SwoAppender =
{
    .Init = SwoAppenderInit,
    .Write = SwoAppenderWrite
};

const LogAppender_T *Logging_SwoAppender = &SwoAppender;

#endif /* if BCDS_FEATURE_LOGGING */
