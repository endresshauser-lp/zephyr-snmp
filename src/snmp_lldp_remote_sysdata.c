/**
 * @file
 * Link Layer Discovery Protocol Management Information Base (IEEE Std 802.1AB-2005)
 * LLDP Remote Systemdata
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
 
/* --- lldp .1.0.8802.1.1.2.1.4.1.1 ----------------------------------------------------- */
static snmp_err_t lldp_remote_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}
 
static snmp_err_t lldp_remote_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}
 
static s16_t lldp_get_remote_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for loc port config table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpRemTimeMark  */
        *value_u32 = 41;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpRemLocalPortNum */
        *value_u32 = 42;
        value_len = sizeof(*value_u32);
        break;
    case 3: /* lldpRemIndex */
        *value_u32 = 43;
        value_len = sizeof(*value_u32);
        break;
    case 4: /* lldpRemChassisIsSubtype */
        *value_u32 = 44;
        value_len = sizeof(*value_u32);
        break;
    case 5: /* lldpRemChassisId */
        *value_u32 = 45;
        value_len = sizeof(*value_u32);
        break;
    case 6: /* ldpRemPortIdSubType */
        *value_u32 = 46;
        value_len = sizeof(*value_u32);
        break;
    case 7: /* lldpRemPortId */
        *value_u32 = 47;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_remote_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* --- lldp .1.0.8802.1.1.2.1.4.2.1 ----------------------------------------------------- */
static snmp_err_t lldp_rem_man_addr_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_rem_man_addr_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_rem_man_addr_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for loc man addr config table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpRemManAddrSubType */
        *value_u32 = 421;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpRemManAddr */
        *value_u32 = 422;
        value_len = sizeof(*value_u32);
        break;
    case 4: /* lldpRemManAddrIfId */
        *value_u32 = 424;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_rem_man_addr_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* the following nodes access variables in the lldp stack from callbacks */

static const struct snmp_table_col_def lldp_remote_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpRemTimeMark */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpremLocalPortNum */
    {3, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpRemIndex */
    {4, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpRemChassisIsSubtype */
    {5, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpRemChassisId */
    {6, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* ldpRemPortIdSubType */
    {7, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpRemPortId */
};

static const struct snmp_table_node lldp_remote_table = SNMP_TABLE_CREATE(
    1, lldp_remote_table_columns,
    lldp_remote_table_get_cell_instance, lldp_remote_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_remote_table_value, NULL, NULL
);

static const struct snmp_table_col_def lldp_remote_man_address_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpRemManAddrSubType */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpRemManAddr  */
    {4, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY} /*  lldpRemManAddrIfId */
};

static const struct snmp_table_node lldp_remote_man_address_table = SNMP_TABLE_CREATE(
    2, lldp_remote_man_address_table_columns,
    lldp_rem_man_addr_table_get_cell_instance, lldp_rem_man_addr_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_rem_man_addr_table_value, NULL, NULL
);


static const struct snmp_node *const lldp_remote_system_data_nodes[] = {
    &lldp_remote_table.node.node,
    &lldp_remote_man_address_table.node.node
};

const struct snmp_tree_node snmp_lldp_remote_system_data_root = SNMP_CREATE_TREE_NODE(4, lldp_remote_system_data_nodes);
