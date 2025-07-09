/**
 * @file
 * SNMP netconn frontend.
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Dirk Ziegelmeier <dziegel@gmx.de>
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef __ZEPHYR__

	#include <netinet/in.h>
	#include <unistd.h>

#else

	#include <zephyr/net/socket.h>
	#include <zephyr/kernel.h>

#endif

#include <zephyr/net/socket_select.h>
#include <fcntl.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/net_if.h>
#include <app_version.h>
#include <zephyr/net/phy.h>

#include "lwip/apps/snmp_opts.h"

#if LWIP_SNMP && SNMP_USE_ZEPHYR

	#include <string.h>
	#include "lwip/ip.h"
	#include "snmp_msg.h"
	#include "lwip/sys.h"
	#include "lwip/prot/iana.h"
	#include "lwip/apps/snmp_mib2.h"

    #include <zephyr/logging/log.h>
	LOG_MODULE_REGISTER(snmp_log, CONFIG_LIB_SNMP_LOG_LEVEL);

	static void zephyr_snmp_agent(void *data0, void *data1, void *data2);
	K_THREAD_DEFINE(zephyr_snmp_thread, CONFIG_SNMP_STACK_SIZE, zephyr_snmp_agent, NULL, NULL,
        NULL, CONFIG_SNMP_THREAD_PRIORITY, 0, 0);

	typedef struct
	{
		int socket_161;  /* SNMP/serv socket */
		int socket_162;  /* SNMP/trap socket */
		struct timeval timeout; /* Maximum time to wait for an event. */
		int select_max;  /* The max parameter for select. */
	} socket_set_t;

	const ip_addr_t ip_addr_any;

/** udp_pcbs export for external reference (e.g. SNMP agent) */
/** Yes, a global variable. */
	struct udp_pcb * udp_pcbs;

/** Global variable containing lwIP internal statistics. Add this to your debugger's watchlist. */
	struct stats_ lwip_stats;

/** Wake up the thread 'z_snmp_client' in order to send a trap. */
	void snmp_send_zbus(void);

	static socket_set_t socket_set;

	#define CHAR_BUF_LEN  512 /* declared on the heap at first time use. */
	char char_buffer[CHAR_BUF_LEN];

	static int create_socket(unsigned port)
	{
		int socket_fd = -1;
		int opt;
		int ret;
		socklen_t optlen = sizeof( int );

		struct sockaddr_in6 bind_addr =
		{
			.sin6_family = AF_INET,
			.sin6_addr   = IN6ADDR_ANY_INIT,
			.sin6_port   = htons( port ),
		};

		socket_fd = zsock_socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

		if( socket_fd < 0 )
		{
			LOG_INF("create_socket: error: socket: %d errno: %d", socket_fd, errno );
			return -1;
		}
		else
		{
			LOG_INF("create_socket: socket: %d %s (OK)",
				socket_fd,
				(port == LWIP_IANA_PORT_SNMP_TRAP) ? "traps" : "server");

			ret = zsock_getsockopt( socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, &optlen );

			if (ret == 0 && opt != 0)
			{
				LOG_INF("create_socket: IPV6_V6ONLY option is on, turning it off." );

				opt = 0;
				ret = zsock_setsockopt( socket_fd, IPPROTO_IPV6, IPV6_V6ONLY,
								  &opt, optlen );

				if( ret < 0 )
				{
					LOG_INF("create_socket: Cannot turn off IPV6_V6ONLY option" );
				}
			}

			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 10000;
			int rc = zsock_setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
			LOG_INF("process_udp: zsock_setsockopt %d", rc);

			if( zsock_bind( socket_fd, ( struct sockaddr * ) &bind_addr, sizeof( bind_addr ) ) < 0 )
			{
				LOG_INF("create_socket: bind: %d", errno );
				zsock_close(socket_fd);
				return -1;
			}
		}
		return socket_fd;
	}

	static int max_int(int left, int right)
	{
		int rc = left;
		if (right > rc)
		{
			rc = right;
		}
		return rc;
	}

	void snmp_loop()
	{
		int rc_select;
		zsock_fd_set read_set; /* A set of file descriptors. */
		ZSOCK_FD_ZERO(&read_set);
		ZSOCK_FD_SET(socket_set.socket_161, &read_set);
		ZSOCK_FD_SET(socket_set.socket_162, &read_set);
		socket_set.select_max = max_int(socket_set.socket_161, socket_set.socket_162) + 1;

		rc_select = zsock_select(socket_set.select_max, &read_set, NULL, NULL, &(socket_set.timeout));
		if (rc_select > 0) for (int index = 0; index < 2; index++)
		{
			int udp_socket = (index == 0) ? socket_set.socket_161 : socket_set.socket_162;
			if (ZSOCK_FD_ISSET(udp_socket, &read_set))
			{
				struct sockaddr client_addr;
				struct sockaddr_in * sin = (struct sockaddr_in *) &client_addr;
				socklen_t client_addr_len = sizeof client_addr;
				int len;
				len = zsock_recvfrom( udp_socket,
								char_buffer,
								sizeof char_buffer,
								0, // flags
								&client_addr,
								&client_addr_len );
				if (len > 0) {
					int port = (index == 0) ? 161 : 162;
					char sin_addr_str[INET_ADDRSTRLEN + 1];
					net_addr_ntop(AF_INET, &sin->sin_addr, sin_addr_str, sizeof(sin_addr_str));
					sin_addr_str[sizeof(sin_addr_str) - 1] = '\0';
					LOG_DBG("recv[%u]: %d bytes from %s:%u", port, len, sin_addr_str, ntohs(sin->sin_port));
				}
				if (len > 0) //  && index == 0)
				{
					struct pbuf * pbuf = pbuf_alloc( PBUF_TRANSPORT, len, PBUF_RAM );

					if( pbuf != NULL )
					{
						pbuf->next = NULL;
						memcpy( pbuf->payload, char_buffer, len );
						pbuf->tot_len = len;
						pbuf->len = len;
						pbuf->ref = 1;

						ip_addr_t from_address;
						from_address.addr = sin->sin_addr.s_addr;
						snmp_receive( (void*) udp_socket, pbuf, &from_address, sin->sin_port);
						pbuf_free (pbuf);
					}
				} /* if (len > 0 && index == 0) */
			} /* FD_ISSET */
		} /* for (int index = 0 */
	}

/**
 * Starts SNMP Agent.
 */
	int snmp_init(void)
	{
		static int has_created = false;
		if (has_created == false) {
			has_created = true;

			/* Create the sockets. */
			socket_set.socket_161 = create_socket(LWIP_IANA_PORT_SNMP);
			if (socket_set.socket_161 < 0) {
				return -1;
			}
			socket_set.socket_162 = create_socket(LWIP_IANA_PORT_SNMP_TRAP);
			if (socket_set.socket_162 < 0) {
				zsock_close(socket_set.socket_161);
				return -1;
			}

			/* The lwIP SNMP driver owns a socket for traps 'snmp_traps_handle'. */
			snmp_traps_handle = ( void * ) socket_set.socket_162;

			socket_set.timeout.tv_sec = 0;
			socket_set.timeout.tv_usec = 10000U;
		}
		return 0;
	}

	/* send a UDP packet to the LAN using a network-endian
	 * port number and IP-address. */
	err_t snmp_sendto( void * handle,
					   struct pbuf * p,
					   const ip_addr_t * dst,
					   u16_t port )
	{
		int rc; /* Store the result of sendto(). */
		struct sockaddr client_addr;
		struct sockaddr_in * sin = (struct sockaddr_in *) &client_addr;
		socklen_t client_addr_len = sizeof(client_addr);

		sin->sin_addr.s_addr = dst->addr;
		sin->sin_port = port;
		sin->sin_family = AF_INET;
		// snmp_sendto: hnd = 8 port = 162, IP=C0A80213, len = 65

		rc = zsock_sendto ((int) handle, p->payload, p->len, 0, &client_addr, client_addr_len);
		char sin_addr_str[INET_ADDRSTRLEN + 1];
		net_addr_ntop(AF_INET, &sin->sin_addr, sin_addr_str, sizeof(sin_addr_str));
		sin_addr_str[sizeof(sin_addr_str) - 1] = '\0';
		LOG_DBG("snmp_sendto: hnd = %d port = %u, IP=%s, len = %d, rc %d", (int) handle, ntohs (port), sin_addr_str, p->len, rc);

		return rc;
	}

	u8_t snmp_get_local_ip_for_dst( void * handle,
									const ip_addr_t * dst,
									ip_addr_t * result )
	{
		ip_addr_copy( *result, *dst );
		return 1;
	}

	void * mem_malloc( mem_size_t size )
	{
		return k_malloc( size );
	}

	void mem_free( void * rmem )
	{
		k_free( rmem );
	}

	void * mem_trim( void * rmem,
					 mem_size_t newsize )
	{
		( void ) rmem;
		( void ) newsize;
		return rmem;
	}

	void * memp_malloc( memp_t type )
	{
		__ASSERT( false, "memp_malloc() should not be called" );
		return NULL;
	}

	void memp_free( memp_t type,
					void * mem )
	{
		( void ) type;
		( void ) type;
		( void ) mem;
		__ASSERT( false, "memp_free() should not be called" );
	}

#endif /* LWIP_SNMP && SNMP_USE_ZEPHYR */

void debug_log_oid(size_t oid_len, const u32_t *words, const char *func, const char *file, int line)
{
	char buf[128];
	int length = 0;
	int written;

	for (size_t i = 0; i < oid_len; i++) {
		written = snprintf (&buf[length], sizeof(buf) - length, "%u", words[i]);
		if (written < 0 || (size_t) written >= sizeof(buf)) {
			return;
		}
		length += written;
		if (i != oid_len - 1 && length < sizeof(buf)) {
			buf[length] = '.';
			length += 1;
		}
	}
	LOG_DBG("%s: oid '%s'", func, buf);
}

/* Use this function while stepping through the lwIP code. */
const char *leafNodeName (unsigned aType)
{
	switch (aType) {
	case SNMP_NODE_TREE:         return "Tree";         // 0x00
/* predefined leaf node types */
	case SNMP_NODE_SCALAR:       return "Scalar";       // 0x01
	case SNMP_NODE_SCALAR_ARRAY: return "Scalar-array"; // 0x02
	case SNMP_NODE_TABLE:        return "Table";        // 0x03
	case SNMP_NODE_THREADSYNC:   return "Threadsync";   // 0x04
	}
	return "Unknown";
}

static void zephyr_snmp_agent(void *data0, void *data1, void *data2)
{
    ARG_UNUSED(data0);
    ARG_UNUSED(data1);
    ARG_UNUSED(data2);

    int ok = snmp_init();
    if (ok < 0) {
        LOG_ERR("Failed to init snmp");
        return;
    }

    while (1)
    {
        snmp_loop();
    }
}

