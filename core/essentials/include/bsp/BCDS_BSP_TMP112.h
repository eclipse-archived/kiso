/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) 2015-2018 Bosch Connected Devices and Solutions GmbH. 
 * Copyright (C) 2019 Robert Bosch GmbH.
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
 * @defgroup    BCDS_HAL_BSP_TMP112 Temperature Sensor TMP112
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for TMP112 Temperature sensor from Texas Instruments.
 * <A HREF="http://www.ti.com/lit/ds/symlink/tmp112.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_TMP112_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_TMP112_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. SPI). The Device is then **controllable** through the microcontroller.
 *
 * BSP_TMP112_Disable() will revert back what the BSP_TMP112_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_TMP112_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration.
 * e.g. high impedance. 
 *
 * BSP_TMP112_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the TMP112 temperature sensor:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_TMP112_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_TMP112_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_TMP112_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_TMP112_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_TMP112_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_TMP112_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_TMP112_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_TMP112_H_
#define BCDS_BSP_TMP112_H_

#include "BCDS_HAL.h"
/* Code is only effective if feature is enabled in configuration */
#if BCDS_FEATURE_BSP_TMP112

/**
 * @brief       Type definition for the sensor interrupt callback
 *
 * @details     The application should pass a function pointer of the same signature (**void function(uint32_t,uint32_t)**)
 *              to BSP_SensorNode_Enable() in order to handle interrupts coming from dedicated sensor's interrupt
 *              lines.
 */
typedef void (*BSP_TMP112_InterruptCallback_T)(uint32_t deviceId, uint32_t event);

/**
 * @brief       Maps the GPIO pins to their desired function.
 *
 * @details     This function once called will map the GPIO pins to their desired functions to drive the device.
 *              Basic IO operations are allowed after the successful execution of this function.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your/bsp/repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP112_Connect(uint32_t deviceId);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the accelerometer and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_TMP112_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. This id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is reserved to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP112_Enable(uint32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the sensor and if possible power it off.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP in
 *              /your/bsp/repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_TMP112_Disable(uint32_t deviceId);

/**
 * @brief       Disconnects the temperature sensor.
 *
 * @details     This function will tristate the GPIOs of the sensor, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_TMP112_Disconnect(uint32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the temperature sensor.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @param       deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your-bsp-repo/include/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_TMP112_GetHandle(void);

/**
 * @brief       Enables interrupt handling for the peripheral.
 *
 * @details     This function allows interrupting the MCU on level change at ALERT pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @pre         BSP_TMP112_Connect() successfully executed.
 *
 * @param[in]   callback: Function passed by the application to process interrupts
 *
 * @param[in]   deviceId    bsp device Id.
 *
 *@return       RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_TMP112_InterruptEnable(uint32_t deviceId, BSP_TMP112_InterruptCallback_T callback);

/**
 * @brief       Enables interrupt handling for the peripheral.
 *
 * @details     This function disables interrupts coming from the sensor specified by deviceId.
 *
 * @pre         BSP_TMP112_Connect() successfully executed.
 *
 *
 * @param[in]   deviceId: BSP device Id.
 *
 * @return      RETCODE_OK in the case of success or an error code otherwise.
 */
Retcode_T BSP_TMP112_InterruptDisable(uint32_t deviceId);

/**@}*//* BCDS_HAL_BSP_TMP112 */

#endif /*BCDS_FEATURE_BSP_TMP112*/
#endif /* BCDS_BSP_TMP112_H_ */
