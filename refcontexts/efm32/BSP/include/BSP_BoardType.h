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

/** @defgroup BSP_BoardType BSP_BoardType
 *  @brief BSP_BoardType
 *  @ingroup BCDS_HAL_BSP_IMP
 *  @{
 */

/**
 * @file
 * @brief Header file for Board support package of Bosch XDK (Cross Development Platform) project .
 *
 * @details This file is aimed to define the on configuration items
 * (e.g function arguments) necessary for the execution of the BSP functions.
 *
 */

#ifndef BSP_BoardType_H
#define BSP_BoardType_H


/** @ingroup BSP_XDK110_LED
 * @{
 */
/**
 * Enumeration representing on board LEDs.
 */
enum BSP_LEDEnableCtrl_E
{
    BSP_XDK_LED_R = 1, /**< Red LED	*/
    BSP_XDK_LED_O = 2, /**< Orange LED */
    BSP_XDK_LED_Y = 3, /**< Yellow LED */
	BSP_XDK_LED_ONBOARD_MAX,
};

/**
 * Enumeration representing Extension board LEDs.
 */
enum BSP_LEDExtension_E
{
	BSP_XDK_EXT_LEDA1 = BSP_XDK_LED_ONBOARD_MAX, /**< Extension LED A1 */
	BSP_XDK_EXT_LEDA2, /**< Extension LED A2 */
	BSP_XDK_EXT_LEDA3, /**< Extension LED A3 */
	BSP_XDK_EXT_LEDA4, /**< Extension LED A4 */
	BSP_XDK_EXT_LEDA5, /**< Extension LED A5 */
	BSP_XDK_EXT_LEDA6, /**< Extension LED A6 */
	BSP_XDK_EXT_LEDA7, /**< Extension LED A7 */
	BSP_XDK_EXT_LEDA8, /**< Extension LED A8 */
	BSP_XDK_EXT_LEDA9, /**< Extension LED A9 */
	BSP_XDK_EXT_LEDA10, /**< Extension LED A10 */
	BSP_XDK_EXT_LEDA11, /**< Extension LED A11 */
	BSP_XDK_EXT_LEDA12, /**< Extension LED A12 */
	BSP_XDK_EXT_LEDA13, /**< Extension LED A13 */
	BSP_XDK_EXT_LEDB1, /**< Extension LED B1 */
	BSP_XDK_EXT_LEDB2, /**< Extension LED B2 */
	BSP_XDK_EXT_LEDB3, /**< Extension LED B3 */
	BSP_XDK_EXT_LEDB4, /**< Extension LED B4 */
	BSP_XDK_EXT_LEDB5, /**< Extension LED B5 */
	BSP_XDK_EXT_LEDB6, /**< Extension LED B6 */
	BSP_XDK_EXT_LEDB7, /**< Extension LED B7 */
	BSP_XDK_EXT_LEDB8, /**< Extension LED B8 */
	BSP_XDK_EXT_LED_MAX
};

/**
 * Enumeration of the implemented LED commands
 */
enum BSP_LED_Command_E
{
    BSP_LED_COMMAND_OFF = 1, /**< LED OFF command */
    BSP_LED_COMMAND_ON = 2, /**< LED ON command */
    BSP_LED_COMMAND_TOGGLE = 3 /**< LED TOGGLE command */
};

/**@}*/
/** @ingroup BSP_XDK110_SD_CARD
 * @{
 */
/**
 * Enumeration for SD Card detection function.
 */
enum BSP_SDCardDetection_E
{
    BSP_XDK_SDCARD_DETECTION_ERROR = 0,
    BSP_XDK_SDCARD_INSERTED = 1,
    BSP_XDK_SDCARD_EJECTED = 2
};
/**@}*/
/** @ingroup BSP_XDK110_BUTTON
 * @{
 */
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
    BSP_XDK_BUTTON_PRESS = 1, /**< Button has been pressed and debounced */
    BSP_XDK_BUTTON_RELEASE = 2, /**< Button has been released */
};

typedef enum BSP_ButtonPress_E BSP_ButtonPress_T;

/**
 * Enumeration representing on board buttons.
 */
enum BSP_Button_E
{
    BSP_XDK_BUTTON_1 = 1, /**< User button 1 */
    BSP_XDK_BUTTON_2 = 2, /**< user button 2 */
	BSP_XDK_BUTTON_MAX = 3, /**< user button max */
};

/**
 * Enumeration representing Extension board buttons.
 */
enum BSP_ButtonExtension_E
{
	BSP_XDK_EXT_BUTTONA1 = BSP_XDK_BUTTON_MAX, /**< Extension button A1 */
	BSP_XDK_EXT_BUTTONA2, /**< Extension button A2 */
	BSP_XDK_EXT_BUTTONA3, /**< Extension button A3 */
	BSP_XDK_EXT_BUTTONA4, /**< Extension button A4 */
	BSP_XDK_EXT_BUTTONA5, /**< Extension button A5 */
	BSP_XDK_EXT_BUTTONA6, /**< Extension button A6 */
	BSP_XDK_EXT_BUTTONA7, /**< Extension button A7 */
	BSP_XDK_EXT_BUTTONA8, /**< Extension button A8 */
	BSP_XDK_EXT_BUTTONA9, /**< Extension button A9 */
	BSP_XDK_EXT_BUTTONA10, /**< Extension button A10 */
	BSP_XDK_EXT_BUTTONA11, /**< Extension button A11 */
	BSP_XDK_EXT_BUTTONA12, /**< Extension button A12 */
	BSP_XDK_EXT_BUTTONA13, /**< Extension button A13 */
	BSP_XDK_EXT_BUTTONB1, /**< Extension button B1 */
	BSP_XDK_EXT_BUTTONB2, /**< Extension button B2 */
	BSP_XDK_EXT_BUTTONB3, /**< Extension button B3 */
	BSP_XDK_EXT_BUTTONB4, /**< Extension button B4 */
	BSP_XDK_EXT_BUTTONB5, /**< Extension button B5 */
	BSP_XDK_EXT_BUTTONB6, /**< Extension button B6 */
	BSP_XDK_EXT_BUTTONB7, /**< Extension button B7 */
	BSP_XDK_EXT_BUTTONB8 /**< Extension button B8 */
};


/**
 * Enumeration used to report the button logic state (pressed or released).
 */
enum BSP_ButtonStatus_E
{
    BSP_XDK_BUTTON_ERROR = 0, /**< invalid parameter passed or button not enabled*/
    BSP_XDK_BUTTON_PRESSED = 1, /**< Button is pressed */
    BSP_XDK_BUTTON_RELEASED = 2, /**< Button is released */
};

/**
 * type definition for the button status.
 */
typedef enum BSP_ButtonStatus_E BSP_ButtonStatus_T;

/**@}*/
/** @ingroup BSP_XDK110_SENSOR_NODE
 * @{
 */
/**
 * Enumeration used to report the interrupt that has occured.
 * In addition to the communication interface interrupts the sensor node could
 * support peripherals that have IO interrupts e.g. in case of threshold the
 * interrupt could be used to wakeup the MCU the BSP informs the application
 * that the given peripheral has generated the interrupt indexed by
 * INTERRUPT_INDEX_x.
 */
enum BSP_InterruptIndex_E
{
    BSP_INTERRUPT_INDEX_1 = 1,
    BSP_INTERRUPT_INDEX_2 = 2,
    BSP_INTERRUPT_INDEX_MAX
};
/**
 * Enumeration used to report the edge that resulted in an interrupt
 */
enum BSP_InterruptEdge_E
{
    BSP_FALLING_EDGE,/**< BSP_FALLING_EDGE */
    BSP_RISING_EDGE  /**< BSP_RISING_EDGE */
};

/**
 * Enumeration representing on board sensors. Those are the sensors connected
 * to the I2C0 bus of the MCU
 */
enum BSP_Sensor_E
{ 
    BSP_XDK_SENSOR_BMA280 = 1, /**< Accelerometer Sensor */
    BSP_XDK_SENSOR_BMG160 = 2, /**< Gyroscope sensor */
    BSP_XDK_SENSOR_BME280 = 3, /**< Barometric Pressure and Humidity Sensor */
    BSP_XDK_SENSOR_BMM150 = 4, /**< Geomagnetic sensor */
    BSP_XDK_SENSOR_MAX44009 = 5, /**< Ambiant light sensor */
    BSP_XDK_SENSOR_BMI160 = 6 /**< Accelerometer and Gyroscope */
};
/**@}*/
/**@ingroup BSP_XDK110_BOARD
 * @{
 */
/**
 * @brief Enumeration to represent the power-supply types.
 * @see BSP_Board_DetectPowerSupply().
 */
enum BSP_PowerSupply_E
{
    POWER_SUPPLY_UNKNOWN = 0,
    POWER_SUPPLY_USB = 1,
    POWER_SUPPLY_BATTERY = 2
};

enum BSP_PowerRailControl_E
{
    BSP_XDK_RAIL_3V3_DISABLE = 1, /**< Disable on board 3V3 power rail */
    BSP_XDK_RAIL_2V5_SNOOZE = 2, /**< Snooze on board 2V5 power rail */
    BSP_XDK_RAIL_3V3_EXT_DISABLE = 3 /**< disable 3V3 extension board power rail (e.g. cellular/GPS extension module) */
};

/**
 * Enumeration representing the configurable power modes
 */
enum BSP_BoardPowerMode_E
{
    BSP_XDK_MODE_EM_2 = 2, /**< MCU Energy Mode 2 mode */
};

/**
 * Enumeration representing the possible wakeup events.
 */
enum BSP_WakeupEvent_E
{
    BSP_XDK_WAKE_EVENT_NONE = 1,
};

/**
 * Enumeration representing the logic states of an input
 */
enum BSP_IOState_E
{
    BSP_XDK_IO_STATE_ERROR = 0, /**< BSP_INPUT_STATE_ERROR */
    BSP_XDK_IO_STATE_LOW = 1, /**< BSP_INPUT_STATE_LOW */
    BSP_XDK_IO_STATE_HIGH = 2, /**< BSP_INPUT_STATE_HIGH */
};

enum BSP_IOPin_E
{
    BSP_XDK_IO_EXT_CHG_N,
    BSP_XDK_IO_EXT_PGOOD_N
};

enum BSP_BoardInitializeCtrl_E
{
    BSP_XDK_INIT_DEFAULT,
};

/**
 * Enumeration representing on Extension Bus Support Pins on XDK
 */
enum BSP_GPIO_ID_E
{
   BSP_GPIO_EXT_TEMPERATURE_IN_A = 0, /*< External Temperature Sensor interface >*/
   BSP_GPIO_EXT_TEMPERATURE_IN_B, /*< External Temperature Sensor interface >*/
   BSP_GPIO_EXT_TEMPERATURE_OUT_A, /*< External Temperature Sensor interface >*/
   BSP_GPIO_EXT_TEMPERATURE_OUT_B, /*< External Temperature Sensor interface >*/
   BSP_GPIO_EXT_BUTTON, /*< External Button >*/
   BSP_GPIO_EXT_LED, /*< External Led >*/
   BSP_GPIO_EMPTY, /* Out of range */
};

/**
 * type definition for the wakeup event.
 */
typedef enum BSP_WakeupEvent_E BSP_WakeupEvent_T;

/**
 * type definition for the power mode parameters.
 */
typedef enum BSP_BoardPowerMode_E BSP_BoardPowerMode_T;
/**
 * type definition for the power rail control parameters.
 */
typedef enum BSP_PowerRailControl_E BSP_BoardPowerRailCtrl_T;

/**
 * type definition for the power supply values.
 */
typedef enum BSP_PowerSupply_E BSP_PowerSupply_T;

/**
 * type definition for the input state values.
 */
typedef enum BSP_IOState_E BSP_IOState_T;

/**@}*/
/**@ingroup BSP_XDK110_CHARGER
 * @{
 */
/**
 * Enumeration representing the analog signals that are to be measured for the
 * battery charge monitoring.
 */
enum BSP_AnalogSignal_E
{
    BSP_XDK_VOUT = 1, /**< Battery voltage */ /**< BSP_XDK_VBAT */
};

/**
 * Enumeration representing the possible charging levels in the current HW
 * configuration
 */
enum BSP_ChargingSpeed_E
{
    BSP_XDK_CHARGING_SPEED_1 = 2, /**< BSP_XDK_CHARGING_SPEED_1: charging current set to 100mA */
    BSP_XDK_CHARGING_SPEED_2 = 3, /**< BSP_XDK_CHARGING_SPEED_2: charging current set to 500mA */
};

/**
 * Enumeration used to report the current charge state
 */
enum BSP_ChargeState_E
{
    BSP_XDK_CHARGE_STATUS_UNDEFINED = 0, /**< Charge status could not been defined */
    BSP_XDK_CHARGE_STATUS_NO_BATTERY, /**< Battery could not be detected, */
    BSP_XDK_CHARGE_STATUS_ON, /**< Battery is being charged */
    BSP_XDK_CHARGE_STATUS_CRITICAL, /**< Battery voltage is critical */
    BSP_XDK_CHARGE_STATUS_LOW, /**< Battery voltage is low */
    BSP_XDK_CHARGE_STATUS_NORMAL, /**< Battery voltage is normal */
    BSP_XDK_CHARGE_STATUS_FULL, /**< Battery voltage is full */
    BSP_XDK_CHARGE_STATUS_ERROR, /**< if charger has not been enabled by calling enable function with correct parameters */
};

enum BSP_ChargerMeasurement_E
{
    BSP_XDK_EXT_SENSE_VBAT_ADC = 55
};

/**
 * type definition for the charge state
 */
typedef enum BSP_ChargeState_E BSP_ChargeState_T;

/**@}*/
/**@ingroup BSP_XDK110_BT_MODULE
 * @{
 */

/**@}*/
/**@ingroup BSP_XDK110_MIC_AKU340
 * @{
 */
/**@}*/
/**@ingroup BSP_XDK110_WIFI_CC3100MOD
 * @{
 */
/**@}*/

#endif /* BSP_BoardType_H */
/**@}*/

