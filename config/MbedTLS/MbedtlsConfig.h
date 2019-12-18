/**
 * \file config.h
 *
 * \brief Configuration options (set of defines)
 *
 *  This set of compile-time options may be used to enable
 *  or disable features selectively, and reduce the global
 *  memory footprint.
 */
/*
 *  Copyright (C) 2006-2018, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#if SERVAL_ENABLE_DTLS && SERVAL_ENABLE_TLS
#error "SERVAL_ENABLE_DTLS and SERVAL_ENABLE_TLS both are not supported simultaneously"
#endif

#if SERVAL_ENABLE_TLS && SERVAL_TLS_MBEDTLS
#include "MbedtlsConfigTLS.h"
#endif

#if SERVAL_ENABLE_DTLS && SERVAL_TLS_MBEDTLS
#include "MbedtlsConfigDTLS.h"
#endif

#endif /* MBEDTLS_CONFIG_H */
