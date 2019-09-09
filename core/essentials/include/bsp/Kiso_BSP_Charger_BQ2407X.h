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
 * @defgroup    KISO_HAL_BSP_CHARGER_BQ2407X Battery charger BQ2407X
 * @ingroup     KISO_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for BQ2407X charger from Texas Instruments.
 * <A HREF="http://www.ti.com/lit/ds/symlink/bq24073.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_Charger_BQ2407X_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_Charger_BQ2407X_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_Charger_BQ2407X_Disable() will revert back what the BSP_Charger_BQ2407X_Enable() has done and set the
 * device and its related MCU resources into disabled state.
 *
 * BSP_Charger_BQ2407X_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_Charger_BQ2407X_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the charger device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Charger_BQ2407X_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Charger_BQ2407X_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Charger_BQ2407X_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Charger_BQ2407X_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Charger_BQ2407X_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Charger_BQ2407X_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Charger_BQ2407X_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef KISO_BSP_Charger_BQ2407X_H
#define KISO_BSP_Charger_BQ2407X_H

#include "Kiso_HAL.h"
#include "BSP_BoardType.h"

/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_CHARGER_BQ2407X

/*
 * enumeration of custom return codes
 */
enum BSP_CHARGER_BQ2407X_Retcode_E
{
    RETCODE_BSP_CHARGER_NOT_ENABLED = RETCODE_FIRST_CUSTOM_CODE,
};

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.,
 */
Retcode_T BSP_Charger_BQ2407X_Connect(void);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the charger and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_Charger_BQ2407X_Connect() successfully executed.
 *
 * @param       control Options for the enable function depending from the charger hardware implementation.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Charger_BQ2407X_Enable(uint32_t control);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Charger_BQ2407X_Disable(void);

/**
 * @brief       Disconnects the charger.
 *
 * @details     This function will tristate the GPIOs of the accelerometer, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Charger_BQ2407X_Disconnect(void);

/**
 * @brief       Returns the charging status
 *
 * @details:    The status of the charger and its associated battery are to be fetched by this function
 *
 * @param[out]  state:  returns the charge state depending on the board specific implementation. The state values are
 *              located in the BSP implementation in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @retval      RETCODE_OK in case of successfully getting the charge status.
 * @retval      RETCODE_NULL_POINTER in case of NULL pointer passed.
 * @retval      RETCODE_BSP_CHARGER_NOT_ENABLED in case of accessing before enabling the charger.
 *
 */
Retcode_T BSP_Charger_BQ2407X_GetStatus(BSP_ChargeState_T* state);

/**
 * @brief       Measures the specified voltage signal
 *
 * @details:    The analog signals from the charger routed to the MCU could be fetched using this function. This
 *              function once called will trigger an AD conversion return the value of the specified signal in millivolts.
 *
 * @param[in]   signal to be measured defined by the BSP implementation in BSP implementation in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @param[out]  output: measured battery voltage in millivolts defined by the BSP implementation in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @retval      RETCODE_OK in case of successfully getting the battery voltage.
 * @retval      RETCODE_NULL_POINTER in case of NULL pointer passed.
 * @retval      RETCODE_BSP_CHARGER_NOT_ENABLED in case of accessing before enabling the charger.
 */
Retcode_T BSP_Charger_BQ2407X_MeasureSignal(uint32_t signal, uint32_t* output);

/**
 * @brief       Returns the charging status of the battery.
 *
 * @pre         BSP_Charger_BQ2407X_Enable() successfully executed.
 *
 *
 * @param[out]  state:  returns the charge state depending on the board specific implementation.The state values are
 *              located in the BSP implementation in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @param[in]   BatteryVoltage: measured battery voltage in millivolts.
 *
 * @retval      RETCODE_OK in case of successfully getting the charge status.
 * @retval      RETCODE_NULL_POINTER in case of NULL pointer passed.
 */
Retcode_T BSP_Charger_BQ2407X_CheckStatus(BSP_ChargeState_T* state, uint32_t BatteryVoltage);

/**
 * @brief       Sets the sysoff pin to high voltage level.
 *
 * @pre         BSP_Charger_BQ2407X_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Charger_BQ2407X_SetSysoffHigh(void);

/**
 * @brief       Sets the sysoff pin to low voltage level.
 *
 * @pre         BSP_Charger_BQ2407X_Connect() successfully executed.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_Charger_BQ2407X_SetSysoffLow(void);

#endif /* KISO_FEATURE_BSP_CHARGER_BQ2407X */
#endif /* KISO_BSP_Charger_BQ2407X_H */
/**  @} */
