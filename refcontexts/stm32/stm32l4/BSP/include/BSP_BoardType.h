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

#ifndef BCDS_BSP_BOARD_H_
#define BCDS_BSP_BOARD_H_

/**
 * @defgroup BSP                    Board Support Package
 * @ingroup  BCDS_HAL_BSP_IMP       HAL/BSP Implementation
 * @{
 * @defgroup BSP_BOARD              BSE Board
 * @defgroup BSP_BT_MODULE          BLuetooth Module EM9301
 * @defgroup BSP_BUTTON             Button
 * @defgroup BSP_GNSS_MODULE        GNSS Module G7020
 * @defgroup BSP_GSM_MODULE         GSM Module SIM800H
 * @defgroup BSP_PWM_BUZZER         Buzzer Module
 * @defgroup BSP_SENSOR_NODE        I2C Sensor Node Module
 * @defgroup BSP_MEMORY_MODULE      Memory Module W25Q256FV
 * @defgroup BSP_CAN_MODULE         CAN Module SN65HVD234
 * @defgroup BSP_CHARGING_MODULE    Battery Charging Module BQ2407X
 * @defgroup BSP_TEST_IF            Test Interface
 * @defgroup BSP_LED_LM2755         Charge Pump LED Controller Module LM2755
 * @defgroup BSP_STARTUP            System Start-up Module
 * @ingroup BSP
 * @{
 */

/**
 * @file
 * @brief Header file for Board support package for BSE project.
 *
 * @details This file is aimed to define the on board components.
 *
 */

/**
 * @ingroup BSP_PWM_BUZZER
 * @{
 */

/**
 * Enumeration representing on board buzzers.
 */
enum BSP_BUZZER_E
{
    BSP_BUZZER_PWM = 1 /**< System buzzer */
};

typedef enum BSP_BUZZER_E BSP_BUZZER_T;

/**@}*//* @ingroup BSP_PWM_BUZZER */

/**
 * @ingroup BSP_PWM_LED
 * @{
 */

/**
 * Enumeration representing on board LEDs.
 */
enum BSP_LED_E
{
    BSP_LED_R_PWM = 1, //!< Red LED
    BSP_LED_G_PWM = 2, //!< Green LED
    BSP_LED_B_PWM = 3 //!< Blue LED
};

typedef enum BSP_LED_E BSP_LED_T;

/**
 * Macro to assess if the passed argument corresponds to one of the LEDs
 */
#define IS_LED(LED_ID)                  (((LED_ID) == BSP_LED_R_PWM) || \
                                         ((LED_ID) == BSP_LED_G_PWM) || \
                                         ((LED_ID) == BSP_LED_B_PWM))
/**@}*//* @ingroup BSP_PWM_LED */

/** @ingroup BSP_BUTTON
 * @{
 */

/**
 * Enumeration representing on board buttons.
 */
enum BSP_Button_E
{
    BSP_BUTTON_ECALL = 1, //!< Emergency call button
    BSP_BUTTON_RESET = 2 //!< Reset Button @note in the current implementation this button is not interrupt driven
};

typedef enum BSP_Button_E BSP_Button_T;

/**
 * Macro to assess if the passed argument corresponds to one of the buttons
 */
#define IS_BUTTON(BUTTON_ID)            (((BUTTON_ID) == BSP_BUTTON_ECALL) || \
                                         ((BUTTON_ID) == BSP_BUTTON_RESET))
/**@}*//* @ingroup BSP_BUTTON */

/**
 * Enumeration used to report the press nature either short press or long press
 *  - A button press is evaluated as short if it has been steady for minimum
 * specified duration, the event is reported when the button is released
 *
 *  - A Button Press is evaluated as long if it has lasted for longer than a
 * specified time duration e.g. 5 seconds, the event is reported during the
 * button press.
 */
enum BSP_ButtonPress_E
{
    BSP_BUTTON_PRESS = 1, /**< Button has been pressed and debounced */
    BSP_BUTTON_SHORT_PRESS = 2, /**< Button has been released after short period */
    BSP_BUTTON_LONG_PRESS = 3 /**< Button has been pressed for long period */
};

typedef enum BSP_ButtonPress_E BSP_ButtonPress_T;

/* state machine states for processing the button events*/
enum ButtonState_E
{
    BUTTON_NO_TRIGGER = 0, /**< Button has not been triggered (initial state)*/
    BUTTON_PRESSED = 1, /**< Button has been pressed */
    BUTTON_DEBOUNCED = 2, /**< Button has been debounced */
    BUTTON_PRESSED_LONG = 3, /**< Button has been pressed for long time */
    BUTTON_RELEASED = 4 /**< Button has bee released */
};

typedef enum ButtonState_E ButtonState_T;

/** @ingroup BSP_SENSOR_NODE
 * @{
 */

/**
 * Enumeration representing on board sensors. Those are the sensors connected
 * to the I2C1 bus of the MCU
 */
enum BSP_Sensor_E
{
    BSP_SENSOR_BMI160 = 1, //!< Accelerometer and Gyroscope sensor
    BSP_SENSOR_BMP280 = 2, //!< Barometric pressure sensor
    BSP_SENSOR_BMM150 = 3 //!< Geo-magnetic sensor
};

typedef enum BSP_Sensor_E BSP_Sensor_T;

/**
 * Macro to assess if the passed argument corresponds to one of the sensors
 */
#define IS_SENSOR(SENSOR_ID)            (((SENSOR_ID) == BSP_SENSOR_BMI160)|| \
                                         ((SENSOR_ID) == BSP_SENSOR_BMP280)|| \
                                         ((SENSOR_ID) == BSP_SENSOR_BMM150))
/**@}*//* @ingroup BSP_SENSOR_NODE */

/**
 * Enumeration used to report the interrupt that has occured.
 * In addition to the communication interface interrupts the sensor node could
 * support peripherals that have IO interrupts e.g. in case of threshold the
 * interrupt could be used to wakeup the MCU the BSP informs the application
 * that the given peripheral has generated the interrupt indexed by
 * INTERRUPT_INDEX_x.
 */
enum InterruptIndex_E
{
    BSP_INTERRUPT_INDEX_1 = 1,
    BSP_INTERRUPT_INDEX_2 = 2,
    BSP_INTERRUPT_INDEX_MAX
};

typedef enum InterruptIndex_E InterruptIndex_T;

/**
 * Enumeration used to report the interrupt on Rising Edge or Falling Edge.
 */
enum BSP_InterruptEdge_E
{
    BSP_FALLING_EDGE, BSP_RISING_EDGE
};

typedef enum BSP_InterruptEdge_E BSP_InterruptEdge_T;

/**
 * @ingroup BSP_CHARGING_MODULE
 * @{
 */
/**
 * Enumeration representing the analog signals that are to be measured for the
 * battery charge monitoring.
 */
enum BSP_AnalogSignal_E
{
    BSP_VREF = 0, /**< VREF+ voltage reference */
    BSP_TS = 1, /**< MCU Internal temperature */
    BSP_VBAT = 2, /**< Internal battery voltage */
    BSP_IBAT = 3, /**< Internal battery charging current */
    BSP_VEXT = 4, /**< External battery voltage */
    BSP_ANALOG_SIGNAL_COUNT
};

typedef enum BSP_AnalogSignal_E BSP_AnalogSignal_T;

/**
 * Macro to assess if the passed argument corresponds to one of the valid analog
 * signals.
 */
#define IS_ANALOG(SIGNAL_ID)            (((SIGNAL_ID) == BSP_VEXT) || \
                                         ((SIGNAL_ID) == BSP_VBAT) || \
                                         ((SIGNAL_ID) == BSP_IBAT))

/**
 * Enumeration representing the possible charging levels in the current HW
 * configuration
 */
enum BSP_ChargingLevel_E
{
    /** @todo 2016-12-16, BCDS/ENG1: decouple battery voltage monitoring from charger */
    BSP_CHARGING_LEVEL_0 = 1, //!< BSP_CHARGING_LEVEL_0: Only signals monitoring no charging current is sinked
    BSP_CHARGING_LEVEL_1 = 2, //!< BSP_CHARGING_LEVEL_1: charging current set to 100mA
    BSP_CHARGING_LEVEL_2 = 3, //!< BSP_CHARGING_LEVEL_2: charging current set to 500mA
    BSP_CHARGING_LEVEL_3 = 4 //!< BSP_CHARGING_LEVEL_3: charging current set by R_lim
};

typedef enum BSP_ChargingLevel_E BSP_ChargingLevel_T;

/**
 * Macro to assess if the passed argument corresponds to one of the valid
 * charging levels.
 */
#define IS_CHARGING_LEVEL(LEVEL_ID)     (((LEVEL_ID) == BSP_CHARGING_LEVEL_0) || \
                                         ((LEVEL_ID) == BSP_CHARGING_LEVEL_1) || \
                                         ((LEVEL_ID) == BSP_CHARGING_LEVEL_2) || \
                                         ((LEVEL_ID) == BSP_CHARGING_LEVEL_3))
/**@}*//* @ingroup BSP_CHARGING_MODULE */

/**
 * Enumeration used to report the current charging status
 */
enum ChargingStatus_E
{
    BSP_STATUS_CHARGING_ON = 1,
    BSP_STATUS_CHARGING_OFF = 2,
    BSP_STATUS_CHARGING_ERROR = 3
};

/** Old-style charging state type */
typedef enum ChargingStatus_E ChargingStatus_T;

/** New-style charging state type */
typedef enum ChargingStatus_E BSP_ChargeState_T;

/**
 * @ingroup BSP_BOARD
 * @{
 */

/**
 * @brief Enumeration to represent the power-supply types.
 * @see BSP_Board_DetectPowerSupply().
 */
enum BSP_PowerSupply_E
{
    POWER_SUPPLY_UNKNOWN = 0,
    POWER_SUPPLY_EXTERNAL = 1,
    POWER_SUPPLY_BATTERY = 2
};

typedef enum BSP_PowerSupply_E BSP_PowerSupply_T;

enum BSP_PowerRail_E
{
    BSP_RAIL_1_3V3
};

typedef enum BSP_PowerRail_E BSP_PowerRail_T;

#define IS_POWER_RAIL(RAIL_ID)          (RAIL_ID == BSP_RAIL_1_3V3)

/**
 * Enumeration representing the configurable power modes
 * @note currently only switching to stop 2, standby and shutdown mode are implemented.
 */
enum BSP_PowerMode_E
{
    BSP_MODE_RESET = 1, /**< Reset MCU */
    BSP_MODE_SHORT_TERM_STORAGE = 2, /**< system Short term storage mode (72hr) : previously was Summer mode */
    BSP_MODE_LONG_TERM_STORAGE = 3, /**< system Long term storage mode (2 months) : previously was Winter mode */
    BSP_MODE_TRANSPORTATION = 4, /**< system transportation or shutdown mode */
    BSP_MODE_STANDBY = 5, /**< not a system mode for now */
};

typedef enum BSP_PowerMode_E BSP_PowerMode_T;

#define IS_POWER_MODE(MODE_ID)          ((MODE_ID == BSP_MODE_RESET)  || \
                                        (MODE_ID == BSP_MODE_SHORT_TERM_STORAGE)  || \
                                        (MODE_ID == BSP_MODE_LONG_TERM_STORAGE) || \
                                        (MODE_ID == BSP_MODE_TRANSPORTATION))

/**
 * Enumeration representing the possible wakeup events.
 */
enum BSP_WakeupEvent_E
{
    BSP_WAKE_EVENT_NONE = 0,
    BSP_WAKE_EVENT_ACCELEROMETER = 1,
    BSP_WAKE_EVENT_EXT_POWER = 2,
    BSP_WAKE_EVENT_COMBINATION = 3,
};

typedef enum BSP_WakeupEvent_E BSP_WakeupEvent_T;

/**@}*//* @ingroup BSP_BOARD */
/**@}*//* @ingroup BSP */
/**@}*//* @ingroup BCDS_HAL_BSP_IMP */

#endif /* BCDS_BSP_BOARD_H_ */
