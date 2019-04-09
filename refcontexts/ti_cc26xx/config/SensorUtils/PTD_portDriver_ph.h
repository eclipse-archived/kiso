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


#ifndef BCDS_PTD_PORTDRIVER_PH_H_
#define BCDS_PTD_PORTDRIVER_PH_H_

#define PTD_CONFIG_ENABLED        0XAA
#define PTD_CONFIG_DISABLED       0X99

#define PTD_GPIO_ODD_INTERRUPT    PTD_CONFIG_ENABLED
#define PTD_GPIO_EVEN_INTERRUPT   PTD_CONFIG_ENABLED

/* DATA pin for I2C */
#define PTD_PORT_I2C0_SDA2        (gpioPortA)
#define PTD_PIN_I2C0_SDA2         (UINT16_C(0))
#define PTD_MODE_I2C0_SDA2        (gpioModeWiredAnd)
#define PTD_DRIVE_MODE_I2C0_SDA2  (gpioDriveModeStandard)
#define PTD_DOUT_I2C0_SDA2        (UINT16_C(1))

/* CLOCK pin for I2C */
#define PTD_PORT_I2C0_SCL2        (gpioPortA)
#define PTD_PIN_I2C0_SCL2         (UINT16_C(1))
#define PTD_MODE_I2C0_SCL2        (gpioModePushPull)
#define PTD_DRIVE_MODE_I2C0_SCL2  (gpioDriveModeStandard)
#define PTD_DOUT_I2C0_SCL2        (UINT16_C(1))

/** Maximum Number of Port and Pin */
#define PTD_MAXIMUM_PORT          gpioPortF
#define PTD_MAXIMUM_PIN           15

#endif /* BCDS_PTD_PORTDRIVER_PH_H_ */
