/******************************************************************************/
/**
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
 *
 *  This file provides Priority configurations for the BSE Application.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef PRIORITYCONFIG_H_
#define PRIORITYCONFIG_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */

#define TASK_PRIORITY_TIMER                         5
#define TASK_PRIORITY_GSM                           UINT8_C(3)
#define TASK_PRIORITY_APP                           UINT8_C(2)

#define TASK_PRIORITY_CAN                           (UINT32_C(2))
#define TASK_PRIORITY_BLE                           (UINT32_C(4))
#define IRQ_PRIORITY_UART_BLE                       BCDS_UART_IRQ_PRIORITY_15

#define IRQ_PRIORITY_GPIO_BUTTON                    GPIO_INTERRUPT_PRIORITY_5

#define IRQ_PRIORITY_TIMER_BLE                      (0x0F)

#define IRQ_PRIORITY_CAN1_RX0                       (UINT32_C(0))
#define IRQ_PRIORITY_CAN1_TX                        (UINT32_C(0))
#define IRQ_PRIORITY_CAN1_SCE                       (UINT32_C(0))

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* PRIORITYCONFIG_H_ */

/** ************************************************************************* */
