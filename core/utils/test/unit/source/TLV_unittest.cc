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
 *      Module test specification for the TLV_unittest.cc module.
 * 
 * @details
 *      The unit test file template follows the Four-Phase test pattern.
 * 
 * @file
 **/

/* Setup testing framework ************************************************** */

/* Include system headers */
#include <string.h>

/* Include gtest interface */
#include <gtest.h>

/* Start of global scope symbol and fake definitions section */
extern "C"
{
/* Setup compile time configuration defines */
#include "Kiso_Utils.h"
#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_UTILS_MODULE_ID_TLV

#if KISO_FEATURE_TLV

/* Include faked interfaces */
#include "Kiso_Assert_th.hh"

/* Include module under test */
#include "TLV.c"

    /* End of global scope symbol and fake definitions section */
}

/* Create test fixture initializing all variables automatically */

typedef enum tlvTestTypes_e
{
    TLV_TEST_TYPE_0 = 10,
    TLV_TEST_TYPE_1 = 0,
    TLV_TEST_TYPE_2 = 255,
    TLV_TEST_TYPE_3 = 256,
    TLV_TEST_TYPE_4 = 32768,
    TLV_TEST_TYPE_5 = 65535,
    TLV_TEST_TYPE_6 = 128,
    TLV_TEST_TYPE_7 = 1,
    TLV_TEST_TYPE_8 = 2,
    TLV_TEST_TYPE_9 = 3
} tlvTestTypes_t;

typedef enum tlvTestValueID_e
{
    TLV_TEST_VALUE_ID_0,
    TLV_TEST_VALUE_ID_1,
    TLV_TEST_VALUE_ID_2,
    TLV_TEST_VALUE_ID_3,
    TLV_TEST_VALUE_ID_4,
    TLV_TEST_VALUE_ID_5,
    TLV_TEST_VALUE_ID_6,
    TLV_TEST_VALUE_ID_7,
    TLV_TEST_VALUE_ID_8,
    TLV_TEST_VALUE_ID_9
} tlvTestValueID_t;

typedef struct string_s
{
    const char *string;
    uint16_t Length;
} string_t;

#define STRING_LITERAL(string)     \
    {                              \
        (string), (sizeof(string)) \
    }

const string_t tlvTestValuesTable[] =
    {
        STRING_LITERAL(""),
        STRING_LITERAL("2"),
        STRING_LITERAL("33"),
        STRING_LITERAL("444"),
        STRING_LITERAL("testtesttest"),
        STRING_LITERAL("blabla"),
        STRING_LITERAL("00000000"),
        STRING_LITERAL("\0\0\0\0 "),
        STRING_LITERAL("!%/!/(/=(/=(!%=!%/+/!%=!%"),
        STRING_LITERAL("                ")};

class TLVTest : public testing::Test
{
protected:
    /* Remember that SetUp() is run immediately before a test starts. */
    virtual void SetUp()
    {
        /* Initialize execution environment */
        FFF_RESET_HISTORY()

        dataBufferSize = 256;
        DataBuffer = new uint8_t[dataBufferSize];
    }

    /* TearDown() is invoked immediately after a test finishes. */
    virtual void TearDown()
    {
        delete[] DataBuffer;
        DataBuffer = NULL;
    }

    uint32_t getGroupHandleSize(void)
    {
        uint32_t groupHandleSize;

        TLV_Element_T *Element = TLV_AddElement(TLV_AddGroup(DataBuffer, 256), TLV_TEST_TYPE_0, 1, "");

        if (Element)
        {
            /* Calculate group Handle Size based on the base address of the application Buffer
             * and the address of the created first Element
             */
            groupHandleSize = (uint64_t)Element - (uint64_t)DataBuffer;
        }
        else
        {
            /* on error, make an assumption */
            groupHandleSize = 8;
        }

        return (groupHandleSize);
    }

    uint8_t *DataBuffer;
    uint16_t dataBufferSize;
};

/* Specify test cases ******************************************************* */

/** The TLV data handler module must implement a Type-Length-Value database
 *  management API set.
 *
 * Requirements against the implementation:
 * - must hide internal TLV database representation from end users
 * - must not allocate memory from the system heap
 * - must not allocate memory statically at compilation time
 * - shall be portable to other 32 bit platforms
 *
 * Constraints against the implementation:
 * - maximum supported Buffer Size shall be limited to 16 bits
 * - only aligned access is allowed to memory
 * - shall support only 32 bit architectures
 */

TEST_F(TLVTest, tlvGroupAdd)
{
    /**
     * @testcase{TLV::tlvGroupAdd: }
     *
     * The API under test is TLV_AddGroup(). Requirements against the API:
     * - return NULL on invalid Buffer pointer
     * - return NULL if internal data does not fit into Buffer
     * - return NULL if Buffer's base address is not machine word (32 bits) aligned
     * - return non-NULL on successful Handle initialization
     */

    {
        TLV_GroupHandle_TP Handle;

        typedef struct parameterSet_s
        {
            void *Buffer;
            uint16_t bufferSize;
            bool expectedResult;
        } parameterSet_t;

        parameterSet_t parameterSet[] = {
            {DataBuffer, dataBufferSize, true},
            {NULL, dataBufferSize, false},
            {DataBuffer, 0, false}};

        for (uint8_t index = 0; index < (sizeof(parameterSet) / sizeof(parameterSet_t)); index++)
        {
            Handle = TLV_AddGroup(parameterSet[index].Buffer, parameterSet[index].bufferSize);

            if (parameterSet[index].expectedResult)
            {
                /* API call should return a valid Handle */
                EXPECT_NE((TLV_GroupHandle_TP)NULL, Handle);
            }
            else
            {
                /* API call should return invalid Handle */
                EXPECT_EQ((TLV_GroupHandle_TP)NULL, Handle);
            }

            /* clean up resources */
            TLV_RemoveGroup(Handle);
        }
    }
}

TEST_F(TLVTest, tlvGroupRemove)
{
    /**
     * @testcase{TLV::tlvGroupRemove: }
     *
     * The API under test is TLV_RemoveGroup(). Requirements against the API:
     * - free up resources associated with the Handle passed as argument
     */

    /* N/A */
}

TEST_F(TLVTest, tlvElementAdd)
{
    /**
     * @testcase{TLV::tlvElementAdd: }
     *
     * The API under test is TLV_AddElement(). Requirements against the API:
     * - return NULL if Handle is not valid
     * - return NULL if failed to add new Element
     * - return pointer to new Element on success
     * - new Element in database should match Element passed as argument
     */

    {
        TLV_GroupHandle_TP Handle;
        TLV_Element_T *elementHandle;
        uint16_t testSpecificBufferSize;

        typedef struct parameterSet_s
        {
            tlvTestTypes_t Type;
            uint16_t Length;
            const void *Value;
        } parameterSet_t;

        parameterSet_t parameterSet[] = {
            /* add new unique Element */
            {TLV_TEST_TYPE_0, tlvTestValuesTable[TLV_TEST_VALUE_ID_2].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_2].string},
            /* add new unique Element */
            {TLV_TEST_TYPE_1, tlvTestValuesTable[TLV_TEST_VALUE_ID_4].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_4].string},
            /* add new unique Element */
            {TLV_TEST_TYPE_2, tlvTestValuesTable[TLV_TEST_VALUE_ID_3].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_3].string},
            /* update existing Element with shorter Value Length */
            {TLV_TEST_TYPE_1, tlvTestValuesTable[TLV_TEST_VALUE_ID_1].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_1].string},
            /* add new unique Element */
            {TLV_TEST_TYPE_3, tlvTestValuesTable[TLV_TEST_VALUE_ID_9].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_9].string},
            /* update previously added Element with longer Value Length */
            {TLV_TEST_TYPE_3, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].string}};

        typedef struct parameterSet2_s
        {
            tlvTestTypes_t Type;
            uint16_t parameterSetTableIndex;
        } parameterSet2_t;

        parameterSet2_t parameterSet2[] = {
            {TLV_TEST_TYPE_0, 0},
            {TLV_TEST_TYPE_1, 3},
            {TLV_TEST_TYPE_2, 2},
            {TLV_TEST_TYPE_3, 5}};

        /* the test shall exercise use cases where the Buffer space would not
         * be sufficient to hold all TLVs - calculate required Size of unique
         * Elements in the parameterSet table with their latest Value Length
         *
         * mind that the available Buffer space could be less than expected
         * as the implementation may use the Buffer to store the TLV group
         * related information
         */
        testSpecificBufferSize = 4 * sizeof(TLV_Element_T) +
                                 getGroupHandleSize() +
                                 tlvTestValuesTable[TLV_TEST_VALUE_ID_2].Length +
                                 tlvTestValuesTable[TLV_TEST_VALUE_ID_1].Length +
                                 tlvTestValuesTable[TLV_TEST_VALUE_ID_3].Length +
                                 tlvTestValuesTable[TLV_TEST_VALUE_ID_8].Length;

        ASSERT_LE(testSpecificBufferSize, dataBufferSize);

        Handle = TLV_AddGroup(DataBuffer, testSpecificBufferSize);

        ASSERT_NE((TLV_GroupHandle_TP)NULL, Handle);

        for (uint8_t index = 0; index < (sizeof(parameterSet) / sizeof(parameterSet_t)); index++)
        {
            elementHandle = TLV_AddElement(Handle, parameterSet[index].Type, parameterSet[index].Length, parameterSet[index].Value);

            ASSERT_NE((TLV_Element_T *)NULL, elementHandle) << "Test set index: " << (char)('0' + index) << "";
            EXPECT_NE((uint8_t *)NULL, elementHandle->DataBuffer);
            EXPECT_EQ(parameterSet[index].Type, elementHandle->DataType);
            EXPECT_EQ(parameterSet[index].Length, elementHandle->DataLength);
            EXPECT_EQ(0, memcmp(elementHandle->DataBuffer, parameterSet[index].Value, parameterSet[index].Length));
        }

        /* check whether TLVs had been corrupted */
        for (uint8_t index = 0; index < (sizeof(parameterSet2) / sizeof(parameterSet2_t)); index++)
        {
            elementHandle = TLV_GetElement(Handle, parameterSet2[index].Type);
            ASSERT_NE((TLV_Element_T *)NULL, elementHandle) << "Test set index: " << (char)('0' + index) << "";
            EXPECT_EQ(0, memcmp(elementHandle->DataBuffer,
                                parameterSet[parameterSet2[index].parameterSetTableIndex].Value,
                                parameterSet[parameterSet2[index].parameterSetTableIndex].Length));
        }

        /* Testing the negative case by invalid Buffer Length as parameter */
        uint8_t TestdataBuffer[5];
        testSpecificBufferSize = 7;
        Handle = TLV_AddGroup(TestdataBuffer, testSpecificBufferSize);
        EXPECT_EQ((TLV_GroupHandle_TP)NULL, Handle);

        /* clean up resources */
        TLV_RemoveGroup(Handle);
    }
}

TEST_F(TLVTest, tlvElementRemove)
{
    /**
     * @testcase{TLV::tlvElementRemove: }
     *
     * The API under test is TLV_RemoveElement(). Requirements against the API:
     * - after removal of a TLV from a group, TLV_GetElement() shall return NULL
     */

    {
        TLV_GroupHandle_TP Handle;

        Handle = TLV_AddGroup(DataBuffer, dataBufferSize);

        ASSERT_NE((TLV_GroupHandle_TP)NULL, Handle);

        /* add TLVs to the group */
        TLV_AddElement(Handle, TLV_TEST_TYPE_4, tlvTestValuesTable[TLV_TEST_VALUE_ID_2].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_2].string);
        TLV_AddElement(Handle, TLV_TEST_TYPE_5, tlvTestValuesTable[TLV_TEST_VALUE_ID_5].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_5].string);
        TLV_AddElement(Handle, TLV_TEST_TYPE_6, tlvTestValuesTable[TLV_TEST_VALUE_ID_3].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_3].string);
        TLV_AddElement(Handle, TLV_TEST_TYPE_7, tlvTestValuesTable[TLV_TEST_VALUE_ID_9].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_9].string);

        /* stop test run if set up phase fails */
        ASSERT_NE((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_4));
        ASSERT_NE((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_5));
        ASSERT_NE((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_6));
        ASSERT_NE((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_7));

        TLV_RemoveElement(Handle, TLV_TEST_TYPE_3);
        TLV_RemoveElement(Handle, TLV_TEST_TYPE_5);
        TLV_RemoveElement(Handle, TLV_TEST_TYPE_7);
        TLV_RemoveElement((TLV_GroupHandle_TP)NULL, TLV_TEST_TYPE_6);

        /* check test results */
        EXPECT_NE((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_4));
        EXPECT_EQ((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_5));
        EXPECT_NE((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_6));
        EXPECT_EQ((TLV_Element_T *)NULL, TLV_GetElement(Handle, TLV_TEST_TYPE_7));

        /* clean up resources */
        TLV_RemoveGroup(Handle);
    }
}

TEST_F(TLVTest, tlvElementGet)
{
    /**
     * @testcase{TLV::tlvElementGet: }
     *
     * The API under test is TLV_GetElement(). Requirements against the API:
     * - return NULL if passed group Handle is invalid
     * - return NULL if requested TLV Type is not present in group
     * - return a valid TLV Handle if TLV Type is present in group
     */

    {
        TLV_GroupHandle_TP Handle;
        TLV_Element_T *elementHandle;

        Handle = TLV_AddGroup(DataBuffer, dataBufferSize);

        typedef struct parameterSet_s
        {
            tlvTestTypes_t Type;
            bool expectedResult;
        } parameterSet_t;

        parameterSet_t parameterSet[] = {
            {TLV_TEST_TYPE_0, false},
            {TLV_TEST_TYPE_3, false},
            {TLV_TEST_TYPE_7, true},
            {TLV_TEST_TYPE_8, true},
            {TLV_TEST_TYPE_9, false}};

        ASSERT_NE((TLV_GroupHandle_TP)NULL, Handle);

        /* add TLVs to the group */
        TLV_AddElement(Handle, TLV_TEST_TYPE_8, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].string);
        TLV_AddElement(Handle, TLV_TEST_TYPE_3, tlvTestValuesTable[TLV_TEST_VALUE_ID_7].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_7].string);
        TLV_AddElement(Handle, TLV_TEST_TYPE_7, tlvTestValuesTable[TLV_TEST_VALUE_ID_6].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_6].string);

        /* remove TLV TLV_TEST_TYPE_3 */
        TLV_RemoveElement(Handle, TLV_TEST_TYPE_3);

        for (uint8_t index = 0; index < (sizeof(parameterSet) / sizeof(parameterSet_t)); index++)
        {
            elementHandle = TLV_GetElement(Handle, parameterSet[index].Type);

            if (parameterSet[index].expectedResult)
            {
                /* API call should return a valid Handle */
                EXPECT_NE((TLV_Element_T *)NULL, elementHandle);
            }
            else
            {
                /* API call should return invalid Handle */
                EXPECT_EQ((TLV_Element_T *)NULL, elementHandle);
            }
        }

        /* Testing the negative case by passing NULL as parameter */
        elementHandle = TLV_AddElement(Handle, TLV_TEST_TYPE_8, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].Length, NULL);
        EXPECT_EQ((TLV_Element_T *)NULL, elementHandle);

        /* Testing the negative case by passing NULL as parameter */
        elementHandle = TLV_AddElement(NULL, TLV_TEST_TYPE_8, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].Length, tlvTestValuesTable[TLV_TEST_VALUE_ID_8].string);
        EXPECT_EQ((TLV_Element_T *)NULL, elementHandle);
    }
}

TEST_F(TLVTest, IsElementValidFail)
{
    /**
     * @testcase{TLV::IsElementValidFail: }
     *
     * The API under test is IsElementValid(). Requirements against the API:
     */
    TLV_Element_T element;
    uint32_t status = 1;
    uint8_t dataVal = 0;
    element.DataBuffer = &dataVal;
    element.DataLength = 0U;
    status = IsElementValid(&element);
    EXPECT_EQ(0U, status);
}
TEST_F(TLVTest, GarbageCollectGroupTest)
{
    /**
     * @testcase{TLV::GarbageCollectGroupTest: }
     *
     * The API under test is GarbageCollectGroup(). Requirements against the API:
     */
    TLV_GroupHandle_TP Handle;
    uint16_t sizeLeft = 0;

    typedef struct parameterSet_s
    {
        void *Buffer;
        uint16_t bufferSize;
        bool expectedResult;
    } parameterSet_t;

    parameterSet_t parameterSet[] = {
        {DataBuffer, dataBufferSize, true},
    };
    Handle = TLV_AddGroup(parameterSet[0].Buffer, parameterSet[0].bufferSize);
    (void)TLV_AddElement(Handle, TLV_TEST_TYPE_0, 0, "");

    sizeLeft = GarbageCollectGroup(Handle);
    EXPECT_EQ((dataBufferSize - sizeof(TLV_Element_T)), sizeLeft);
}
/** ************************************************************************* */
#else
}
#endif /* if KISO_FEATURE_TLV */
