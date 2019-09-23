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

#include "Kiso_CellularModules.h"
#define KISO_MODULE_ID KISO_CELLULAR_MODULE_ID_HARDWARE

#include "Hardware.h"
#include "Kiso_CellularConfig.h"

#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "Kiso_Assert.h"

#include "Kiso_MCU_UART.h"

static bool Initialized = false;
static HWHandle_T UartHandle;
static uint8_t* UartByte;

Retcode_T Hardware_Initialize(MCU_UART_Callback_T uartCallback, uint8_t* uartRxByte)
{
    if (Initialized)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_DOUBLE_INITIALIZATION);
    }

    Retcode_T retcode = CELLULAR_BSP_CONNECT();

    if (RETCODE_OK == retcode)
    {
        UartHandle = CELLULAR_BSP_GET_UART_HANDLE();
        UartByte = uartRxByte;
        retcode = MCU_UART_Initialize((UART_T) UartHandle, uartCallback);
    }

    if (RETCODE_OK == retcode)
    {
        Initialized = true;
    }

    return retcode;
}

Retcode_T Hardware_Startup(void)
{
    Retcode_T retcode;

    if (Initialized)
    {
        retcode = CELLULAR_BSP_ENABLE();

        if (RETCODE_OK == retcode)
        {
            /* Start the receive process, this will enable UART interrupts and trigger
             * a callback on receive
             */
            retcode = MCU_UART_Receive((UART_T) UartHandle, UartByte, UINT32_C(1));
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    return retcode;
}

Retcode_T Hardware_Shutdown(void)
{
    Retcode_T retcode;

    if (Initialized)
    {
        /* Stop the receive process, this will disable UART interrupts */
        retcode = MCU_UART_Receive((UART_T) UartHandle, UartByte, UINT32_C(0));

        if (RETCODE_OK == retcode)
        {
            retcode = CELLULAR_BSP_DISABLE();
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    return retcode;
}

Retcode_T Hardware_Reset(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (Initialized)
    {
        retcode = CELLULAR_BSP_RESET();
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    return retcode;
}

Retcode_T Hardware_GetCommunicationChannel(UART_T* uartHandle)
{
    Retcode_T ret = RETCODE_OK;

    if (NULL == uartHandle)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (Initialized)
    {
        *uartHandle = (UART_T) UartHandle;
    }
    else
    {
        *uartHandle = (UART_T) NULL;
        ret = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    return ret;
}

Retcode_T Hardware_Deinitialize(void)
{
    Retcode_T retcode = RETCODE_OK;

    if (Initialized)
    {
        retcode = MCU_UART_Deinitialize((UART_T) UartHandle);

        if (RETCODE_OK == retcode)
        {
            retcode = CELLULAR_BSP_DISCONNECT();
        }

        if (RETCODE_OK == retcode)
        {
            Initialized = false;
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    return retcode;
}
