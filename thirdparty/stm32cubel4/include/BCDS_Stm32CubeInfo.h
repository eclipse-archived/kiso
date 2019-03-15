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
 * @brief Add a brief description here.
 *
 * @details Put here the documentation of this header file. Explain the interface exposed
 * by this header, e.g. what is the purpose of use, how to use it, etc.
 */

#ifndef BCDS_STM32CUBEINFO_H_
#define BCDS_STM32CUBEINFO_H_

/* No Includes here to avoid dependencies */

/**
 * @brief Declares an enumeration used to identify the module of the package
 * in case of errors.
 * @details Each component of the HAL package should have an unique ID assigned.
 * This ID is then used to identify the component of the package when an error
 * code is created. The error code will then contain:
 *  - Package ID
 *  - Module ID
 *  - Severity and
 *  - Error value
 * The basisc module provides error code functions to get the particular
 * information back from the combined error code.
 * @see Retcode_getPackage()
 * @see Retcode_getSeverity()
 * @see Retcode_getCode()
 *
 *  This is done by assigning the module ID to define BCDS_MODULE_ID which is
 *  then used by the Retcode macros from the essentials module to create the
 *  combined error code.
 *
 *  @attention Each header and source file of a module has to place the following
 *  code after the include block of its header and source files like
 *  e.g. for I2C:
 *  @code
 *  #include ...
 *  #include ...
 *  #undef BCDS_MODULE_ID
 *  #define BCDS_MODULE_ID BCDS_HAL_MODULE_ID_I2C
 *  @endcode
 *
 */

enum BCDS_STM32CUBE_ModuleID_E
{
    BCDS_STM32CUBE_MODULE_ID_HAL_DMA = 1

/* Define next BSP module ID here and assign a value to it! */
};

#endif /* BCDS_STM32CUBEINFO_H_ */

