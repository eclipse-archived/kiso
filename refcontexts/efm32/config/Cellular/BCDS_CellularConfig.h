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

#ifndef BCDS_CELLULAR_CONFIG_H
#define BCDS_CELLULAR_CONFIG_H

#ifndef CELLULAR_SOCKET_COUNT
#define CELLULAR_SOCKET_COUNT           UINT32_C(7)
#endif  /* CELLULAR_SOCKET_COUNT */

/**< Max UDP received/sent packet size */
#define CELLULAR_UDP_SOCKET_SIZE        UINT32_C(1460)

/**< Max UDP data receive length */
#define CELLULAR_RX_BUFFER_SIZE         UINT32_C(1500)

/**< Max UDP data transfer length */
#define CELLULAR_TX_BUFFER_SIZE         UINT32_C(1500)

/** Max. message length limit */
#define CELLULAR_SMS_TEXT_MESSAGE_SIZE  UINT32_C(160)

/**< Network specific configuration defines */
#define CELLULAR_COUNTRY_CODE_LENGTH    UINT32_C(3)
#define CELLULAR_NETWORK_CODE_LENGTH    UINT32_C(3)
#define CELLULAR_LOCATION_CODE_LENGTH   UINT32_C(4)
#define CELLULAR_CELL_ID_LENGTH         UINT32_C(4)
#define CELLULAR_APN_NAME_SIZE          UINT32_C(101)
#define CELLULAR_APN_USER_SIZE          UINT32_C(31)
#define CELLULAR_APN_PASSWORD_SIZE      UINT32_C(31)

/**< RTOS task specific configuration defines */
#define CELLULAR_DRV_TASK_PRIORITY      UINT32_C(3)
#define CELLULAR_DRV_TASK_STACK_SIZE    UINT32_C(512)
#define CELLULAR_RESP_TASK_PRIORITY     UINT32_C(3)
#define CELLULAR_RESP_TASK_STACK_SIZE   UINT32_C(1024)

#endif /* BCDS_CELLULAR_CONFIG_H */
