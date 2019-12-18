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
 * @defgroup    KISO_HAL_BSP_Board Board
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for Board
 *
 * @details The following naming convention has been adopted:
 *
 *   - Board     : A board is an electrical system equipped with processing
 *                 capabilities and containing different interrelated electrical
 *                 components to serve for a defined mission.
 *
 *   - MCU       : Main/Micro Controller Unit is the component inside a board
 *                 responsible for controlling and correlating the different
 *                 board components.
 *
 *   - Peripheral: Any board component that is not the MCU and that is meant to
 *                 establish a connection  to the MCU in order to serve for the
 *                 board mission.
 *
 * After the MCU has executed its startup sequence and pointed to the main
 * function the application shall call directly BSP_Board_Initialize(param1, param2).
 *
 */

#ifndef KISO_BSP_Board_H
#define KISO_BSP_Board_H

#include "Kiso_HAL.h"

/**
 * @brief       Type definition for the SysTick callback
 *
 * @details     If an operating system is in use it may need to perform some operations each system clock tick, this
 *              callback has to be provided by the application through BSP_Board_OSTickInitialize() function.
 */
typedef void (*BSP_Systick_Callback_T)(void);

/**
 * @brief       Type definition for the delay function which is passed from the OS to the BSP in order to
 *              replace the blocking delay function.
 *
 * @param       delay expected parameter type.
 */
typedef void (*OStoBSPDelayFunc_T)(uint32_t delay);

/**
 * @brief       Initializes the entire board.
 *
 * @details     This function brings up the board to a determined safe state after Power on reset.
 *              Among others the function:
 *               - Initializes board pins for which the tristated state could pose a safety or security risk.
 *               - Initializes the microcontroller resources (SPI, UART, I2C, Watchdog, etc.).
 *               - Initializes the microcontroller power system.
 *               - Initializes the microcontroller memory system.
 *               - Initializes the microcontroller clock system.
 *               It could be the case that one needs an initialization in the bootloader context that differs from the
 *               initialization in the normal operation context, param1 and param2 are abstract parameters to allow
 *               doing so.
 *
 * @param       param1 : Initialization parameter depending on the implementation.
 *
 * @param       param2 : Initialization parameter depending on the implementation.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Board_Initialize(uint32_t param1, void *param2);

/**
 * @brief       Switches between different power modes.
 *
 * @details     This function is responsible of preparing switching between the available MCU power modes if the
 *              transition from the current mode to the next mode is allowed. Depending on the selected mode this
 *              function implements the recovery routines in order to bring the system to the state before it switched
 *              to that power mode.
 *
 * @param       powerMode: 32 bit value designating the desired power mode according to what is specified in the
 *              implementation under /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Board_SwitchPowerMode(uint32_t powerMode);

/**
 * @brief       Returns the last wake-up source
 *
 * @details     This function will return the last event that resulted in the wake-up of the MCU from a sleep/deep-sleep
 *              state.
 *
 * @return      The latest wake-up event source specified in /your-bsp-repo/include/BSP_<yourBoardName>.h.
 */
uint32_t BSP_Board_GetWakeupSource(void);

/**
 * @brief       Returns the power-supply mode
 *
 * @details     This function detects whether the MCU is being supplied using a battery or using an external power
 *              supply.
 *
 * @return      The current power supply type specified in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 */
uint32_t BSP_Board_DetectPowerSupply(void);

/**
 * @brief       Returns the BSP Revision.
 *
 * @return      BSP Revision.
 */
uint32_t BSP_Board_GetBoardRevision(void);

/**
 * @brief       Controls the on-board power rails.
 *
 * @details     Provided that there are controllable power rails on the board, the BSP will provide the necessary
 *              commands that are possible to have in order to control them safely.
 *
 * @param[in]   control: the desired power rail command implemented by the BSP.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Board_ControlPowerRail(uint32_t control);

/**
 * @brief       Initializes pre/post tick increment processing in OS based applications.
 *
 * @details     If an operating system needs to perform some operations before and/or after* the system tick count is
 *              incremented the application needs to call this function and provide the callbacks which are called
 *              before and/or after the tick increment.
 *
 * @param[in]   preCallback: A pointer to the function which the system needs to execute before incrementing the tick
 *              count of the system tick.
 * @note        If no pre-processing is needed provide NULL.
 *
 * @param[in]   postCallback: A pointer that points to the function which the system needs to execute after incrementing
 *              the tick count of the system tick.
 * @note        If no post-processing is needed provide NULL.
 */
void BSP_Board_OSTickInitialize(BSP_Systick_Callback_T preCallback, BSP_Systick_Callback_T postCallback);

/**
 * @brief       Provide accurate delay in ms based on variable incremented.
 *
 * @param[in]   delayInMs: specifies the delay time
 */
void BSP_Board_Delay(uint32_t delayInMs);

/**
 * @brief       Provide accurate delay  based on variable incremented in mikro seconds.
 *
 * @param[in]   delayInUs: specifies the delay time in us
 */
void BSP_Board_DelayUs(uint32_t delayInUs);

/**
 * @brief       Provides a time stamp based on variable incremented in milli-seconds.
 */
uint64_t BSP_Board_GetTick(void);

/**
 * @brief       Passes  delay function from OS to BSP.
 *
 * @details     If the application needs to replace the blocking delays with non blocking delays it has to assign the
 *              delay function to this function pointer.
 *
 * @param       delayFunc: delay function.
 *
 */
void BSP_Board_SetOStoBSPDelayFunc(OStoBSPDelayFunc_T delayFunc);

/**
 * @brief       Enable the Core SysTick
 *
 * @retval      RETCODE_OK if success
 * @retval      RETCODE_FAILURE if failure
 */
Retcode_T BSP_Board_EnableSysTick(void);

/**
 * @brief       Disable the Core SysTick
 */
void BSP_Board_DisableSysTick(void);

/**
 * @brief       Invoke Board Soft Reset. This puts the MCU registers to appropriate state applicable for soft reset.
 *              Please note this is not equivalent to hard reset.
 */
void BSP_Board_SoftReset(void);

#endif /* KISO_BSP_Board_H */

/**  @} */
