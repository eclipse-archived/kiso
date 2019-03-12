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
 * @defgroup    BCDS_HAL_BSP_Sensor_MAX44009 Light Sensor MAX44009
 * @ingroup     BCDS_HAL_BSP_IF
 * @{
 *
 * @brief       BSP API for MAX44009 light sensor from Maxim Integrated.
 * <A HREF="https://datasheets.maximintegrated.com/en/ds/MAX44009.pdf"><B>Datasheet</B></A>
 * @details
 * After reset, the microcontroller GPIOs are mostly tristated. Once the device is requested a call to
 * BSP_MAX44009_Connect() function is required to map its GPIOs to their desired function (input, output, SPI, etc).
 *
 * A call to BSP_MAX44009_Enable() will execute the required sequence to enable the device and its dedicated
 * MCU resources (e.g. I2C). The Device is then **controllable** through the microcontroller.
 *
 * BSP_MAX44009_Disable() will revert back what the BSP_MAX44009_Enable() has done and set the device and its related
 * MCU resources into disabled state.
 *
 * BSP_MAX44009_Disconnect() will put the GPIO pins to the lowest power consumption and risk free configuration
 * e.g. high impedance.
 *
 * BSP_MAX44009_GetHandle() will return a pointer to the MCU resource structure created and initialized by the BSP
 * implementation. This structure is MCU specific and its proper initialization is mandatory for the proper function of
 * the device. The Handle is the only contract between MCU layer and BSP layer.
 *
 * The following diagram describes the states and transitions for the light sensor:
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_MAX44009_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_MAX44009_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_MAX44009_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_MAX44009_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_MAX44009_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_MAX44009_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_MAX44009_Disconnect()"]
 *  }
 * @enddot
 */

#ifndef BCDS_BSP_MAX44009_H_
#define BCDS_BSP_MAX44009_H_

#include "BCDS_HAL.h"

#if(1 == BCDS_FEATURE_BSP_MAX44009)

/* ************************************************************************** */
/* I2C DEVICE ADDRESSES ***************************************************** */
/* ************************************************************************** */
#define BSP_MAX44009_I2C_ADDR0  UINT8_C(0x4A)   /**< MAX44009 Device Address 0 */
#define BSP_MAX44009_I2C_ADDR1  UINT8_C(0x4B)   /**< MAX44009 Device Address 1 */

/* ************************************************************************** */
/* RETURN CODES ************************************************************* */
/* ************************************************************************** */
#define BSP_MAX44009_I2C_FAIL       INT8_C(-1)  /**< MAX44009 I2C operation failed return code */
#define BSP_MAX44009_I2C_SUCCESS    INT8_C( 0)  /**< MAX44009 I2C operation succeeded return code */

/**
 * @brief       Type definition for the sensor interrupt callback
 *
 * @details     This callback is created by the application whenever the sensor has dedicated interrupt lines.
 *              The application should then pass the callback to the BSP_MAX44009_InterruptEnable() function.
 *              If the interrupt is triggered, the function will be called passing to the upper layer the edge
 *              resulted in the interrupt (falling edge or rising). Refer to the implementation for more details.
 */
typedef void (*BSP_MAX44009_InterruptCallback_T)(int32_t deviceId, uint32_t interruptEdge);

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
Retcode_T BSP_MAX44009_Connect(int32_t deviceId);

/**
 * @brief       Enables the control over the peripheral.
 *
 * @details     This function will execute the required sequence to enable the sensor and allows it to receive,
 *              execute commands, and transmit data.
 *
 * @pre         BSP_MAX44009_Connect() successfully executed.
 *
 * @param[in]   deviceId: Id of the targeted device. This id is given by the BSP in
 *              /your-bsp-repo/include/BSP_<yourBoardName>.h
 * @note        deviceId=0 is reserved to address all devices of this type.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_MAX44009_Enable(int32_t deviceId);

/**
 * @brief       Disables the control over the peripheral.
 *
 * @details     This function once called will disable control over the light sensor and if possible power it off.
 *
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your/bsp/repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_MAX44009_Disable(int32_t deviceId);

/**
 * @brief       Disconnects the light sensor.
 *
 * @details     This function will tristate the GPIOs of the sensor, except if a functional or security or safety
 *              risk opposes that, then it will put them into lowest power consumption safest configuration.
 *
 * @param[in]   deviceId: Id of the targeted device. The device id is given by the BSP
 *
 * @note        deviceId=0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_MAX44009_Disconnect(int32_t deviceId);

/**
 * @brief       Returns the handle of the communication interface used by the sensor.
 *
 * @details     The application calls this function in order to get the handle needed by the MCU resource drivers.
 *              The handle is a pointer to the MCU resource control structure created and initialized in the BSP
 *              implementation. The control structure is MCU specific and only known to BSP and MCU layers.
 *
 * @return      HWHandle_T: Handle for the used MCU resource.
 */
HWHandle_T BSP_MAX44009_GetHandle(int32_t deviceId);

/**
 * @brief       Returns the initialization parameters
 *
 * @details     Returns the reference to the initialization parameters associated with the deviceId
 *
 * @returns     Reference to the initialization parameters needed by the driver initialization routine
 */
void * BSP_MAX44009_GetInitParams(int32_t deviceId);

/**
 * @brief       Enables the interrupt handling for the nINT pin.
 *
 *
 * @details     This function allows interrupting the MCU on level change at INT pin. It shall appropriately configure
 *              the pin, accordingly assign the application callback, and enable the associated interrupt controller.
 *              Unless otherwise specified, the pin polarity shall correspond to the default device configuration.
 * @note        All the signals are to be regarded from device's perspective.
 *
 * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
 *              in /your/bsp/repo/include/BSP_<yourBoardName>.h file.
 *
 * @note        deviceId=0 is used to address all devices of this type.
 *
 * @param[in]   callback Function passed by the application to process interrupts coming from interrupt line.
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_MAX44009_IntEnable(int32_t deviceId, BSP_MAX44009_InterruptCallback_T callback);

/**
 * @brief       Disables INT interrupt Handling.
 *
 * @details     This function once called will disable the INT interrupt handling at BSP level e.g. in the NVIC and
 *              will stop forwarding events to the application through the interrupt callback.
 *
 * @param[in]   deviceId    Device Id given by the BSP in /your/bsp/repo/include/BSP_<yourBoardName>.h
 *
 * @note        deviceId = 0 is used to address all devices of this type
 *
 * @return      RETCODE_OK in the case of success, error code otherwise.
 */
Retcode_T BSP_MAX44009_IntDisable(int32_t deviceId);

/**@}*/

#endif /*BCDS_FEATURE_BSP_MAX44009*/
#endif /* BCDS_BSP_MAX44009_H_ */
