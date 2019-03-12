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
#ifndef BCDS_USB_TH_HH_
#define BCDS_USB_TH_HH_

/* include system header files */
#include "em_usb_th.hh"

/* wrap the real interface header */
#include "BCDS_USB.h"

/* faked variables needs to be initialized by the test fixture */

/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(USB_DisConnect);
FAKE_VOID_FUNC(USB_init,USB_lineCoding_tp, USB_rxCallback);
FAKE_VALUE_FUNC(USB_returnCode_t , USB_transmitByte , uint8_t);
FAKE_VALUE_FUNC(USB_returnCode_t , USB_receiveByte , uint8_t*);
FAKE_VALUE_FUNC(USB_returnCode_t , USB_transmitData , uint8_t*, uint32_t);
FAKE_VOID_FUNC(USB_stateChange, USBD_State_TypeDef, USBD_State_TypeDef);
FAKE_VALUE_FUNC(int, USB_setupCommand, const USB_Setup_TypeDef*);
FAKE_VALUE_FUNC(USB_returnCode_t,USB_callBackMapping,USB_rxCallback );
#endif /* BCDS_USB_TH_HH_ */

/** ************************************************************************* */
