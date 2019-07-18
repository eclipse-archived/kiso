/******************************************************************************/
/** @file magic.cc
 *  @brief The module provides run-time relocated symbol address look-up APIs.
 *
 ******************************************************************************/

/******************************************************************************/
/** @page page_magic_id Header information of gtest extension module
 *******************************************************************************
 @par Copyright
 @verbatim
 Robert Bosch Kft.
 Gyomroi street 120.
 H-1103 Budapest

 Copyright (c) 2013           All rights reserved.
 This software is copyright protected and proprietary to Robert Bosch Kft.
 @endverbatim
 *******************************************************************************
 @par Authors
 @verbatim
 User        Name                  Department
 klt2bp      Tamas Kleiber         AE/EMS-Bp
 @endverbatim
 *******************************************************************************
 @par Project
 @verbatim
 Project: F011500111
 Processor: PC x86/64
 Compiler: GCC/MinGW
 @endverbatim
 *******************************************************************************
 @par Module history
 @verbatim
 2013-07-07   klt2bp   N/A   Initial implementation.
 2013-08-21   klt2bp   N/A   Support file static variables, assert on unrecoverable errors
 @endverbatim
 **************************************************************************** */

/******************************************************************************/
/** @defgroup module_magic_id gtest extension module
 *
 * The module implements a method to fetch the address of symbols with local
 * or file scope linkage at run-time. This makes it possible to reset static
 * local variables' run-time value within functions under tests.
 *
 * The module requires GCC's nm tool and has a few WinAPI dependencies. Code
 * is targeted for windows systems and GCC/MinGW compilers only.
 *
 **************************************************************************** */

/******************************************************************************/
/** @defgroup module_magic_id_cc C++ file of the gtest extension module
 * Definition section of the gtest extension module.
 *
 * @todo CPP file documentation.
 *
 * @ingroup module_magic_id
 * @{
 **************************************************************************** */

/* module definition ******************************************************** */

/* system header files */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <new>
#include <string>
#include <iostream>
#include <assert.h>

/* configure the pcre engine for static linking */
#define PCRE_STATIC
#include <pcrecpp.h>

/* additional interface header files */

/* own header files */
#include "magic.h"

/* constant definitions ***************************************************** */

#define ASSERT(expression, error, msg) \
    if(!expression) \
    { \
        std::cerr << error; \
        assert(expression); \
    } \


#define FAIL(error, msg) \
    std::cerr << error; \
    assert(0); \


#define FILE_NAME "unittest.nm"

/* local variables ********************************************************** */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/* global functions ********************************************************* */

Magic::Magic() :
    m_isInited(false), m_dataBuffer(NULL)
{

}

Magic::~Magic()
{
    if (m_dataBuffer)
    {
        delete[] m_dataBuffer;
    }

    DeleteFile(FILE_NAME);
}

/** @} ********************************************************************** */

void* Magic::GetAddress(const char* expression)
{
    string pattern("^([0-9a-fA-F]+)\\s[a-zA-Z]\\s.*:RULE:.*$");
    DWORD address = 0UL;

    if (!m_isInited)
    {
        InitDataFile();
    }

    // post process input expression
    {
        string symbol(expression);

        // in case of file scope static variables the :: separator is omitted
        if (0 == pcrecpp::RE("::").GlobalReplace(".*", &symbol))
        {
            // prepend negative look behind test to omit matching against the fake magic variable
            symbol = "(?<!MGC_)" + symbol;
        }

        {
            pcrecpp::StringPiece inputPiece(symbol);

            ASSERT(1 == pcrecpp::RE(":RULE:").GlobalReplace(inputPiece, &pattern),
                "[gtest::magic] Internal error 001.\n",
                "The :RULE: substring must be replaced by the symbol.")
        }
    }

    // fetch address from nm's temporary output
    {
        pcrecpp::RE_Options options;
        pcrecpp::StringPiece inputPiece(m_dataBuffer);

        options.set_multiline(true);
        ASSERT(pcrecpp::RE(pattern, options).PartialMatch(inputPiece, pcrecpp::Hex(&address)),
            std::string("[gtest::magic] Symbol \"") + std::string(expression) + std::string("\" could not be found.\n"),
            "The address must be found - otherwise exit program.")
    }

    ASSERT(0UL == address,
        std::string("[gtest::magic] Symbol \"") + std::string(expression) + std::string(" \"could not be found.\n"),
        "NULL is not a valid, allowed address!")

    return ((void*) address);
}

unsigned long Magic::GetFileSizeEx(void* hFile)
{
    unsigned long fileSize;

    /* get file size */
    fileSize = GetFileSize(hFile, NULL);

    if ((INVALID_FILE_SIZE == fileSize) && (NO_ERROR != GetLastError()))
    {
        /* error case */
        fileSize = 0UL;
    }

    return (fileSize);
}

void Magic::ReadData(void)
{
    HANDLE hFile = CreateFile(FILE_NAME, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

    if (INVALID_HANDLE_VALUE != hFile)
    {
        DWORD fileSize = (DWORD) GetFileSizeEx((void*) hFile);

        if (fileSize)
        {
            DWORD bytesRead;
            if (m_dataBuffer)
            {
                delete[] m_dataBuffer;
            }

            m_dataBuffer = new (std::nothrow) unsigned char[fileSize];

            if (m_dataBuffer)
            {
                ZeroMemory(m_dataBuffer, fileSize);

                if (ReadFile(hFile, m_dataBuffer, fileSize, &bytesRead, NULL) && (fileSize == bytesRead) && CloseHandle(hFile))
                {
                    return;
                }
            }
        }
    }

    FAIL("[gtest::magic] Internal error 002.\n", "Data read operation failed.")
}

void Magic::InitDataFile(void)
{
    TCHAR buffer[128];

    ZeroMemory(buffer, 128);

    if (GetModuleFileName(NULL, buffer, 128UL))
    {
        if (system(NULL))
        {
            DWORD status;
            char cmdline[255];

            ZeroMemory(cmdline, 255);
            snprintf(cmdline, 255, "nm --no-demangle %s >" FILE_NAME, buffer);
            status = system(cmdline);

            if (status == 0UL)
            {
                WIN32_FIND_DATA FindFileData;
                HANDLE handle;

                handle = FindFirstFile(FILE_NAME, &FindFileData);
                if (handle != INVALID_HANDLE_VALUE)
                {
                    FindClose(handle);
                }

                ReadData();

                m_isInited = true;

                return;
            }
        }
    }

    FAIL("[gtest::magic] Internal error 003.\n", "System initialization failed.")
}
