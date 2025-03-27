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

 #include "lwip/snmp.h"
 #include "lwip/apps/snmp.h"
 #include "lwip/apps/snmp_scalar.h"
 
 
 /* --- lldp .1.0.8802.1.1.2 ----------------------------------------------------- */
 static s16_t
 lldp_get_value(const struct snmp_scalar_array_node_def *node, void *value)
 {
   u32_t *uint_ptr = (u32_t *)value;
   switch (node->oid) {
     case 1: /* lldpMessageTxInterval */
       *uint_ptr = 1;
       break;
     case 2: /* lldpMessageTxHoldMultiplier */
       *uint_ptr = 2;
       break;
     case 3: /* lldpReinitDelay */
       *uint_ptr = 3;
       break;
     case 4: /* lldpTxDelay */
       *uint_ptr = 4;
       break;
     case 5: /* lldpNotificationInterval */
       *uint_ptr = 5;
       break;
     default:
       LWIP_DEBUGF(SNMP_MIB_DEBUG, ("lldp_get_value(): unknown id: %"S32_F"\n", node->oid));
       return 0;
   }
 
   return sizeof(*uint_ptr);
 }
 
 /* the following nodes access variables in the lldp stack from callbacks */
 static const struct snmp_scalar_array_node_def lldp_config_nodes[] = {
   { 1, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY},  /* lldpMessageTxInterval */
   { 2, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY},  /* lldpMessageTxHoldMultiplier */
   { 3, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY},  /* lldpReinitDelay */
   { 4, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY},  /* lldpTxDelay */
   { 5, SNMP_ASN1_TYPE_INTEGER, SNMP_NODE_INSTANCE_READ_ONLY},  /* lldpNotificationInterval  */
 };
 
 const struct snmp_scalar_array_node lldp_mib_root = SNMP_SCALAR_CREATE_ARRAY_NODE(12, lldp_config_nodes, lldp_get_value, NULL, NULL);
