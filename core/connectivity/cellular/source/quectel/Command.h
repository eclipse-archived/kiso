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
 * @brief AT command set for QUECTEL UC20 wireless cellular module
 */

#ifndef QCTEL_COMMAND_H
#define QCTEL_COMMAND_H

#define ATCMD_CARD_IDENTIFICATION               "AT+QCCID\r\n"
#define ATCMD_CARD_IDENTIFICATION_RESP          "QCCID"
#define ATCMD_SET_CMD_ECHO_FMT                  "ATE%d\r\n"
#define ATCMD_NETWORK_REGISTRATION              "AT+CREG?\r\n"
#define ATCMD_NETWORK_REGISTRATION_RESP         "CREG"
#define ATCMD_SET_NETWORK_REGISTRATION_FMT      "AT+CREG=%u\r\n"
#define ATCMD_OPERATOR_SELECTION_DEREGISTER     "AT+COPS=2\r\n"
#define ATCMD_OPERATOR_SELECTION_AUTOMATIC      "AT+COPS=0\r\n"
#define ATCMD_OPERATOR_SELECTION                "AT+COPS?\r\n"
#define ATCMD_OPERATOR_SELECTION_RESP           "COPS"
#define ATCMD_SET_CONTEXT_PROFILE_FMT           "AT+QICSGP=%u,%u,\"%s\",\"%s\",\"%s\",%u\r\n"
#define ATCMD_PDP_CONTEXT_ACTIVATE_FMT          "AT+QIACT=%u\r\n"
#define ATCMD_PDP_CONTEXT_DEACTIVATE_FMT        "AT+QIDEACT=%u\r\n"
#define ATCMD_GET_PDP_CONTEXT                   "AT+QIACT?\r\n"
#define ATCMD_GET_PDP_CONTEXT_RESP              "QIACT"
#define ATCMD_SET_NETWORK_PARAMETER_FMT         "AT+QNWCFG=\"%s\",%u\r\n"
#define ATCMD_SIGNAL_QUALITY_REPORT             "AT+CSQ\r\n"
#define ATCMD_SIGNAL_QUALITY_REPORT_RESP        "CSQ"
#define ATCMD_OPEN_SOCKET_CLIENT_FMT            "AT+QIOPEN=%u,%u,\"%s\",\"%s\",%u\r\n"
#define ATCMD_OPEN_SOCKET_SERVER_FMT            "AT+QIOPEN=%u,%u,\"%s\",\"%s\",%u,%u\r\n"
#define ATCMD_OPEN_SOCKET_RESP                  "QIOPEN"
#define ATCMD_CLOSE_SOCKET_FMT                  "AT+QICLOSE=%u\r\n"
#define ATCMD_SEND_DATA_FMT                     "AT+QISEND=%u,%u\r\n"
#define ATCMD_SEND_DATA_UDP_SERVICE_FMT         "AT+QISEND=%u,%u,\"%s\",%u\r\n"
#define ATCMD_SEND_DATA_ECHO                    "AT+QISEND"
#define ATCMD_SEND_DATA_START_RESP              ">"
#define ATCMD_SEND_DATA_OK_RESP                 "SEND OK"
#define ATCMD_RECEIVE_DATA_FMT                  "AT+QIRD=%u,%u\r\n"
#define ATCMD_RECEIVE_DATA_RESP                 "QIRD:"
#define ATCMD_RECEIVE_DATA_RESP_FMT             "QIRD: %u,\"%16[^\"]\",%u"
#define ATCMD_RECEIVE_DATA_RESP_ARG_COUNT       3
#define ATCMD_SOCKET_URC                        "QIURC"
#define ATCMD_CME_ERROR_RESP                    "CME ERROR"
#define ATCMD_MODULE_ON_RESP                    "RDY"
#define ATCMD_MODULE_OFF_RESP                   "POWERED DOWN"
#define ATCMD_MODULE_NORMAL_OFF_RESP            "NORMAL POWER DOWN"
#define ATCMD_CFUN_RESP                         "CFUN"
#define ATCMD_CPIN_RESP                         "CPIN"
#define ATCMD_USIM_RESP                         "QUSIM"
#define ATCMD_IND_RESP                          "QIND"
#define ATCMD_SET_DEFAULT_CONFIGURATION         "AT+QIMODE=0;+QURCCFG=\"urcport\",\"uart1\";E1\r\n"
#define ATCMD_AT                                "AT\r\n"
#define ATCMD_AT_RESP                           "AT"

#endif /* QCTEL_COMMAND_H */
