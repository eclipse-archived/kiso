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
#define IRQ_PRIORITY_UART_BLE                       KISO_UART_IRQ_PRIORITY_15

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
