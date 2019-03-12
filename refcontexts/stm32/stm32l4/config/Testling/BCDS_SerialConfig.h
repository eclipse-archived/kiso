/******************************************************************************/
/*
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Configuration header for the Communication channel
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef BCDS_SERIALCCHANNELCONFIG_H_
#define BCDS_SERIALCCHANNELCONFIG_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */
#ifdef efm32 /* For EFM32 */
#define CCHANNEL_SERIAL_HWTYPE           (SER_HWTYPE_LEUART)

#define CCHANNEL_UARTPORT                (SER_LEUART1)

#define CCHANNEL_UART_ROUTE_LOCATION     (SER_ROUTE_LOCATION0)

#define CCHANNEL_SERIAL_CLOCK            (cmuClock_LEUART1)

#define CCHANNEL_PORT_TX                 (gpioPortC)

#define CCHANNEL_PIN_TX                  (UINT16_C(6))

#define CCHANNEL_MODE_TX                 (gpioModePushPull)

#define CCHANNEL_DRIVE_MODE_TX           (gpioDriveModeStandard)

#define CCHANNEL_DOUT_TX                 (UINT8_C(1))

#define CCHANNEL_PORT_RX                 (gpioPortC)

#define CCHANNEL_PIN_RX                  (UINT16_C(7))

#define CCHANNEL_MODE_RX                 (gpioModeInputPull)

#define CCHANNEL_DRIVE_MODE_RX           (gpioDriveModeStandard)

#define CCHANNEL_DOUT_RX				    (UINT8_C(1))

#define CCHANNEL_TX_BUFFER_SIZE          (UINT8_C(255))

#define CCHANNEL_RX_BUFFER_SIZE          (UINT8_C(255))

#define CCHANNEL_UARTBAUDRATE            (UINT32_C(9600))

#else /* For STM32 */
#define BCDS_UART3_RESOURCE			true						/**< Enabling the required Communication channel (UART) */

#define CCHANNEL_UART_BAUDRATE      (UINT32_C(115200))			/**< Communication channel (UART) baud rate */

#define CCHANNEL_UART				BCDS_Uart3					/**< Communication channel (UART) selection */

#define CCHANNEL_UART_PRIORITY	    BCDS_UART_IRQ_PRIORITY_15	/**< Communication channel (UART) IRQ priority */

#define CCHANNEL_BUFFER_SIZE		(UINT32_C(512))				/**< Test Runner RX and TX buffer size */
#endif

#endif /* BCDS_SERIALCCHANNELCONFIG_H_ */

/** ************************************************************************* */
