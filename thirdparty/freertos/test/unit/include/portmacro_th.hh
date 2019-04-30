/*----------------------------------------------------------------------------*/
/*
 * Copyright (c) 2010-2019 Robert Bosch GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Robert Bosch GmbH - initial contribution
 */
/*----------------------------------------------------------------------------*/
 
#ifndef BCDS_portmacro_th_HH
#define BCDS_portmacro_th_HH

/* ban or wrap the real interface header */
#include"portmacro.h"

/* Include the needed headers */

/* Faked variables needs to be initialized by the test fixture */
#undef portYIELD_FROM_ISR
#define portYIELD_FROM_ISR(a)

/* Mock-ups for the provided interfaces */

#endif /* BCDS_portmacro_th_HH */
