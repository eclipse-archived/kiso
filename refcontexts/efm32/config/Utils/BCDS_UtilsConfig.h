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
 * @brief Utils config header.
 *
 * @details Provides configuration interface for the Utils components.
 */

#ifndef BCDS_UTILSCONFIG_H_
#define BCDS_UTILSCONFIG_H_

#ifndef BCDS_FEATURE_CMDLINEDEBUGGER
/** @brief Enable (1) or disable (0) the CmdLineDebugger feature. */
#define BCDS_FEATURE_CMDLINEDEBUGGER    1
#endif

#ifndef BCDS_FEATURE_CMDPROCESSOR
/** @brief Enable (1) or disable (0) the CmdProcessor feature. */
#define BCDS_FEATURE_CMDPROCESSOR       1
#endif

#ifndef BCDS_FEATURE_CRC
/** @brief Enable (1) or disable (0) the Crc feature. */
#define BCDS_FEATURE_CRC                1
#endif

#ifndef BCDS_FEATURE_EVENTHUB
/** @brief Enable (1) or disable (0) the EventHub feature. */
#define BCDS_FEATURE_EVENTHUB           1
#endif

#ifndef BCDS_FEATURE_GUARDEDTASK
/** @brief Enable (1) or disable (0) the GuardedTask feature. */
#define BCDS_FEATURE_GUARDEDTASK        1
#endif

#ifndef BCDS_FEATURE_LEANB2CAP
/** @brief Enable (1) or disable (0) the LeanB2Cap feature. */
#define BCDS_FEATURE_LEANB2CAP          1
#endif

#ifndef BCDS_FEATURE_ERRORLOGGER
/** @brief Enable (1) or disable (0) the ErrorLogger feature. */
#define BCDS_FEATURE_ERRORLOGGER        1
#endif

#ifndef BCDS_FEATURE_LOGGING
/** @brief Enable (1) or disable (0) the Logging feature. */
#define BCDS_FEATURE_LOGGING            1
#endif

#ifndef BCDS_FEATURE_QUEUE
/** @brief Enable (1) or disable (0) the Queue feature. */
#define BCDS_FEATURE_QUEUE              1
#endif

#ifndef BCDS_FEATURE_RINGBUFFER
/** @brief Enable (1) or disable (0) the RingBuffer feature. */
#define BCDS_FEATURE_RINGBUFFER         1
#endif

#ifndef BCDS_FEATURE_SLEEPCONTROL
/** @brief Enable (1) or disable (0) the SleepControl feature. */
#define BCDS_FEATURE_SLEEPCONTROL       1
#endif

#ifndef BCDS_FEATURE_TASKMONITOR
/** @brief Enable (1) or disable (0) the TaskMonitor feature. */
#define BCDS_FEATURE_TASKMONITOR        1
#endif

#ifdef BCDS_FEATURE_TASKMONITOR
/**
 * @brief BCDS_TASKMONITOR_MAX_TASKS
 * Maximum number of TaskMonitor tickets to reserve for the system.
 */
#define BCDS_TASKMONITOR_MAX_TASKS      10
#endif /* ifdef BCDS_FEATURE_TASKMONITOR */

#ifndef BCDS_FEATURE_TLV
/** @brief Enable (1) or disable (0) the Tlv feature. */
#define BCDS_FEATURE_TLV                1
#endif

#ifndef BCDS_FEATURE_UARTTRANSCEIVER
/** @brief Enable (1) or disable (0) the UartTransceiver feature. */
#define BCDS_FEATURE_UARTTRANSCEIVER    1
#endif

#ifndef BCDS_FEATURE_I2CTRANSCEIVER
/** @brief Enable (1) or disable (0) the I2CTransceiver feature. */
#define BCDS_FEATURE_I2CTRANSCEIVER    1
#endif

#ifndef BCDS_FEATURE_XPROTOCOL
/** @brief Enable (1) or disable (0) the XProtocol feature. */
#define BCDS_FEATURE_XPROTOCOL          1
#endif

#ifndef CDDK_FEATURE_PIPEANDFILTER
/** @brief Enable (1) or disable (0) the pipe & filter pattern feature. */
#define CDDK_FEATURE_PIPEANDFILTER          1
#endif

#endif /* BCDS_UTILSCONFIG_H_ */
