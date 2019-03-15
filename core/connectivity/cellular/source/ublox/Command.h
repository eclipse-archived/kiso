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
