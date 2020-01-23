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
 *      Module test specification for the Filter_unittest.cc module.
 *
 * @detail
 *      The unit test file template follows the Four-Phase test pattern.
 *
 * @file
 **/

/* Include gtest interface */
#include <gtest.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Module includes */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_LOGGING_FILTER

#if KISO_FEATURE_LOGGING

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"
#include "LogConfig.h"

#include "Filter.c"
    /* End of global scope symbol and fake definitions section */
}

class Logging_Filter : public testing::Test
{
protected:
    virtual void SetUp()
    {
        FFF_RESET_HISTORY()
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        /* Wipe out the filter item buffer */
        memset(LogFilterItems, 0, sizeof(LogFilterItem_T) * LOG_FILTER_ITEM_COUNT);
    }
};

TEST_F(Logging_Filter, Logging_FilterAddSuccess)
{
    LogFilterId_T id = 0;

    id = LogFilter_Add(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);

    EXPECT_NE(UINT32_C(0), id);
}

TEST_F(Logging_Filter, Logging_FilterAddOverflow)
{
    LogFilterId_T id = 0;

    uint32_t i = 0;
    for (i = 0; i < LOG_FILTER_ITEM_COUNT; i++)
    {
        id = LogFilter_Add(LOG_LEVEL_FATAL, 0, i);
        EXPECT_EQ(i, id);
    }

    id = LogFilter_Add(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);

    EXPECT_EQ(UINT32_MAX, id);
}

TEST_F(Logging_Filter, Logging_FilterDeleteSuccess)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilterId_T id = 0;

    id = LogFilter_Add(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);

    retcode = LogFilter_Delete(id);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ((uint8_t)LOG_LEVEL_NONE, LogFilterItems[id].Level);
}

TEST_F(Logging_Filter, Logging_FilterDeleteOutOfBound)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilterId_T id = LOG_FILTER_ITEM_COUNT + 1;

    retcode = LogFilter_Delete(id);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(Logging_Filter, Logging_FilterCfgSuccess)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilterId_T id = 0;

    id = LogFilter_Add(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);

    retcode = LogFilter_Configure(id, LOG_LEVEL_DEBUG, 0, 3);

    EXPECT_EQ(RETCODE_OK, retcode);
    EXPECT_EQ((uint8_t)LOG_LEVEL_DEBUG, LogFilterItems[id].Level);
}

TEST_F(Logging_Filter, Logging_FilterCfgOutOfBound)
{
    Retcode_T retcode = RETCODE_OK;
    LogFilterId_T id = LOG_FILTER_ITEM_COUNT + 1;

    retcode = LogFilter_Configure(id, LOG_LEVEL_DEBUG, 0, 3);

    EXPECT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), retcode);
}

TEST_F(Logging_Filter, Logging_FilterApplySuccess)
{
    (void)LogFilter_Add(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    bool output = false;

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);

    EXPECT_EQ(true, output);
}

TEST_F(Logging_Filter, Logging_FilterApplyToAllPackagesSuccess)
{
    (void)LogFilter_Add(LOG_LEVEL_FATAL, LOG_PACKAGE_ID_ALL, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    bool output = false;

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    EXPECT_EQ(true, output);

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 1, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    EXPECT_EQ(true, output);
}

TEST_F(Logging_Filter, Logging_FilterApplyToAllModulesSuccess)
{
    (void)LogFilter_Add(LOG_LEVEL_FATAL, 0, LOG_MODULE_ID_ALL);
    bool output = false;

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    EXPECT_EQ(true, output);

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 0, 10);
    EXPECT_EQ(true, output);
}

TEST_F(Logging_Filter, Logging_FilterApplyNotFilter)
{
    bool output = false;

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    EXPECT_EQ(true, output);

    output = LogFilter_Apply(LOG_LEVEL_INFO, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    EXPECT_EQ(true, output);

    output = LogFilter_Apply(LOG_LEVEL_FATAL, 5, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    EXPECT_EQ(true, output);
}

TEST_F(Logging_Filter, Logging_FilterApplyWrongFilter)
{
    (void)LogFilter_Add(LOG_LEVEL_FATAL, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);
    bool output = false;

    output = LogFilter_Apply(LOG_LEVEL_INFO, 0, KISO_UTILS_MODULE_ID_LOGGING_FILTER);

    EXPECT_EQ(false, output);
}

#else
}

#endif /* if KISO_FEATURE_LOGGING */
