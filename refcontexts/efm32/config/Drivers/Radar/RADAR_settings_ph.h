/******************************************************************************/
/**
 *  Copyright (C) Robert Bosch. All Rights Reserved. Confidential.
 *
 *  Distribution only to people who need to know this information in
 *  order to do their job.(Need-to-know principle).
 *  Distribution to persons outside the company, only if these persons
 *  signed a non-disclosure agreement.
 *  Electronic transmission, e.g. via electronic mail, must be made in
 *  encrypted form.
 *
 *  @file
 *
 *  Configuration header for the RADAR_settings_ph module.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef RADAR_SETTINGS_PH_H_
#define RADAR_SETTINGS_PH_H_

/*@brief SPI port, SPEED and route selection macros*/
#define RDA_SPI_PORT_NUMBER         SPI0                               /**< Defines SPI port NUMBER */
#define RDA_SPI_CLK_MODE            SPI_CLOCK_MODE_CPOL0_CPHA0         /**< Defines SPI mode selection */
#define RDA_SPI_ROUTE_LOCATION      SPI_ROUTE_LOCATION0                /**< Defines SPI driver route Location */

#endif /* RADAR_SETTINGS_PH_H_ */

/** ************************************************************************* */
