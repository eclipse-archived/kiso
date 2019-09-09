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
 * @file
 * @defgroup
 * @ingroup
 * @{
 *
 * @brief Provides pin mapping and internally exposed GPIO handling functions
 *
 * @details 
 */
#ifndef PROTECTED_GPIO_H_
#define PROTECTED_GPIO_H_

#include "Kiso_HAL.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"

/*---------------------- EXPORTED MACROS ----------------------------------------------------------------------------*/

#define PINA_GPS_MTX             GPIO_PIN_0
#define PINA_GPS_MRX             GPIO_PIN_1
#define PINA_BMA_INT             GPIO_PIN_3
#define PINA_EN_POW_SENS         GPIO_PIN_5
#define PINA_EN_POW_GPS          GPIO_PIN_7
#define PINA_VUSBM               GPIO_PIN_9
#define PINA_EN_POW_GSM          GPIO_PIN_10
#define PINA_USBD_N              GPIO_PIN_11
#define PINA_USBD_P              GPIO_PIN_12
#define PINA_SWIO                GPIO_PIN_13
#define PINA_SWCLK               GPIO_PIN_14

#define PINB_POW_GPS_BAK         GPIO_PIN_1
#define PINB_SWO                 GPIO_PIN_3
#define PINB_NJTRST              GPIO_PIN_4
#define PINB_GSM_PON             GPIO_PIN_5
#define PINB_DBG_TX              GPIO_PIN_6
#define PINB_DBG_RX              GPIO_PIN_7
#define PINB_GPS_RESN            GPIO_PIN_10
#define PINB_BLE_RESN            GPIO_PIN_11
#define PINB_EN_POW_BLE          GPIO_PIN_12
#define PINB_EN_POW_MEM          GPIO_PIN_13
#define PINB_BLE_IND             GPIO_PIN_15

#define PINC_SENS_SCL            GPIO_PIN_0
#define PINC_SENS_SDA            GPIO_PIN_1
#define PINC_AIN                 GPIO_PIN_2
#define PINC_VBATM               GPIO_PIN_3
#define PINC_BLE_WKP             GPIO_PIN_5
#define PINC_EN_POW_5V_B         GPIO_PIN_6
#define PINC_EN_POW_5V_A         GPIO_PIN_7
#define PINC_1WIRE_DIR           GPIO_PIN_8
#define PINC_1WIRE_UC            GPIO_PIN_9
#define PINC_EnSDI12             GPIO_PIN_10
#define PINC_STB_CAN             GPIO_PIN_11
#define PINC_SDI12_TX            GPIO_PIN_12
#define PINC_OSC_IN              GPIO_PIN_14
#define PINC_OSC_OUT             GPIO_PIN_15

#define PIND_CAN_MRX             GPIO_PIN_0
#define PIND_CAN_MTX             GPIO_PIN_1
#define PIND_SDI12_RX            GPIO_PIN_2
#define PIND_GSM_MCTS            GPIO_PIN_3
#define PIND_GSM_MRTS            GPIO_PIN_4
#define PIND_GSM_MTX             GPIO_PIN_5
#define PIND_GSM_MRX             GPIO_PIN_6
#define PIND_PG_5V0              GPIO_PIN_7
#define PIND_BLE_MTX             GPIO_PIN_8
#define PIND_BLE_MRX             GPIO_PIN_9
#define PIND_LED_G                GPIO_PIN_11
#define PIND_LED_R               GPIO_PIN_12
#define PIND_LED_B               GPIO_PIN_13

#define PINE_EN_POW_CAN          GPIO_PIN_1
#define PINE_EN_LVL              GPIO_PIN_2
#define PINE_POW_LVL             GPIO_PIN_3
#define PINE_GSM_RESN            GPIO_PIN_4
#define PINE_GSM_GPIO1           GPIO_PIN_5
#define PINE_EN_VBATM            GPIO_PIN_6
#define PINE_MEM_WP              GPIO_PIN_10
#define PINE_MEM_HOLD            GPIO_PIN_11
#define PINE_MEM_CS              GPIO_PIN_12
#define PINE_MEM_SCK             GPIO_PIN_13
#define PINE_MEM_MISO            GPIO_PIN_14
#define PINE_MEM_MOSI            GPIO_PIN_15

/*---------------------- EXPORTED TYPES ------------------------------------------------------------------------------*/
/**
 * GPIO ports available in the MCU
 */
enum GPIO_Ports_E
{
    GPIO_PORT_A,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
};

/*---------------------- EXPORTED FUNCTIONS -------------------------------------------------------------------------*/

/**
 * Opens GPIO clock gate and saves its state
 */
void GPIO_OpenClockGate(enum GPIO_Ports_E port, uint16_t pinNames);

/**
 * Closes GPIO clock gate and saves its state
 */
void GPIO_CloseClockGate(enum GPIO_Ports_E port, uint16_t pinNames);

/**
 * Returns true if GPIO clock gate is open and false if closed.
 */
bool GPIO_GetClockGate(enum GPIO_Ports_E port);

#endif /* PROTECTED_GPIO_H_ */
