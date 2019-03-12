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
 * @brief Lora Drivers config header.
 */

#ifndef BCDS_LORADRIVERSCONFIG_H_
#define BCDS_LORADRIVERSCONFIG_H_

/* Include all headers which are needed by this file. */

/* Put the type and macro definitions here */
/*
 * Below are the Features that are supported by this Package.
 * Please enable/disable them, based on the Project demand.
 */
/** 1 = Enabled, 0 = Disabled */
#ifndef BCDS_RN2483_LORA_DRIVER
#define BCDS_RN2483_LORA_DRIVER        (1)         /**< Enable BCDS_RN2483_LORA_DRIVER */
#endif
/* Put the function declarations here */


#endif /* BCDS_LORADRIVERSCONFIG_H_ */

