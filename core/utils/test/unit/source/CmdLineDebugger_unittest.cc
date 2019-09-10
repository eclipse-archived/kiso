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

/* include gtest interface */
#include <gtest.h>

extern "C"
{

#define KISO_MODULE_ID 0

#include "Kiso_Utils.h"
#if KISO_FEATURE_CMDLINEDEBUGGER

#include "Kiso_Basics.h"
#include "Kiso_Retcode_th.hh"
#include "Kiso_Assert_th.hh"

/* Module under Test */
#include "CmdLineDebugger.c"

} /* End of extern-C */

extern "C"
{

FAKE_VALUE_FUNC(Retcode_T, testCallback, uint32_t, const char* const*)



struct CmdLineDbg_Element_S testCommand = {
        testCallback, /* Callback */
        (const char *) "TestCommand", /* commandString*/
        NULL
};

struct CmdLineDbg_Element_S testCommandNull = {
        NULL,
        (const char *) "TestCommandNull",
        NULL
};

/* Global variable containing the Test List */
struct CmdLineDbg_Element_S testList[] =
        {
                {
                        testCallback, /* Callback */
                        (const char *) "TestCommand", /* commandString*/
                        NULL
                },
                {
                        testCallback, /* Callback */
                        (const char *) "Test1", /* commandString*/
                        NULL
                },
                {
                        testCallback, /* Callback */
                        (const char *) "Test2", /* commandString*/
                        NULL
                },
                {
                        testCallback, /* Callback */
                        (const char *) "sudo", /* commandString*/
                        NULL
                },
                {
                        testCallback, /* Callback */
                        (const char *) "Help", /* commandString*/
                        NULL
                }
        };

size_t const testListElements = sizeof(testList) / sizeof(testList[0]);

const char * testStrings[] =
        {
                (const char*) "TestCommand", /* 0 */
                (const char *) "Test1", /* 1�*/
                (const char *) "Test2", /* 2 */
                (const char *) "sudo", /* 3 */
                (const char *) "Help", /* 4 */
                (const char *) "unknown" /* 5 */
        };

}

class CommandLineInterface: public testing::Test
{
protected:

    /* Copy of the Test List object */
    struct CmdLineDbg_Element_S * testListCpy;

    virtual void SetUp()
    {
        FFF_RESET_HISTORY()

        RESET_FAKE(testCallback);

        /* Make a copy of the test object */
        testListCpy = (struct CmdLineDbg_Element_S *) malloc(sizeof(testList));
        memcpy((void *) testListCpy, (void *) testList, sizeof(testList));

    }

    virtual void TearDown()
    {
        /* Restore Test Element from memory */
        memcpy((void *) testList, (void *) testListCpy, sizeof(testList));
        free(testListCpy);

    }
};

/* Tests for the Public part */


/* Tests for the Command Register Method */
TEST_F(CommandLineInterface, RegisterCommand)
{
    Retcode_T rc;

    ASSERT_EQ(NULL, testList[0].next);
    ASSERT_EQ(NULL, testList[1].next);
    ASSERT_EQ(NULL, testList[2].next);
    ASSERT_EQ(NULL, testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    /* Register a list element with itself */
    rc = CmdLineDbg_RegisterCmd(testList, &testList[0]);
    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(NULL, testList[0].next);
    ASSERT_EQ(NULL, testList[1].next);
    ASSERT_EQ(NULL, testList[2].next);
    ASSERT_EQ(NULL, testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    rc = CmdLineDbg_RegisterCmd(testList, &testList[1]);
    ASSERT_EQ(RETCODE_OK, rc);

    ASSERT_EQ(&testList[1], testList[0].next);
    ASSERT_EQ(NULL, testList[1].next);
    ASSERT_EQ(NULL, testList[2].next);
    ASSERT_EQ(NULL, testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    rc = CmdLineDbg_RegisterCmd(testList, &testList[2]);
    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(&testList[1], testList[0].next);
    ASSERT_EQ(&testList[2], testList[1].next);
    ASSERT_EQ(NULL, testList[2].next);
    ASSERT_EQ(NULL, testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    rc = CmdLineDbg_RegisterCmd(testList, &testList[3]);
    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(&testList[1], testList[0].next);
    ASSERT_EQ(&testList[2], testList[1].next);
    ASSERT_EQ(&testList[3], testList[2].next);
    ASSERT_EQ(NULL, testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    rc = CmdLineDbg_RegisterCmd(testList, &testList[4]);
    ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(&testList[1], testList[0].next);
    ASSERT_EQ(&testList[2], testList[1].next);
    ASSERT_EQ(&testList[3], testList[2].next);
    ASSERT_EQ(&testList[4], testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);
}

TEST_F(CommandLineInterface, RegisterCmdNUll)
{
    Retcode_T rc;

    rc = CmdLineDbg_RegisterCmd(NULL, &testList[0]);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

    rc = CmdLineDbg_RegisterCmd(testList, NULL);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}

/**
 *  Test Case used to test the RegisterCmdArray
 */
TEST_F(CommandLineInterface, RegisterArray)
{
    Retcode_T rc;

    /*
     * testList only contains null elements
     */
    ASSERT_EQ(NULL, testList[0].next);
    ASSERT_EQ(NULL, testList[1].next);
    ASSERT_EQ(NULL, testList[2].next);
    ASSERT_EQ(NULL, testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    rc = CmdLineDbg_RegisterCmdArray(testList, 5);

    ASSERT_EQ(&testList[1], testList[0].next);
    ASSERT_EQ(&testList[2], testList[1].next);
    ASSERT_EQ(&testList[3], testList[2].next);
    ASSERT_EQ(&testList[4], testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

    ASSERT_EQ(RETCODE_OK, rc);

}

/**
 *  Try to register element null
 */
TEST_F(CommandLineInterface, RegisterArray_NULL)
{
    Retcode_T rc;

    rc = CmdLineDbg_RegisterCmdArray(NULL, 1);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

    rc = CmdLineDbg_RegisterCmdArray(testList, 0);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM), rc);
}

TEST_F(CommandLineInterface, ConvertArrayToLinkedList_Complex)
{
    ASSERT_EQ(NULL, testList[0].next);
    ASSERT_EQ(5u, testListElements);

    CmdLineDbg_RegisterCmdArray(testList, 1);
    ASSERT_EQ(NULL, testList[0].next);

    CmdLineDbg_RegisterCmdArray(testList, testListElements);
    /* Append Command in List */
    //ASSERT_EQ(RETCODE_OK, rc);
    ASSERT_EQ(&testList[1], testList[0].next);
    ASSERT_EQ(&testList[2], testList[1].next);
    ASSERT_EQ(&testList[3], testList[2].next);
    ASSERT_EQ(&testList[4], testList[3].next);
    ASSERT_EQ(NULL, testList[4].next);

}



/* ************************************************************************** */
/* COMMAND FINDER TESTS ***************************************************** */
/* ************************************************************************** */
TEST_F(CommandLineInterface, FindCommandByString)
{
    struct CmdLineDbg_Element_S * result;

    ASSERT_STREQ(testList[0].commandString, "TestCommand");
    ASSERT_STREQ(testList[1].commandString, "Test1");
    ASSERT_STREQ(testList[2].commandString, "Test2");
    ASSERT_STREQ(testList[3].commandString, "sudo");
    ASSERT_STREQ(testList[4].commandString, "Help");

    /* Initialize Linked List */
    CmdLineDbg_RegisterCmdArray(testList, testListElements);

    /* Find first command in list */
    result = CmdLine_FindCmd(testList, "TestCommand");
    ASSERT_EQ(&testList[0], result);

    /* Find second command in list */
    result = CmdLine_FindCmd(testList, "Test1");
    ASSERT_EQ(&testList[1], result);

    /* Find third command in list */
    result = CmdLine_FindCmd(testList, "Test2");
    ASSERT_EQ(&testList[2], result);

    /* Find fourth command in list */
    result = CmdLine_FindCmd(testList, "sudo");
    ASSERT_EQ(&testList[3], result);

    /* Find fifth command in list */
    result = CmdLine_FindCmd(testList, "Help");
    ASSERT_EQ(&testList[4], result);

    /* Find sixth command in list */
    result = CmdLine_FindCmd(testList, "unknown");
    ASSERT_EQ(NULL, result); /* Command was not found */

}

TEST_F(CommandLineInterface, FindSoloCommand)
{
    struct CmdLineDbg_Element_S * result;

    ASSERT_STREQ(testList[0].commandString, testStrings[0]);
    ASSERT_STREQ(testList[1].commandString, testStrings[1]);
    //  ASSERT_STREQ((char * )(testList[2].commandString), (char * )testStrings[2]);
    ASSERT_STREQ(testList[3].commandString, testStrings[3]);

    /* Linked list is not initialized! The Command line interface only sees one command in the list */
    /* This fixes a bug where the FindCmd function was jumping directly to the second element of the list */
    result = CmdLine_FindCmd(testList, "TestCommand");

    ASSERT_EQ(&testList[0], result);
    ASSERT_STREQ(testList[0].commandString, "TestCommand");

    result = CmdLine_FindCmd(testList, "unknown");
    ASSERT_EQ(NULL, result); /* Command was not found */

}



/* ************************************************************************** */
/* TOKEN EXTRACTOR TESTS **************************************************** */
/* ************************************************************************** */
TEST_F(CommandLineInterface, TokenizeString1)
{
    char parserTest[] = " Test1 param1 param2 param3  param4 ";
    char parserTest1[] = "  ";
    char parserTest2[] = "Test1";
    char parserTest3[] = "Test1 param1";


    char * argv[16];

    size_t i = CmdLine_ExtractTokensFromString(argv, parserTest); /* Split elements by inserting string-terminators */
    EXPECT_EQ((size_t)5,i); /* 5-Arguments found */
    ASSERT_STREQ("Test1", argv[0]);
    ASSERT_STREQ("param1", argv[1]);
    ASSERT_STREQ("param2", argv[2]);
    ASSERT_STREQ("param3", argv[3]);
    ASSERT_STREQ("param4", argv[4]);

    i = CmdLine_ExtractTokensFromString(argv, parserTest1);
    EXPECT_EQ((size_t)0,i); /* 0-Arguments found */
    ASSERT_EQ(NULL, argv[0]);

    i = CmdLine_ExtractTokensFromString(argv, parserTest2);
    EXPECT_EQ((size_t)1,i); /* 1 Argument found */
    ASSERT_STREQ("Test1", argv[0]);
    ASSERT_EQ(NULL, argv[1]);

    i = CmdLine_ExtractTokensFromString(argv, parserTest3);
    EXPECT_EQ((size_t)2,i); /* 2 Arguments found */
    ASSERT_STREQ("Test1", argv[0]);
    ASSERT_STREQ("param1", argv[1]);
    ASSERT_EQ(NULL, argv[2]);
}

TEST_F(CommandLineInterface, TokenizeString2)
{
    char inputString[] = "git add file1 file2 file3";

    char * GitParameters[16];
    size_t i;

    i = CmdLine_ExtractTokensFromString(GitParameters, inputString);

    EXPECT_EQ((size_t)5, i);

    EXPECT_EQ(&inputString[0], GitParameters[0]);
    EXPECT_EQ(&inputString[4], GitParameters[1]);
    EXPECT_EQ(&inputString[4+4], GitParameters[2]);
    EXPECT_EQ(&inputString[4+10], GitParameters[3]);
    EXPECT_EQ(&inputString[4+16], GitParameters[4]);
    EXPECT_EQ(NULL, GitParameters[5]);
}



/* ************************************************************************** */
/* COMMAND EXECUTOR TESTS *************************************************** */
/* ************************************************************************** */
TEST_F(CommandLineInterface,CmdLine_ExecuteCmd)
{
  const char * argv[] = {
        "TestBuffer",
        "param1",
        "param2",
        "param3",
  };

  size_t argc = sizeof(argv)/sizeof(argv[0]);

  Retcode_T rc;

  /* Test NULL pointer check */
  rc = CmdLine_ExecuteCmd(NULL, argv, argc);
  ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

  /* Test with invalid callback */
  rc = CmdLine_ExecuteCmd(&testCommandNull, argv, argc);
  ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

  /* Execute Command and Inspect Callback Arguments*/
  rc = CmdLine_ExecuteCmd(&testCommand, argv, argc);
  ASSERT_EQ((const char *)argv, (const char *)testCallback_fake.arg1_val);
  ASSERT_EQ((size_t)4, testCallback_fake.arg0_val);
  ASSERT_EQ(RETCODE_OK, rc);

}

/* ************************************************************************** */
/* COMMAND PARSER TESTS ***************************************************** */
/* ************************************************************************** */
TEST_F(CommandLineInterface, CmdLine_Parse_NULL)
{
    char inputBuffer[] = "TestCommand param1 param2";
    Retcode_T rc;

    /* NULL Checks */
    rc = CmdLineDbg_Parse(NULL, inputBuffer);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);

    rc = CmdLineDbg_Parse(testList, NULL);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER), rc);
}

TEST_F(CommandLineInterface, CmdLine_Parse_VALID)
{
    char inputBuffer[] = "TestCommand param1 param2";
    Retcode_T rc;

    /*  Valid input */
     rc = CmdLineDbg_Parse(testList, inputBuffer);
     ASSERT_EQ(RETCODE_OK, rc);
     ASSERT_EQ(3u, testCallback_fake.arg0_val);

}

TEST_F(CommandLineInterface, CmdLine_Parse_INVALID_CMD)
{
    char inputBuffer[] = "Test param1 param2";
    Retcode_T rc;

    /* Searching for an unknown command: */
    rc = CmdLineDbg_Parse(testList, inputBuffer);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CMDLINE_DEBUGGER_COMMAND_NOT_FOUND), rc);

    ASSERT_EQ(0u, testCallback_fake.arg0_val);
}

TEST_F(CommandLineInterface, CmdLine_Parse_NOARC)
{
    char inputBuffer[] = "  ";
    Retcode_T rc;

    /* Searching for an unknown command: */
    rc = CmdLineDbg_Parse(testList, inputBuffer);
    ASSERT_EQ(RETCODE(RETCODE_SEVERITY_INFO, RETCODE_CMDLINE_DEBUGGER_COMMAND_NOT_FOUND), rc);

    ASSERT_EQ(0u, testCallback_fake.arg0_val);
}
#else
}
#endif /* if KISO_FEATURE_CMDLINEDEBUGGER */
