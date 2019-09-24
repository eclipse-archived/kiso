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
 *  @ingroup    KISO_ESSENTIALS
 *  @defgroup   KISO_RETCODE Retcode
 *  @{
 *
 *  @brief      Return code composition and handling.
 *
 *  @details
 *  This module provides helpful functions for creation and handling of return codes. It supports general error codes
 *  (see #Retcode_General_E), package and module IDs, severity information (see #Retcode_Severity_E) and a callback
 *  that can be registered to be called whenever a module calls Retcode_RaiseError() or Retcode_RaiseErrorFromIsr().
 *
 *  @file
 **/

#ifndef KISO_RETCODE_H_
#define KISO_RETCODE_H_

#include "Kiso_Basics.h"

#ifndef KISO_PACKAGE_ID
#warning "KISO_PACKAGE_ID not defined. The KISO_PACKAGE_ID is set to the default value 0"
#define KISO_PACKAGE_ID 0
#endif

#ifndef KISO_MODULE_ID
#define KISO_MODULE_ID 0
#endif

/**
 * @brief       This is the return code data type of the KISO platform.
 *
 * @details     Each retcode is composed of four parts: package, module, severity and code.
 *              - The package (most left byte - 8-bit value) indicates the package where the retcode has been composed.
 *              - The module (second left byte - 8-bit value) indicates the module where the retcode has been composed.
 *              -The severity  (Bits 15 - 12, 4-bit value) indicates the impact of the retcode, i.e. warning, error,
 *              fatal error or just an info.
 *              - The code (Bits 11 - 0, - 12 bit value) indicates the error code. There are some general error codes
 *              defined in #Retcode_General_E, ranging from 0 up to #RETCODE_FIRST_CUSTOM_CODE. Packages and projects
 *              may define their own codes starting from #RETCODE_FIRST_CUSTOM_CODE up to the code 4095.
 */
typedef uint32_t Retcode_T;

/**
 * @warning The below enums are provided for backward compatibility.
 * @note - Interface users are requested to not use this, since it will be removed going forward.
 */
#define RETCODE_INCONSITENT_STATE RETCODE_INCONSISTENT_STATE
#define RETCODE_DOPPLE_INITIALIZATION RETCODE_DOUBLE_INITIALIZATION
#define RETCODE_RTOS_QUEUE_ERROR RETCODE_QUEUE_ERROR
#define RETCODE_INVALID_SEMAPHORE_IN_ISR RETCODE_INVALID_SIGNAL_IN_ISR

/**
 * @enum        Enumeration to represent the General return codes.
 *
 * @details     The first codes from 0 to #RETCODE_FIRST_CUSTOM_CODE are reserved as general error codes.
 *              Packages and projects may define their own codes starting from #RETCODE_FIRST_CUSTOM_CODE up to the
 *              code 4095.
 */
enum Retcode_General_E
{
    RETCODE_SUCCESS = 0,
    RETCODE_FAILURE,               /**< An unspecified failure has occurred e.g. error in third party code */
    RETCODE_OUT_OF_RESOURCES,      /**< System resources have been exhausted e.g. not enough RAM */
    RETCODE_INVALID_PARAM,         /**< One or more passed input parameters was invalid (e.g. passed a null pointing buffer) */
    RETCODE_NOT_SUPPORTED,         /**< The requested operation is not supported by the current implementation */
    RETCODE_INCONSISTENT_STATE,    /**< The module is in a state preventing it from executing the requested operation*/
    RETCODE_UNINITIALIZED,         /**< The module has not been initialized */
    RETCODE_NULL_POINTER,          /**< During function execution a null pointer has been encountered */
    RETCODE_UNEXPECTED_BEHAVIOR,   /**< The function has produced an unintended side effect*/
    RETCODE_DOUBLE_INITIALIZATION, /**< Reinitializing the module may produce inconsistency */
    RETCODE_TIMEOUT,               /**< A timeout has occurred while waiting for the operation to complete */
    RETCODE_TIMER_ERROR,
    RETCODE_SEMAPHORE_ERROR,
    RETCODE_QUEUE_ERROR,
    RETCODE_INVALID_SIGNAL_IN_ISR,
    //...
    RETCODE_FIRST_CUSTOM_CODE = 65,
};

/**
 * @enum        Enumeration to represent the Severity of errors.
 */
enum Retcode_Severity_E
{
    RETCODE_SEVERITY_NONE = 0, /**< Reserved */
    RETCODE_SEVERITY_FATAL,    /**< An unrecoverable error which could lead to system failure has occurred in the issuing
                                module. Only a full  module reset or a full MCU reset may resolve the issue.*/
    RETCODE_SEVERITY_ERROR,    /**< The requested function could not successfully execute due to a recoverable error
                                (e.g. wrong input parameters).*/
    RETCODE_SEVERITY_WARNING,  /**< The requested function was successful but some trouble has been faced during execution
                                (e.g. a response time longer than usual).*/
    RETCODE_SEVERITY_INFO,     /**< Although the execution was fully successful, the function returns INFO to inform the
                                API user about something */
};

/**
 * @brief       Typedef to represent the Severity of error.
 */
typedef enum Retcode_Severity_E Retcode_Severity_T;

/**
 * @brief       Typedef to represent the Callback function to be called for handling the Error.
 *
 * @param[in]   error: The error that was raised
 * @param[in]   isFromIsr: TRUE if the error was raised from ISR, FALSE if not
 */
typedef void (*Retcode_ErrorHandlingFunc_T)(Retcode_T error, bool isFromIsr);

/**
 * @brief       This function initializes the Retcode module.
 *
 * @details     This function initializes the Retcode module. It registers an
 *              error handling callback function. When an error is raised,
 *              registered callback function of the corresponding component is called.
 *              This function shall be called only once.
 *
 * @param[in]   func : A callback function which is executed when an error is raised.
 *
 * @retval      #RETCODE_OK : On successful init
 *
 * @retval      #RETCODE_INCONSITENT_STATE : When this function is called more than once
 *
 * @retval      #RETCODE_NULL_POINTER : When the function pointer is NULL
 */
Retcode_T Retcode_Initialize(Retcode_ErrorHandlingFunc_T func);

/**
 * @brief       Raises the error to the error handling function.
 *
 * @details     Each component may use this function to raise an error to the application if it cannot handle the error
 *              itself. This call results in the invocation of the error handling function which has been given while
 *              the initialization of the Retcode module using the function Retcode_Initialize().
 *
 * @param[in]   error : The error to be raised to the application
 *
 * @see         Retcode_Initialize
 * @see         Retcode_ErrorHandlingFunc_T
 */
void Retcode_RaiseError(Retcode_T error);

/**
 * @brief       Raises the error from ISR (Interrupt) context only.
 *
 * @details     Each component may use this function to raise an error from ISR to the application. This call results
 *              in the invocation of the error handling function with the isFromIsr flag set. As this is called from
 *              ISR, the application can define a custom approach (in the registered callback function) based on the
 *              isFromIsr flag on how it wants to deal with the error from ISR. i.e. The application can log it into
 *              user page, post-process the error and so on, which cannot be done from ISR.
 *
 * @param[in]   error : The error to be raised to the application
 *
 * @see         Retcode_Initialize
 * @see         Retcode_ErrorHandlingFunc_T
 */
void Retcode_RaiseErrorFromIsr(Retcode_T error);

/**
 * @brief       A helper macro to make to create the package
 *
 * @note        Do not use directly, use #RETCODE(Severity, Code) instead!
 */
#define RETCODE_MK_PACKAGE(Package) ((Package & 0x000000FFUL) << 24UL)

/**
 * @brief       A helper macro to make Module Id
 * @note        Do not use directly, use #RETCODE(Severity, Code) instead!
 */
#define RETCODE_MK_MODULEID(ModuleID) ((ModuleID & 0x000000FFUL) << 16UL)

/**
 * @brief       A helper macro to make severity
 *
 * @note        Do not use directly, use #RETCODE(Severity, Code) instead!
 */
#define RETCODE_MK_SEVERITY(Severity) ((Severity & 0x0000000FUL) << 12UL)

/**
 * @brief       A helper macro to make code
 *
 * @note        Do not use directly, use #RETCODE(Severity, Code) instead!
 */
#define RETOCDE_MK_CODE(Code) (Code & 0x00000FFFUL)

/**
 * @brief       This Macro composes a return code based on input parameters.
 *
 * @details     This Macro composes a return code based on input parameters.
 *
 * @param [in]  PackageID : Package ID of the corresponding module
 *
 * @param [in]  ModuleID : Package ID of the corresponding module
 *
 * @param [in]  Severity : Severity of the error
 *
 * @param [in]  Code : Error code
 *
 * @note        Do not use directly, use #RETCODE(Severity, Code) instead!
 */
#define RETCODE_COMPOSE(PackageID, ModuleID, Severity, Code) \
    ((Retcode_T)(RETCODE_MK_PACKAGE(PackageID) | RETCODE_MK_MODULEID(ModuleID) | RETCODE_MK_SEVERITY(Severity) | RETOCDE_MK_CODE(Code)))

/**
 * @brief       This macro should be used to compose a Retcode_T value.
 *
 * @details     This macro creates a #Retcode_T from its four parts package, module, severity and code. While severity
 *              and code are passed as parameters, the package and module are taken from the macro KISO_PACKAGE_ID
 *              and KISO_MODULE_ID, which must be defined in each package / module.
 *
 * @param[in]   Severity : Severity of the error, see #Retcode_Severity_E
 *
 * @param[in]   Code : Error code
 *
 * Example:
 * @code
 *      #define KISO_PACKAGE_ID 42 // can be defined in makefile of the package, must be unique within global scope
 *      #undef KISO_MODULE_ID
 *        #define KISO_MODULE_ID KISO_MODULE_ID_MY_MODULE // must be unique within package scope
 *
 *        Retcode_T MyFunction(void)
 *        {
 *            ...
 *            return RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
 *        }
 * @endcode
 */
#define RETCODE(Severity, Code) \
    (RETCODE_COMPOSE((uint32_t)KISO_PACKAGE_ID, (uint32_t)KISO_MODULE_ID, (uint32_t)Severity, (uint32_t)Code))

/**
 * @brief       A helper macro to return success.
 *
 * @details     RETCODE_OK is used to inform about successful execution. It does not follow Retcode_T composition scheme
 *              i.e no package, module, nor severity is needed. It is defined as such to allow faster checks for success
 */
#define RETCODE_OK ((Retcode_T)RETCODE_SUCCESS)

/**
 * @brief       Provides the Package ID from a return code.
 *
 * @details     This function extracts the package id from the passed retcode value.
 *
 * @param[in]   retcode : return code.
 *
 * @return      Package ID from the provided retcode.
 */
static KISO_INLINE uint32_t Retcode_GetPackage(Retcode_T retcode)
{
    return (((uint32_t)retcode >> 24UL) & 0x000000FFUL);
}

/**
 * @brief       Provides the Module Id from a return code.
 *
 * @details     This function extracts the module id from the passed retcode value.
 *
 * @param[in]   retcode : return code
 *
 * @return      Module ID from the provided retcode.
 */
static KISO_INLINE uint32_t Retcode_GetModuleId(Retcode_T retcode)
{
    return (((uint32_t)retcode >> 16UL) & 0x000000FFUL);
}

/**
 * @brief       Provides the Severity of a return code.
 *
 * @details     This function extracts the severity value from the passed retcode value.
 *
 * @param[in]   retcode : return code
 *
 * @return      uint32_t : Severity value from the provided retcode.
 */
static KISO_INLINE Retcode_Severity_T Retcode_GetSeverity(Retcode_T retcode)
{
    return ((Retcode_Severity_T)(((uint32_t)retcode >> 12UL) & 0x0000000FUL));
}

/**
 * @brief       Provides the Error code from a return code.
 *
 * @details     This function extracts the error code from the passed retcode value. *
 *
 * @param[in]   retcode : return code
 *
 * @return      uint32_t : Error code from the provided retcode.
 */
static KISO_INLINE uint32_t Retcode_GetCode(Retcode_T retcode)
{
    return ((uint32_t)retcode & 0x00000FFFUL);
}

static KISO_INLINE uint32_t Retcode_getPackage(Retcode_T retcode)
{
    return Retcode_GetPackage(retcode);
}

/** LCOV_EXCL_STOP : End of section where code coverage is ignored. */
#endif /* KISO_RETCODE_H_ */
/**@} */
