/*----------------------------------------------------------------------------*/
/*
 * Copyright (C) Robert Bosch GmbH. 
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

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include "protected/usbd_cdc_if.h"

/*---------------------- MACROS DEFINITION --------------------------------------------------------------------------*/
/*---------------------- LOCAL FUNCTIONS DECLARATION ----------------------------------------------------------------*/
/*---------------------- VARIABLES DECLARATION ----------------------------------------------------------------------*/
/*---------------------- EXPOSED FUNCTIONS IMPLEMENTATION -----------------------------------------------------------*/
/*---------------------- LOCAL FUNCTIONS IMPLEMENTATION -------------------------------------------------------------*/

int _write(int file, char *ptr, int len)
{
    BCDS_UNUSED(file);
    if ((CDC_Transmit_FS((uint8_t*) ptr, (uint16_t) len)) != USBD_OK)
    {
        len = 0;
    }
    return len;
}
