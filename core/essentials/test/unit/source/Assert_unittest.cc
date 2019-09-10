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
 *  @file
 *
 *  @brief Module test specification for the KISO_Assert module.
 *
 **/

/* include gtest interface */
#include <gtest.h>

extern "C"
{

/* setup compile time configuration defines */

#undef assert
#undef for
#define for(X)	while(0)
#define STATIC_ASSERT_FLAG	UINT8_C(0)

#undef KISO_MODULE_ID
#define KISO_MODULE_ID KISO_ESSENTIALS_MODULE_ID_ASSERT

/* include faked interfaces */
#include "Kiso_Retcode_th.hh"

/* include module under test */
#include "Assert.c"

}

static uint8_t assertHitFlag;
static uint32_t assertHitLineNumber;

/** @testcase{ Assert::static_assert: }
 *
 * The following expectations are tested here :
 * - Static assertion of a macro.
 */
static_assert((UINT8_C(0) == STATIC_ASSERT_FLAG), "This macro is defined in the same file");

class Assert : public testing::Test
{

protected:

   static void assertCallbackFn(const unsigned long line, const unsigned char * const file)
   {
      /* @testcase{ Assert::assert: }
       *
       * The following expectations are tested here :
       * - Valid Filename.
       * - Valid Line Number.
       */

      const char assertFileName[] = __FILE__;

      assertHitFlag = UINT8_C(1);
      EXPECT_EQ(UINT8_C(0), strncmp((const char*)assertFileName, (const char*)file, sizeof(assertFileName)));

      EXPECT_EQ(assertHitLineNumber,line);
   }

   virtual void SetUp()
   {
      assertHitFlag = UINT8_C(0);

      FFF_RESET_HISTORY()
   }

   /* TearDown() is invoked immediately after a test finishes. */
   virtual void TearDown()
   {
      ; /* nothing to do if clean up is not required */
   }
};

/* specify test cases ******************************************************* */

/**
 * Module to test the callback registration
 */
TEST_F(Assert, Assert_Initialize)
{
   /** @testcase{ Assert::Assert_initialize: }
    *
    * The following expectations are tested here :
    * - Invalid Function registration.
    * - Valid Function registration.
    */

   Retcode_T retcode = Assert_Initialize(NULL);
   EXPECT_EQ(RETCODE_INVALID_PARAM, Retcode_GetCode(retcode));
   EXPECT_EQ(RETCODE_SEVERITY_FATAL, Retcode_GetSeverity(retcode));
   EXPECT_EQ((uint32_t)KISO_PACKAGE_ID, Retcode_GetPackage(retcode));
   EXPECT_EQ(KISO_ESSENTIALS_MODULE_ID_ASSERT, Retcode_GetModuleId(retcode));

   retcode = Assert_Initialize(assertCallbackFn);
   EXPECT_EQ(RETCODE_OK, retcode);
}

/**
 * Module to test dynamic assertion
 */
TEST_F(Assert, assert)
{
   /** @testcase{ Assert::assert: }
    *
    * The following expectations are tested here :
    * - Failing Assertion.
    * - Passing Assertion with valid callback.
    * - Passing Assertion with invalid callback.
    */

   assert(UINT8_C(1));
   EXPECT_EQ(UINT8_C(0),assertHitFlag);

   assertHitLineNumber = (uint32_t)__LINE__ + UINT32_C(1);
   assert(UINT8_C(0));
   EXPECT_EQ(UINT8_C(1),assertHitFlag);

   assertCallback = NULL;
   assertHitFlag = UINT8_C(0);
   assert(UINT8_C(0));
   EXPECT_EQ(UINT8_C(0),assertHitFlag);
}

