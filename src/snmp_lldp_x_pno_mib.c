/**
 * @file
 * PNO Management Information Base
 * LLDP Extension
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
 
/* --- lldp .1.0.8802.1.1.2.1.5.3791.1.2.1 ----------------------------------------------------- */
static snmp_err_t lldp_pno_local_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_pno_local_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_pno_local_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for local pno table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpXPnoLocLPDValue  */
        *value_u32 = 21;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpXPnoLocPortTxDValue */
        *value_u32 = 22;
        value_len = sizeof(*value_u32);
        break;
    case 3: /* lldpXPnoLocPortRxDValue */
        *value_u32 = 23;
        value_len = sizeof(*value_u32);
        break;
    case 6: /* lldpXPnoLocPortNoS */
        *value_u32 = 26;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_pno_local_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* --- lldp .1.0.8802.1.1.2.1.5.3791.1.3.1 ----------------------------------------------------- */
static snmp_err_t lldp_pno_remote_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_pno_remote_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_pno_remote_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for remote pno table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 1: /* lldpXPnoRemLPDValue */
        *value_u32 = 31;
        value_len = sizeof(*value_u32);
        break;
    case 2: /* lldpXPnoRemPortTxDValue */
        *value_u32 = 32;
        value_len = sizeof(*value_u32);
        break;
    case 3: /* lldpXPnoRemPortRxDValue */
        *value_u32 = 33;
        value_len = sizeof(*value_u32);
        break;
    case 6: /* lldpXPnoRemPortNoS */
        *value_u32 = 36;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_rem_man_addr_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* --- lldp dot3 .1.0.8802.1.1.2.1.5.4623.1.2 ----------------------------------------------------- */
static snmp_err_t lldp_dot3_local_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_dot3_local_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_dot3_local_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for remote dot3 table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 2: /* lldpXdot3LocPortAutoNegEnabled */
        *value_u32 = 22;
        value_len = sizeof(*value_u32);
        break;
    case 4: /* lldpXdot3LocPortOperMauType */
        *value_u32 = 24;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_dot3_local_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* --- lldp dot3 .1.0.8802.1.1.2.1.5.4623.1.3 ----------------------------------------------------- */
static snmp_err_t lldp_dot3_remote_table_get_cell_instance(const u32_t *column, const u32_t * row_oid, u8_t row_oid_len, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static snmp_err_t lldp_dot3_remote_table_get_next_cell_instance(const u32_t *column, struct snmp_obj_id *row_oid, struct snmp_node_instance *cell_instance)
{
    // TODO implement
    return SNMP_ERR_NOERROR;
}

static s16_t lldp_get_dot3_remote_table_value(struct snmp_node_instance *instance, void *value)
{
    // TODO struct for remote dot3 table
    u32_t *value_u32 = (u32_t *)value;
    //s32_t *value_s32 = (s32_t *)value;
    u16_t value_len;

    switch (SNMP_TABLE_GET_COLUMN_FROM_OID(instance->instance_oid.id)) {
    case 2: /* lldpXdot3RemPortAutoNegEnabled */
        *value_u32 = 32;
        value_len = sizeof(*value_u32);
        break;
    case 4: /* lldpXdot3RemPortOperMauType */
        *value_u32 = 34;
        value_len = sizeof(*value_u32);
        break;
    default:
        LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_dot3_remote_table_value(): unknown id: %" S32_F "\n", instance->instance_oid.id));
        return 0;
    }

    return value_len;
}

/* the following nodes access variables in the lldp stack from callbacks */
/* ----------------- PNO ----------------- */
static const struct snmp_table_col_def lldp_local_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXPnoLocLPDValue */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXPnoLocPortTxDValue */
    {3, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXPnoLocPortRxDValue */
    {6, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpXPnoLocPortNoS */
};

static const struct snmp_table_node lldp_pno_local_data_table = SNMP_TABLE_CREATE(
    1, lldp_local_table_columns,
    lldp_pno_local_table_get_cell_instance, lldp_pno_local_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_pno_local_table_value, NULL, NULL
);

static const struct snmp_table_col_def lldp_remote_table_columns[] = {
    {1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXPnoRemLPDValue */
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXPnoRemPortTxDValue  */
    {3, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXPnoRemPortRxDValue */
    {6, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpXPnoRemPortNoS */
};

static const struct snmp_table_node lldp_pno_remote_data_table = SNMP_TABLE_CREATE(
    1, lldp_remote_table_columns,
    lldp_pno_remote_table_get_cell_instance, lldp_pno_remote_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_pno_remote_table_value, NULL, NULL
);

static const struct snmp_node *const lldp_pno_local_data_nodes[] = {
    &lldp_pno_local_data_table.node.node
};

static const struct snmp_tree_node lldp_pno_local_data = SNMP_CREATE_TREE_NODE(2, lldp_pno_local_data_nodes);

static const struct snmp_node *const lldp_pno_remote_data_nodes[] = {
    &lldp_pno_remote_data_table.node.node
};

static const struct snmp_tree_node lldp_pno_remote_data = SNMP_CREATE_TREE_NODE(3, lldp_pno_remote_data_nodes);


static const struct snmp_node *const lldp_x_pno_objects_nodes[] = {
    &lldp_pno_local_data.node,
    &lldp_pno_remote_data.node
};

static const struct snmp_tree_node lldp_x_pno_object = SNMP_CREATE_TREE_NODE(1, lldp_x_pno_objects_nodes);

static const struct snmp_node *const lldp_x_pno_mib_nodes[] = {
    &lldp_x_pno_object.node
};

static const struct snmp_tree_node snmp_lldp_x_pno_mib_root = SNMP_CREATE_TREE_NODE(3791, lldp_x_pno_mib_nodes);

/* ----------------- dot3 ----------------- */
static const struct snmp_table_col_def lldp_dot3_local_table_columns[] = {
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXdot3LocPortAutoNegEnabled */
    {4, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpXdot3LocPortOperMauType */
};

static const struct snmp_table_node lldp_dot3_local_data_table = SNMP_TABLE_CREATE(
    1, lldp_dot3_local_table_columns,
    lldp_dot3_local_table_get_cell_instance, lldp_dot3_local_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_dot3_local_table_value, NULL, NULL
);

static const struct snmp_table_col_def lldp_dot3_remote_table_columns[] = {
    {2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}, /* lldpXdot3RemPortAutoNegEnabled */
    {4, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY}  /* lldpXdot3RemPortOperMauType */
};

static const struct snmp_table_node lldp_dot3_remote_data_table = SNMP_TABLE_CREATE(
    1, lldp_dot3_remote_table_columns,
    lldp_dot3_remote_table_get_cell_instance, lldp_dot3_remote_table_get_next_cell_instance,
    //NULL, NULL,
    lldp_get_dot3_remote_table_value, NULL, NULL
);

static const struct snmp_node *const lldp_x_dot3_local_data_nodes[] = {
    &lldp_dot3_local_data_table.node.node
};

static const struct snmp_tree_node lldp_x_dot3_local_data = SNMP_CREATE_TREE_NODE(2, lldp_x_dot3_local_data_nodes);

static const struct snmp_node *const lldp_x_dot3_remote_data_nodes[] = {
    &lldp_dot3_remote_data_table.node.node
};

static const struct snmp_tree_node lldp_x_dot3_remote_data = SNMP_CREATE_TREE_NODE(3, lldp_x_dot3_remote_data_nodes);

static const struct snmp_node *const lldp_x_dot3_objects_nodes[] = {
    &lldp_x_dot3_local_data.node,
    &lldp_x_dot3_remote_data.node
};

static const struct snmp_tree_node lldp_x_dot3_objects = SNMP_CREATE_TREE_NODE(1, lldp_x_dot3_objects_nodes);

static const struct snmp_node *const lldp_x_dot3_mib_nodes[] = {
    &lldp_x_dot3_objects.node
};

static const struct snmp_tree_node snmp_lldp_x_dot3_mib_root = SNMP_CREATE_TREE_NODE(4623, lldp_x_dot3_mib_nodes);

static const struct snmp_node *const snmp_lldp_extension_nodes[] = {
   &snmp_lldp_x_pno_mib_root.node,
   &snmp_lldp_x_dot3_mib_root.node
};
 
const struct snmp_tree_node snmp_lldp_extension_root = SNMP_CREATE_TREE_NODE(5, snmp_lldp_extension_nodes);
 