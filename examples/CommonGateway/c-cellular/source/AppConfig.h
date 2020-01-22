/*******************************************************************************
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *
 ******************************************************************************/

/**
 * @brief
 *      App config header
 *
 * @details
 *      Application definitions
 */

#ifndef APPCONFIG_H_
#define APPCONFIG_H_

// clang-format off
#define APP_CMDPROCESSOR_MAIN_PRIORITY (UINT32_C(1))
#define APP_CMDPROCESSOR_MAIN_STACKSIZE (UINT16_C(1024))
#define APP_CMDPROCESSOR_MAIN_QLEN (UINT32_C(10))

#define APP_TASK_APP_PRIORITY (UINT32_C(2))
#define APP_TASK_APP_STACKSIZE (UINT16_C(1024))

#define APP_NUMBER_OF_RETRIES (UINT32_C(3))
#define APP_SIM_PIN (NULL)
#define APP_RAT (CELLULAR_RAT_LTE_CAT_M1)
// #define APP_APN_NAME ("${APP_APN_NAME}")
// clang-format on
#define APP_APN_AUTHMETHOD (CELLULAR_APNAUTHMETHOD_NONE) /* Currently the only supported option */
#define APP_APN_USER (NULL)                              /* Currently the only supported option */
#define APP_APN_PASSWORD (NULL)                          /* Currently the only supported option */
// clang-format off
#define APP_DNS_TEST_FQDN ("echo.u-blox.com")
// #define APP_UDP_TEST_FQDN ("dummy")
// clang-format on
#ifndef APP_UDP_TEST_FQDN
// clang-format off
#define APP_UDP_TEST_IP ((Cellular_IpAddress_T){.Type = CELLULAR_IPADDRESSTYPE_IPV4, .Address.IPv4[3] = 195, .Address.IPv4[2] = 34, .Address.IPv4[1] = 89, .Address.IPv4[0] = 241})
// clang-format on
#endif
// clang-format off
#define APP_UDP_TEST_PORT (UINT16_C(7))
#define APP_UDP_TEST_DATA ("HELLO WORLD!")
// #define APP_TCP_TEST_FQDN ("dummy")
// clang-format on
#ifndef APP_TCP_TEST_FQDN
// clang-format off
#define APP_TCP_TEST_IP ((Cellular_IpAddress_T){.Type = CELLULAR_IPADDRESSTYPE_IPV4, .Address.IPv4[3] = 195, .Address.IPv4[2] = 34, .Address.IPv4[1] = 89, .Address.IPv4[0] = 241})
// clang-format on
#endif
// clang-format off
#define APP_TCP_TEST_PORT (UINT16_C(7))
#define APP_TCP_TEST_DATA ("HELLO WORLD!")
// clang-format on

#endif
