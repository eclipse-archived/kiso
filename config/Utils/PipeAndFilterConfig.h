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

#ifndef CONFIG_PIPEANDFILTERCFG_H_
#define CONFIG_PIPEANDFILTERCFG_H_

#define PIPE_SIZE 100         /**< Size of the internal buffer used by the pipe */
#define FILTER_STACK_SIZE 500 /**< Should be higher than 3*PIPE_SIZE */
#define FILTER_PRIORITY 1     /**< Priority of the pipe & filter construction in an RTOS context */

#endif /* CONFIG_PIPEANDFILTERCFG_H_ */
