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

/* --- lldp .1.0.8802.1.1.2.1.3 ----------------------------------------------------- */
static s16_t lldp_sysdata_get_value(struct snmp_node_instance *instance, void *value)
{
    s32_t *sint_ptr = (s32_t *)value;
    switch (instance->node->oid) {
    case 1: /* lldpLocChassisIdSubType  */
        *sint_ptr = 1;
        return sizeof(*sint_ptr);
    case 2: /* lldpLocChassisId */
        *sint_ptr = (s32_t)"Id";
        return sizeof(*sint_ptr);
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_value(): unknown id: %" S32_F "\n", instance->node->oid));
        return 0;
    }
}

/* --- lldp .1.0.8802.1.1.2.1.3.7.1 ----------------------------------------------------- */
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

static s16_t lldp_get_loc_port_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for loc port config table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpLocPortNum */
        *value_u32 = 71;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpLocPortIdSubtype */
        *value_u32 = 72;
        value_len = sizeof(*value_u32);
        break;
    case 3: /* lldpLocPortId */
        *value_u32 = 73;
        value_len = sizeof(*value_u32);
        break;
    case 4: /* lldpLocPortDesc */
        *value_u32 = (u32_t)"Descr";
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_loc_port_config_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* --- lldp .1.0.8802.1.1.2.1.3.8.1 ----------------------------------------------------- */
static snmp_err_t lldp_loc_man_addr_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_loc_man_addr_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_loc_man_addr_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for loc man addr config table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpLocManAddrSubType */
        *value_u32 = 81;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpLocaManAddr */
        *value_u32 = 82;
        value_len = sizeof(*value_u32);
        break;
    case 5: /* lldpLocManAddrIfId */
        *value_u32 = 85;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_loc_man_addr_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* the following nodes access variables in the lldp stack from callbacks */
static const struct snmp_scalar_node lldp_loc_chassis_id_subtype  = SNMP_SCALAR_CREATE_NODE_READONLY(1, SNMP_ASN1_TYPE_INTEGER, lldp_sysdata_get_value);
static const struct snmp_scalar_node lldp_loc_chassis_id = SNMP_SCALAR_CREATE_NODE_READONLY(2, SNMP_ASN1_TYPE_OCTET_STRING, lldp_sysdata_get_value);

static const struct snmp_table_col_def lldp_port_config_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpLocPortNum */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpLocPortIdSubtype */
    {3, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpLocPortId */
    {4, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpLocPortDesc */
};

static const struct snmp_table_node lldp_loc_port_table = SNMP_TABLE_CREATE(
    7, lldp_port_config_table_columns,
    lldp_table_get_cell_instance, lldp_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_loc_port_table_value, NULL, NULL
);

static const struct snmp_table_col_def lldp_loc_man_address_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpLocManAddrSubType */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpLocaManAddr */
    {5, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY} /*  lldpLocManAddrIfId */
};

static const struct snmp_table_node lldp_loc_man_address_table = SNMP_TABLE_CREATE(
    8, lldp_loc_man_address_table_columns,
    lldp_loc_man_addr_table_get_cell_instance, lldp_loc_man_addr_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_loc_man_addr_table_value, NULL, NULL
);


static const struct snmp_node *const lldp_system_data_nodes[] = {
    &lldp_loc_chassis_id_subtype.node.node,
    &lldp_loc_chassis_id.node.node,
    &lldp_loc_port_table.node.node,
    &lldp_loc_man_address_table.node.node
};

const struct snmp_tree_node snmp_lldp_local_system_data_root = SNMP_CREATE_TREE_NODE(3, lldp_system_data_nodes);
