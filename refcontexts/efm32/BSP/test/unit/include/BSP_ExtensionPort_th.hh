/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Bosch Connected Devices and Solutions GmbH. 
 * All Rights Reserved. Confidential.
 *
 * Distribution only to people who need to know this information in
 * order to do their job.(Need-to-know principle).
 * Distribution to persons outside the company, only if these persons
 * signed a non-disclosure agreement.
 * Electronic transmission, e.g. via electronic mail, must be made in
 * encrypted form.
 */
/*----------------------------------------------------------------------------*/

/* header definition ******************************************************** */
#ifndef BSP_EXTENSIONPORT_TH_HH_
#define BSP_EXTENSIONPORT_TH_HH_

/* include system header files */

/* wrap the real interface header */
#include "BSP_ExtensionPort.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */

FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ConnectSpi);
FAKE_VALUE_FUNC(HWHandle_T,BSP_ExtensionPort_GetSpiHandle);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_EnableSpi);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisableSpi);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisonnectSpi);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_SetSpiConfig,BSP_ExtensionPortSpiConfig_T,uint32_t,void *);
FAKE_VALUE_FUNC(HWHandle_T,BSP_ExtensionPort_GetI2cHandle);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ConnectI2c);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_EnableI2c);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisableI2c);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisonnectI2c);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_SetI2cConfig,BSP_ExtensionPortI2cConfig_T,uint32_t,void *);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisonnectUart);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisableUart);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_EnableUart);
FAKE_VALUE_FUNC(HWHandle_T,BSP_ExtensionPort_GetUartHandle);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ConnectUart);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_SetUartConfig,BSP_ExtensionPortUartConfig_T,uint32_t,void *);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ConnectGpio,BSP_ExtensionPort_PinId_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisconnectGpio,BSP_ExtensionPort_PinId_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ToggleGpio,BSP_ExtensionPort_PinId_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_SetGpio,BSP_ExtensionPort_PinId_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ClearGpio,BSP_ExtensionPort_PinId_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_ReadGpio,BSP_ExtensionPort_PinId_T,uint8_t *);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_SetGpioConfig,BSP_ExtensionPort_PinId_T,BSP_ExtensionPortGpioConfig_T,uint32_t,void *);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_EnableGpio,BSP_ExtensionPort_PinId_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_DisableGpio,BSP_ExtensionPort_PinId_T );
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_RegisterGpioInt,BSP_ExtensionPort_PinId_T, BSP_ExtensionPort_GpioIntMode_T,BSP_ExtensionPort_GpioCallback_T);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_Connect);
FAKE_VALUE_FUNC(Retcode_T,BSP_ExtensionPort_Disconnect);

#endif /* BSP_EXTENSIONPORT_TH_HH_ */

/** ************************************************************************* */
