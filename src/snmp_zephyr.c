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
#define POSIX_FOREVER -1
#define TIMEOUT_RCV_US 10000

LOG_MODULE_REGISTER(snmp_log, CONFIG_LIB_SNMP_LOG_LEVEL);

static int create_socket(unsigned port)
{
    int socket_fd = -1;
    int ret;
    struct sockaddr_in bind_addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port   = htons( port ),
    };
    struct timeval timeout = {
        .tv_sec = 0,
        .tv_usec = TIMEOUT_RCV_US
    };

    socket_fd = zsock_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd < 0) {
        LOG_ERR("error: socket: %d errno: %d", socket_fd, errno );
        return -1;
    }

    LOG_INF("socket: %d %d", socket_fd, port);

    ret = zsock_setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (ret < 0) {
        LOG_ERR("failed to set SO_RCVTIMEO %d", errno);
        goto error;
    }

    ret = zsock_bind(socket_fd, (struct sockaddr *) &bind_addr, sizeof(bind_addr));
    if (ret < 0) {
        LOG_ERR("bind error: %d", errno );
        goto error;
    }

    return socket_fd;

error:
    zsock_close(socket_fd);
    return -1;

}

static void zephyr_snmp_agent(void *data0, void *data1, void *data2)
{
    ARG_UNUSED(data0);
    ARG_UNUSED(data1);
    ARG_UNUSED(data2);

    struct zsock_pollfd read_set[2];
    struct pbuf * pbuf = NULL;
    struct sockaddr client_addr;
    socklen_t client_addr_len;
    ssize_t length;

    /* Create the sockets. */
    read_set[0].fd = create_socket(LWIP_IANA_PORT_SNMP);
    read_set[0].events = ZSOCK_POLLIN;
    read_set[0].revents = 0;
    if (read_set[0].fd < 0) {
        LOG_ERR("Failed to init snmp");
        return;
    }
    read_set[1].fd = create_socket(LWIP_IANA_PORT_SNMP_TRAP);
    read_set[1].events = ZSOCK_POLLIN;
    read_set[1].revents = 0;
    if (read_set[1].fd < 0) {
        LOG_ERR("Failed to init snmp");
        goto cleanup_socket_0;
    }

    /* The lwIP SNMP driver owns a socket for traps 'snmp_traps_handle'. */
    // TODO racy
    // snmp_traps_handle = ( void * ) read_set[1].fd;

    pbuf = pbuf_alloc(PBUF_TRANSPORT, BUF_SIZE, PBUF_RAM);
    if( pbuf == NULL ) {
        LOG_ERR("Failed to alloc pbuf");
        goto cleanup_sockets;
    }
    pbuf->next = NULL;
    pbuf->ref = 1;

    while (1)
    {
        int ret = zsock_poll(read_set, ARRAY_SIZE(read_set), POSIX_FOREVER);
        if (ret < 0) {
            LOG_ERR("poll error: %d", errno);
            break;
        }

        for (size_t i = 0; i < ARRAY_SIZE(read_set); ++i) {
            if (read_set[i].revents & ZSOCK_POLLIN) {
                length = zsock_recvfrom(read_set[i].fd, pbuf->payload, BUF_SIZE, 0, &client_addr, &client_addr_len);
                if (length > 0) {
                    pbuf->tot_len = length;
                    pbuf->len = length;
                    snmp_receive(read_set[i].fd, pbuf, &client_addr);
                }
            }
        }
    }

    pbuf_free(pbuf);
cleanup_sockets:
    zsock_close(read_set[1].fd);
cleanup_socket_0:
    zsock_close(read_set[0].fd);

}

K_THREAD_DEFINE(zephyr_snmp_thread, CONFIG_SNMP_STACK_SIZE, zephyr_snmp_agent, NULL, NULL,
        NULL, CONFIG_SNMP_THREAD_PRIORITY, 0, 0);
