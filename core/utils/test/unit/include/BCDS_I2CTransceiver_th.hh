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

#ifndef BCDS_I2CTRANSCEIVER_TH_HH_
#define BCDS_I2CTRANSCEIVER_TH_HH_

/* Include the real interface header */
#include "BCDS_I2CTransceiver.h"

/* Include the needed headers */

/* Faked variables needs to be initialized by the test fixture */

/* Mock-ups for the provided interfaces */
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Init,I2cTranceiverHandlePtr_T,I2C_T);
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Read,I2cTranceiverHandlePtr_T,uint8_t,uint8_t,uint8_t*,uint8_t);
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Write,I2cTranceiverHandlePtr_T,uint8_t,uint8_t,uint8_t*,uint8_t);
FAKE_VOID_FUNC(I2CTransceiver_LoopCallback, I2cTranceiverHandlePtr_T,struct MCU_I2C_Event_S );
FAKE_VALUE_FUNC(Retcode_T, I2CTransceiver_Deinit,I2cTranceiverHandlePtr_T);
#endif /* BCDS_I2CTRANSCEIVER_TH_HH_ */
