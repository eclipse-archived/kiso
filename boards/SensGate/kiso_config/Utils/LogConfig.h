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
 * @brief Configuration header for the Logging module.
 *
 */

#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

/* Configuration for the Asyncronous Recorder */
#define LOG_BUFFER_SIZE             (UINT16_C(256))
#define LOG_QUEUE_BUFFER_SIZE       (UINT16_C(2048))
#define LOG_TASK_STACK_SIZE         (UINT16_C(128))
#define LOG_TASK_PRIORITY           (UINT8_C(4))
#define LOG_FILTER_ITEM_COUNT       (UINT8_C(8))

/* Default log level for package and module */
#define LOG_LEVEL_PACKAGE_DEFAULT   (LOG_LEVEL_INFO)
#define LOG_LEVEL_MODULE_DEFAULT    (LOG_LEVEL_INFO)

#endif /* LOG_CONFIG_H */
