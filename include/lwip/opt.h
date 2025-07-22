/*
 * The lwIP opt.h file
 */

#include <stddef.h>

#ifndef LWIP_OPT_H
#define LWIP_OPT_H

#define LWIP_SNMP      1
#define LWIP_IPV4      1

typedef signed char     s8_t;
typedef signed short   s16_t;
typedef signed int     s32_t;

typedef unsigned   char u8_t;
typedef unsigned short u16_t;
typedef unsigned   int u32_t;

#ifdef LWIP_ERR_T
typedef LWIP_ERR_T err_t;
#else /* LWIP_ERR_T */
typedef s8_t err_t;
#endif /* LWIP_ERR_T*/

#define MIB2_STATS   1

#define LWIP_UDP     1

#include "lwip/stats.h"

/** In the following macro, message will contain
 *  a const char string. */
#include "zephyr/sys/__assert.h"
#define LWIP_ASSERT(phrase, expression) __ASSERT(expression, phrase)

#define SNMP_USE_RAW             0  /* lwIP raw sockets. */
#define SNMP_USE_NETCONN         0  /* lwIP netconn. */
#define SNMP_USE_ZEPHYR          1  /* Use Zephyr TCP//IP stack. */

/**
 * IP_DEFAULT_TTL: Default value for Time-To-Live used by transport layers.
 */
#if !defined IP_DEFAULT_TTL || defined __DOXYGEN__
#define IP_DEFAULT_TTL                  255
#endif

#endif /* LWIP_OPT_H */
