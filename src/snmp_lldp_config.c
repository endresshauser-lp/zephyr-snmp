/**
 * @file
 * Link Layer Discovery Protocol Management Information Base Config (IEEE Std 802.1AB-2005)
 * LLDP configuration
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

#include "lwip/snmp.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_scalar.h"
#include "lwip/apps/snmp_table.h"

/* --- lldp .1.0.8802.1.1.2.1.1 ----------------------------------------------------- */
static s16_t lldp_get_value(struct snmp_node_instance *instance, void *value)
{
    s32_t *sint_ptr = (s32_t *)value;
    switch (instance->node->oid) {
    case 1: /* lldpMessageTxInterval */
        *sint_ptr = 1;
        return sizeof(*sint_ptr);
    case 2: /* lldpMessageTxHoldMultiplier */
        *sint_ptr = 2;
        return sizeof(*sint_ptr);
    case 3: /* lldpReinitDelay */
        *sint_ptr = 3;
        return sizeof(*sint_ptr);
    case 4: /* lldpTxDelay */
        *sint_ptr = 4;
        return sizeof(*sint_ptr);
    case 5: /* lldpNotificationInterval */
        *sint_ptr = 5;
        return sizeof(*sint_ptr);
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_value(): unknown id: %" S32_F "\n", instance->node->oid));
        return 0;
    }
}

/* --- lldp .1.0.8802.1.1.2.1.6 ----------------------------------------------------- */
static snmp_err_t lldp_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_port_config_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for port config table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpPortConfigPortNum */
        *value_u32 = 1;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpConfigAdminStatus */
        *value_u32 = 2;
        value_len = sizeof(*value_u32);
        break;
    case 3: /* lldpConfigNotificationEnable */
        *value_u32 = 3;
        value_len = sizeof(*value_u32);
        break;
    case 4: /* lldpPortConfigTLVsTxEnable */
        *value_u32 = 0;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_port_config_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* the following nodes access variables in the lldp stack from callbacks */
static const struct snmp_scalar_node lldp_message_tx_interval = SNMP_SCALAR_CREATE_NODE_READONLY(1, SNMP_ASN1_TYPE_INTEGER, lldp_get_value);
static const struct snmp_scalar_node lldp_message_tx_hold_multiplier = SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_INTEGER, lldp_get_value);
static const struct snmp_scalar_node lldp_reinit_delay = SNMP_SCALAR_CREATE_NODE_READONLY(3, SNMP_ASN1_TYPE_INTEGER, lldp_get_value);
static const struct snmp_scalar_node lldp_tx_delay = SNMP_SCALAR_CREATE_NODE_READONLY(4, SNMP_ASN1_TYPE_INTEGER, lldp_get_value);
static const struct snmp_scalar_node lldp_notification_interval = SNMP_SCALAR_CREATE_NODE_READONLY(5, SNMP_ASN1_TYPE_INTEGER, lldp_get_value);

static const struct snmp_table_col_def lldp_port_config_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpPortConfigPortNum */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpConfigAdminStatus */
    {3, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpConfigNotificationEnable */
    {4, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpPortConfigTLVsTxEnable */
};

static const struct snmp_table_node lldp_port_config_table = SNMP_TABLE_CREATE(
    6, lldp_port_config_table_columns,
    lldp_table_get_cell_instance, lldp_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_port_config_table_value, NULL, NULL
);

static const struct snmp_node *const lldp_config_nodes[] = {
    &lldp_message_tx_interval.node.node,
    &lldp_message_tx_hold_multiplier.node.node,
    &lldp_reinit_delay.node.node,
    &lldp_tx_delay.node.node,
    &lldp_notification_interval.node.node,
    &lldp_port_config_table.node.node
};

const struct snmp_tree_node snmp_lldp_config_root = SNMP_CREATE_TREE_NODE(1, lldp_config_nodes);
