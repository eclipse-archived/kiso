/****************************************************************************/
/**
 * @defgroup  PTD
 * @ingroup USER
 **@{*
 *  Copyright (C) Robert Bosch.. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to te_boolean repeat outside the company, only if these te_boolean repeat
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *   This file deals with configuration of PTD_portDriver_cc.c file
 *
 ****************************************************************************/

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
