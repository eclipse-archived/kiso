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
