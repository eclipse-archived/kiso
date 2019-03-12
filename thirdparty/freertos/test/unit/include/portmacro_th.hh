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
