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
 * @brief This File represents the Module IDs for the Application C modules
 *
 */

#ifndef XDK_APPINFO_H_
#define XDK_APPINFO_H_


#define TASK_PRIO_MAIN_CMD_PROCESSOR                (UINT32_C(1))
#define TASK_STACK_SIZE_MAIN_CMD_PROCESSOR          (UINT16_C(700))
#define TASK_Q_LEN_MAIN_CMD_PROCESSOR                (UINT32_C(10))

/**
 * @brief BCDS_APP_MODULE_ID for Application C module of XDK
 * @info  usage:
 *      #undef BCDS_APP_MODULE_ID
 *      #define BCDS_APP_MODULE_ID BCDS_APP_MODULE_ID_xxx
 */
enum XDK_App_ModuleID_E
{
    XDK_APP_MODULE_ID_MAIN,
    XDK_APP_MODULE_ID_LED_BUTTON,
/* Define next module ID here and assign a value to it! */
};

#endif /* XDK_APPINFO_H_ */
