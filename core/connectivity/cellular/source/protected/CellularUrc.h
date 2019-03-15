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
 *
 * @brief Defines prototypes for various URC handling routines.
 *
 * @details Defines prototypes for various URC (unsolicited response code) handling routines, which
 * are implemented in other driver modules. As such modules generally include a reference
 * to the CellularEngine (which implements URC handling), we have to prevent a circular
 * dependency - thus we can't simply include the headers of the other modules in CellularEngine.
 *
 * To implement a custom URC handler, place the prototype here and call it in CellularEngine_HandleUrcResponses()
 * implemented in CellularEngine.c
 *
 **/

#ifndef CELLULAR_URC_H
#define CELLULAR_URC_H

#include "BCDS_Basics.h"
#include "BCDS_Retcode.h"

typedef Retcode_T (*CellularUrcHandler_T)(void);

/**
 * @brief Implements URC handling for miscellaneous events.
 * 		  This function never blocks longer than required to handle the URC.
 * 		  If no URC is available, this function returns immediately.
 *
 * @retval RETCODE_OK URC handling was finished successfully.
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT If the URC this handler deals with was not present in the response queue
 */
Retcode_T CellularUrc_Miscellaneous(void);

/**
 * @brief Triggers the handling of unsolicited response code events in the AT response queue (if there are any).
 *
 * @retval RETCODE_OK If all URC handlers returned successfully. One URC handler failing will not prevent the others from being called.
 * @retval RETCODE_CELLULAR_URC_NOT_PRESENT If no URC handler had to do any work
 */
Retcode_T CellularUrc_HandleResponses(void);

#endif /* CELLULAR_URC_H */
