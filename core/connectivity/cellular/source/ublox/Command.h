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
 *
 * @brief AT command set for UBLOX LISA-U200 wireless cellular module
 */

#ifndef UBLOX_COMMAND_H
#define UBLOX_COMMAND_H

#define ATCMD_AT                                "AT\r\n"
#define ATCMD_AT_RESP                           "AT"
#define ATCMD_ENABLE_HEX_MODE                   "AT+UDCONF=1,1\r\n"
#define ATCMD_CELL_ENV_DESC                     "AT+CGED=3\r\n"
#define ATCMD_CELL_ENV_DESC_FMT                 "AT+CGED=%u\r\n"
#define ATCMD_CELL_ENV_DESC_RESP                "CGED"
#define ATCMD_CELL_ENV_DESC_RESP_FMT            "MCC:%u, MNC:%u, LAC:%x, CI:%x"
#define ATCMD_CELL_ENV_DESC_ARG_COUNT           4

#endif /* UBLOX_COMMAND_H */
