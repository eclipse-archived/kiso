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


#include "BCDS_TestAppInfo.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_TEST_APP_MODULE_ID_TEST_SECTION

/* additional interface header files */
#include "BCDS_Tests.h"

#include "BCDS_TestSection.h"

#include "TestSuite_I2C.h"
#include "TestSuite_SPI.h"
#include "TestSuite_UART.h"
#include "TestSuite_TIMER.h"
#include "TestSuite_Sleep.h"

#define TEST_SECTION_ID 1

Retcode_T TestSection_Setup(CCMsg_T *ccmsg);
Retcode_T TestSection_Teardown(CCMsg_T *ccmsg);

Retcode_T TestSection_Initialize(void)
{
    Retcode_T retcode = RETCODE_OK;

    retcode = Tests_Initialize(TEST_SECTION_ID, TestSection_Setup, TestSection_Teardown);
#ifdef efm32
    if (RETCODE_OK == retcode)
    {
        retcode = TestSuite_I2C_initialize((uint8_t)1);
    }

    if (RETCODE_OK == retcode)
    {
        retcode = TestSuite_SPI_initialize((uint8_t)2);
    }
#endif /* efm32 */
    if (RETCODE_OK == retcode)
    {
        retcode = TestSuite_UART_initialize((uint8_t)3);
    }
#ifdef efm32
    if(RETCODE_OK == retcode)
    {
        retcode = TestSuite_Timer_initialize((uint8_t)4);
    }
#endif /* efm32 */
    return retcode;
}

Retcode_T TestSection_Setup(CCMsg_T *ccmsg)
{
	BCDS_UNUSED(ccmsg);
	return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED);
}


Retcode_T TestSection_Teardown(CCMsg_T *ccmsg)
{
	BCDS_UNUSED(ccmsg);
	return RETCODE(RETCODE_SEVERITY_FATAL, RETCODE_NOT_SUPPORTED);
}
