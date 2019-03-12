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
 * @brief Configuration header for the BCDS ServalPal components.
 *
 * @details Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */

#ifndef BCDS_SERVALPALCONFIG_H_
#define BCDS_SERVALPALCONFIG_H_

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */
#ifndef SERVALPAL_SOCKET_COUNT_MAX
#define SERVALPAL_SOCKET_COUNT_MAX		         UINT8_C(5)  /**< Maximum Number of Sockets */
#endif /* SERVALPAL_SOCKET_COUNT_MAX */

#ifndef SERVALPAL_NETIF_COUNT_MAX
#define SERVALPAL_NETIF_COUNT_MAX                UINT8_C(2)  /**< Maximum Number of Network Interfaces  */
#endif /* SERVALPAL_NETIF_COUNT_MAX */

/**
 * This is the number of buffers managed by the the buffer pool. These
 * buffers are used if an incoming message should be kept for longer
 * time, e.g. waiting for additional packets or for longer processing.
 *
 */
#ifndef SERVALPAL_MAX_NUM_ADDITIONAL_COMM_BUFFERS
#define SERVALPAL_MAX_NUM_ADDITIONAL_COMM_BUFFERS     10
#endif

/**
 * Dynamic memory maximum allowed for buffer pool
 *
 */
#define SERVALPAL_MAXIMUM_DYNAMIC_MEMORY (SERVALPAL_MAX_NUM_ADDITIONAL_COMM_BUFFERS * 1500)

#ifndef BCDS_SERVALPAL_MULTINETIF_SUPPORT
#define BCDS_SERVALPAL_MULTINETIF_SUPPORT           0
#endif /* BCDS_SERVALPAL_MULTINETIF_SUPPORT */

#ifndef BCDS_SERVALPAL_MULTINETIF_CELLULAR
#define BCDS_SERVALPAL_MULTINETIF_CELLULAR          0
#endif /* BCDS_SERVALPAL_MULTINETIF_CELLULAR */

#ifndef BCDS_SERVALPAL_MULTINETIF_BLE
#define BCDS_SERVALPAL_MULTINETIF_BLE               0
#endif /* BCDS_SERVALPAL_MULTINETIF_BLE */

#ifndef BCDS_SERVALPAL_MULTINETIF_LORA
#define BCDS_SERVALPAL_MULTINETIF_LORA              0
#endif /* BCDS_SERVALPAL_MULTINETIF_LORA */

#ifndef BCDS_SERVALPAL_MULTINETIF_WIFI
#define BCDS_SERVALPAL_MULTINETIF_WIFI              0
#endif /* BCDS_SERVALPAL_MULTINETIF_WIFI */

#ifndef BCDS_SERVALPAL_CELLULAR
#define BCDS_SERVALPAL_CELLULAR                     0
#endif /* BCDS_SERVALPAL_CELLULAR */

#ifndef BCDS_SERVALPAL_BLE
#define BCDS_SERVALPAL_BLE                          0
#endif /* BCDS_SERVALPAL_BLE */

#ifndef BCDS_SERVALPAL_LORA
#define BCDS_SERVALPAL_LORA                         0
#endif /* BCDS_SERVALPAL_LORA */

#ifndef BCDS_SERVALPAL_WIFI
#define BCDS_SERVALPAL_WIFI                         0
#endif /* BCDS_SERVALPAL_WIFI */

/**< Macro to represent task size of the WiFi Socket Monitor task. Used only when WiFi is enabled. */
#ifndef BCDS_SERVALPAL_WIFI_SOC_MONITOR_TASK_SIZE
#define BCDS_SERVALPAL_WIFI_SOC_MONITOR_TASK_SIZE               2400UL
#endif /* BCDS_SERVALPAL_WIFI_SOC_MONITOR_PERIOD */

/**< Macro to represent priority of the WiFi Socket Monitor task. Used only when WiFi is enabled. */
#ifndef BCDS_SERVALPAL_WIFI_SOC_MONITOR_PRIORITY
#define BCDS_SERVALPAL_WIFI_SOC_MONITOR_PRIORITY                4UL
#endif /* BCDS_SERVALPAL_WIFI_SOC_MONITOR_PRIORITY */

/**< Macro to represent sleep time of the Socket Monitor task. Used only when WiFi is enabled. */
#ifndef BCDS_SERVALPAL_WIFI_SOC_MONITOR_PERIOD
#define BCDS_SERVALPAL_WIFI_SOC_MONITOR_PERIOD                  100UL
#endif /* BCDS_SERVALPAL_WIFI_SOC_MONITOR_PERIOD */

/* Put the function declarations here */

#endif /* BCDS_SERVALPALCONFIG_H_ */

