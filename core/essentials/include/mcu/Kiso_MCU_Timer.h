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
 * @defgroup KISO_HAL_MCU_TIMER MCU Timer
 * @ingroup KISO_HAL_MCU_IF
 * @{
 *
 * @brief Unified HAL API for Timer hardware
 *
 * @details
 * Use this component to control the timer hardware peripheral The timer is being set in the BSP including
 * clock setup The application will get a driver handle by the BSP in order to control, start, stop the timer
 * Before using the timer it has to be initialized by a call to MCU_TIMER_Initialize
 *
 * @code{.c}
 * Retcode_T retcode = MCU_TIMER_Initialize(timer0, MyTimer0EventCallback);
 * if(RETCODE_OK != retcode)
 * {
 *      // handle the error
 * }
 * else
 * {
 *     // use the timer
 * }
 * @endcode
 *
 * Once the timer is successfully initialized, it can be used
 *
 * @note It is possible to disable the entire TIMER feature in the HAL BSP configuration file. i.e. the TIMER code will
 * only be compiled and linked if the feature <b>KISO_FEATURE_TIMER</b> is set to 1 in the configuration file.
 */

#ifndef KISO_MCU_Timer_H
#define KISO_MCU_Timer_H

#include "Kiso_HAL.h"

#if (KISO_FEATURE_TIMER)

/**
 * @brief       Timer Status
 */
enum MCU_Timer_Status_E
{
    MCU_TIMER_STATUS_INVALID,         /**< Invalid state */
    MCU_TIMER_STATUS_BSP_INITIALIZED, /**< Timer has been initialized by the BSP */
    MCU_TIMER_STATUS_INITIALIZED,     /**< Timer was initialized by the driver */
    MCU_TIMER_STATUS_ACTIVE,          /**< Timer is running */
    MCU_TIMER_STATUS_IDLE,            /**< Timer is idle */
    MCU_TIMER_STATUS_DEINITIALIZED,   /**<  Timer has been deinitialized */
    MCU_TIMER_STATUS_ERROR,           /**< Timer driver is in error state */
};

typedef enum MCU_Timer_Status_E MCU_Timer_Status_T;

/**
 * @brief       Type definition for the timer handle which is used to identify the timer instance to work with.
 */
typedef HWHandle_T Timer_T;

/**
 * @brief       Structure to represent the events that can be received from the timer in the callback function.
 */
struct MCU_Timer_Event_S
{

    uint8_t CompareMatch : 1; /**< A compare match has been detected on the compare channel. */
    uint8_t Overflow : 1;     /**< An overflow has been detected */
    uint8_t Underflow : 1;    /**< Underflow was detected */
    uint8_t reserved : 5;
    uint32_t CompareChannel : 8; /**< Which compare channel the event is related to. */
    uint32_t ErrorCode : 16;     /**< an error has been detected */
};

/**
 * @brief       Type definition for the Timer event callback.
 *
 * @details     Upon initialization of the timer, the user should pass a reference to a function of this signature
 *              (**void function(SPI_T, struct MCU_Timer_Event_S)**) handling event notifications from the Timer
 *              driver.
 *
 * @param [in]  timer : Is the handle of the timer instance that issued the event.
 *
 * @param [in]  event : Structure containing actual event information.
 */
typedef void (*MCU_TIMER_Callback_T)(Timer_T timer, struct MCU_Timer_Event_S event);

/**
 * @brief       This function initializes the timer and registers an event callback
 *
 * @details     The timer's clock base and settings are configure by the BSP After calling this function the timer is
 *              not yet active
 *
 * @param[in]   timer : The timer reference/handle (provided by the BSP)
 *
 * @param[in]   callback : A reference to a function which processes the SPI events issued from the SPI instance
 * @warning     The callback function is usually invoked in interrupt context. The implementation of the callback
 *              function must take this into account.
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_Initialize(Timer_T timer, MCU_TIMER_Callback_T callback);

/**
 * @brief       This function de-initializes the timer and de-registers the event callback
 *
 * @note        After having de-initialized the timer all calls to the timer API will return an error
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_Deinitialize(Timer_T timer);

/**
 * @brief       This function is a convenience function to set the compare value for a specific channel
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_SetCompareValue(Timer_T timer, uint32_t channel, uint32_t value);

/**
 * @brief       This function is a convenience function to get the compare value for a specific channel
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_GetCompareValue(Timer_T timer, uint32_t channel, uint32_t *value);

/**
 * @brief       This function is a convenience function to get the current count value of the timer
 *
 * @param[in]   timer : the timer reference
 *
 * @param[out]  countValue : Current count value of timer
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_GetCountValue(Timer_T timer, uint32_t *countValue);

/**
 * @brief       This function returns the maximum count value possible by the HW
 *
 * @param[in]   timer : the timer reference
 *
 * @param[out]  maxCount : Maximum count value supported by the timer
 */
Retcode_T MCU_Timer_GetMaxCount(Timer_T timer, uint32_t *maxCount);

/**
 * @brief       This function will start the timer (set to active state)
 *
 * @param[in]   timer: the timer handle to be started
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_Start(Timer_T timer);

/**
 * @brief       This function is a generic function to get a specific property of the timer
 *
 * @param[in]   timer: the timer handle to be stopped
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_Stop(Timer_T timer);

/**
 * @brief       This function returns the status of the timer driver
 *
 * @param[in]   timer: the timer handle to be queried
 *
 * @return      a state code indicating current driver state
 *
 */
MCU_Timer_Status_T MCU_Timer_GetStatus(Timer_T timer);

/**
 * @brief       This function is called by the Interrupt handler
 *
 * @param[in]   timer: the timer handle
 *
 */
void MCU_Timer_Callback(Timer_T timer);

/**
 * @brief       Get the Timer Clock Tick frequency
 *
 * @param[in]   timer : The timer reference
 *
 * @param[out]  frequency : Current value of clock frequency in Hz
 *
 * @return      RETCODE_OK in case of success, error code otherwise.
 */
Retcode_T MCU_Timer_GetClockFrequency(Timer_T timer, uint32_t *frequency);

#endif /* KISO_FEATURE_TIME */
#endif /* KISO_MCU_Timer_H */
/** @} */
