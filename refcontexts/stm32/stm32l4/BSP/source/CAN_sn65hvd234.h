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

#ifndef CAN_SN65HVD234_H_
#define CAN_SN65HVD234_H_

#include "BCDS_BSP_CAN_SN65HVD234.h"

#if BCDS_FEATURE_BSP_CAN_SN65HVD234

/* Include all headers which are needed by this file. */

/**
 * @brief Gets status of the module
 */
enum BSP_ModuleState_E CAN_SN65HVD234_getState(void);

/* Put the type and macro definitions here */

/* Put the function declarations here */

#endif /* BCDS_FEATURE_BSP_CAN_SN65HVD234 */

#endif /* CAN_SN65HVD234_H_ */

