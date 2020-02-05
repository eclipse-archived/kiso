---
title: "Essentials Developer's Guide"
weight: 2
---

## Introduction

**Essentials** package contains the most basic software components for the development of an embedded application on **Kiso**, it came to serve the needs for agility in software and hardware development, where changing the underlying hardware does not necessarily mean scrapping all the already developed software. The package has been developed with ARM Cortex M micro-controllers in mind and gained a respectable level of maturity over the years of development.

Essentials API provides a common software layer to build applications on different hardware platforms. If an application needs to migrate from a hardware A to hardware B, it needs to have the BSP/MCU implementation for the B hardware without the need to port the entire application.

The most important component of Essentials is **HAL (Hardware Abstraction Layer)** which consists of two sub-components: BSP (Board Support Package) and MCU (Micro Controller Unit).

- The BSP is a collection of APIs for peripherals that could populate a hardware board such as LEDs, Buttons, Sensors, communication interfaces, etc. The creation and implementation of a BSP component follows a guideline detailed in the wiki page.
- The MCU sub-component contains common APIs for internal MCU resources such as UART, SPI, I2C, CRC, RNG, etc, and their implementation for the supported micro-controllers.

Until now the following MCU families have been supported or will be supported in a short while:

| MCU | Vendor | Core CPU | UART | SPI | I2C | RNG | CRC | Watchdog | Flash |
| :-- | :----- | :------- | :--- | :-- | :-- | :-- | :-- | :------- | :---- |
| STM32L4 | ST Microelectronics | Cortex M4 | [x] | [x] | [x] | [x] | [x] | [x] | [x] |
| STM32F7 | ST Microelectronics | Cortex M7 | [x] | [x] | [x] | [x] | [ ] | [ ] | [x] |
| EFM32GG | Silicon Labs | Cortex M3 | [x] | [x] | [x] | [ ] | [ ] | [x] | [x] |


Besides MCU and BSP, Essentials also contains:

* Basics module provides compiler abstractions,
* Retcode module provides a common basis for for error reporting and handling,
* HAL_Delay module provides active waiting functions hooking is possible in an OS environment,
* HAL_Signal module provides safe signaling mechanism in no OS environment, hooking is possible in an OS environment,
* HAL_Mutex module provides an abstraction for Os mutex,
* HAL_CriticalSection provides exclusion of code execution.

An embedded product contains one or more MCUs in a board populated with one or more peripherals. There are two major guiding principles for the creation of KISO's HAL:

1. Have a trivial interface for each peripheral which describes its integration into the board,
2. Separate peripheral related configurations (e.g. baudrate, clock speed, data format) from the common MCU resources usage (e.g sending a buffer of bytes over uart).

One more guideline related to the application development is that only MCU and BSP may provide low level abstractions i.e. no dependency from the application to vendor libraries as depicted in the picture1.


<div style="width:100%; text-align:center">
    <img src="../../images/HAL.jpg">
<p>Picture1</p>
</div>



Such an approach brings huge benefits for the end product. The splitting of the hardware abstraction layer into two sub-components MCU and BSP is the natural consequence of a translation of the circuit board of an embedded product into code, where the BSP takes care of the Board representation and peripherals configuration, and the MCU implements the core assets of a micro-controller's resources without the burden of configuration, which makes code portability to different micro-controllers a seamless task.
## Create a BSP abstraction


For constrained embedded systems, during handover from hardware developer to the software developer five major questions are  to be answered:

1. How are the components routed (CONNECTED) to the microcontroller?
2. What is to be done in order to bring a component to the operational state (ENABLED) ?
3. What is to be done in order to bring a component to OFF state (DISABLED) ?
4. Once the Component is disabled what GPIO configurations do we need to maintain in order to achieve the lowest power consumption safest state (DISCONNECTED) ?
5. What special considerations need to be taken to keep the whole system in operational state ?

Those questions are the core of the KISO BSP concept, and their answer is directly translated into code in the implementation.

The BSP interface should reflect the components datasheet and the BSP implementation should translate the circuit diagram into code. This is mostly what the embedded software developer needs to know in order to start working on BSP creation. We assume that we have a circuit board populated with a Micro-controller and some peripherals(e.g. sensors, modems...) as in Picture2.

<div style="width:100%; text-align:center">
    <img src="../../images/Circuit.jpg">
<p>Picture2</p>
</div>

### How to create a new BSP peripheral interface?

The first step in the creation of BSP interfaces is to assess which board components qualify for a new interface header. Per Definition, a BSP component is a peripheral on the circuit board which has a direct electrical connection to the micro-controller and is intended to be controlled directly from the micro-controller.

Above, in Picture2, Peripheral1 and Peripheral2 are considered BSP components but Peripheral3 is not, because it does not have any ties to the micro-controller and thus it is probably controllable only through Peripheral1.

Once done with the identification, and if the component does not have already a BSP interface, you can copy the template file Kiso_BSP_Template.h in essentials/include/bsp rename it to Kiso_BSP_<ComponentDesignation>.h where ComponentDesignation is ideally the official manufacturer's designation, reason for that is that another BSP developer can easily check whether for his component there has been an API created or not.

Now you can start tailoring the template to you needs.

After the copyright section you have to adjust the file documentation which will appear in the API's automated documentation.

~~~~
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

 *  @defgroup   KISO_HAL_BSP_Template Template

 *  @brief      Generic interface to Template Peripheral  BSP API

 *  @ingroup  KISO_HAL_BSP_IF
 *  @{

 * A prior call to BSP_Board_Initialize(param1, param2) function is necessary to initialize
 * the MCU resources needed by the Peripheral.

 * Once the Peripheral is requested a call to BSP_Template_Connect(deviceId) function
 * is required to map the internal MCU resources to their associated function.
 * The application needs get the deviceHandle via BSP_Template_GetHandle() and initializes
 * the resource driver by calling the MCU_<RESOURCE>_Initialize() and providing it with the previously
 * acquired BusHandle member of the deviceHandle and an event callback function.
 * @see HAL_DeviceHandle_S in Kiso_HAL.h
 * Then a call to BSP_Template_Enable(deviceId) will execute the required sequence
 * to enable the specified Peripheral peripheral and the MCU resources dedicated for
 * it (UART, SPI, I2C, etc.) and will associate the interrupt requests to their
 * appropriate service routines.

 * The BSP_Template_Disable(deviceId) will revert back what the
 * BSP_Template_Enable(deviceId) has done and set the Peripheral and the MCU resource
 * into disabled state.
 * The BSP_Template_Disconnect(deviceId) will put the GPIO pins associated to the
 * Peripherals peripheral in lowest possible power consumption configuration.

 * The following diagram describes the states and transitions for the Template Peripheral Device
 * @dot
 * digraph state_diagram {
 *      INIT[color="blue", shape=record]
 *      INIT -> CONNECTED[style="dashed", label="BSP_Template_Connect()"]
 *      CONNECTED -> DISCONNECTED [label="BSP_Template_Disconnect()"]
 *      DISCONNECTED -> CONNECTED [label="BSP_Template_Connect()"]
 *      CONNECTED -> ENABLED [label="BSP_Template_Enable()"]
 *      ENABLED -> DISABLED [label="BSP_Template_Disable()"]
 *      DISABLED -> ENABLED [label="BSP_Template_Enable()"]
 *      DISABLED -> DISCONNECTED [label="BSP_Template_Disconnect()"]
 *  }
 * @enddot

 * @file

 */
 *
~~~~

Rename the guarding macro according to your header file name and include "Kiso_HAL.h"
~~~~
#ifndef KISO_BSP_Template_H_
#define KISO_BSP_Template_H_
#include "Kiso_HAL.h"
~~~~


The feature switch will prevent the compiler from accessing the function declarations if the feature has not been enabled in your configuration file Kiso_HALConfig.h. This is particulary useful if in your project you have included this file and referred to its functions but you still do not have the implementation of the functions, What will happen if this feature switch does not exist is that the compiler will not complain, only at link time you will get an error. you can Imagine 30 minutes of compilation time wasted because of this feature switch.

~~~~
/* Code is only effective if feature is enabled in configuration */
#if KISO_FEATURE_BSP_TEMPLATE
~~~~


Now we come to the APIs. There have to be four core functions in each component BSP API, Connect(), Enable(), Disable() and Disconnect(). Each function has a defined behaviour that can be maintained even if the micro-controller changes:
* Connect() : After reset,to avoid current sinking/sourcing, the micro-controller pins are normally tristated (High Z), this results in nearly no electrical current flow between the micro-controller and the peripheral. The Connect() function establishes this current flow by configuring the GPIOs in the micro-controller to their desired function (Input, output, route to SPI, etc)without powering the peripheral.
~~~~
     /**
     * @brief       Maps the GPIO pins to their desired function.

     * @details     This function once called will map the GPIO pins related to the Peripheral to their desired functions.

     * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
     *              in /your/bsp/repo/include/BSP_<yourBoardName>.h

     * @note        deviceId = 0 is used to address all devices of this type.

     * @return      RETCODE_OK in the case of success, error code otherwise.
     */
    Retcode_T BSP_Template_Connect(int32_t deviceId);
~~~~
* Enable() : Now that the electrical connection has been established, the enable() function will make sure that the peripheral is in operation mode. That means power is supplied to the peripheral and the corresponding MCU resource has been activated and correctly configured, i.e., in case of UART, the UART micro-controller resource has been enabled (e.g clock gate opened) the baud-rate has been written to the baud-rate register, etc.
~~~~
     /**
     * @brief       Enables the control over the peripheral.

     * @details     This function once called will enable the peripheral and allow it receive data, execute commands
     *              and transmit process data.

     * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
     *              in /your/bsp/repo/include/BSP_<yourBoardName>.h

     * @note        deviceId=0 is used to address all devices of this type.

     * @return      RETCODE_OK in the case of success, error code otherwise.
     */
    Retcode_T BSP_Template_Enable(int32_t deviceId);
~~~~
* Disable() : As the name could tell this function should roll back what Enable() did.
~~~~
    /**
     * @brief       Disables the control over the peripheral.

     * @details     This function once called will disable control over the requested
     *              peripheral by executing the disabling procedure as prescribed in the
     *              vendor datasheet.

     * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
     *              in /your/bsp/repo/include/BSP_<yourBoardName>.h

     * @note        deviceId=0 is used to address all devices of this type

     * @return      RETCODE_OK in the case of success, error code otherwise.
     */
    Retcode_T BSP_Template_Disable(int32_t deviceId);
~~~~
* Disconnect() : As the name could tell this function should roll back what Connect() did.
~~~~
    /**
     * @brief       Disconnects the peripheral.

     * @details     This function disconnects the GPIO pins dedicated to the peripheral
     *              and put them into low power consumption configuration.

     * @param[in]   deviceId: Id of the targeted device. The deviceID is given by the BSP
     *              in /your/bsp/repo/include/BSP_<yourBoardName>.h

     * @note        deviceId=0 is used to address all devices of this type

     * @return      RETCODE_OK in the case of success, error code otherwise.
     */
    Retcode_T BSP_Template_Disconnect(int32_t deviceId);
~~~~
* In case the Peripheral is a communicator device the MCU layer should get a handle from the BSP pointing to the Peripheral's control structure, this handle is the contract between BSP and MCU and should therefore be accordingly initilized for the proper function of the peripheral. Since the application is not hardware dependent the Handle is passed as an abstract type known only from the BSP and MCU layers.
~~~~
    /**
     * @brief       Returns the handle of the communication interface used by the peripheral.

     * @details     After successful execution of BSP_Board_Initialize(param1, param2), the
     *              application is free to call this function in order to get the handle needed
     *              by the resources drivers. This handle will be used by the MCU drivers for the
     *              intended resources.

     * @return      HWHandle: Handle to the SPI interface
     */
    HWHandle_T BSP_Template_GetHandle(int32_t deviceId);
~~~~
* Besides these core functions the datasheet of the peripheral may dictate other helper functions, the recommendation for deciding on the helper functions is to spot in the components datasheet where GPIO operations are needed and to create functions for them. take for example the Bosch BMM150 Magnetometer sensor, in its BSP API interface in Kiso_BSP_BMM150.h you can see that there has been added helper functions which are managing CS, DATA_READY and INT lines and you can remark the difference of handling between input (DATA_READY, INT) and output (CS).


Now that you created your component's API please do not hide them into your project, rather commit them to Essentials and let the community review and refine them.
### How to create a new BSP implementation?

As said before, the BSP implementation should mirror you circuit diagram (though it is a bit more). let us concertize this into code. We will consider a the below simplified circuit diagram for the implementation of the Board Support Package, We will name it Project Compass. We start with creating our BSP repository for our Project Compass.


<div style="width:100%; text-align:center">
    <img src="../../images/Schematic.jpg">
<p>Picture3</p>
</div>

* BSP Repository Structure

  * /include folder shall contain interfaces specific to your BSP implementation required by your application,
  * /source folder contains the implementation,
  * /test folder contains unit test files.


<div style="width:100%; text-align:center">
    <img src="../../images/BSP_Repo.jpg">
<p>Picture4</p>
</div>


* From circuit diagram and datasheets to code
  * include/BSP_Compass.h is the interface between your bsp specific implementation and the application it enumerates the board components and tells your BSP specific functions parameters.

~~~~
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
     * @defgroup BSP_COMPASS  COMPASS BSP
     * @ingroup  BSP_COMPASS
     * @{
     * @file
     * @brief Project Compass Board Support Package
     *
     */

    #ifndef BSP_COMPASS_H_
    #define BSP_COMPASS_H_

    #include "Kiso_Basics.h"
    #include "BSP_CompassRetcodes.h"

    /**
     * IDs of Available buttons
     */
    enum Compass_ButtonIds_E
    {
        COMPASS_BUTTON1_ID = 1
    };

    /**
     * IDs of available LEDs
     */
    enum Compass_LED_Ids_E
    {
        COMPASS_LED_RED_ID = 1,
    };

    /**
     * IDs of available BMM150 sensors
     */
    enum Compass_BMM150Ids_E
    {
        COMPASS_BMM150_ID = 1,
    };

    /**
     * Enumeration of BSP states
     */
    enum BSP_States_E
    {
        BSP_STATE_RESET, /**< BSP_STATE_RESET the device is at reset state */
        BSP_STATE_INIT, /**< BSP_STATE_INIT the device has been initialized */
        BSP_STATE_CONNECTED, /**< BSP_STATE_CONNECTED the device is connected*/
        BSP_STATE_ENABLED, /**< BSP_STATE_ENABLED the device is enabled */
        BSP_STATE_DISABLED, /**< BSP_STATE_DISABLED the device is at disabled state */
        BSP_STATE_DISCONNECTED, /**< BSP_STATE_DISCONNECTED the device is at disconnected state */
        BSP_STATE_ERROR, /**< BSP_STATE_ERROR the device is at error state */
    };

    enum BSP_ButtonState_E
    {
        BSP_BUTTON_STATE_ERROR = -1,  /**< BSP_BUTTON_STATE_ERROR */
        BSP_BUTTON_STATE_RELEASED,/**< BSP_BUTTON_STATE_PRESSED */
        BSP_BUTTON_STATE_PRESSED /**< BSP_BUTTON_STATE_RELEASED */
    };


    /**< Default param1 for BSP_Board_Initialize() */
    #define BSP_BOARD_DEFAULT_PARAM1       UINT32_C(0)

    /**< Default param2 for BSP_Board_Initialize() */
    #define BSP_BOARD_DEFAULT_PARAM2       NULL

    #endif /* BSP_COMPASS_H_ */

    /**@}*/

~~~~

  * include/BSP_CompassRetcodes.h enumerates the bsp modules (.c files) and the specific return codes from your bsp implementation.
~~~~

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
     * @brief Contains Retcode Module Ids and BSP's implementation specific return codes

     * @details Enumerates ".c" files as retcode module Ids and specific error codes from each module's implementation.
     */
    #ifndef INCLUDE_BSP_RETCODES_H_
    #define INCLUDE_BSP_RETCODES_H_
    #include "Kiso_Retcode.h"
    /**
     * Module IDs provided by the BSP for retcode composition
     */
    enum BSP_COMPASS_Modules_E
    {
        /* BSP Modules */
        MODULE_BSP_API_BOARD,
        MODULE_BSP_API_BUTTON,
        MODULE_BSP_API_BMM150,
        MODULE_BSP_API_LED,
    };
    /**
     * Return Codes provided by the BSP
     */
    enum BSP_Retcode_E
    {
        /* BSP Return codes */
    };
    #endif /* INCLUDE_BSP_RETCODES_H_ */


~~~~
  * include/Kiso_HALConfig.h is where HAL features switches for implemented features are defined.

~~~~

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
     * @brief Contains a default configuration to enable or disable HAL components.
     * @details This header file is usually included by the Kiso_HAL.h from the
     * HAL (Hardware Abstraction Layer) module. It is used to allow a per project
     * configuration of the features provided by the HAL component.
     *
     * Features are enabled or disabled by defining a particular feature to either
     * 1 or 0 e.g.
     * @code
     *   #define KISO_FEATURE_I2C    1   // Enable HAL feature I2C
     *   #define KISO_FEATURE_SPI    0   // Disable HAL feature SPI
     * If no defines are made then all HAL features will be enabled.
     */
    #ifndef KISO_HAL_CONFIG_H_
    #define KISO_HAL_CONFIG_H_

    #define KISO_FEATURE_UART                                                       0
    #define KISO_FEATURE_I2C                                                        1
    #define KISO_FEATURE_SPI                                                        0
    #define KISO_FEATURE_FLASH                                                      0
    #define KISO_FEATURE_WATCHDOG                                                   0
    #define KISO_FEATURE_PWM                                                        0
    #define KISO_FEATURE_SLEEP                                                      0

    #if (KISO_FEATURE_UART)
    /* Defines the default count of possible UART handles */
    #define KISO_UART_COUNT                                                         (UINT32_C(1))
    #endif /* if (KISO_FEATURE_UART) */

    #if (KISO_FEATURE_I2C)
    /* Defines the default count of possible I2C handles */
    #define KISO_I2C_COUNT                                                          (UINT32_C(1))
    #endif /* if (KISO_FEATURE_I2C) */

    #if (KISO_FEATURE_SPI)
    /* Defines the default count of possible SPI handles */
    #define KISO_SPI_COUNT                                                          (UINT32_C(2))
    #endif /* if (KISO_FEATURE_SPI) */


    /* Define the BSP features which are supported by the Compass board BSP */
    #define KISO_FEATURE_BSP_PWM_LED                                                1
    #define KISO_FEATURE_BSP_BUTTON                                                 1
    #define KISO_FEATURE_BSP_BMM150                                                 1

    #endif /*KISO_HAL_CONFIG_H_*/
~~~~


The source folder is where this translation from circuit diagram to code takes place. For our Project Compass, the source folder as in Picture4 contains a common settings header file and the implementation of the different BSP API functions for the board components.

  * source/protected/BSP_Settings.h is where your board settings are defined (e.g pin mapping, pin attributes) and the circuit diagram translated into defines which will be later used in the implementation. Below is the settings file for our Project Compass

~~~~
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
     * @defgroup BSP_COMPASS_BOARD_SETTINGS     COMPASS Board Settings
     * @ingroup  BSP_COMPASS
     * @{
     * @file
     * @brief Board settings header file.
     * @details This file holds static board component configurations e.g pin map, pin attributes
     * and general use defines.
     */

    #ifndef BSP_SETTINGS_H_
    #define BSP_SETTINGS_H_

    #define DEVICE_FAMILY                       cc26x0r2
    #define DeviceFamily_CC26X0R2

    #include <drivers/PIN.h>
    #include <pin/PINCC26XX.h>
    #include <driverlib/ioc.h>
    #include <drivers/Power.h>
    #include <power/PowerCC26XX.h>
    #include <drivers/I2C.h>
    #include <i2c/I2CCC26XX.h>
    #include <drivers/UART.h>
    #include <uart/UARTCC26XX.h>
    #include <drivers/SPI.h>
    #include <spi/SPICC26XXDMA.h>
    #include <driverlib/udma.h>
    #include <drivers/PWM.h>
    #include <drivers/pwm/PWMTimerCC26XX.h>
    #include <drivers/timer/GPTimerCC26XX.h>
    #include <driverlib/cpu.h>
    #include <driverlib/systick.h>
    #include <driverlib/sys_ctrl.h>
    #include <dma/UDMACC26XX.h>
    #include <drivers/Watchdog.h>
    #include <watchdog/WatchdogCC26XX.h>
    #include <rf/RF.h>

    #include <sysbios/knl/Task.h>
    #include <sysbios/knl/Clock.h>
    #include <sysbios/family/arm/m3/Hwi.h>
    #include <crypto/CryptoCC26XX.h>
    #include <TRNGCC26XX.h>

    #include <ti/drivers/dpl/HwiP.h>
    #include <hw_ints.h>
    #include <hw_memmap.h>
    #include <vims.h>
    #include <systick.h>

    /* ************************************************************************** */
    /* MCU INFORMATION ********************************************************** */
    /* ************************************************************************** */

    /**< TI Device Family */

    /**< CPU Clock Frequency */
    #define CPU_CLOCK_FREQUENCY                 48000000Ul

    //*****************************************************************************
    //
    // Defined CPU delay macro with microseconds as input
    // Quick check shows: (To be further investigated)
    // At 48 MHz RCOSC and VIMS.CONTROL.PREFETCH = 0, there is 5 cycles
    // At 48 MHz RCOSC and VIMS.CONTROL.PREFETCH = 1, there is 4 cycles
    // At 24 MHz RCOSC and VIMS.CONTROL.PREFETCH = 0, there is 3 cycles
    //
    //*****************************************************************************
    #define VIMS_CACHE_PREFETCHING      1

    #if (1 == VIMS_CACHE_PREFETCHING)
    #define MCU_CYCLES_PER_DELAY      UINT32_C(4) /**< Number of MCU cycles wasted by a call to delay */
    #define DELAY_CYCLES_MS           UINT32_C(12000) /**< Amount of delay-cycles needed for a 1ms delay */
    #define DELAY_MS_MAX_NOSYSTICK    UINT32_C(357913) /* Avoid Saturation against 2^32 */
    #else
    #define MCU_CYCLES_PER_DELAY      UINT32_C(5) /**< Number of MCU cycles wasted by a call to delay */
    #define DELAY_CYCLES_MS           UINT32_C(9600) /**< Amount of delay-cycles needed for a 1ms delay */
    #define DELAY_MS_MAX_NOSYSTICK    UINT32_C(447392) /* Avoid Saturation against 2^32 */
    #endif

    /* ************************************************************************** */
    /* PIN IDs ****************************************************************** */
    /* ************************************************************************** */
    #define BOARD_PIN_BUTTON                    IOID_0    /**< */
    #define BOARD_PIN_LED_R_A                   IOID_1    /**< */
    #define BOARD_PIN_BMM150_INT                IOID_2    /**< */
    #define BOARD_PIN_BMM150_DRDY               IOID_3    /**< */
    #define BOARD_PIN_I2C_CLK                   IOID_4    /**< */
    #define BOARD_PIN_I2C_DATA                  IOID_5    /**< */
    #define BOARD_PIN_BMM150_PWR                IOID_6    /**< */

    /* ************************************************************************** */
    /* BUTTON SETTINGS ********************************************************** */
    /* ************************************************************************** */

    /* The button is pulled up by the switch press action (pressed == logic high) */
    #define BOARD_PIN_BUTTON_VALUE_PRESSED      UINT32_C(1)
    #define BOARD_PIN_BUTTON_VALUE_RELEASED     UINT32_C(0)

    #define BOARD_PIN_BUTTON_INIT               (BOARD_PIN_BUTTON | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_PULLDOWN | PIN_DRVSTR_MIN | PIN_HYSTERESIS)
    #define BOARD_PIN_BUTTON_CONNECT            (BOARD_PIN_BUTTON | PIN_INPUT_EN | PIN_GPIO_OUTPUT_DIS | PIN_PULLDOWN | PIN_DRVSTR_MIN | PIN_HYSTERESIS)
    #define BOARD_PIN_BUTTON_IRQ_ENABLE         (BOARD_PIN_BUTTON | PIN_IRQ_BOTHEDGES)
    #define BOARD_PIN_BUTTON_IRQ_DISABLE        (BOARD_PIN_BUTTON | PIN_IRQ_DIS)

    /* ************************************************************************** */
    /* BUTTON WAKEUP SETTINGS *************************************************** */
    /* ************************************************************************** */
    #define BOARD_PIN_BUTTON_ENABLE_WAKEUP  (BOARD_PIN_BUTTON | PIN_GPIO_OUTPUT_DIS | PIN_INPUT_EN | PIN_PULLDOWN | PIN_HYSTERESIS | PINCC26XX_WAKEUP_POSEDGE)

    /* ************************************************************************** */
    /* BUTTON DEBOUNCE SETTINGS *************************************************** */
    /* ************************************************************************** */
    #define BORAD_PIN_BUTTON_DEBOUNCE_PRESS_TIME_MS     50
    #define BORAD_PIN_BUTTON_DEBOUNCE_RELEASE_TIME_MS   50

    /* ************************************************************************** */
    /* I2C SETTINGS ******************************************************* */
    /* ************************************************************************** */

    /* I2C Resting Configuration */
    #define BOARD_PIN_I2C_CLK_INIT              (BOARD_PIN_I2C_CLK | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_NOPULL | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN)
    #define BOARD_PIN_I2C_DATA_INIT             (BOARD_PIN_I2C_DATA | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_NOPULL | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN)

    /* ************************************************************************** */
    /* BMM150 MAGNETOMETER SENSOR *********************************************** */
    /* ************************************************************************** */

    #define BOARD_PIN_BMM150_INT_INIT           (BOARD_PIN_BMM150_INT | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_GPIO_LOW | PIN_NOPULL | PIN_IRQ_DIS | PIN_DRVSTR_MIN)
    #define BOARD_PIN_BMM150_DRDY_INIT          (BOARD_PIN_BMM150_DRDY | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_GPIO_LOW | PIN_NOPULL | PIN_IRQ_DIS | PIN_DRVSTR_MIN)
    #define BOARD_PIN_BMM150_PWR_INIT           (BOARD_PIN_BMM150_PWR | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_GPIO_LOW | PIN_NOPULL | PIN_DRVSTR_MIN)

    #define BOARD_PIN_BMM150_INT_CONNECT        (BOARD_PIN_BMM150_INT | PIN_INPUT_EN | PIN_GPIO_OUTPUT_DIS  | PIN_NOPULL | PIN_IRQ_DIS | PIN_HYSTERESIS)
    #define BOARD_PIN_BMM150_DRDY_CONNECT       (BOARD_PIN_BMM150_DRDY | PIN_INPUT_EN | PIN_GPIO_OUTPUT_DIS | PIN_NOPULL | PIN_IRQ_DIS  | PIN_HYSTERESIS)
    #define BOARD_PIN_BMM150_PWR_CONNECT        (BOARD_PIN_BMM150_PWR | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_EN  | PIN_PULLDOWN | PIN_GPIO_LOW |PIN_DRVSTR_MAX)

    #define BOARD_PIN_BMM150_INT_ENABLE_IRQ     (BOARD_PIN_BMM150_INT  | PIN_IRQ_POSEDGE)
    #define BOARD_PIN_BMM150_DRDY_ENABLE_IRQ    (BOARD_PIN_BMM150_DRDY | PIN_HYSTERESIS)

    #define BOARD_PIN_BMM150_INT_DISABLE_IRQ    (BOARD_PIN_BMM150_INT |PIN_IRQ_DIS)
    #define BOARD_PIN_BMM150_DRDY_DISABLE_IRQ   (BOARD_PIN_BMM150_DRDY | PIN_IRQ_DIS)

    #define BOARD_BMM150_POWERUP_IN_MS     UINT32_C(2)

    /* ************************************************************************** */
    /* LED ********************************************************************** */
    /* ************************************************************************** */

    /* LED control resting configuration */
    #define BOARD_PIN_LED_R_A_INIT                  (BOARD_PIN_LED_R_A | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_DIS | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MIN)

    #define BOARD_PIN_LED_R_A_CONNECT               (BOARD_PIN_LED_R_A | PIN_INPUT_DIS | PIN_GPIO_OUTPUT_EN  | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX)

    /* ************************************************************************** */
    /* GENERIC PIN SETTINGS ***************************************************** */
    /* ************************************************************************** */
    #define BOARD_PIN_VALUE_HIGH                UINT32_C(1)
    #define BOARD_PIN_VALUE_LOW                 UINT32_C(0)

    #endif /* BSP_SETTINGS_H_ */
    /**@}*/
~~~~

> **_NOTE:_**  Particulary for the settings section, it is possible to split the above settings file into small fractions defining each component settings, this is useful if your board contains many components, in whichcase, a single file approach would hinder the maintainability of your code. You can choose whatever approach is suitable for your project.


  * source/BSP_API_Board.c is where you implement functions in Kiso_BSP_Board.h interface particularly BSP_Board_Initialize() which shall bring up your board to a determined safe state after Power on reset. Among others the function:

    * Initializes board pins for which the tristated state could pose a safety or security risk,
    * Initializes the micro-controller resources (SPI, UART, I2C, Watchdog, etc.)
    * Initializes the micro-controller power system,
    * Initializes the micro-controller memory system,
    * Initializes the micro-controller clock system,

BSP_Board_Initialize() will be the first function to call in your Application main. Param1 and param2 are to be defined in BSP_Compass.h file and are implementation specific. It could be the case that you need an initialization in the bootloader contex that differs from the initialization in normal operation context, those abstract parameters are there to allow you to do so. The function for our Compass project looks approximately like below.


~~~~
        /**
         * Refer to API interface for description
         */
        Retcode_T BSP_Board_Initialize(uint32_t param1, void* param2)
        {
            KISO_UNUSED(param1);
            KISO_UNUSED(param2);

            Power_init();
        #ifdef CACHE_AS_RAM
            // retain cache during standby
            Power_setConstraint(PowerCC26XX_SB_VIMS_CACHE_RETAIN);
            Power_setConstraint(PowerCC26XX_NEED_FLASH_IN_IDLE);
        #else
            // Enable iCache prefetching
            VIMSConfigure(VIMS_BASE, TRUE, TRUE);
            // Enable cache
            VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);
        #endif //CACHE_AS_RAM

        #if !defined( POWER_SAVING ) || defined( USE_FPGA )
            /* Set constraints for Standby, powerdown and idle mode */
            // PowerCC26XX_SB_DISALLOW may be redundant
            Power_setConstraint(PowerCC26XX_SB_DISALLOW);
            Power_setConstraint(PowerCC26XX_IDLE_PD_DISALLOW);
        #endif // POWER_SAVING | USE_FPGA

            PIN_init(BoardGpioInitTable);
            PWM_init();

        #if KISO_FEATURE_I2C
            I2C_init();
        #endif
            return RETCODE_OK;
        }
~~~~


We will not dig deep into other Board APIs rather we will move forward to other modules.

  * source/BSP_API_BMM150.c : In this file we want to implement the functions in the BSP API for the BMM150. We want to step by step through the implementation
The heading of the BSP_API_BMM150.c file contains, after doxygen documentation header, inclusion of the API header file. The rest of the sources shall be protected with a feature switch macro.
After including the rest of headers needed, it is recommended to overwrite KISO_MODULE_ID macro to assign it the  id of the current module, here MODULE_BSP_API_BMM150 defined in include/BSP_CompassRetcodes.h

~~~~
    /**
     * @defgroup BSP_BMM150 Compass BMM150 BSP implementation
     * @ingroup BSP_COMPASS_IP
     * @{
     */
    /**
     * @file
     * @brief  Implementation of Magnetometer BMM150 BSP functions ModuleID: MODULE_BSP_API_BMM150
     */

    #include "Kiso_BSP_BMM150.h"

    /* Code is effective when the feature is enabled */
    #if KISO_FEATURE_BSP_BMM150

    #include "protected/BSP_Common.h"

    /* Include target's I2C handle definition */
    #include "ti_cc26xx/Kiso_MCU_I2C_Handle.h"

    /* Redefine retcode Module ID */
    #undef KISO_MODULE_ID
    #define KISO_MODULE_ID  MODULE_BSP_API_BMM150

    Macros, static functions and variables can then be declared (preferably in that order).

    /*=============Macro definitions ==============================================*/

    /*=============Function declaration ===========================================*/

    /**
     * Interrupt service routine for interrupt lines INT and DRDY
     */
    static void BMM150_IntLineCallback(PIN_Handle handle, PIN_Id pinId);

    /**
     * Default application callback (to avoid null pointer exception)
     */
    static void BMM150_DefaultAppCallback(int32_t deviceId, uint32_t event);

    /*=============Variables declaration ==========================================*/
    /**
     *  Pin config needed by Ti_cc26xx PIN driver
     */
    static const PIN_Config BMM150_GPIOList[] =
            {
            BOARD_PIN_BMM150_INT_CONNECT,
            BOARD_PIN_BMM150_DRDY_CONNECT,
            BOARD_PIN_BMM150_PWR_CONNECT,
            PIN_TERMINATE /* End of the list indicator, needed by the API PIN_open() */
            };

    /**
     * Storage for BMM150 PIN client state
     */
    static PIN_State BMM150_GPIOState;

    /**
     * Storage for I2C structure
     */
    static struct MCU_I2C_Handle_S I2CStruct =
            {
                    .handle = NULL,
                    .callMode = KISO_HAL_CALL_MODE_BLOCKING,
            };

    /**
     * Device state for BSP internal state transition
     */
    static enum BSP_States_E deviceState = BSP_STATE_INIT;

    /**
     * Application callback for INT line
     */
    static BSP_BMM150_InterruptCallback_T appIntCallback = BMM150_DefaultAppCallback;
    /**
     * Application callback for DRDY line
     */
    static BSP_BMM150_InterruptCallback_T appDrdyCallback = BMM150_DefaultAppCallback;
~~~~
Now we come to the API implementation and we start it with the connect() function. Here the goal of the function, like in API description, is to configure and control pins to their desired function. I2C_SCL and I2C_SDA, are here exception because they are managed through the I2C driver from TI_CC26xx SDK. PWR, INT, and DRDY pins will be configured during the call to PIN_open() API.  the implementation is as follows:
~~~~
    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @retval RETCODE_OK in case of success
     * @retval RETCODE_INCONSISTENT_STATE if device state prohibits routine execution
     * @retval RETCODE_FAILURE if PIN_open failed to execute
     *
     */
    Retcode_T BSP_BMM150_Connect(int32_t deviceId)
    {
        /* Only because there is one instance of the BMM150 sensor on board, processing of deviceId is omitted */
        KISO_UNUSED(deviceId);

        Retcode_T retcode = RETCODE_OK;

        /*Check device status*/
        if ((BSP_STATE_INIT != deviceState) && (BSP_STATE_DISCONNECTED != deviceState))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
        /* Connect BMM150 GPIOs */
        if (RETCODE_OK == retcode)
        {
            if (NULL == PIN_open(&BMM150_GPIOState, BMM150_GPIOList))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        /* validate state transition */
        if (RETCODE_OK == retcode)
        {
            deviceState = BSP_STATE_CONNECTED;
        }
        return retcode;
    }

~~~~
Enbale() function will power on the device via setting to high PWR pin and enable the I2C peripheral. After call to this function successfully executed, it should be possible to communicate with the sensor via MCU_I2C APIs.
~~~~
    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @retval RETCODE_OK in case of success
     * @retval RETCODE_INCONSISTENT_STATE if device state prohibits routine execution
     * @retval RETCODE_FAILURE if PIN_setOutputValue or I2C_open failed to execute
     */
    Retcode_T BSP_BMM150_Enable(int32_t deviceId)
    {
        /* Only because there is one instance of the BMM150 sensor on board, processing of deviceId is omitted */
        KISO_UNUSED(deviceId);

        Retcode_T retcode = RETCODE_OK;

        /* Check current state */
        if ((BSP_STATE_CONNECTED != deviceState) && (BSP_STATE_DISABLED != deviceState))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
        /* Power On the device */
        if (RETCODE_OK == retcode)
        {
            if (PIN_SUCCESS != PIN_setOutputValue(&BMM150_GPIOState, BOARD_PIN_BMM150_PWR, 1))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
            BSP_Board_Delay(BOARD_BMM150_POWERUP_IN_MS);
        }
        /* Configure I2C resource accordingly */
        if (RETCODE_OK == retcode)
        {
            I2C_Params parameters = {
                    I2C_MODE_BLOCKING, /* transferMode */
                    NULL, /* transferCallbackFxn */
                    I2C_400kHz, /* bitRate */
                    NULL /* custom */
            };
            I2CStruct.handle = I2C_open(BOARD_COMPASS_I2C_ID, &parameters);
            if (I2CStruct.handle == NULL)
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        /* Validate state transition */
        if (RETCODE_OK == retcode)
        {
            deviceState = BSP_STATE_ENABLED;
        }
        return retcode;
    }
~~~~
Disable() and Disconnect() roll back Enable() and Connect() functions
~~~~
    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @retval RETCODE_OK in case of success
     * @retval RETCODE_INCONSISTENT_STATE if device state prohibits routine execution
     * @retval RETCODE_FAILURE if PIN_setOutputValue failed to execute
     */
    Retcode_T BSP_BMM150_Disable(int32_t deviceId)
    {
        /* Only because there is one instance of the BMM150 sensor on board, processing of deviceId is omitted */
        KISO_UNUSED(deviceId);

        Retcode_T retcode = RETCODE_OK;

        /* Check device state */
        if ((BSP_STATE_ENABLED != deviceState))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
        /* Close I2C resource */
        if (RETCODE_OK == retcode)
        {
            I2C_close(BOARD_COMPASS_I2C_ID);
            I2CStruct.handle = NULL;
        }
        /* Power-off the device  */
        if (RETCODE_OK == retcode)
        {
            if (PIN_SUCCESS != PIN_setOutputValue(&BMM150_GPIOState, BOARD_PIN_BMM150_PWR, 0))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        /* validate state transition */
        if (RETCODE_OK == retcode)
        {
            deviceState = BSP_STATE_DISABLED;
        }
        return retcode;
    }


    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @retval RETCODE_OK in case of success
     * @retval RETCODE_INCONSISTENT_STATE if device state prohibits routine execution
     */
    Retcode_T BSP_BMM150_Disconnect(int32_t deviceId)
    {
        /* only because there is one instance of the BMM150 sensor on board, processing of deviceId is omitted */
        KISO_UNUSED(deviceId);

        Retcode_T retcode = RETCODE_OK;

        /* Check device state */
        if ((BSP_STATE_CONNECTED != deviceState) && (BSP_STATE_DISABLED != deviceState))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
        /* Close pin resource */
        if (RETCODE_OK == retcode)
        {
            PIN_close(&BMM150_GPIOState);
            deviceState = BSP_STATE_DISCONNECTED;
        }
        return RETCODE_OK;
    }
~~~~
GetHandle() function returns a pointer to I2CStruct variable previously declared in this file. this pointer will be used as handle for communication with the device over I2C bus via the MCU_I2C APIs. Before sending or receiving it is mandatory to call the function MCU_I2C_Initialize to "Initialize" the driver context according to which communication mode has been choosed by the BSP.
~~~~
    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @return Pointer to #I2CStruct
     */
    HWHandle_T BSP_BMM150_GetHandle(int32_t deviceId)
    {
        /* Only because there is one instance of the BMM150 sensor on board, processing of deviceId is omitted */
        KISO_UNUSED(deviceId);
        return (HWHandle_T) &I2CStruct;
    }
~~~~
Time now to implement APIs specific to the BMM150 chip.
IntEnable()  Assigns an interrupt callback passed from the application to process events coming from INT line and enables this Interrupt.
~~~~
    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @retval RETCODE_OK in case of success
     * @retval RETCODE_INCONSISTENT_STATE if device state prohibits routine execution
     * @retval RETCODE_FAILURE if PIN_registerIntCb or PIN_setInterrupt failed to execute
     */
    Retcode_T BSP_BMM150_IntEnable(int32_t deviceId, BSP_BMM150_InterruptCallback_T callback)
    {
        /* only because there is one instance of the BMM150 sensor on board processing of deviceId is omitted */
        KISO_UNUSED(deviceId);

        Retcode_T retcode = RETCODE_OK;
        /* check input parameters */
        if (NULL == callback)
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INVALID_PARAM);
        }
        /* Check device state */
        if (RETCODE_OK == retcode)
        {
            if ((BSP_STATE_CONNECTED != deviceState) && ((BSP_STATE_ENABLED != deviceState)))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
            }
        }
        /* Register interrupt service routine and application callback*/
        if (RETCODE_OK == retcode)
        {
            appIntCallback = callback;

            if (PIN_SUCCESS != PIN_registerIntCb(&BMM150_GPIOState, BMM150_IntLineCallback))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        /* Enable interrupt */
        if (RETCODE_OK == retcode)
        {
            if (PIN_SUCCESS != PIN_setInterrupt(&BMM150_GPIOState, BOARD_PIN_BMM150_INT_ENABLE_IRQ))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        return retcode;
    }
~~~~
IntDisable() Disables this interrupt and de-assigns the application callback.
~~~~
    /**
     * Refer to interface header Kiso_BSP_BMM150.h for API documentation
     * @retval RETCODE_OK in case of success
     * @retval RETCODE_INCONSISTENT_STATE if device state prohibits routine execution
     * @retval RETCODE_FAILURE if PIN_setInterrupt failed to execute
     */
    Retcode_T BSP_BMM150_IntDisable(int32_t deviceId)
    {
        /* Only because there is one instance of the BMM150 sensor on board processing of deviceId is omitted  */
        KISO_UNUSED(deviceId);

        Retcode_T retcode = RETCODE_OK;

        /* Check device state  */
        if ((BSP_STATE_CONNECTED != deviceState) && ((BSP_STATE_ENABLED != deviceState)))
        {
            retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_INCONSISTENT_STATE);
        }
        /* Disable interrupt */
        if (RETCODE_OK == retcode)
        {
            if (PIN_SUCCESS != PIN_setInterrupt(&BMM150_GPIOState, BOARD_PIN_BMM150_INT_DISABLE_IRQ))
            {
                retcode = RETCODE(RETCODE_SEVERITY_ERROR, RETCODE_FAILURE);
            }
        }
        /* reset callback */
        if (RETCODE_OK == retcode)
        {
            appIntCallback = BMM150_DefaultAppCallback;
        }
        return retcode;
    }
~~~~

