/*
 * The lwIP opt.h file
 */

#ifndef LWIP_OPT_H
#define LWIP_OPT_H

/** In the following macro, message will contain
 *  a const char string. */
#include "zephyr/sys/__assert.h"
#define LWIP_ASSERT(phrase, expression) __ASSERT(expression, phrase)

#endif /* LWIP_OPT_H */
