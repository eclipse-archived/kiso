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
 * ****************************************************************************/

#ifndef CONFIG_PIPEANDFILTERCFG_H_
#define CONFIG_PIPEANDFILTERCFG_H_

#define PIPE_SIZE 100 /**< Size of the internal buffer used by the pipe */
#define FILTER_STACK_SIZE 500 /**< Should be higher than 3*PIPE_SIZE */
#define FILTER_PRIORITY 1 /**< Priority of the pipe & filter construction in an RTOS context */

#endif /* CONFIG_PIPEANDFILTERCFG_H_ */
