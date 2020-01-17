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
 *
 * @brief
 *      Filter Interface Implementation
 *
 * @details
 *      Implements log generic filter.
 *      This source file implements following features:
 *      - LogFilter_Add()
 *      - LogFilter_Delete()
 *      - LogFilter_Configure()
 *      - LogFilter_Apply()
 * 
 * @file
 **/
#include "LogConfig.h"

/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LOGGING_FILTER

#if KISO_FEATURE_LOGGING

/* Include Kiso_Logging interface header */
#include "Kiso_Logging.h"

/* KISO header files */
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"

#define LOG_PACKAGE_ID_ALL (0)
#define LOG_MODULE_ID_ALL (0)
#define LOG_FILTER_ID_INVALID (LOG_FILTER_ITEM_COUNT)

/**
 * @brief Log filter item.
 */
typedef struct
{
    uint8_t Level;
    uint8_t Package;
    uint8_t Module;
} __attribute__((packed)) LogFilterItem_T;

static LogFilterItem_T LogFilterItems[LOG_FILTER_ITEM_COUNT] =
    {
        {.Level = (uint8_t)LOG_LEVEL_PACKAGE_DEFAULT,
         .Package = LOG_PACKAGE_ID_ALL,
         .Module = LOG_MODULE_ID_ALL}};

/*  The description of the function is available in Kiso_Logging.h */
LogFilterId_T LogFilter_Add(LogLevel_T level, uint8_t package, uint8_t module)
{
    uint32_t i;

    for (i = 0; i < LOG_FILTER_ID_INVALID; ++i)
    {
        if ((uint8_t)LOG_LEVEL_NONE == LogFilterItems[i].Level)
        {
            LogFilterItems[i].Level = (uint8_t)level;
            LogFilterItems[i].Package = package;
            LogFilterItems[i].Module = module;
            break;
        }
    }

    /* When no place was found, set the id to the maximum (as error) */
    if (i >= LOG_FILTER_ID_INVALID)
    {
        i = UINT32_MAX;
    }

    return i;
}

/*  The description of the function is available in Kiso_Logging.h */
Retcode_T LogFilter_Delete(LogFilterId_T id)
{
    if (id >= LOG_FILTER_ITEM_COUNT)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    LogFilterItems[id].Level = (uint8_t)LOG_LEVEL_NONE;
    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_Logging.h */
Retcode_T LogFilter_Configure(LogFilterId_T id, LogLevel_T level, uint8_t package, uint8_t module)
{
    if (id >= LOG_FILTER_ITEM_COUNT)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
    }

    LogFilterItems[id].Level = (uint8_t)level;
    LogFilterItems[id].Package = package;
    LogFilterItems[id].Module = module;

    return RETCODE_OK;
}

/*  The description of the function is available in Kiso_Logging.h */
bool LogFilter_Apply(LogLevel_T level, uint8_t package, uint8_t module)
{
    bool enabled = true;

    for (uint32_t i = 0; i < LOG_FILTER_ITEM_COUNT; ++i)
    {
        if ((uint8_t)LOG_LEVEL_NONE != LogFilterItems[i].Level)
        {
            enabled = false;
            if ((uint8_t)level <= LogFilterItems[i].Level && ((uint8_t)LOG_PACKAGE_ID_ALL == LogFilterItems[i].Package || package == LogFilterItems[i].Package) && ((uint8_t)LOG_MODULE_ID_ALL == LogFilterItems[i].Module || module == LogFilterItems[i].Module))
            {
                return true;
            }
        }
    }

    return enabled;
}

#endif /* if KISO_FEATURE_LOGGING */
