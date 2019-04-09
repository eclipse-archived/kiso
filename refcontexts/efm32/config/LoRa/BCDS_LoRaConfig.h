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

#ifndef BCDS_LORACONFIG_H_
#define BCDS_LORACONFIG_H_

/*@brief SPI port, SPEED and route selection macros*/
#define SPI_PORT_NUMBER 		SPI2   				 		       /**< Defines SPI port NUMBER */
#define SPI_CLK_MODE 	 		SPI_CLOCK_MODE_CPOL0_CPHA0         /**< Defines SPI mode selection */
#define SPI_ROUTE_LOCATION		SPI_ROUTE_LOCATION0                /**< Defines SPI driver route Location */

/** 1 = Enabled, 0 = Disabled */
#define LORA_OVER_AIR_ENABLE                        UINT8_C(1)		/**< Enable LoRa over the air */
#define LORA_DUTY_CYCLE_ENABLE 	        			UINT8_C(1)		/**< Enable LoRa duty cycle */

/* local type and macro definitions */
#define RX_TIMEOUT_VALUE                            UINT32_C(1000) /*Input in terms of milli seconds*/
#define LORA_VALUE_ZERO_U8                          UINT8_C(0)
#define LORA_TRANSMIT_FAILURE                       UINT8_C(2)
#define LORA_DEFAULT_BUFFER_LENGTH                  UINT8_C(0)
#define OVER_THE_AIR_ACTIVATION_DUTYCYCLE           UINT32_C(10000)  /* 10 [s] value Input in terms of milli seconds */

#define LORA_RECEIVE_WINDOW1_DELAY                  UINT32_C( 1000 ) /**< LoRa Receive Window1 delay Input in terms of milli seconds  */
#define LORA_RECEIVE_WINDOW2_DELAY                  UINT32_C( 2000 ) /**< LoRa Receive Window2 delay Input in terms of milli seconds  */

#define LORA_JOIN_WINDOW1_DELAY                     UINT32_C( 5000 )  /**< LoRa JoinAccpet Window1 delay Input in terms of milli seconds  */
#define LORA_JOIN_WINDOW2_DELAY                     UINT32_C( 6000 )  /**< LoRa JoinAccept Window2 delay Input in terms of milli seconds  */

#define LORA_DUMMY_TIMEOUT                          UINT32_C(43000)
#define LORA_QUEUE_WINDOW_TIMEOUT                   UINT32_C(130000)

#define LORA_PORT_UDP           UINT32_C(20)                /* Will be remove after the bugfix of servalPal_LoRa */
#define LORA_PORT_DATA_LOGGER   UINT32_C(21)                /* Will be remove after the bugfix of servalPal_LoRa */
#define LORA_DEBUG_PORT         UINT32_C(200)               /**< Port number for Debug functionalities */

#define LWM2M_RECEIVER_UDP_PORT UINT32_C(5683)
#define LWM2M_SENDER_UDP_PORT   UINT32_C(5566)
#define XUDP_PORT_DATA_LOGGER   UINT32_C(8000)

#define LORA_MAPPING_SIZE       UINT8_C(3)                      /**< Size of table that will map loraPort <=>udpPort */

/** TODO: When mapping will be done on gateway side, we will need to change the LoRa port here!!! */
#define LORA_MAP {                              \
        {                                       \
            .ipPort = LWM2M_RECEIVER_UDP_PORT,  \
            .loraPort = LORA_PORT_UDP           \
        }, /**< Lwm2m receiver*/                \
        {                                       \
            .ipPort = LWM2M_SENDER_UDP_PORT,    \
            .loraPort = LORA_PORT_UDP           \
        }, /**< Lwm2m sender*/                  \
        {                                       \
            .ipPort = XUDP_PORT_DATA_LOGGER,    \
            .loraPort = LORA_PORT_DATA_LOGGER   \
        }  /**< dataLogger */                   \
    };


#endif /* BCDS_LORACONFIG_H_ */



