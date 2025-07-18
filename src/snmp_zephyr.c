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

#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#include "lwip/apps/snmp_opts.h"

#include "snmp_msg.h"
#include "lwip/prot/iana.h"

#include <zephyr/logging/log.h>

#define BUF_SIZE  512

LOG_MODULE_REGISTER(snmp_log, CONFIG_LIB_SNMP_LOG_LEVEL);

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

static void zephyr_snmp_agent(void *data0, void *data1, void *data2)
{
    ARG_UNUSED(data0);
    ARG_UNUSED(data1);
    ARG_UNUSED(data2);

    int socket_161;
    int socket_162;

    /* Create the sockets. */
    socket_161 = create_socket(LWIP_IANA_PORT_SNMP);
    if (socket_161 < 0) {
        LOG_ERR("Failed to init snmp");
        return;
    }
    socket_162 = create_socket(LWIP_IANA_PORT_SNMP_TRAP);
    if (socket_162 < 0) {
        zsock_close(socket_161);
        LOG_ERR("Failed to init snmp");
        return;
    }

    /* The lwIP SNMP driver owns a socket for traps 'snmp_traps_handle'. */
    // TODO racy
    // snmp_traps_handle = ( void * ) socket_162;

    struct pbuf * pbuf = pbuf_alloc( PBUF_TRANSPORT, BUF_SIZE, PBUF_RAM );

    if( pbuf == NULL ) {
        LOG_ERR("Failed to alloc pbuf");
        return;
    }
    pbuf->next = NULL;
    pbuf->ref = 1;

    while (1)
    {
        int rc_select;
        zsock_fd_set read_set; /* A set of file descriptors. */
        ZSOCK_FD_ZERO(&read_set);
        ZSOCK_FD_SET(socket_161, &read_set);
        ZSOCK_FD_SET(socket_162, &read_set);
        int select_max = MAX(socket_161, socket_162) + 1;

        struct timeval timeout={0}; /* Maximum time to wait for an event. */
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000U;

        /* TODO switch to zsock_poll, better */
        rc_select = zsock_select(select_max, &read_set, NULL, NULL, &(timeout));
        if (rc_select > 0) for (int index = 0; index < 2; index++)
        {
            int udp_socket = (index == 0) ? socket_161 : socket_162;
            if (ZSOCK_FD_ISSET(udp_socket, &read_set))
            {
                struct sockaddr client_addr;
                socklen_t client_addr_len;
                pbuf->len = zsock_recvfrom( udp_socket,
                        pbuf->payload,
                        BUF_SIZE,
                        0, // flags
                        &client_addr,
                        &client_addr_len );
                pbuf->tot_len = pbuf->len;
                if (pbuf->len > 0) {
                        snmp_receive(udp_socket, pbuf, &client_addr);
                } 
            } /* FD_ISSET */
        } /* for (int index = 0 */
    }
}

K_THREAD_DEFINE(zephyr_snmp_thread, CONFIG_SNMP_STACK_SIZE, zephyr_snmp_agent, NULL, NULL,
        NULL, CONFIG_SNMP_THREAD_PRIORITY, 0, 0);
