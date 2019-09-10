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
 * @ingroup UTILS
 *
 * @defgroup LOGGING Logging
 * @{
 *
 * @brief Defines the user API for logging.
 *
 * @details By default the package and module log levels are set to LOG_LEVEL_PACKAGE_DEFAULT
 *          and LOG_LEVEL_MODULE_DEFAULT correspondingly.
 *          To override default setting the LOG_LEVEL_PACKAGE and LOG_LEVEL_MODULE should be defined
 *          before including the Kiso_Logging.h:
 *
 * @code{.c}
 * // example of overriding package and module default log levels
 *
 * #define LOG_LEVEL_PACKAGE   LOG_LEVEL_DEBUG
 * #define LOG_LEVEL_MODULE    LOG_LEVEL_DEBUG
 * #include "Kiso_Logging.h"
 *
 * ...
 * @endcode
 *
 * @code{.c}
 * // example of usage the Logging
 *
 * #include "Kiso_Logging.h"
 *
 * Retcode_T MainInit(void)
 * {
 *     // Initialize the logging engine
 *     Retcode_T retcode = Logging_Init(Logging_AsyncRecorder, Logging_UARTAppender);
 *     if (RETCODE_OK != retcode)
 *     {
 *         return retcode;
 *     }
 *
 *     LOG_INFO("Logging has been initialized successfully");
 *     ...
 *     return retcode;
 * }
 *
 * @endcode
 *
 * @file
 **/

#ifndef KISO_LOGGING_H
#define KISO_LOGGING_H

#include "Kiso_Utils.h"

#if KISO_FEATURE_LOGGING

#include <stdarg.h>
#include "Kiso_Basics.h"
#include "Kiso_Retcode.h"
#include "LogConfig.h"

#define LOG_LEVEL_VAR(a)        LOG_LEVEL_COMPOSE(__log_level_, a)
#define LOG_LEVEL_COMPOSE(a, b) a ## b

#ifndef MIN
    #define MIN(a, b)           (((a) < (b)) ? (a) : (b))
#endif /* MIN */

/*For IAR compiler __builtin_va_list is not available in stdarg.h file, In IAR type defined as  defined as va_list*/
#ifndef ti_cc26xx
#ifndef va_list
    typedef __builtin_va_list va_list;
#endif /* va_list */
#endif

/**
 * @brief A macro to get current package log level.
 *
 * @retval              A package log level.
 */
#define LOG_LEVEL_GET_CURRENT_PACKAGE() (LOG_LEVEL_VAR(KISO_PACKAGE_ID))

/**
 * @brief A macro to get current module log level.
 *
 * @retval              A module log level.
 */
#define LOG_LEVEL_GET_CURRENT_MODULE()  (LOG_LEVEL_VAR(KISO_MODULE_ID))

/**
 * @brief A macro to get log level for the current package module.
 *
 * @return              The current package module log level.
 */
#define LOG_LEVEL_GET_CURRENT() (MIN(LOG_LEVEL_GET_CURRENT_PACKAGE(), LOG_LEVEL_GET_CURRENT_MODULE()))

/**
 * @brief Enumeration of supported log levels.
 */
typedef enum
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    /* Add more log levels here if required */

    LOG_LEVEL_COUNT
} LogLevel_T;

/**
 * @brief A macro to set package log level in runtime.
 *
 * @param [in] package  A package id.
 * @param [in] level    A log level to be set for the package.
 */
#define LOG_LEVEL_SET_PACKAGE(package, level)   LOG_LEVEL_VAR(package) = (uint8_t) level;

/**
 * @brief A macro to set module log level in runtime.
 *
 * @param [in] module   A module id.
 * @param [in] level    A log level to be set for the module.
 */
#define LOG_LEVEL_SET_MODULE(module, level)     LOG_LEVEL_VAR(module) = (uint8_t) level;

#ifdef KISO_LOGGING

/**
 * @brief A package log level variable.
 */
#ifdef LOG_LEVEL_PACKAGE
uint8_t LOG_LEVEL_VAR(KISO_PACKAGE_ID) = (uint8_t) LOG_LEVEL_PACKAGE;
#else
__attribute__(( weak )) uint8_t LOG_LEVEL_VAR(KISO_PACKAGE_ID) = (uint8_t) LOG_LEVEL_PACKAGE_DEFAULT;
#endif /* LOG_LEVEL_PACKAGE */

/**
 * @brief A module log level variable.
 */
#ifdef LOG_LEVEL_MODULE
uint8_t LOG_LEVEL_VAR(KISO_MODULE_ID) = (uint8_t) LOG_LEVEL_MODULE;
#else
__attribute__(( weak )) uint8_t LOG_LEVEL_VAR(KISO_MODULE_ID) = (uint8_t) LOG_LEVEL_MODULE_DEFAULT;
#endif /* LOG_LEVEL_MODULE */

/**
 * @brief A macro to log a message.
 *
 * @param [in] level    A log level with followed message arguments.
 */
#define LOG(level, ...) { \
    if (level <= LOG_LEVEL_GET_CURRENT()) { \
        (void) Logging_Log(level, KISO_PACKAGE_ID, KISO_MODULE_ID, __FILE__, __LINE__, __VA_ARGS__); \
    } \
}

/**
 * @brief A macro to be used to log fatal level messages.
 */
#define LOG_FATAL(...)      LOG(LOG_LEVEL_FATAL, __VA_ARGS__)

/**
 * @brief A macro to be used to log error level messages.
 */
#define LOG_ERROR(...)      LOG(LOG_LEVEL_ERROR, __VA_ARGS__)

/**
 * @brief A macro to be used to log warning level messages.
 */
#define LOG_WARNING(...)    LOG(LOG_LEVEL_WARNING, __VA_ARGS__)

/**
 * @brief A macro to be used to log info level messages.
 */
#define LOG_INFO(...)       LOG(LOG_LEVEL_INFO, __VA_ARGS__)

/**
 * @brief A macro to be used to log debug level messages.
 */
#define LOG_DEBUG(...)      LOG(LOG_LEVEL_DEBUG, __VA_ARGS__)

#else
#define LOG(...)            do {} while(0)
#define LOG_FATAL(...)      do {} while(0)
#define LOG_ERROR(...)      do {} while(0)
#define LOG_WARNING(...)    do {} while(0)
#define LOG_INFO(...)       do {} while(0)
#define LOG_DEBUG(...)      do {} while(0)
#endif /* KISO_LOGGING */

/**
 * @brief External reference to package log level variable.
 */
extern uint8_t LOG_LEVEL_VAR(KISO_PACKAGE_ID);

/**
 * @brief External reference to module log level variable.
 */
extern uint8_t LOG_LEVEL_VAR(KISO_MODULE_ID);

/**
 * @brief A function pointer type to log recorder initializer.
 */
typedef Retcode_T (*LogRecordInit_T)(void *self);

/**
 * @brief A function pointer type to log recorder deinitializer.
 */
typedef Retcode_T (*LogRecordDeinit_T)(void *self);

/**
 * @brief A function pointer type to log recorder writer.
 */
typedef Retcode_T (*LogRecordWrite_T)(void *self, LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, va_list args);

/**
 * @brief A function pointer type to log appender initializer.
 */
typedef Retcode_T (*LogAppenderInit_T)(void *wakeup);

/**
 * @brief A function pointer type to log appender writer.
 */
typedef Retcode_T (*LogAppenderWrite_T)(const char *message, uint32_t length);

/**
 * @brief A log recorder operations type.
 */
typedef struct
{
    LogAppenderInit_T Init;
    LogAppenderWrite_T Write;
} LogAppender_T;

/**
 * @brief A log recorder operations type.
 */
typedef struct
{
    LogRecordInit_T Init;
    LogRecordDeinit_T Deinit;
    LogRecordWrite_T Write;
    void *Wakeup;
    LogAppender_T Appender;
} LogRecorder_T;

/**
 * @brief External reference to log asynchronous recorder.
 */
extern const LogRecorder_T *Logging_AsyncRecorder;

/**
 * @brief External reference to log synchronous recorder.
 */
extern const LogRecorder_T *Logging_SyncRecorder;

/**
 * @brief External reference to log UART appender.
 */
extern const LogAppender_T *Logging_UARTAppender;
/**
 * @brief External reference to log RTT appender.
 */
extern const LogAppender_T *Logging_RttAppender;

/**
 * @brief External reference to log external flash appender.
 */
extern const LogAppender_T *Logging_ExtFlashMemAppender;

/**
 * @brief Initializes the log recorder and appender engines.
 *
 * @param [in] recorder    A log recorder implementation.
 * @param [in] appender    A log appender implementation.
 *
 * @retval RETCODE_OK            if successfully initialized
 * @retval RETCODE_INVALID_PARAM if recorder pointer, any of recorder pointer attributes (see #LogRecorder_T)
 *                               or appender pointer, any of appender pointer attributes (see #LogAppender_T) is null
 * @return errors from recorder.init (see #LogRecordInit_T) or from appender.Init (see #LogAppenderInit_T)
 *         will be returned
 */
Retcode_T Logging_Init(const LogRecorder_T *recorder, const LogAppender_T *appender);

/**
 * @brief Log a message
 *
 * @retval #RETCODE_OK               if successfully executed
 * @retval #RETCODE_UNINITIALIZED    if the logger has not been initialized previously
 * @retval #RETCODE_NOT_SUPPORTED    if there is no matching filter (see #LogFilter_Apply)
 * @return #errors from recorder.Write (see #LogRecorder_T.Write) will be returned
 */
Retcode_T Logging_Log(LogLevel_T level, uint8_t package, uint8_t module, const char *file, uint32_t line, const char *fmt, ...);

/**
 * @brief Log filter id type.
 */
typedef uint32_t LogFilterId_T;

/**
 * @brief Adds log filter item.
 *
 * @param [in] level    A filter log level.
 * @param [in] package  A filter log package id.
 * @param [in] module   A filter log module id.
 * @return              A filter id if successfully added, LOG_FILTER_ID_INVALID (see #LOG_FILTER_ITEM_COUNT set to same value) otherwise.
 */
LogFilterId_T LogFilter_Add(LogLevel_T level, uint8_t package, uint8_t module);

/**
 * @brief Deletes log filter item.
 *
 * @param[in] id   A filter id to be removed.
 *
 * @retval #RETCODE_OK               if successfully deleted
 * @retval #RETCODE_INVALID_PARAM    if id is bigger than #LOG_FILTER_ITEM_COUNT
 */
Retcode_T LogFilter_Delete(LogFilterId_T id);

/**
 * @brief Modifies existing log filter item.
 *
 * @param [in] id       A filter id.
 * @param [in] level    A new filter log level.
 * @param [in] package  A new filter log package id.
 * @param [in] module   A new filter log module id.
 *
 * @retval #RETCODE_OK               if filter is successfully configured
 * @retval #RETCODE_INVALID_PARAM    if id is bigger than #LOG_FILTER_ITEM_COUNT
 */
Retcode_T LogFilter_Configure(LogFilterId_T id, LogLevel_T level, uint8_t package, uint8_t module);

/**
 * @brief An internal function to check if there is matching filter present.
 *
 * @param [in] level    A log level.
 * @param [in] package  A package id.
 * @param [in] module   A log module id.
 *
 * @return True if the matching filter present or the latter is inactive, false otherwise.
 */
bool LogFilter_Apply(LogLevel_T level, uint8_t package, uint8_t module);

#endif /* if KISO_FEATURE_LOGGING */

#endif /* KISO_LOGGING_H */
/** @} */
