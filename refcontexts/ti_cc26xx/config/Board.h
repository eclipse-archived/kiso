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
 * @brief Board configuration settings
 */
#ifndef __BOARD_H__
#define __BOARD_H__

/* public interface declaration */

/*
*   This is a simple gateway to the real Board.h file
*   Based on your include path to Board.h, the corresponding Board.c will also be included
*
*/

#if defined(R01) || defined(R00)
    #include "R01/Board_R01.h"
#else
    #error "Please set include path to point to appropriate CC2650EM device"
#endif

#endif /* __BOARD_H__ */
