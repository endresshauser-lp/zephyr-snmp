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

#define LWIP_DONT_PROVIDE_BYTEORDER_FUNCTIONS  1

/* _HT_ define not used any more. */
#define DEFAULT_THREAD_STACKSIZE  1024
#define DEFAULT_THREAD_PRIO       5

#define MEM_ALIGNMENT           4U
#define PBUF_POOL_BUFSIZE      1470

#ifdef LWIP_ERR_T
typedef LWIP_ERR_T err_t;
#else /* LWIP_ERR_T */
typedef s8_t err_t;
#endif /* LWIP_ERR_T*/

#define LWIP_PBUF_CUSTOM_DATA_INIT( pbuf )  do {} while (0)

/** This is the aligned version of ip4_addr_t,
   used as local variable, on the stack, etc. */
/*
struct ip4_addr {
  u32_t addr;
};
*/

/** ip4_addr_t uses a struct for convenience only, so that the same defines can
 * operate both on ip4_addr_t as well as on ip4_addr_p_t. */
typedef struct ip4_addr ip4_addr_t;

//typedef ip4_addr_t ip_addr_t;

/**
 * IP_FORWARD==1: Enables the ability to forward IP packets across network
 * interfaces. If you are going to run lwIP on a device with only one network
 * interface, define this to 0.
 */
#if !defined IP_FORWARD || defined __DOXYGEN__
#define IP_FORWARD                      0
#endif

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
 * LWIP_PBUF_REF_T: Refcount type in pbuf.
 * Default width of u8_t can be increased if 255 refs are not enough for you.
 */
#if !defined LWIP_PBUF_REF_T || defined __DOXYGEN__
#define LWIP_PBUF_REF_T                 u8_t
#endif

#define LWIP_PBUF_CUSTOM_DATA  /* Nothing */

/**
 * IP_DEFAULT_TTL: Default value for Time-To-Live used by transport layers.
 */
#if !defined IP_DEFAULT_TTL || defined __DOXYGEN__
#define IP_DEFAULT_TTL                  255
#endif

#ifndef LWIP_ERROR
#ifdef LWIP_DEBUG
#define LWIP_PLATFORM_ERROR(message) LWIP_PLATFORM_DIAG((message))
#else
#define LWIP_PLATFORM_ERROR(message)
#endif

/* if "expression" isn't true, then print "message" and execute "handler" expression */
#define LWIP_ERROR(message, expression, handler) do { if (!(expression)) { \
  LWIP_PLATFORM_ERROR(message); handler;}} while(0)
#endif /* LWIP_ERROR */

#endif /* LWIP_OPT_H */
