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
 *
 * @brief Configuration header for the EventHub module.
 *
 */

#ifndef KISO_EVENTHUB_CONFIG_H_
#define KISO_EVENTHUB_CONFIG_H_

/**
 * Number of observers allowed within the eventHub.
 * One observer increases the RAM footprint by 12 bytes for each EventHub instance.
 * May be overridden by the projects compiler definitions.
 */
#ifndef CONFIG_EVENTHUB_MAX_OBSERVERS
#define CONFIG_EVENTHUB_MAX_OBSERVERS (16)
#endif

#endif // KISO_EVENTHUB_CONFIG_H_
