/*----------------------------------------------------------------------------*/
/*
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 */
/*----------------------------------------------------------------------------*/

/**
 * @file
 *
 * @brief Controls board specific parts of the module.
 */

/* This is used to define the correct module ID for generation of module
 * error codes
 */

#define BCDS_MODULE_ID  BCDS_CELLULAR_MODULE_ID_BSP

#include "CellularModule.h"
#include "BCDS_Assert.h"
#include "BSP_BoardType.h"
#include "BCDS_CellularConfig.h"
#include "BCDS_BSP_UMTS_QuectelUC20.h"
#include "CellularBsp.h"

static bool initialized = false;
static UART_T umtsUartHandle;

/**
 * @brief Initializes this module by setting up GPIO pins and a serial interface.
 * After this method has been called successfully (returned RETCODE_OK),
 * CellularBsp_GetCommunicationChannel() returns a valid value.
 *
 * @return RETCODE_OK          If the cellular hardware was initialized
 * @return RETCODE_FAILURE     If the serial initialization failed
 */
__attribute__(( weak )) Retcode_T CellularBsp_Init(
        MCU_UART_Callback_T uartCallback, uint8_t *uartRxByte)
{
    Retcode_T retcode;

    if (initialized)
    {
        return RETCODE(RETCODE_SEVERITY_WARNING, RETCODE_DOPPLE_INITIALIZATION);
    }

    umtsUartHandle = BSP_UMTS_QuectelUC20_GetUARTHandle();
    retcode = MCU_UART_Initialize(umtsUartHandle, uartCallback);

    if (RETCODE_OK != retcode)
    {
        return retcode;
    }

    retcode = BSP_UMTS_QuectelUC20_Enable();
    /* Start the receive process, this will enable UART interrupts and trigger
     * a callback on receive
     */
    (void) MCU_UART_Receive(umtsUartHandle, uartRxByte, UINT32_C(1));

    initialized = (RETCODE_OK == retcode);

    return retcode;
}

/**
 * @brief Powers on the cellular modem by triggering the POWER_ON GPIO pin.
 * This method does not communicate with the modem, so it has to be
 * initialized afterwards. The register UART RX IRQ is also activated
 *
 * @return RETCODE_OK              If the HW pins were switched correctly.
 *                                    This does not guarantee that the modem started.
 * @return RETCODE_UNINITIALIZED   If the BspCellular was not initialized correctly yet.
 */
__attribute__(( weak )) Retcode_T CellularBsp_Startup(void)
{
    if (!initialized)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    BSP_UMTS_QuectelUC20_PowerOn();
    return RETCODE_OK;
}

/**
 * @brief Powers off the cellular modem by triggering the POWER_ON GPIO pin.
 *
 * @return RETCODE_OK              If the HW pins were switched correctly
 *                                 and the modem is power off.
 *
 * @return RETCODE_UNINITIALIZED   If the BspCellular was not initialized correctly yet.
 */
__attribute__(( weak )) Retcode_T CellularBsp_Shutdown(void)
{
    if (!initialized)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    BSP_UMTS_QuectelUC20_PowerOff();
    return RETCODE_OK;
}

/**
 * @brief Resets the cellular modem by triggering the RESET GPIO pin.
 * This method does not guarantee that the modem was correctly reset.
 * Verify the correct restart by listening on the UART.
 *
 * @return RETCODE_OK              If the HW pins were switched correctly.
 * @return RETCODE_UNINITIALIZED   If the BspCellular was not initialized correctly yet.
 */
__attribute__(( weak )) Retcode_T CellularBsp_Reset(void)
{
    if (!initialized)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    BSP_UMTS_QuectelUC20_Reset();
    return RETCODE_OK;
}

/**
 * @brief Returns the handle for communicating with the modem. Before calling
 * CellularBsp_Init() this method returns NULL. After successful initialization
 * this method returns the initialized UART handle.
 *
 * @return NULL or a serial device handle for communicating with the modem.
 */
__attribute__(( weak )) Retcode_T CellularBsp_GetCommunicationChannel(
        UART_T* uartHandle)
{
    if (NULL == uartHandle)
    {
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_NULL_POINTER);
    }

    if (!initialized)
    {
        *uartHandle = (UART_T) NULL;
        return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    *uartHandle = umtsUartHandle;
    return RETCODE_OK;
}

/** @brief   Deinitialize the module
 *
 *  @warning CellularBsp_Shutdown should be previously called to avoid any interrupt during the Deinit
 *
 * @return  RETCODE_OK if the module is successfully deinitialized, a retcode compose otherwise.
 *
 */
__attribute__(( weak )) Retcode_T CellularBsp_Deinit(void)
{
    Retcode_T retcode;

    if (initialized)
    {
        uint8_t receivedByte;
        /* Stop the receive process, this will disable UART interrupts */
        (void) MCU_UART_Receive(umtsUartHandle, &receivedByte, UINT32_C(0));

        retcode = BSP_UMTS_QuectelUC20_Disable();
        if (RETCODE_OK == retcode)
        {
            retcode = MCU_UART_Deinitialize(umtsUartHandle);
        }
        if (RETCODE_OK == retcode)
        {
            initialized = false;
        }
    }
    else
    {
        retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_UNINITIALIZED);
    }

    return retcode;
}
