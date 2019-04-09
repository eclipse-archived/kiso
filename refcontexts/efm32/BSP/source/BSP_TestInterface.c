
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
 * @ingroup BSP_BSE_TEST_IF
 * @{
 */
/**
 * @file
 * @brief  Implementation of test interface BSP functions
 */

#include "BCDS_BSP.h"
#undef BCDS_MODULE_ID
#define BCDS_MODULE_ID BCDS_BSP_MODULE_ID_TEST_INTERFACE

#include "BCDS_HAL.h"

#if BCDS_FEATURE_BSP_TEST_INTERFACE

#include "BSP_TestInterface.h"
#include "BSP_BoardSettings.h"
#include "BSP_BoardShared.h"
#if BCDS_BOARD_XDK110
#include "BCDS_USB.h"
#elif BCDS_BOARD_APLM
#include "BCDS_MCU_LEUART.h"
#include "BCDS_MCU_LEUART_Handle.h"
#endif

#if BCDS_BOARD_XDK110

static USB_lineCoding_tp BSP_TestIF_USB_Handle = NULL;

static USB_rxCallback TestIF_USB_ISRCallback = NULL;
#endif /*BCDS_BOARD_XDK110*/

#if BCDS_BOARD_APLM

struct MCU_LEUART_Handle_S BSP_LEUART_Handle;

#endif /*BCDS_BOARD_APLM*/



#if BCDS_BOARD_XDK110
/*
 * Refer to interface header for description
 */
Retcode_T BSP_TestInterface_Connect(void)
{
    BSP_TestIF_USB_Handle = &USB_lineCoding_mds;
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_TestInterface_Enable(void)
{
    /* Dont enable if the callback has not been registered */
    if ( NULL == BSP_TestIF_USB_Handle || NULL == BSP_TestIF_USB_Handle->usbRxCallback)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    TestIF_USB_ISRCallback = BSP_TestIF_USB_Handle->usbRxCallback;
    return RETCODE_OK;
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_TestInterface_Disable(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}

/*
 * Refer to interface header for description
 */
Retcode_T BSP_TestInterface_Disconnect(void)
{
    return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NOT_SUPPORTED);
}
/*
 * Refer to interface header for description
 */
HWHandle_T BSP_TestInterface_GetUSBHandle(void)
{
    if (NULL == BSP_TestIF_USB_Handle)
    {
        return (HWHandle_T) NULL;
    }

    return (HWHandle_T) BSP_TestIF_USB_Handle;
}

#endif /*BCDS_BOARD_XDK110*/

#if BCDS_BOARD_APLM

Retcode_T BSP_TestInterface_Connect(void)
{
    CMU_ClockEnable(BSP_SERIAL_LEUART_CLOCK, true);

	/* configure the LEUART RX pin as input */
	GPIO_PinModeSet(BSP_LEUART_RX_PORT, BSP_LEUART_RX_PIN, BSP_LEUART_RX_MODE, BSP_LEUART_RX_DOUT);
	/* configure the LEUART TX pin as output */
	GPIO_PinModeSet(BSP_LEUART_TX_PORT, BSP_LEUART_TX_PIN, BSP_LEUART_TX_MODE, BSP_LEUART_TX_DOUT);

    BSP_LEUART_Handle.leuart_ptr = BSP_SERIAL_LEUART_PORT;
    BSP_LEUART_Handle.TransferMode = BCDS_HAL_TRANSFER_MODE_INTERRUPT;

	BSP_SERIAL_LEUART_PORT->ROUTE = LEUART_ROUTE_LOCATION_LOC0 | LEUART_ROUTE_TXPEN | LEUART_ROUTE_RXPEN;
    return RETCODE_OK;
}


Retcode_T BSP_TestInterface_Enable(void)
{
    Retcode_T retcode = RETCODE_OK;

    NVIC_SetPriority(LEUART1_IRQn, 5);

	/* clear pending interrupts */
	NVIC_ClearPendingIRQ(LEUART1_IRQn);

	NVIC_EnableIRQ(LEUART1_IRQn);

    return retcode;
}


Retcode_T BSP_TestInterface_Disable(void)
{
    Retcode_T retcode = RETCODE_OK;
	NVIC_DisableIRQ(LEUART1_IRQn);
    return retcode;
 }


Retcode_T BSP_TestInterface_Disconnect(void)
{
    GPIO_PinModeSet(BSP_LEUART_RX_PORT, BSP_LEUART_RX_PIN, gpioModeDisabled, 0);

    GPIO_PinModeSet(BSP_LEUART_TX_PORT, BSP_LEUART_TX_PIN, gpioModeDisabled, 0);

    CMU_ClockEnable(BSP_SERIAL_LEUART_CLOCK, true);

    BSP_SERIAL_LEUART_PORT->ROUTE = _LEUART_ROUTE_RESETVALUE;

    CMU_ClockEnable(BSP_SERIAL_LEUART_CLOCK, false);

    return RETCODE_OK;
}


HWHandle_T BSP_TestInterface_GetLEUARTHandle(void)
{

    return (HWHandle_T) &BSP_LEUART_Handle;
}

void BSP_LEUART1_IRQHandler(void)
{
    if (NULL != BSP_LEUART_Handle.IrqRxCallback)
    {
        /* All LEUART interrupt processing is to be done at the MCU level */
    	BSP_LEUART_Handle.IrqRxCallback((LEUART_T) &BSP_LEUART_Handle);
    	BSP_LEUART_Handle.IrqTxCallback((LEUART_T) &BSP_LEUART_Handle);
    }
}
#endif /*BCDS_BOARD_APLM*/

#endif /* BCDS_FEATURE_BSP_TEST_INTERFACE */

/**@}*/
