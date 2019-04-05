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

/**
 * @file
 *
 * @brief Unit Test Mock-up Header
 *
 * This unit test header substitutes "em_gpio.h"
 *
 * Please consult the original header for documentation
 *
 * @warning This file is not for redistribution
 *
 */

#ifndef EM_GPIO_TH_HH_
#define EM_GPIO_TH_HH_

/* include system header files */

#include "em_device_th.hh"
/* ban or wrap the real em_gpio.h interface header */
#define GPIO_COUNT 2
#define GPIO_IntClear BANNED_GPIO_IntClear
#define GPIO_InputSenseSet BANNED_GPIO_InputSenseSet
#define GPIO_IntDisable BANNED_GPIO_IntDisable
#define GPIO_IntEnable BANNED_GPIO_IntEnable
#define GPIO_IntSet BANNED_GPIO_IntSet
#define GPIO_Lock BANNED_GPIO_Lock
#define GPIO_PinInGet BANNED_GPIO_PinInGet
#define GPIO_PinOutClear BANNED_GPIO_PinOutClear
#define GPIO_PinOutGet BANNED_GPIO_PinOutGet
#define GPIO_PinOutSet BANNED_GPIO_PinOutSet
#define GPIO_PinOutToggle BANNED_GPIO_PinOutToggle
#define GPIO_PortOutClear BANNED_GPIO_PortOutClear
#define GPIO_PortOutSet BANNED_GPIO_PortOutSet
#define GPIO_PortOutSetVal BANNED_GPIO_PortOutSetVal
#define GPIO_PortOutToggle BANNED_GPIO_PortOutToggle
#define GPIO_PortInGet BANNED_GPIO_PortInGet
#define GPIO_PortOutGet BANNED_GPIO_PortOutGet
#define GPIO_Unlock BANNED_GPIO_Unlock
#define GPIO_IntGet BANNED_GPIO_IntGet
#define GPIO_IntGetEnabled BANNED_GPIO_IntGetEnabled

#define __EM_GPIO_H


/** GPIO ports identificator. */
typedef enum
{
  gpioPortA = 0, /**< Port A */
  gpioPortB = 1, /**< Port B */
  gpioPortC = 2, /**< Port C */
  gpioPortD = 3, /**< Port D */
  gpioPortE = 4, /**< Port E */
  gpioPortF = 5  /**< Port F */
} GPIO_Port_TypeDef;

/** GPIO drive mode. */
typedef enum
{
  /** Default 6mA */
  gpioDriveModeStandard = GPIO_P_CTRL_DRIVEMODE_STANDARD,
  /** 0.5 mA */
  gpioDriveModeLowest   = GPIO_P_CTRL_DRIVEMODE_LOWEST,
  /** 20 mA */
  gpioDriveModeHigh     = GPIO_P_CTRL_DRIVEMODE_HIGH,
  /** 2 mA */
  gpioDriveModeLow      = GPIO_P_CTRL_DRIVEMODE_LOW
} GPIO_DriveMode_TypeDef;

/** Pin mode. For more details on each mode, please refer to the EFM32
 * reference manual. */
typedef enum
{
  /** Input disabled. Pullup if DOUT is set. */
  gpioModeDisabled                  = _GPIO_P_MODEL_MODE0_DISABLED,
  /** Input enabled. Filter if DOUT is set */
  gpioModeInput                     = _GPIO_P_MODEL_MODE0_INPUT,
  /** Input enabled. DOUT determines pull direction */
  gpioModeInputPull                 = _GPIO_P_MODEL_MODE0_INPUTPULL,
  /** Input enabled with filter. DOUT determines pull direction */
  gpioModeInputPullFilter           = _GPIO_P_MODEL_MODE0_INPUTPULLFILTER,
  /** Push-pull output */
  gpioModePushPull                  = _GPIO_P_MODEL_MODE0_PUSHPULL,
  /** Push-pull output with drive-strength set by DRIVEMODE */
  gpioModePushPullDrive             = _GPIO_P_MODEL_MODE0_PUSHPULLDRIVE,
  /** Wired-or output */
  gpioModeWiredOr                   = _GPIO_P_MODEL_MODE0_WIREDOR,
  /** Wired-or output with pull-down */
  gpioModeWiredOrPullDown           = _GPIO_P_MODEL_MODE0_WIREDORPULLDOWN,
  /** Open-drain output */
  gpioModeWiredAnd                  = _GPIO_P_MODEL_MODE0_WIREDAND,
  /** Open-drain output with filter */
  gpioModeWiredAndFilter            = _GPIO_P_MODEL_MODE0_WIREDANDFILTER,
  /** Open-drain output with pullup */
  gpioModeWiredAndPullUp            = _GPIO_P_MODEL_MODE0_WIREDANDPULLUP,
  /** Open-drain output with filter and pullup */
  gpioModeWiredAndPullUpFilter      = _GPIO_P_MODEL_MODE0_WIREDANDPULLUPFILTER,
  /** Open-drain output with drive-strength set by DRIVEMODE */
  gpioModeWiredAndDrive             = _GPIO_P_MODEL_MODE0_WIREDANDDRIVE,
  /** Open-drain output with filter and drive-strength set by DRIVEMODE */
  gpioModeWiredAndDriveFilter       = _GPIO_P_MODEL_MODE0_WIREDANDDRIVEFILTER,
  /** Open-drain output with pullup and drive-strength set by DRIVEMODE */
  gpioModeWiredAndDrivePullUp       = _GPIO_P_MODEL_MODE0_WIREDANDDRIVEPULLUP,
  /** Open-drain output with filter, pullup and drive-strength set by DRIVEMODE */
  gpioModeWiredAndDrivePullUpFilter = _GPIO_P_MODEL_MODE0_WIREDANDDRIVEPULLUPFILTER
} GPIO_Mode_TypeDef;

#undef GPIO_IntClear
#undef GPIO_InputSenseSet
#undef GPIO_IntDisable
#undef GPIO_IntEnable
#undef GPIO_IntSet
#undef GPIO_Lock
#undef GPIO_PinInGet
#undef GPIO_PinOutClear
#undef GPIO_PinOutGet
#undef GPIO_PinOutSet
#undef GPIO_PinOutToggle
#undef GPIO_PortOutClear
#undef GPIO_PortOutSet
#undef GPIO_PortOutSetVal
#undef GPIO_PortOutToggle
#undef GPIO_PortInGet
#undef GPIO_PortOutGet
#undef GPIO_Unlock
#undef GPIO_IntGet
#undef GPIO_IntGetEnabled

/** Validation of pin typically usable in assert statements. */
#define GPIO_PIN_VALID(pin)           ((pin) < 16)

/** Validation of port typically usable in assert statements. */
#define GPIO_PORT_VALID(port)         ((port) <= gpioPortF)

/* faked variables needs to be initialized by the test fixture */
/* mock-ups for the provided interfaces */
FAKE_VOID_FUNC(GPIO_IntClear,uint32_t);
FAKE_VOID_FUNC(GPIO_Unlock);
FAKE_VOID_FUNC(GPIO_InputSenseSet,uint32_t, uint32_t);
FAKE_VOID_FUNC(GPIO_IntDisable,uint32_t);
FAKE_VOID_FUNC(GPIO_IntEnable,uint32_t);
FAKE_VOID_FUNC(GPIO_IntSet,uint32_t);
FAKE_VOID_FUNC(GPIO_Lock);
FAKE_VOID_FUNC(GPIO_PinOutClear,GPIO_Port_TypeDef,unsigned int);
FAKE_VOID_FUNC(GPIO_PinOutSet,GPIO_Port_TypeDef,unsigned int);
FAKE_VOID_FUNC(GPIO_PinOutToggle,GPIO_Port_TypeDef,unsigned int);
FAKE_VOID_FUNC(GPIO_PortOutClear,GPIO_Port_TypeDef,uint32_t);
FAKE_VOID_FUNC(GPIO_PortOutSet,GPIO_Port_TypeDef,uint32_t);
FAKE_VOID_FUNC(GPIO_PortOutSetVal,GPIO_Port_TypeDef,uint32_t,uint32_t);
FAKE_VOID_FUNC(GPIO_PortOutToggle,GPIO_Port_TypeDef,uint32_t);
FAKE_VOID_FUNC(GPIO_IntConfig,GPIO_Port_TypeDef,unsigned int,bool,bool,bool);
FAKE_VALUE_FUNC( unsigned int,GPIO_PinOutGet,GPIO_Port_TypeDef, unsigned int);
FAKE_VALUE_FUNC(unsigned int,GPIO_PinInGet,GPIO_Port_TypeDef, unsigned int);
FAKE_VALUE_FUNC(uint32_t,GPIO_PortInGet,GPIO_Port_TypeDef);
FAKE_VALUE_FUNC(uint32_t,GPIO_PortOutGet,GPIO_Port_TypeDef);
FAKE_VALUE_FUNC(uint32_t,GPIO_IntGet);
FAKE_VALUE_FUNC(uint32_t,GPIO_IntGetEnabled);
FAKE_VOID_FUNC(GPIO_PinModeSet,GPIO_Port_TypeDef,unsigned int,GPIO_Mode_TypeDef,unsigned int );
FAKE_VOID_FUNC(GPIO_DriveModeSet, GPIO_Port_TypeDef,GPIO_DriveMode_TypeDef);

#endif /* EM_GPIO_TH_HH_ */

/** ************************************************************************* */
