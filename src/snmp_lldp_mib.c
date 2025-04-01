/**
 * @file
 * Link Layer Discovery Protocol Management Information Base (IEEE Std 802.1AB-2005)
 * LLDP configuration, statistics, local system data and remote systems data components.
 */

/*
 * Copyright (c) 2025
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
 */

#include "lwip/apps/snmp_scalar.h"

/* --- lldp .1.0.8802.1.1.2 ----------------------------------------------------- */
extern const struct snmp_tree_node snmp_lldp_config_root;
extern const struct snmp_tree_node snmp_lldp_local_system_data_root;
extern const struct snmp_tree_node snmp_lldp_remote_system_data_root;
// TODO implement
//extern const struct snmp_scalar_array_node snmp_lldp_config_man_address_root;

static const struct snmp_node *const lldp_nodes[] = {
  &snmp_lldp_config_root.node,
  &snmp_lldp_local_system_data_root.node,
  &snmp_lldp_remote_system_data_root.node
  //&snmp_lldp_config_man_address_root.node.node
};

static const struct snmp_tree_node lldp_root = SNMP_CREATE_TREE_NODE(1, lldp_nodes);

static const u32_t lldp_base_oid_arr[] = {1, 0, 8802, 1, 1, 2, 1};
const struct snmp_mib lldp_mib = SNMP_MIB_CREATE(lldp_base_oid_arr, &lldp_root.node);
