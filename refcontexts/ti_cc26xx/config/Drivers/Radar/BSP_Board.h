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

#ifndef BSP_BOARD_H
#define BSP_BOARD_H

/* Fake interface so that the compiler is happy ==> File not to copy to a project!! */

/***************************************************************************//**
 * @brief Enable 3V3 domain.
 * @details Enables 3V3 domain and increment keep active counter.
 * @return None.
  ******************************************************************************/
void BSP_enable3V3Domain(void);

/***************************************************************************//**
 * @brief Disable 3V3 domain.
 * @details Decrement keep active counter and disables 3V3 domain if counter
 * reached zero.
 * @return None.
 ******************************************************************************/
void BSP_disable3V3Domain(void);

#endif /* BSP_BOARD_H */

